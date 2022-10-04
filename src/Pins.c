#include <driver/adc.h>
#include "Pins.h"

void init_pins() {

    gpio_reset_pin(LED_PIN_1);
    gpio_reset_pin(LED_PIN_2);
    gpio_reset_pin(BUTTON_PIN);
    gpio_reset_pin(MIC_OUT);

    gpio_set_direction(LED_PIN_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_PIN_2, GPIO_MODE_OUTPUT);
    // gpio_set_direction(MIC_OUT, GPIO_MODE_INPUT);

    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_PIN);

}