#include <driver/gpio.h>
#include "esp_task_wdt.h"

#include "Pins.h"
#include "Sampler.h"

void app_main() {

    init_adc();
    start_sampling(40000);

}
