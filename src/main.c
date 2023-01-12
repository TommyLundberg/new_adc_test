/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */



#include <zephyr.h>
#include <stdio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <math.h>
#include <zephyr/drivers/adc.h>

#define ADC_RESOLUTION 14
#define ADC_CHANNEL_0  0
#define ADC_CH0_OVERSAMPLE 8
#define BUFFER_SIZE 8
static int16_t sample_buffer_0;
int err;
bool state;
int32_t real_mv;
int32_t gain_inv = 5;
int32_t ref_mv = 600;


/* adc read sequence struct with options to be fed into 
the adc_read() function */
const struct adc_sequence_options ch0_sequence_opts = {
	.interval_us = 0,
	.callback = NULL,
	.user_data = NULL,
	.extra_samplings = 0,
	};
struct adc_sequence adc_ch0_seq = {
		.options = &ch0_sequence_opts,
		.channels = BIT(ADC_CHANNEL_0),
		.buffer = &sample_buffer_0,
		.buffer_size = sizeof(sample_buffer_0),
		.resolution = ADC_RESOLUTION,
		.oversampling = ADC_CH0_OVERSAMPLE,
		.calibrate = true,
	};



void main(void){

	printk("Board model: %s\n", CONFIG_BOARD);
	static const struct adc_channel_cfg ch0_cfg_dt = ADC_CHANNEL_CFG_DT(DT_CHILD(DT_NODELABEL(adc), channel_0));
	const struct device *dev_adc = DEVICE_DT_GET(DT_NODELABEL(adc));
	
	err = adc_channel_setup(dev_adc, &ch0_cfg_dt);
		if (err!=0){
			printk("ADC channel setup failed, error %d\n",err);
			return -1;
		}
		else{
			printk("ADC channel setup successfull\n");
		}

	state = device_is_ready(dev_adc);
		if (state==false){
			printk("ADC Channel not ready");
			return -1;
		}
	
	while (1){

		
		err = adc_read(dev_adc, &adc_ch0_seq);
			if (!err){
				printk("ADC read successfull\n");
				printk("Differential input A0-A1 read\n");
				k_sleep(K_MSEC(10));
				printk("Raw value read: %d\n",sample_buffer_0);
				/*real value = (readout*ref)/(gain*2^(RESOLUTION-1))*/
				real_mv = (ref_mv*gain_inv*sample_buffer_0)>>(ADC_RESOLUTION-1);
				printk("mV: %d\n", real_mv);
				
			}
			else{
				printk("NO READOUT FOR YOU!!!\n");
				printk("ERROR %d\n",err);
				return -1;
			}

		k_sleep(K_SECONDS(5));
	}
	

	

}
