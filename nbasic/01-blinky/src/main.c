//==============================================================================
// 03-button.c
// Get button configuration from the devicetree sw0 alias. This is mandatory.
//
// Created by Hugo Pristauz on 2022-Jan-09
// Copyright © 2022 Bluenetics. All rights reserved.
//==============================================================================
// Copyright (c) 2016 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//==============================================================================

  #include <zephyr.h>
  #include <device.h>
  #include <devicetree.h>
  #include <drivers/gpio.h>

//==============================================================================
// defines
//==============================================================================

  #define SLEEP_TIME_MS   1000   // 1000 ms = 1s

  #define LED0_NODE DT_ALIAS(led0)  // devicetree node ID for "led0" alias

  #if DT_NODE_HAS_STATUS(LED0_NODE, okay)
    #define LED0	DT_GPIO_LABEL(LED0_NODE, gpios)
    #define PIN	DT_GPIO_PIN(LED0_NODE, gpios)
    #define FLAGS	DT_GPIO_FLAGS(LED0_NODE, gpios)
  #else

    // A build error here means your board isn't set up to blink an LED

  #error "Unsupported board: led0 devicetree alias is not defined"
    #define LED0	""
    #define PIN	0
    #define FLAGS	0
  #endif

//==============================================================================
// main
//==============================================================================

  void main(void)
  {
  	const struct device *dev;
  	bool led_is_on = true;
  	int ret;

  	dev = device_get_binding(LED0);
  	if (dev == NULL) {
  		return;
  	}

  	ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
  	if (ret < 0) {
  		return;
  	}

  	while (1)
    {
  		gpio_pin_set(dev, PIN, (int)led_is_on);
  		led_is_on = !led_is_on;
  		k_msleep(SLEEP_TIME_MS);
  	}
  }
