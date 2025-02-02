#ifndef LED_MATRIZ_H
#define LED_MATRIX_H


#include "pico/stdlib.h"
#include "hardware/pio.h"

#define MTX_NUM_LEDS 25     //Total number of leds 
#define MTX_NUM_ROWS 5      //Number of rows

extern const uint GPIO_LED_MTX; //Pin for WS2812


/*
*   @brief Function for defining the LED value for WS2812
*   @param  b - intensity for blue led
*   @param  r - intensity for red led
*   @param  g - intensity for green led
*/
uint32_t matrix_rgb(double b, double r, double g);


/*
*   @brief Turns all 25 leds of the WS2812 to the corresponding RGB intensities
*   @param  pio - pio used for the WS2812
*   @param  state_machine - state machine configured for the WS2812
*   @param  red - intensity for the red led
*   @param  blue - intensity for the blue led
*   @param  green - intensity for the green led
*/
void matrix_turn_all(PIO pio, uint state_machine, double red, double green, double blue);

#endif