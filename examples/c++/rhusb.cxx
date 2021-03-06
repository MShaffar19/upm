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

#include <iostream>
#include <signal.h>
#include <string>

#include "rhusb.hpp"
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]

    string defaultDev = "/dev/ttyUSB0";

    // if an argument was specified, use it as the device instead
    if (argc > 1)
        defaultDev = string(argv[1]);

    cout << "Using device " << defaultDev << endl;
    cout << "Initializing..." << endl;

    // Instantiate an RHUSB instance on defaultDev
    upm::RHUSB sensor(defaultDev);

    // output the firmware ID
    cout << "Firmware ID: " << sensor.getFirmwareID() << endl;
    cout << endl;

    // update and print available values every second
    while (shouldRun) {
        // update our values from the sensor
        sensor.update();

        // we show both C and F for temperature
        cout << "Temperature: " << sensor.getTemperature() << " C / " << sensor.getTemperature(true)
             << " F" << endl;

        cout << "Humidity: " << sensor.getHumidity() << " %" << endl;

        cout << endl;

        upm_delay(1);
    }

    cout << "Exiting..." << endl;

    //! [Interesting]

    return 0;
}
