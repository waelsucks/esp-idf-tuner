#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "sdkconfig.h"

#include "TimerTest.h"

static const char * TAG = "TIMER_TEST";

static int count = 0;

static void timer_1_callback();
static void timer_2_callback();

static esp_timer_handle_t timer_1_handler;
static esp_timer_handle_t timer_2_handler;

void start_timers() {

    ESP_LOGI(TAG, "Initiating timers...");

    const esp_timer_create_args_t timer_1_args = {
            .callback = &timer_1_callback,
            .name = "timer_1"
    };

    const esp_timer_create_args_t timer_2_args = {
            .callback   = &timer_2_callback,
            .arg        = (void *) timer_1_handler,
            .name       = "timer_2"
    };
    
    ESP_ERROR_CHECK(esp_timer_create(&timer_1_args, &timer_1_handler));
    ESP_ERROR_CHECK(esp_timer_create(&timer_2_args, &timer_2_handler));

    ESP_LOGI(TAG, "Starting timers!");

    ESP_ERROR_CHECK(esp_timer_start_periodic(timer_1_handler, 5 * 1000000));
    ESP_ERROR_CHECK(esp_timer_start_once(timer_2_handler, 3 * 1000000));

    for (int i = 0; i < 5; ++i) {
        ESP_ERROR_CHECK(esp_timer_dump(stdout));
        usleep(2 * 1000000);
    }

}

static void timer_1_callback(void * arg) {

    ESP_LOGI(TAG, "%d...", count);

    count ++;

}

static void timer_2_callback(void * arg) {

    ESP_LOGI(TAG, "Timer TWO called! Stopping timer 1.");

    esp_timer_handle_t h = (esp_timer_handle_t) arg;


    printf("\n\n");

    ESP_LOGI(TAG, "ADRESS: %p", &h);
    ESP_LOGI(TAG, "ADRESS_OG: %p", &timer_1_handler);

    ESP_ERROR_CHECK(esp_timer_stop(timer_1_handler));

}