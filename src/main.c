#include <driver/gpio.h>
#include "esp_task_wdt.h"

#include "Pins.h"
#include "Sampler.h"
#include "Notes.h"

void app_main() {

    init_adc();
    start_sampling(10000);

    char note[2];

    while (1)
    {

        float freq = process_frequency();

        // printf("Frequency: %.2f\n", freq);

        freq_to_note(freq, note);

        printf("Note: %s\n", note);
        
        reset_sampling();
        vTaskDelay(pdMS_TO_TICKS(0.5 * 1000));

    }
    
}
