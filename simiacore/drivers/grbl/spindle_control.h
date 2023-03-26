/*
  spindle_control.h - spindle control methods

  Part of grblHAL

  Copyright (c) 2017-2022 Terje Io
  Copyright (c) 2012-2015 Sungeun K. Jeon
  Copyright (c) 2009-2011 Simen Svale Skogsrud

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

#ifndef _SPINDLE_CONTROL_H_
#define _SPINDLE_CONTROL_H_

typedef int8_t spindle_id_t;
typedef int8_t spindle_num_t;

// if changed to > 8 bits planner_cond_t needs to be changed too
typedef union {
    uint8_t value;
    uint8_t mask;
    struct {
        uint8_t on               :1,
                ccw              :1,
                pwm              :1, //!< NOTE: only used for PWM inversion setting
                reserved         :1,
                override_disable :1,
                encoder_error    :1,
                at_speed         :1, //!< Spindle is at speed.
                synchronized     :1;
    };
} spindle_state_t;

/*! \brief  Bitmap flags for spindle capabilities. */
typedef union {
    uint8_t value; //!< All bitmap flags.
    struct {
        uint8_t variable          :1, //!< Variable spindle speed is supported.
                direction         :1, //!< Spindle direction (M4) is supported.
                at_speed          :1, //!< Spindle at speed feedback is supported.
                laser             :1, //!< Spindle can control a laser.
                pwm_invert        :1, //!< Spindle PWM output can be inverted.
                pid               :1,
                pwm_linearization :1,
                rpm_range_locked  :1; //!< Spindle RPM range (min, max) not inherited from settings.
    };
} spindle_cap_t;

/*! \brief Used when HAL driver supports spindle synchronization. */
typedef struct {
    float rpm;
    float rpm_low_limit;
    float rpm_high_limit;
    float angular_position; //!< Number of revolutions since last reset
    float rpm_programmed;
    uint32_t index_count;
    uint32_t pulse_count;
    uint32_t error_count;
    spindle_state_t state_programmed;
} spindle_data_t;

typedef enum {
    SpindleData_Counters,       //!< 0
    SpindleData_RPM,            //!< 1
    SpindleData_AngularPosition //!< 2
} spindle_data_request_t;

typedef enum {
    SpindleType_PWM,        //!< 0
    SpindleType_Basic,      //!< 1 - on/off + optional direction
    SpindleType_VFD,        //!< 2
    SpindleType_Solenoid,   //!< 3
    SpindleType_Null,       //!< 4
} spindle_type_t;

typedef enum {
    SpindleHAL_Raw,         //!< 0 - NOTE: read-only
    SpindleHAL_Configured,  //!< 1
    SpindleHAL_Active,      //!< 2
} spindle_hal_t;

/*! \brief Pointer to function for setting the spindle state.
\param state a \a spindle_state_t union variable.
\param rpm spindle RPM.
*/
typedef void (*spindle_set_state_ptr)(spindle_state_t state, float rpm);

/*! \brief Pointer to function for getting the spindle state.
\returns state in a \a spindle_state_t union variable.
*/
typedef spindle_state_t (*spindle_get_state_ptr)(void);

/*! \brief Pointer to function for converting a RPM value to a PWM value.

Typically this is a wrapper for the spindle_compute_pwm_value() function provided by the core.

\param rpm spindle RPM.
\returns the corresponding PWM value.
*/
typedef uint_fast16_t (*spindle_get_pwm_ptr)(float rpm);

/*! \brief Pointer to function for updating spindle speed on the fly.
\param pwm new spindle PWM value.
\returns the actual PWM value used.

__NOTE:__ this function will be called from an interrupt context.
*/
typedef void (*spindle_update_pwm_ptr)(uint_fast16_t pwm);


/*! \brief Pointer to function for updating spindle RPM.
\param rpm spindle RPM.
*/
typedef void (*spindle_update_rpm_ptr)(float rpm);

/*! \brief Pointer to function for getting spindle data.
\param request request type as a \a #spindle_data_request_t enum.
\returns pointer to the requested information in a spindle_data_t structure.

__NOTE:__ this function requires input from a spindle encoder.
*/
typedef spindle_data_t *(*spindle_get_data_ptr)(spindle_data_request_t request);

/*! \brief Pointer to function for resetting spindle data. */
typedef void (*spindle_reset_data_ptr)(void);

/*! \brief Pointer to function for outputting a spindle on pulse.
Used for Pulses Per Inch (PPI) laser mode.
\param pulse_length spindle on length in microseconds.
*/
typedef void (*spindle_pulse_on_ptr)(uint_fast16_t pulse_length);

struct spindle_param; // members defined below

/*! \brief Handlers and data for spindle support. */
struct spindle_ptrs {
    spindle_id_t id;                    //!< Spindle id, assingned on spindle registration .
    struct spindle_param *param;        //!< Pointer to current spindle parameters, assigned when spindle is enabled.
    spindle_type_t type;                //!< Spindle type.
    spindle_cap_t cap;                  //!< Spindle capabilities.
    uint_fast16_t pwm_off_value;        //!< Value for switching PWM signal off.
    float rpm_min;                      //!< Minimum spindle RPM.
    float rpm_max;                      //!< Maximum spindle RPM.
    bool (*config)(struct spindle_ptrs *spindle); //!< Optional handler for configuring the spindle.
    spindle_set_state_ptr set_state;    //!< Handler for setting spindle state.
    spindle_get_state_ptr get_state;    //!< Handler for getting spindle state.
    spindle_get_pwm_ptr get_pwm;        //!< Handler for calculating spindle PWM value from RPM.
    spindle_update_pwm_ptr update_pwm;  //!< Handler for updating spindle PWM output.
    spindle_update_rpm_ptr update_rpm;  //!< Handler for updating spindle RPM.
#ifdef GRBL_ESP32
    void (*esp32_off)(void);            //!< Workaround handler for snowflake ESP32 Guru awaken by floating point data in ISR context.
#endif
    // Optional entry points.
    spindle_pulse_on_ptr pulse_on;      //!< Optional handler for Pulses Per Inch (PPI) mode. Required for the laser PPI plugin.
    spindle_get_data_ptr get_data;      //!< Optional handler for getting spindle data. Required for spindle sync, copied from hal.spindle_data.get on selection.
    spindle_reset_data_ptr reset_data;  //!< Optional handler for resetting spindle data. Required for spindle sync, copied from hal.spindle_data.reset on selection.
};

typedef struct spindle_ptrs spindle_ptrs_t;

//! \brief  Data used for Constant Surface Speed (CSS) mode calculations.
typedef struct {
    float surface_speed;    //!< Surface speed in millimeters/min
    float target_rpm;       //!< Target RPM at end of movement
    float delta_rpm;        //!< Delta between start and target RPM
    float max_rpm;          //!< Maximum spindle RPM
    float tool_offset;      //!< Tool offset
    uint_fast8_t axis;      //!< Linear (tool) axis
} spindle_css_data_t;

/*! \brief Structure used for holding the current state of an enabled spindle. */
typedef struct spindle_param {
    float rpm;
    float rpm_overridden;
    spindle_state_t state;
    override_t override_pct;    //!< Spindle RPM override value in percent
    spindle_css_data_t css;     //!< Data used for Constant Surface Speed Mode (CSS) calculations, NULL if not in CSS mode.
    spindle_ptrs_t *hal;
} spindle_param_t;

typedef struct {
    spindle_get_data_ptr get;      //!< Optional handler for getting spindle data. Required for spindle sync.
    spindle_reset_data_ptr reset;  //!< Optional handler for resetting spindle data. Required for spindle sync.
} spindle_data_ptrs_t;

/*! \brief Pointer to function for configuring the spindle.
\returns state in a \a spindle_state_t union variable.
*/
typedef bool (*spindle_config_ptr)(spindle_ptrs_t *spindle);

/*! \brief Structure holding data passed to the callback function called by spindle_enumerate_spindles(). */
typedef struct  {
    spindle_id_t id;
    spindle_num_t num;
    const char *name;
    bool enabled;
    bool is_current;
    const spindle_ptrs_t *hal;
} spindle_info_t;

typedef struct {
    float rpm;
    float start;
    float end;
} pwm_piece_t;

//!* \brief Precalculated values that may be set/used by HAL driver to speed up RPM to PWM conversions if variable spindle is supported. */
typedef struct {
    uint_fast16_t period;
    uint_fast16_t off_value;    //!< NOTE: this value holds the inverted version if software PWM inversion is enabled by the driver.
    uint_fast16_t min_value;
    uint_fast16_t max_value;
    float rpm_min;              //!< Minimum spindle RPM.
    float pwm_gradient;
    bool invert_pwm;            //!< NOTE: set (by driver) when inversion is done in code
    bool always_on;
    int_fast16_t offset;
    uint_fast16_t n_pieces;
    pwm_piece_t piece[SPINDLE_NPWM_PIECES];
} spindle_pwm_t;

/*! \brief Pointer to callback function called by spindle_enumerate_spindles().
\param spindle prointer to a \a spindle_info_t struct.
*/
typedef void (*spindle_enumerate_callback_ptr)(spindle_info_t *spindle);

void spindle_set_override (spindle_ptrs_t *spindle, override_t speed_override);

// Called by g-code parser when setting spindle state and requires a buffer sync.
bool spindle_sync (spindle_ptrs_t *spindle, spindle_state_t state, float rpm);

// Sets spindle running state with direction, enable, and spindle RPM.
bool spindle_set_state (spindle_ptrs_t *spindle, spindle_state_t state, float rpm);

// Spindle speed calculation and limit handling
float spindle_set_rpm (spindle_ptrs_t *spindle, float rpm, override_t speed_override);

// Restore spindle running state with direction, enable, spindle RPM and appropriate delay.
bool spindle_restore (spindle_ptrs_t *spindle, spindle_state_t state, float rpm);

void spindle_all_off (void);

//
// The following functions are not called by the core, may be called by driver code.
//

bool spindle_precompute_pwm_values (spindle_ptrs_t *spindle, spindle_pwm_t *pwm_data, uint32_t clock_hz);

uint_fast16_t spindle_compute_pwm_value (spindle_pwm_t *pwm_data, float rpm, bool pid_limit);

spindle_id_t spindle_register (const spindle_ptrs_t *spindle, const char *name);

spindle_id_t spindle_add_null (void);

uint8_t spindle_get_count (void);

bool spindle_select (spindle_id_t spindle_id);

spindle_cap_t spindle_get_caps (void);

void spindle_update_caps (spindle_ptrs_t *spindle, spindle_pwm_t *pwm_caps);


spindle_ptrs_t *spindle_get_hal (spindle_id_t spindle_id, spindle_hal_t hal);

const char *spindle_get_name (spindle_id_t spindle_id);

spindle_id_t spindle_get_default (void);

spindle_num_t spindle_enable (spindle_id_t spindle_id);

bool spindle_enumerate_spindles (spindle_enumerate_callback_ptr callback);

//

bool spindle_is_enabled (spindle_num_t spindle_num);

bool spindle_is_on (void);

spindle_ptrs_t *spindle_get (spindle_num_t spindle_num);

#endif
