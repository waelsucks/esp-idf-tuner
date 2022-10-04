#include <driver/adc.h>
#include <esp_task_wdt.h>
#include <soc/adc_channel.h>
#include <esp_adc_cal.h>

#include "Sampler.h"


static const adc_channel_t channel  = ADC_CHANNEL_6;
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;

static esp_timer_handle_t sample_timer;

void init_adc() {

    adc1_config_width(width);
    adc1_config_channel_atten(channel, ADC_ATTEN_DB_11);

    const esp_timer_create_args_t sample_timer_args = {
            .callback = &sample,
            .name = "sampler"
    };
    
    esp_timer_create(&sample_timer_args, &sample_timer);

}

void start_sampling(int frequency) {

    printf("Staring!\n");

    uint64_t t = 10000000 / frequency;

    esp_timer_start_periodic(sample_timer, t);

}

void sample() {

    int adc_reading = adc1_get_raw(channel);

    printf(">ADC_READING:%d\n", adc_reading);

}

void stop_sampling() {

    esp_timer_stop(sample_timer);

}

// MULTISAMPLING?

        // for (int i = 0; i < NO_OF_SAMPLES; i++) {
        //     adc_reading += adc1_get_raw((adc1_channel_t)channel);
        // }

        // adc_reading /= NO_OF_SAMPLES;