/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>

#include "rpr220.h"

rpr220_context rpr220_init(int pin)
{
    rpr220_context dev =
        (rpr220_context)malloc(sizeof(struct _rpr220_context));

    if (!dev)
        return NULL;

    dev->gpio = NULL;
    dev->isrInstalled = false;

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        rpr220_close(dev);
        return NULL;
    }

    // MRAA contexts...
    if ( !(dev->gpio = mraa_gpio_init(pin)) )
    {
        printf("%s: mraa_gpio_init() failed\n",
               __FUNCTION__);
        rpr220_close(dev);
        return NULL;
    }

    mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN);

    return dev;
}

void rpr220_close(rpr220_context dev)
{
    assert(dev != NULL);

    if (dev->isrInstalled)
        rpr220_uninstall_isr(dev);

    if (dev->gpio)
        mraa_gpio_close(dev->gpio);

    free(dev);
}

bool rpr220_black_detected(const rpr220_context dev)
{
    assert(dev != NULL);

    return (mraa_gpio_read(dev->gpio) ? true : false);
}

void rpr220_install_isr(const rpr220_context dev,
                        void (*isr)(void *), void *arg)
{
    assert(dev != NULL);

    if (dev->isrInstalled)
        rpr220_uninstall_isr(dev);

    // install our interrupt handler
    if (mraa_gpio_isr(dev->gpio, MRAA_GPIO_EDGE_RISING, isr, arg))
    {
        printf("%s: mraa_gpio_isr() failed.\n", __FUNCTION__);
        return;
    }

    dev->isrInstalled = true;
}

void rpr220_uninstall_isr(const rpr220_context dev)
{
    assert(dev != NULL);

    mraa_gpio_isr_exit(dev->gpio);
    dev->isrInstalled = false;
}
