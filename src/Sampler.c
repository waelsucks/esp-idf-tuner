#include <driver/adc.h>

#include "Sampler.h"

#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"

static const char * TAG = "SAMPLER";

static const adc_channel_t channel  = ADC_CHANNEL_6;
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;

static esp_timer_handle_t sampler_handle;

static float duration;
static float prev_duration;

static int sampling_frequency;

static int adc_reading  = 0;
static int crossings    = 0;

void init_adc() {

    ESP_LOGI(TAG, "Initiating sampler...");

    ESP_ERROR_CHECK(adc1_config_width(width));
    ESP_ERROR_CHECK(adc1_config_channel_atten(channel, width));

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

    int prev_reading = adc_reading;
    adc_reading = adc1_get_raw(channel) - ADC_OFFSET;

    if (adc_reading * prev_reading < 0)
        crossings ++;

    duration = esp_timer_get_time() - prev_duration;

}

void stop_sampling() {

    ESP_ERROR_CHECK(esp_timer_stop(sampler_handle));
    ESP_ERROR_CHECK(esp_timer_delete(sampler_handle));

    duration = esp_timer_get_time() - prev_duration;

}

float process_frequency() {

    return crossings / (2 * (duration / (1 * 1000000)));

}

void reset_sampling() {

    crossings = 0;
    prev_duration = esp_timer_get_time();

}