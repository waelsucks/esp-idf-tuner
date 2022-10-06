#include <driver/gpio.h>
#include "esp_task_wdt.h"

#include "Pins.h"
#include "Sampler.h"

#include "TimerTest.h"

void app_main() {

    init_adc();
    start_sampling(10000);

    vTaskDelay(pdMS_TO_TICKS(2 * 1000));

    
    stop_sampling();

}
