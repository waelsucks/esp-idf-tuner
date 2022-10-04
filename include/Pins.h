#include <driver/gpio.h>

#if !defined(PINS_H_)
#define PINS_H_

#define LED_PIN_1 GPIO_NUM_12
#define LED_PIN_2 GPIO_NUM_14

#define BUTTON_PIN GPIO_NUM_18

#define MIC_OUT GPIO_NUM_34

/**
 * Initiates the pins.
 * */
void init_pins();

#endif // PINS_H_
