#include "Common.h"
#ifndef LINUX
#include "hardware/adc.h"
#include "PicoPrimitivesADC.h"
#include "StackPrimitives.h"
//
// ADC functions
//
void    pico_temp_on         (void){ adc_set_temp_sensor_enabled(true); }
void    pico_temp_off        (void){ adc_set_temp_sensor_enabled(false); }
void    pico_adc_init        (void){ adc_init(); }
void    pico_adc_gpio_init   (void){ adc_gpio_init (PopP); }
void    pico_adc_select_input(void){ adc_select_input (PopP);}
void    pico_adc_read        (void){ PushP = (uint32_t)adc_read(); }
#endif