/*
  simiacore.h - driver code for SimiaCore CNC Controller Board

  Part of grblHAL

  Copyright (c) 2021-2023 Terje Io
  Copyright (c) 2021 Volksolive
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

#if TRINAMIC_ENABLE
#error Trinamic plugin not supported!
#endif

#if N_ABC_MOTORS > 1
#error "Axis configuration is not supported!"
#endif

// Define step pulse output pins.
#define STEP_PORT               GPIO_PIO  // N_AXIS pin PIO SM
#define STEP_PINS_BASE          2         // N_AXIS number of consecutive pins are used by PIO

// Define step direction output pins.
#define DIRECTION_PORT          GPIO_OUTPUT
#define X_DIRECTION_PIN         6
#define Y_DIRECTION_PIN         7
#define Z_DIRECTION_PIN         8
#define DIRECTION_OUTMODE       GPIO_SHIFT5

// Define stepper driver enable/disable output pin.
#define ENABLE_PORT             GPIO_OUTPUT
#define STEPPERS_ENABLE_PIN     14

// Define homing/hard limit switch input pins.
#define X_LIMIT_PIN             18
#define Y_LIMIT_PIN             19
#define Z_LIMIT_PIN             20
#define LIMIT_INMODE            GPIO_MAP

// Define ganged axis or A axis step pulse and step direction output pins.
#define M3_AVAILABLE
#define M3_STEP_PIN             (STEP_PINS_BASE + 3)
#define M3_DIRECTION_PIN        (Z_DIRECTION_PIN + 1)
#define M3_LIMIT_PIN            (Z_LIMIT_PIN + 1)

// Define spindle enable and spindle direction output pins.
#define SPINDLE_PORT            GPIO_OUTPUT
#define SPINDLE_ENABLE_PIN      26
#define SPINDLE_DIRECTION_PIN   27

// Define spindle PWM output pin.
#define SPINDLE_PWM_PORT        GPIO_OUTPUT
#define SPINDLE_PWM_PIN         15

// Define flood OR mist coolant enable output pins. ONLY ONE can be active at any time.
#define COOLANT_PORT            GPIO_OUTPUT
//#define COOLANT_FLOOD_PIN       16
#define COOLANT_MIST_PIN        17

// Define user-control controls (cycle start, reset, feed hold) input pins.
#define RESET_PIN               16
#define FEED_HOLD_PIN           1
#define CYCLE_START_PIN         0

//Define SD card pins
#if SDCARD_ENABLE
#define SPI_PORT            spi1
#define SD_SCK_PIN          10
#define SD_MOSI_PIN         11
#define SD_MISO_PIN         12
#define SD_CS_PIN           13

// Define probe switch input pin.
#define PROBE_PIN               22

#if I2C_STROBE_ENABLE
#define I2C_STROBE_PIN          28
#endif
