#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/scb.h>

#define LED_PORT (GPIOA)  // port is a collection of pins (16 in this case)
#define LED_PIN (GPIO5)   // pin is the connection to the LED

static void rcc_setup(void) {
  rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
}

static void gpio_setup(void) {
  rcc_periph_clock_enable(RCC_GPIOA); // turn on the port (off by default)
  gpio_mode_setup(
    LED_PORT,
    GPIO_MODE_OUTPUT,
    GPIO_PUPD_NONE,
    LED_PIN
  );
}

static void delay_cycles(uint32_t cycles) {
  for (uint32_t i = 0; i < cycles; i++) {
    __asm__ ("nop");
  }
}

static void delay_ms(uint32_t ms) {
  // Calibrated for 84MHz with -Os optimization
  while (ms--) {
    for (volatile uint32_t i = 0; i < 10500; i++) {
      __asm__ volatile ("nop");
    }
  }
}


int main(void) {
  rcc_setup();
  gpio_setup();
  
  while (1) {
    gpio_toggle(LED_PORT, LED_PIN);
    delay_cycles(84000000 / 4);
  }

  return 0;
}
