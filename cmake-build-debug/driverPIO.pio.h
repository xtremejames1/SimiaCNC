// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ------------- //
// stepper_timer //
// ------------- //

#define stepper_timer_wrap_target 0
#define stepper_timer_wrap 4

static const uint16_t stepper_timer_program_instructions[] = {
            //     .wrap_target
    0x8080, //  0: pull   noblock                    
    0xa027, //  1: mov    x, osr                     
    0xa041, //  2: mov    y, x                       
    0x0083, //  3: jmp    y--, 3                     
    0xc000, //  4: irq    nowait 0                   
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program stepper_timer_program = {
    .instructions = stepper_timer_program_instructions,
    .length = 5,
    .origin = -1,
};

static inline pio_sm_config stepper_timer_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + stepper_timer_wrap_target, offset + stepper_timer_wrap);
    return c;
}

static inline void stepper_timer_program_init(PIO pio, uint32_t sm, uint32_t offset, float div) {
    pio_sm_config c = stepper_timer_program_get_default_config(offset);
    // Load our configuration, and jump to the start of the program
    pio_sm_init(pio, sm, offset, &c);
    // Set the clock divider
    pio_sm_set_clkdiv(pio, sm, div);
}
static inline void stepper_timer_set_period(PIO pio, uint32_t sm, uint32_t offset, uint32_t period) {
    static uint32_t period_prev = 0;
    if(period_prev != period) {
        period_prev = period;
        pio_sm_put(pio, sm, period);
        pio_sm_exec(pio, sm, pio_encode_jmp(offset));
    }
    if(!(pio->ctrl & (1 << sm))) {
        pio_sm_set_enabled(pio, sm, true);
        pio->inte0 |= PIO_INTR_SM0_BITS;
    }
 }
static inline void stepper_timer_stop(PIO pio, uint32_t sm) {
    pio_sm_set_enabled(pio, sm, false);
    pio->inte0 &= ~PIO_INTR_SM0_BITS;
}
static inline void stepper_timer_irq_clear(PIO pio) {
    pio->irq = 1;
}

#endif

// ---------- //
// step_pulse //
// ---------- //

#define step_pulse_wrap_target 0
#define step_pulse_wrap 6

static const uint16_t step_pulse_program_instructions[] = {
            //     .wrap_target
    0x80a0, //  0: pull   block                      
    0x6028, //  1: out    x, 8                       
    0x0042, //  2: jmp    x--, 2                     
    0x6028, //  3: out    x, 8                       
    0x6006, //  4: out    pins, 6                    
    0x0045, //  5: jmp    x--, 5                     
    0x6006, //  6: out    pins, 6                    
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program step_pulse_program = {
    .instructions = step_pulse_program_instructions,
    .length = 7,
    .origin = -1,
};

static inline pio_sm_config step_pulse_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + step_pulse_wrap_target, offset + step_pulse_wrap);
    return c;
}

static inline void step_pulse_program_init(PIO pio, uint32_t sm, uint32_t offset, uint32_t startPin, uint32_t pinCount) {
    pio_sm_config c = step_pulse_program_get_default_config(offset); 
    // Map the state machine's OUT pin group to the provided pin in pin count in parameters
    sm_config_set_out_pins(&c, startPin, pinCount);
    // Set these pins GPIO function (connect PIO to the pad)
    for(uint32_t i=0;i<pinCount;i++)
        pio_gpio_init(pio, startPin+i);
    // Set the pin direction to output at the PIO
    pio_sm_set_consecutive_pindirs(pio, sm, startPin, pinCount, true);
    // Set the set pins group to the same pins as the out pins group to reset the pins when the steps are finished
    sm_config_set_set_pins(&c, startPin, pinCount);
    // Load our configuration, and jump to the start of the program
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_clkdiv(pio, sm, 12.5f);
    // Set the state machine running
    pio_sm_set_enabled(pio, sm, true);
}
static inline void step_pulse_generate(PIO pio, uint32_t sm, uint32_t stepPulse) {
    pio_sm_put(pio, sm, stepPulse);
}

#endif

// ------------ //
// step_dir_sr4 //
// ------------ //

#define step_dir_sr4_wrap_target 0
#define step_dir_sr4_wrap 19

#define step_dir_sr4_T3 5

static const uint16_t step_dir_sr4_program_instructions[] = {
            //     .wrap_target
    0x80a0, //  0: pull   block           side 0     
    0x6048, //  1: out    y, 8            side 0     
    0xa0c2, //  2: mov    isr, y          side 0     
    0xa027, //  3: mov    x, osr          side 0     
    0xe047, //  4: set    y, 7            side 0     
    0x6501, //  5: out    pins, 1         side 0 [5] 
    0x0d85, //  6: jmp    y--, 5          side 1 [5] 
    0xd504, //  7: irq    nowait 4        side 2 [5] 
    0xe047, //  8: set    y, 7            side 0     
    0xa0e6, //  9: mov    osr, isr        side 0     
    0x20c5, // 10: wait   1 irq, 5        side 0     
    0x6501, // 11: out    pins, 1         side 0 [5] 
    0x0d8b, // 12: jmp    y--, 11         side 1 [5] 
    0xd506, // 13: irq    nowait 6        side 2 [5] 
    0xe047, // 14: set    y, 7            side 0     
    0xa0e1, // 15: mov    osr, x          side 0     
    0x20c7, // 16: wait   1 irq, 7        side 0     
    0x6501, // 17: out    pins, 1         side 0 [5] 
    0x0d91, // 18: jmp    y--, 17         side 1 [5] 
    0xf540, // 19: set    y, 0            side 2 [5] 
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program step_dir_sr4_program = {
    .instructions = step_dir_sr4_program_instructions,
    .length = 20,
    .origin = -1,
};

static inline pio_sm_config step_dir_sr4_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + step_dir_sr4_wrap_target, offset + step_dir_sr4_wrap);
    sm_config_set_sideset(&c, 2, false, false);
    return c;
}

#include "hardware/gpio.h"
static inline void step_dir_sr4_program_init(PIO pio, uint32_t sm, uint32_t offset, uint32_t dataPin, uint32_t bckPin) {
    pio_sm_config c = step_dir_sr4_program_get_default_config(offset);
    sm_config_set_out_pins(&c, dataPin, 1);
    sm_config_set_sideset_pins(&c, bckPin);
    sm_config_set_clkdiv(&c, 1);
    pio_sm_set_pins_with_mask(pio, sm, (3u << bckPin) | (1u << dataPin), (3u << bckPin) | (1u << dataPin));
    pio_sm_set_pindirs_with_mask(pio, sm, (3u << bckPin) | (1u << dataPin), (3u << bckPin) | (1u << dataPin));
    pio_gpio_init(pio, dataPin);
    pio_gpio_init(pio, bckPin);
    pio_gpio_init(pio, bckPin + 1);
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}
static inline void step_dir_sr4_write(PIO pio, uint32_t sm, uint32_t data) {
    pio_sm_put(pio, sm, data);
}

#endif

// -------- //
// sr_delay //
// -------- //

#define sr_delay_wrap_target 0
#define sr_delay_wrap 5

static const uint16_t sr_delay_program_instructions[] = {
            //     .wrap_target
    0x8080, //  0: pull   noblock                    
    0xa027, //  1: mov    x, osr                     
    0xa041, //  2: mov    y, x                       
    0x20c4, //  3: wait   1 irq, 4                   
    0x0084, //  4: jmp    y--, 4                     
    0xc005, //  5: irq    nowait 5                   
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program sr_delay_program = {
    .instructions = sr_delay_program_instructions,
    .length = 6,
    .origin = -1,
};

static inline pio_sm_config sr_delay_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + sr_delay_wrap_target, offset + sr_delay_wrap);
    return c;
}

static inline void sr_delay_program_init(PIO pio, uint32_t sm, uint32_t offset, float div) {
    pio_sm_config c = sr_delay_program_get_default_config(offset);
    // Load our configuration, and jump to the start of the program
    pio_sm_init(pio, sm, offset, &c);
    // Set the clock divider
    pio_sm_set_clkdiv(pio, sm, div);
    pio_sm_set_enabled(pio, sm, true);
}
static inline void sr_delay_set(PIO pio, uint32_t sm, uint32_t period) {
    pio_sm_put(pio, sm, period);
 }

#endif

// ------- //
// sr_hold //
// ------- //

#define sr_hold_wrap_target 0
#define sr_hold_wrap 5

static const uint16_t sr_hold_program_instructions[] = {
            //     .wrap_target
    0x8080, //  0: pull   noblock                    
    0xa027, //  1: mov    x, osr                     
    0xa041, //  2: mov    y, x                       
    0x20c6, //  3: wait   1 irq, 6                   
    0x0084, //  4: jmp    y--, 4                     
    0xc007, //  5: irq    nowait 7                   
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program sr_hold_program = {
    .instructions = sr_hold_program_instructions,
    .length = 6,
    .origin = -1,
};

static inline pio_sm_config sr_hold_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + sr_hold_wrap_target, offset + sr_hold_wrap);
    return c;
}

static inline void sr_hold_program_init(PIO pio, uint32_t sm, uint32_t offset, float div) {
    pio_sm_config c = sr_hold_program_get_default_config(offset);
    // Load our configuration, and jump to the start of the program
    pio_sm_init(pio, sm, offset, &c);
    // Set the clock divider
    pio_sm_set_clkdiv(pio, sm, div);
    pio_sm_set_enabled(pio, sm, true);
}
static inline void sr_hold_set(PIO pio, uint32_t sm, uint32_t period) {
    pio_sm_put(pio, sm, period);
 }

#endif

// -------- //
// out_sr16 //
// -------- //

#define out_sr16_wrap_target 0
#define out_sr16_wrap 4

#define out_sr16_T3 5

static const uint16_t out_sr16_program_instructions[] = {
            //     .wrap_target
    0x80a0, //  0: pull   block           side 0     
    0xe04f, //  1: set    y, 15           side 0     
    0x6501, //  2: out    pins, 1         side 0 [5] 
    0x0d82, //  3: jmp    y--, 2          side 1 [5] 
    0xf540, //  4: set    y, 0            side 2 [5] 
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program out_sr16_program = {
    .instructions = out_sr16_program_instructions,
    .length = 5,
    .origin = -1,
};

static inline pio_sm_config out_sr16_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + out_sr16_wrap_target, offset + out_sr16_wrap);
    sm_config_set_sideset(&c, 2, false, false);
    return c;
}

#include "hardware/gpio.h"
static inline void out_sr16_program_init(PIO pio, uint32_t sm, uint32_t offset, uint32_t dataPin, uint32_t bckPin) {
    uint32_t mask = (3u << bckPin) | (1u << dataPin);
    pio_sm_config c = out_sr16_program_get_default_config(offset);
    sm_config_set_out_pins(&c, dataPin, 1);
    sm_config_set_sideset_pins(&c, bckPin);
    sm_config_set_clkdiv(&c, 1);
    pio_sm_set_pins_with_mask(pio, sm, mask, mask);
    pio_sm_set_pindirs_with_mask(pio, sm, mask, mask);
    pio_gpio_init(pio, dataPin);
    pio_gpio_init(pio, bckPin);
    pio_gpio_init(pio, bckPin + 1);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}
static inline void out_sr16_write(PIO pio, uint32_t sm, uint32_t data) {
    pio_sm_put(pio, sm, data);
}

#endif

