/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * 	   Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
 *
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "rotary.h"
#include "upm_fti.h"
#include "upm_math.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_rotary_name[] = "Grove Rotary";
const char upm_rotary_description[] = "Analog Grove Rotary Sensor";
const upm_protocol_t upm_rotary_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_rotary_category[] = {UPM_VOLTAGE, UPM_ANGLE};

// forward declarations
const upm_sensor_descriptor_t upm_rotary_get_descriptor();
const void* upm_rotary_get_ft(upm_sensor_t sensor_type);
void* upm_rotary_init_name();
void upm_rotary_close(void* dev);
upm_result_t upm_rotary_get_value_voltage (const void* dev, float* volts);
upm_result_t upm_rotary_get_value_angle(void* dev, float* rotval,
                                        upm_angle_u unit);

const upm_sensor_descriptor_t upm_rotary_get_descriptor() {
    upm_sensor_descriptor_t usd;
    usd.name = upm_rotary_name;
    usd.description = upm_rotary_description;
    usd.protocol_size = 1;
    usd.protocol = upm_rotary_protocol;
    usd.category_size = 1;
    usd.category = upm_rotary_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_rotary_init_name,
    .upm_sensor_close = &upm_rotary_close,
    .upm_sensor_get_descriptor = &upm_rotary_get_descriptor
};

static const upm_voltage_ft vft =
{
    .upm_voltage_get_value = &upm_rotary_get_value_voltage
};

static const upm_angle_ft aft =
{
    .upm_angle_get_value = &upm_rotary_get_value_angle
};

const void* upm_rotary_get_ft(upm_sensor_t sensor_type) {
    if(sensor_type == UPM_SENSOR) {
        return &ft;
    }
    if(sensor_type == UPM_VOLTAGE) {
        return &vft;
    }
    if(sensor_type == UPM_ANGLE) {
        return &aft;
    }
    return NULL;
}

void* upm_rotary_init_name(){
    return NULL;
}

void upm_rotary_close(void* dev)
{
    rotary_close((rotary_context)dev);
}

upm_result_t upm_rotary_get_value_voltage (const void* dev, float* volts)
{
    return rotary_get_value_voltage((rotary_context)dev, volts);
}

upm_result_t upm_rotary_get_value_angle (void* dev, float* rotval,
                                         upm_angle_u unit)
{
    float degrees;

    // returned in degrees
    rotary_get_value_angle((rotary_context)dev, &degrees);

    switch(unit)
        {
        case DEGREES:
            *rotval = degrees;
            return UPM_SUCCESS;

        case RADIANS:
            *rotval = degrees * M_PI / 180.0;

            return UPM_SUCCESS;

        default:
            return UPM_ERROR_INVALID_PARAMETER;
        }
}
