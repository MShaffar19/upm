/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>
#include <stdexcept>

#include "ht9170.hpp"

using namespace upm;
using namespace std;

HT9170::HT9170(int dr, int o1, int o2, int o3, int o4)
{
  // init the gpio's we will need
  if ( !(m_dr = mraa_gpio_init(dr)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(dr) failed, invalid pin?");
      return;
    }
  mraa_gpio_dir(m_dr, MRAA_GPIO_IN);

  if ( !(m_o1 = mraa_gpio_init(o1)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(o1) failed, invalid pin?");
      return;
    }
  mraa_gpio_dir(m_o1, MRAA_GPIO_IN);

  if ( !(m_o2 = mraa_gpio_init(o2)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(o2) failed, invalid pin?");
      return;
    }
  mraa_gpio_dir(m_o2, MRAA_GPIO_IN);

  if ( !(m_o3 = mraa_gpio_init(o3)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(o3) failed, invalid pin?");
      return;
    }
  mraa_gpio_dir(m_o3, MRAA_GPIO_IN);

  if ( !(m_o4 = mraa_gpio_init(o4)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(o4) failed, invalid pin?");
      return;
    }
  mraa_gpio_dir(m_o4, MRAA_GPIO_IN);
}

HT9170::~HT9170()
{
  mraa_gpio_close(m_dr);
  mraa_gpio_close(m_o1);
  mraa_gpio_close(m_o2);
  mraa_gpio_close(m_o3);
  mraa_gpio_close(m_o4);
}

bool HT9170::digitReady()
{
  return (mraa_gpio_read(m_dr) ? true : false);
}

char HT9170::decodeDigit()
{
  // This lookup table is derived from the decode table on page 7 of
  // the HT9170B/HT9170D datasheet.

  static const char decode[16] = {
    'D',     // 0
    '1',     // 1
    '2',     // 2
    '3',     // 3
    '4',     // 4
    '5',     // 5
    '6',     // 6
    '7',     // 7
    '8',     // 8
    '9',     // 9
    '0',     // 10
    '*',     // 11
    '#',     // 12
    'A',     // 13
    'B',     // 14
    'C'      // 15
  };

  uint8_t bits = 0;

  bits = mraa_gpio_read(m_o1);
  bits |= (mraa_gpio_read(m_o2) << 1);
  bits |= (mraa_gpio_read(m_o3) << 2);
  bits |= (mraa_gpio_read(m_o4) << 3);

  return decode[bits];
}
