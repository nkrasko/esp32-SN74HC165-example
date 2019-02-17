/* Texas Instruments SN74HC165 8-bit Shift Register DIP-switch Input Example

   Datasheet: http://www.ti.com/lit/ds/symlink/sn74hc165.pdf

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


#define GPIO_OUTPUT_IO_25 25 // SH/LD PIN
#define GPIO_OUTPUT_IO_26 26 // CLK PIN
#define GPIO_INPUT_IO_27  27 // QH PIN

#define PIN_SHLD GPIO_OUTPUT_IO_25
#define PIN_CLK GPIO_OUTPUT_IO_26
#define PIN_QH GPIO_INPUT_IO_27

#define GPIO_OUTPUT_PIN_SEL ((1<<PIN_SHLD) | (1<<PIN_CLK))
#define GPIO_INPUT_PIN_SEL  ((1<<PIN_QH))

void app_main()
{
  gpio_config_t io_conf;

  io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
  io_conf.pull_down_en = 0;
  io_conf.pull_up_en = 0;
  gpio_config(&io_conf);

  io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
  io_conf.mode = GPIO_MODE_INPUT;
  gpio_config(&io_conf);

  gpio_set_level(PIN_SHLD,1);
  gpio_set_level(PIN_CLK,0);

  int i;
  int value;


  while (1)
  {

  i = 0;
  value =0;

  //SHLD must be low to load from Parallel Inputs (A-H)
  gpio_set_level(PIN_SHLD,0);

  //SHLD must be high to realize the shift function
  gpio_set_level(PIN_SHLD,1);

  printf("Switch Status:\n");

  //Reading from QH
  for(i=0; i<8; i++)
  {
      if (gpio_get_level(PIN_QH)) {
          value = value | (1<<i);
          printf ("%d:On\n",abs(i-8));
      }

      //CLK INH PIN must be low. Toggle CLK to shift data into QH
      gpio_set_level(PIN_CLK,1);
      gpio_set_level(PIN_CLK,0);
  }

  printf("Value in decimal %d\n", value);
  vTaskDelay(1000 / portTICK_RATE_MS);
  }
}
