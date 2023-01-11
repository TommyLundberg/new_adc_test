/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */



#include <zephyr/zephyr.h>
#include <drivers/adc.h>



void main(void)
{
	printk("Hello World! %s\n", CONFIG_BOARD);
	static const struct adc_channel_cfg ch0_cfg_dt = ADC_CHANNEL_CFG_DT(DT_CHILD(DT_NODELABEL(adc), channel_0));
	static const struct adc_channel_cfg ch1_cfg_dt = ADC_CHANNEL_CFG_DT(DT_CHILD(DT_NODELABEL(adc), channel_1));

}
