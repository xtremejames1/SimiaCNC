/*
  pico_cnc.c - driver code for RP2040 ARM processors

  Part of grblHAL

  Copyright (c) 2023 James Xiao

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "simia_core_map.h"
#include "driver.h"

#if defined(BOARD_SIMIA_CORE)

#include "pico/time.h"
#include "hardware/gpio.h"


static limit_signals_t readLimitStates (void)
{
    limit_signals_t signals = {0};

    DIGITAL_OUT(SH_LD, 0);
    time.sleep_us(125);
    DIGITAL_OUT(SH_LD, 1);
    time.sleep_us(125);

    signals.min.x = DIGITAL_IN(QH_BIT);

    DIGITAL_OUT(CLK_BIT, 1);
    time.sleep_us(125);
    DIGITAL_OUT(CLK_BIT, 0);
    time.sleep_us(125);

    signals.min.y = DIGITAL_IN(QH_BIT);

    DIGITAL_OUT(CLK_BIT, 1);
    time.sleep_us(125);
    DIGITAL_OUT(CLK_BIT, 0);
    time.sleep_us(125);

    signals.min.z = DIGITAL_IN(QH_BIT);

    DIGITAL_OUT(CLK_BIT, 1);
    time.sleep_us(125);
    DIGITAL_OUT(CLK_BIT, 0);
    time.sleep_us(125);

    signals.min.a = DIGITAL_IN(QH_BIT);

    DIGITAL_OUT(CLK_BIT, 1);
    time.sleep_us(125);
    DIGITAL_OUT(CLK_BIT, 0);
    time.sleep_us(125);

    return signals;
}

hal.homing.get_state = readLimitStates();

#endif