#include "Sampler.h"

#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"

static const char * TAG = "SAMPLER";

static esp_timer_handle_t sampler_handle;

// TEMP

static int x = 0;

// --

static int duration;
static int sampling_frequency;

void init_adc() {

    ESP_LOGI(TAG, "Initiating sampler...");

    const esp_timer_create_args_t sampler_args = {

        .callback   = &sample,
        .name       = "SAMPLER"

    };

    ESP_ERROR_CHECK(esp_timer_create(&sampler_args, &sampler_handle));

}

void start_sampling(int frequency) {

    sampling_frequency = frequency;

    float period = (1 * 1000000) / sampling_frequency;
    ESP_ERROR_CHECK(esp_timer_start_periodic(sampler_handle, period));

}

void sample() {

    x ++;

    if (x > 500)
        x *= 0;

    duration = esp_timer_get_time() - duration;

}

void stop_sampling() {

    ESP_ERROR_CHECK(esp_timer_stop(sampler_handle));
    ESP_ERROR_CHECK(esp_timer_delete(sampler_handle));

}

void reset_sampling() {

    ESP_ERROR_CHECK(esp_timer_stop(sampler_handle));

    duration = 0;

    // ESP_ERROR_CHECK(esp_timer_start_periodic(sampler_handle, sampling_frequency));

}

int get_x() {
    return x;
}

float get_duration() {
    return duration;
}