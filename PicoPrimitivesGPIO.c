#include "Common.h"
#ifdef PICO
#include "hardware/gpio.h"
#include "PICOprimitivesGPIO.h"
#include "StackPrimitives.h"
//
int32_t m, n;
#define GET_PARAM  m = PopP;
#define GET_PARAMS m = PopP; n = PopP;
//
void pico_gpio_get_dir(void)                     { GET_PARAM  PushP =  (uint32_t) gpio_get_dir(m); }
void pico_gpio_get_drive_strength(void)          { GET_PARAM  PushP =  (uint32_t) gpio_get_drive_strength(m); }
void pico_gpio_get_slew_rate(void)               { GET_PARAM  PushP =  (uint32_t) gpio_get_slew_rate(m); }
void pico_gpio_is_pulled_up (void)               { GET_PARAM  PushP =  (uint32_t) gpio_is_pulled_up(m); }
void pico_gpio_is_pulled_down(void)              { GET_PARAM  PushP =  (uint32_t) gpio_is_pulled_down(m); }
void pico_gpio_is_input_hysteresis_enabled(void) { GET_PARAM  PushP =  (uint32_t) gpio_is_input_hysteresis_enabled(m);}
void pico_gpio_get(void)                         { GET_PARAM  PushP =  (uint32_t) gpio_get(m); }
void pico_gpio_init(void)                        { GET_PARAM  gpio_init(m); }
void pico_gpio_set_dir(void)                     { GET_PARAMS gpio_set_dir(n, m); }
void pico_gpio_put(void)                         { GET_PARAMS gpio_put(n, m); }
void pico_gpio_set_drive_strength (void)         { GET_PARAMS gpio_set_drive_strength(n, m); }
void pico_gpio_set_slew_rate(void)               { GET_PARAMS gpio_set_slew_rate(n, m); }
void pico_gpio_disable_pulls(void)               { GET_PARAM  gpio_disable_pulls(m); }
void pico_gpio_pull_up(void)                     { GET_PARAM  gpio_pull_up(m); }
void pico_gpio_pull_down(void)                   { GET_PARAM  gpio_pull_down(m); }
void pico_gpio_set_input_hysteresis_enabled(void){ GET_PARAMS gpio_set_input_hysteresis_enabled(n, m); }
#endif