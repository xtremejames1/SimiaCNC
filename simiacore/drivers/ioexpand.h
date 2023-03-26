/*

  ioexpand.h - driver code for RP2040 ARM processor

  I2C I/O expander

  Part of grblHAL

  Copyright (c) 2018-2022 Terje Io

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

#ifndef _IOEXPAND_H_
#define _IOEXPAND_H_

#include "driver.h"

#define ioex_out(pin) ioex_outN(pin)
#define ioex_outN(pin) io_expander.out ## pin
#define ioex_in(pin) ioex_inN(pin)
#define ioex_inN(pin) io_expander.in ## pin

typedef union {
    uint8_t mask;
    struct {
        uint8_t out0 :1,
                out1 :1,
                out2 :1,
                out3 :1,
                out4 :1,
                out5 :1,
                out6 :1,
                out7 :1;
    };
    struct {
        uint8_t in0 :1,
                in1 :1,
                in2 :1,
                in3 :1,
                in4 :1,
                in5 :1,
                in6 :1,
                in7 :1;
    };
} ioexpand_t;

void ioexpand_init (void);
void ioexpand_out (ioexpand_t pins);
ioexpand_t ioexpand_in (void);

#endif
