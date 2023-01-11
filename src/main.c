/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */



#include <zephyr.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>

#define ADC_RESOLUTION 12
#define ADC_CHANNEL_0  0
static int16_t sample_buffer_0[8];
static int16_t sample_buffer_1[1];
int err;


void main(void){

	printk("Hello World! %s\n", CONFIG_BOARD);
	static const struct adc_channel_cfg ch0_cfg_dt = ADC_CHANNEL_CFG_DT(DT_CHILD(DT_NODELABEL(adc), channel_0));
	static const struct adc_channel_cfg ch1_cfg_dt = ADC_CHANNEL_CFG_DT(DT_CHILD(DT_NODELABEL(adc), channel_1));
	const struct device *dev_adc = DEVICE_DT_GET(DT_NODELABEL(adc));

	err = adc_channel_setup(dev_adc, &ch0_cfg_dt);
	err = adc_channel_setup(dev_adc, &ch1_cfg_dt);

	const struct adc_sequence_options ch0_sequence_opts = {
	.interval_us = 0,
	.callback = NULL,
	.user_data = NULL,
	.extra_samplings = 7,
	};

	struct adc_sequence adc_ch0_seq = {
		.options = &ch0_sequence_opts,
		.channels = BIT(ADC_CHANNEL_0),
		.buffer = sample_buffer_0,
		.buffer_size = sizeof(sample_buffer_0),
		.resolution = ADC_RESOLUTION,
		.calibrate = true,
	};

}
