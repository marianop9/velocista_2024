#include "state_machine.h"

#include "driver/gpio.h"
#include "esp_timer.h"

enum SpeedsterState {
    STATE_STOPPED,
    STATE_RACING
};

enum SpeedsterState state = STATE_STOPPED;

int led_pin = 0;

int64_t btn_time = 0;
int64_t last_btn_time = 0;

void toggle_state() {
    if (state == STATE_STOPPED) {
        state = STATE_RACING;
        gpio_set_level(led_pin, 0);
    } else {
        state = STATE_STOPPED;
        gpio_set_level(led_pin, 1);
    }
}

void isr_handler() {
    btn_time = esp_timer_get_time();
    if (btn_time - last_btn_time > 250) {
        last_btn_time = btn_time;

        toggle_state();
    }
}

int state_machine_is_stopped() {
    return state == STATE_STOPPED;
}

void state_machine_init(int btnPin, int ledPin) {
    gpio_reset_pin(btnPin);
    gpio_reset_pin(ledPin);
    
    // gpio_config_t io_conf = {
    //     .pin_bit_mask = 1ULL << btnPin,
    //     .mode = GPIO_MODE_INPUT,
    //     .pull_up_en = true,
    //     .pull_down_en = false,
    //     .intr_type = GPIO_INTR_NEGEDGE,     /*!< GPIO interrupt type : falling edge */
    // };

    gpio_set_direction(btnPin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(btnPin, GPIO_PULLDOWN_ONLY);
    gpio_intr_enable(btnPin);
    gpio_set_intr_type(btnPin, GPIO_INTR_NEGEDGE);

    gpio_install_isr_service(0);

    gpio_isr_handler_add(btnPin, isr_handler, NULL);

    gpio_set_direction(ledPin, GPIO_MODE_OUTPUT);
    gpio_set_level(ledPin, 1);
    led_pin = ledPin;
}
