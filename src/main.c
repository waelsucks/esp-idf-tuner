#include <driver/gpio.h>
#include <esp_task_wdt.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "Pins.h"
#include "Sampler.h"
#include "Notes.h"

static char * TAG = "DEBUG";

void IRAM_ATTR gpio_isr_handler();

static TaskHandle_t xSample;
static TaskHandle_t xButton;
static TaskHandle_t xLED;
static TaskHandle_t xTune;

void sample_task();
void button_task();
void led_task();
void tune_task();

static float processed_frequency;

static int sample_frequency = 6000;
static double sample_period = 0.5;

static int tune_target = 0;

int app_main(int argc, char const *argv[])
{

    ESP_LOGI(TAG, "Program Started. Initiating pins and adc");

    init_adc();
    init_pins();

    ESP_LOGI(TAG, "Setting up ISR. Listening for events on GPIO: %d", BUTTON_PIN);

    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(0);

    gpio_isr_handler_add(BUTTON_PIN, gpio_isr_handler, NULL);

    ESP_LOGI(TAG, "Creating tasks");

    xTaskCreate(sample_task , "SAMPLE_TASK" , 4096, NULL, tskIDLE_PRIORITY, &xSample);
    xTaskCreate(button_task , "BUTTON_TASK" , 4096, NULL, tskIDLE_PRIORITY, &xButton);
    xTaskCreate(led_task    , "LED_TASK"    , 4096, NULL, tskIDLE_PRIORITY, &xLED);
    xTaskCreate(tune_task   , "TUNE_TASK"   , 4096, NULL, tskIDLE_PRIORITY, &xTune);

    // Bye Main

    vTaskSuspend(NULL);

    return 0;
}

void button_task() {

    int state = -1;
    int debounce_timer = esp_timer_get_time();

    while (1)
    {

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (esp_timer_get_time() - debounce_timer < 0.5 * 1000000)
            continue;

        switch (state)
        {
            case -1:

                vTaskSuspend(xLED);
                vTaskResume(xTune);

                state ++;

                break;

            case 5:

                vTaskSuspend(xTune);
                vTaskResume(xLED);

                state = -1;

                break;

            default:
                state ++;
                break;
        }

        tune_target = state;

        // Update for debounce

        debounce_timer = esp_timer_get_time();

    }

}

void tune_task() {

    vTaskSuspend(NULL);

    char standard_tuning[] = "EADGBE";

    while (1)
    {

        char note[2];
        freq_to_note(processed_frequency, note);
        int diff = diff_to_note(standard_tuning[tune_target], *note);

        ESP_LOGI(TAG, "Diff to note: %d", diff);

        if (diff < 0) {
            gpio_set_level(LED_PIN_1, 1);
            gpio_set_level(LED_PIN_2, 0); 
        }

        if (diff > 0) {
            gpio_set_level(LED_PIN_1, 0);
            gpio_set_level(LED_PIN_2, 1); 
        }

        if (diff == 0) {
            gpio_set_level(LED_PIN_1, 1);
            gpio_set_level(LED_PIN_2, 1); 
        }

        vTaskDelay(pdMS_TO_TICKS(0.1 * 1000));
    }

}

void led_task() {

    // Blinking LED's

    while (1)
    {
        gpio_set_level(LED_PIN_1, 1);
        gpio_set_level(LED_PIN_2, 1);

        vTaskDelay(pdMS_TO_TICKS(0.1 * 1000));

        gpio_set_level(LED_PIN_1, 0);
        gpio_set_level(LED_PIN_2, 0);

        vTaskDelay(pdMS_TO_TICKS(0.1 * 1000));

        gpio_set_level(LED_PIN_1, 1);
        gpio_set_level(LED_PIN_2, 1);

        vTaskDelay(pdMS_TO_TICKS(0.1 * 1000));

        gpio_set_level(LED_PIN_1, 0);
        gpio_set_level(LED_PIN_2, 0);

        vTaskDelay(pdMS_TO_TICKS(1 * 1000));

    }

}

void sample_task() {

    // Sample Infinity

    start_sampling(sample_frequency);

    while (1)
    {
        processed_frequency = process_frequency();

        reset_sampling();
        vTaskDelay(pdMS_TO_TICKS(sample_period * 1000));
    }
}

void IRAM_ATTR gpio_isr_handler() {
    xTaskNotifyGive(xButton);
}