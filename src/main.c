#include <driver/gpio.h>
#include "esp_task_wdt.h"

#include "Pins.h"
#include "Sampler.h"

#include "TimerTest.h"

void app_main() {

    init_adc();
    start_sampling(10000);

    while (1)
    {
        printf("Frequency: %.2f\n", process_frequency());
        
        reset_sampling();
        vTaskDelay(pdMS_TO_TICKS(0.5 * 1000));

    }
    
}
