
#include "matrix_counter.h"



/* Definition of constants*/

const uint GPIO_BUTTON_A = 5;   
const uint GPIO_BUTTON_B = 6;   
const uint GPIO_RED_LED = 13;   

const double BLUE_INTENSITY = 0.1;
const double RED_INTENSITY = 0;
const double GREEN_INTENSITY = 0.1;

const uint numbers_mapping[10][MTX_NUM_ROWS][MTX_NUM_ROWS] = {
    //0
    {
        { 0, 1, 1, 1, 0 },
        { 0, 1, 0, 1, 0 },
        { 0, 1, 0, 1, 0 },
        { 0, 1, 0, 1, 0 },
        { 0, 1, 1, 1, 0 }
    },
    //1
    {
        { 0, 0, 1, 0, 0 },
        { 0, 1, 1, 0, 0 },
        { 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0 },
        { 0, 1, 1, 1, 0 },
    },
    //2
    {
        { 0, 1, 1, 1, 0 },
        { 0, 0, 0, 1, 0 },
        { 0, 1, 1, 1, 0 },
        { 0, 1, 0, 0, 0 },
        { 0, 1, 1, 1, 0 }
    },
    //3
    {
        { 0, 1, 1, 1, 0 },
        { 0, 0, 0, 1, 0 },
        { 0, 1, 1, 1, 0 },
        { 0, 0, 0, 1, 0 },
        { 0, 1, 1, 1, 0 }
    },
    //4
    {
        { 0, 1, 0, 1, 0 },
        { 0, 1, 0, 1, 0 },
        { 0, 1, 1, 1, 0 },
        { 0, 0, 0, 1, 0 },
        { 0, 0, 0, 1, 0 }
    },
    //5
    {
        { 0, 1, 1, 1, 0 },
        { 0, 1, 0, 0, 0 },
        { 0, 1, 1, 1, 0 },
        { 0, 0, 0, 1, 0 },
        { 0, 1, 1, 1, 0 }
    },
    //6
    {
        { 0, 1, 1, 1, 0 },
        { 0, 1, 0, 0, 0 },
        { 0, 1, 1, 1, 0 },
        { 0, 1, 0, 1, 0 },
        { 0, 1, 1, 1, 0 }
    },
    //7
    {
        { 0, 1, 1, 1, 0 },
        { 0, 0, 0, 1, 0 },
        { 0, 1, 1, 1, 0 },
        { 0, 0, 0, 1, 0 },
        { 0, 0, 0, 1, 0 }
    },
    //8
    {
        { 0, 1, 1, 1, 0 },
        { 0, 1, 0, 1, 0 },
        { 0, 1, 1, 1, 0 },
        { 0, 1, 0, 1, 0 },
        { 0, 1, 1, 1, 0 }
    },
    //9
    {
        { 0, 1, 1, 1, 0 },
        { 0, 1, 0, 1, 0 },
        { 0, 1, 1, 1, 0 },
        { 0, 0, 0, 1, 0 },
        { 0, 1, 1, 1, 0 }
    }
};



/* Global variables */
static volatile uint8_t mtx_counter = 0; // Number shown in matrix

static volatile absolute_time_t button_a_last_pressed_time;     //Stores the last time button A was pressed (debounce)
static volatile absolute_time_t button_b_last_pressed_time;     //Stores the last time button A was pressed (debounce)

PIO pio;
uint state_machine;


/* Internal function prototypes */
static void button_press_handler(uint gpio, uint32_t events);


/*
*   @brief Callback funcion for the IRQ events
*/
void button_press_handler(uint gpio, uint32_t events) {

    if(gpio == GPIO_BUTTON_A && absolute_time_diff_us(button_a_last_pressed_time, get_absolute_time()) > DEBOUNCE_TIME && mtx_counter < 9){
        /* Routine for button A*/
        mtx_counter++;
        mtx_display_number();
        
        
    }else if (gpio == GPIO_BUTTON_B &&  absolute_time_diff_us(button_a_last_pressed_time, get_absolute_time()) > DEBOUNCE_TIME && mtx_counter > 0){
        /* Routine for button B*/
        mtx_counter--;
        mtx_display_number();
    }
}

void mtx_display_number(){
    uint32_t led_value;
    uint pixel_status;

    for (size_t i = 0; i < MTX_NUM_ROWS; i++){
        for (size_t j = 0; j < MTX_NUM_ROWS; j++){

            pixel_status = (i & 1) ? numbers_mapping[mtx_counter][4-i][j] : numbers_mapping[mtx_counter][4-i][4-j];
            
            led_value = matrix_rgb(pixel_status*BLUE_INTENSITY, pixel_status*RED_INTENSITY, pixel_status*GREEN_INTENSITY);
            pio_sm_put_blocking(pio, state_machine, led_value);
            
        }
        
    }
    

}

int main() {
    /* Global configuration */
    pio = pio0;

    button_a_last_pressed_time = get_absolute_time();
    button_b_last_pressed_time = get_absolute_time();


    /* Clock configuration */
    if(set_sys_clock_khz(128000, false)){
        printf("Configuration of system clock completed!\n");
    }else{
        printf("Configuration of system clock failed\n");
        exit(-1);
    }


    /* Initialize pins */
    stdio_init_all();

    gpio_init(GPIO_RED_LED);
    gpio_set_dir(GPIO_RED_LED, GPIO_OUT);

    gpio_init(GPIO_BUTTON_A);
    gpio_set_dir(GPIO_BUTTON_A, GPIO_IN);
    gpio_pull_up(GPIO_BUTTON_A);


    gpio_init(GPIO_BUTTON_B);
    gpio_set_dir(GPIO_BUTTON_B, GPIO_IN);
    gpio_pull_up(GPIO_BUTTON_B);
    

    /* PIO configuration*/

    uint offset = pio_add_program(pio, &matrix_counter_program);
    uint state_machine = pio_claim_unused_sm(pio, true);
    matrix_counter_program_init(pio, state_machine, offset, GPIO_LED_MTX);
    
    
    /* IRQ configuration */
    gpio_set_irq_enabled_with_callback(GPIO_BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_press_handler);
    gpio_set_irq_enabled(GPIO_BUTTON_B, GPIO_IRQ_EDGE_FALL, true);
    

    /* WS2812 initial configuration*/
    matrix_turn_all(pio, state_machine, 0, 0, 0);
    mtx_display_number();

    
    while (true) {
        /* Blink red led */
        gpio_put(GPIO_RED_LED, true);
        sleep_ms(100);
        gpio_put(GPIO_RED_LED, false);
        sleep_ms(100); 
    }
}
