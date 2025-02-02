#ifndef MATRIX_COUNTER_H
#define MATRIX_COUNTER_H

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"

#include "hardware_management/led_matrix.h"


#include "matrix_counter.pio.h"

#define DEBOUNCE_TIME 200000    //Debounce delay in us

extern const double RED_INTENSITY;      //Intensity of the red RGB
extern const double BLUE_INTENSITY;     //Intensity of the blue RGB
extern const double GREEN_INTENSITY;    //Intensity of the green RGB

extern const uint GPIO_BUTTON_A;    // Button A pin
extern const uint GPIO_BUTTON_B;    // Button B pin
extern const uint GPIO_RED_LED;     // RGB red LED pin
extern const uint numbers_mapping[10][MTX_NUM_ROWS][MTX_NUM_ROWS];  //Numbers frames


/*
*   @brief When called, this function displays the digit on the WS2812 corresponding 
*   to the counter value mtx_counter
*/
void mtx_display_number();


#endif