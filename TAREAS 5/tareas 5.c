#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define LED_PIN     2
#define BUTTON_PIN  0  // Cambia esto al pin de tu botón si es otro

static volatile int64_t press_time = 0;
static volatile int64_t duration_ms = 0;
static TimerHandle_t blink_timer = NULL;
static TimerHandle_t stop_timer = NULL;
static bool led_on = false;

void IRAM_ATTR button_isr_handler(void *arg) {
    static bool pressed = false;

    if (!pressed) {
        press_time = esp_timer_get_time(); // tiempo actual en microsegundos
        gpio_set_level(LED_PIN, 1);        // enciende el LED
        pressed = true;
    } else {
        int64_t release_time = esp_timer_get_time();
        duration_ms = (release_time - press_time) / 1000; // convierte a milisegundos
        gpio_set_level(LED_PIN, 0);        // apaga el LED
        pressed = false;

        // Inicia el parpadeo y configura el tiempo
        xTimerStartFromISR(blink_timer, NULL);
        xTimerChangePeriodFromISR(stop_timer, pdMS_TO_TICKS(duration_ms), NULL);
        xTimerStartFromISR(stop_timer, NULL);
    }
}

void blink_timer_callback(TimerHandle_t xTimer) {
    led_on = !led_on;
    gpio_set_level(LED_PIN, led_on);
}

void stop_blinking_callback(TimerHandle_t xTimer) {
    xTimerStop(blink_timer, 0);       // Detiene el parpadeo
    gpio_set_level(LED_PIN, 0);       // Asegura que el LED quede apagado
    led_on = false;
}

void app_main(void) {
    // Configura el LED
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);

    // Configura el botón
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY); // Asumiendo botón hacia GND
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_ANYEDGE);

    // Inicializa ISR
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

    // Crea los timers
    blink_timer = xTimerCreate("BlinkTimer", pdMS_TO_TICKS(150), pdTRUE, NULL, blink_timer_callback);
    stop_timer  = xTimerCreate("StopTimer", pdMS_TO_TICKS(1000), pdFALSE, NULL, stop_blinking_callback);
}
