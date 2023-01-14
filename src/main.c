/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */



#include <zephyr.h>
#include <stdio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>

#define ADC_RESOLUTION     14
#define ADC_CHANNEL_0       0
#define ADC_CH0_OVERSAMPLE  8

volatile int16_t adc_sample_buffer_0;
static int       adc_err;
static bool      adc_state;
static int32_t   adc_real_mv;

/*The internal voltage reference is 600mV, 
gain is set in dts overlay*/
static int32_t   adc_gain_inv = 5;
static int32_t   adc_ref_mv = 600;

/* adc read sequence struct with options to be fed into 
the adc_read() function*/
const struct adc_sequence_options adc_ch0_sequence_opts = 
{
	.interval_us     = 0,
	.callback        = NULL,
	.user_data       = NULL,
	.extra_samplings = 0,
};

struct adc_sequence adc_ch0_seq = 
{
	.options        = &adc_ch0_sequence_opts,
	.channels       = BIT(ADC_CHANNEL_0),
	.buffer         = &adc_sample_buffer_0,
	.buffer_size    = sizeof(adc_sample_buffer_0),
	.resolution     = ADC_RESOLUTION,
	.oversampling   = ADC_CH0_OVERSAMPLE,
	.calibrate      = true,
};

void main(void)
{

	/*Get the channel configuration from dts overlay
	and init the adc device	*/
	static const struct adc_channel_cfg 
	adc_ch0_cfg_dt = ADC_CHANNEL_CFG_DT(DT_CHILD(DT_NODELABEL(adc), channel_0));

	const struct device *adc_dev = DEVICE_DT_GET(DT_NODELABEL(adc));

	adc_err = adc_channel_setup(adc_dev, &adc_ch0_cfg_dt);

	if (adc_err != 0)
	{
		printk("ADC channel setup failed, error %d\n",adc_err);
		return -1;
	}
	else 
	{
			printk("ADC channel setup successfull\n");
	}

	adc_state = device_is_ready(adc_dev);

	if (adc_state == false)
	{
		printk("ADC Channel not ready");
		return -1;
	}
	
	while (1)
	{	
		adc_err = adc_read(adc_dev, &adc_ch0_seq);

		if (adc_err=0){
			printk("ADC read successfull\n");
			printk("Differential input A0-A1 read\n");

			/*Oversampling 2^ADC_CH0_OVERSAMPLE times, each sample 
			takes 20us, set in dts overlay. Give the adc some time 
			to finish sampling (256*20us=5.12ms)*/
			k_sleep(K_MSEC(10));
			printk("Raw value read: %d\n", adc_sample_buffer_0);

			/*real value = (readout*ref)/(gain*2^(RESOLUTION-1))*/
			adc_real_mv = (adc_ref_mv * adc_gain_inv * adc_sample_buffer_0)
			>>(ADC_RESOLUTION -1);
			printk("mV: %d\n", adc_real_mv);	
		}
		else 
		{
			printk("NO READOUT FOR YOU!!!\n");
			printk("ERROR %d\n", adc_err);
			return -1;
		}

		k_sleep(K_SECONDS(5));
	}

}
