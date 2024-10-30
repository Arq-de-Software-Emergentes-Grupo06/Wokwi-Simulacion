#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pin_t analog_pin;
  pin_t alert_pin;
  uint32_t gasValue;
  uint32_t thresholdValue;
} chip_data_t;

void chip_timer_callback(void *data) {
  chip_data_t *chip_data = (chip_data_t*)data;

  // Leer valores de gas y umbral
  int gasValue = attr_read(chip_data->gasValue); 
  int thresholdValue = attr_read(chip_data->thresholdValue); 

  // Convertir gasValue a un rango de 0 a 4095 para el pin DAC
  int dacValue = (int)((gasValue / 100.0) * 4095);
  pin_dac_write(chip_data->analog_pin, dacValue);

  // Lógica de alerta
  if (gasValue >= thresholdValue) {
    pin_write(chip_data->alert_pin, true);  
  } else {
    pin_write(chip_data->alert_pin, false); 
  }
}

void chip_init() {
  printf("Hello from custom chip!\n");
  chip_data_t *chip_data = (chip_data_t*)malloc(sizeof(chip_data_t));

  // Inicializar los atributos
  chip_data->gasValue = attr_init("gasValue", 10); // Valor inicial
  chip_data->thresholdValue = attr_init("thresholdValue", 50); // Valor inicial
  chip_data->analog_pin = pin_init("OUT0", ANALOG);
  chip_data->alert_pin = pin_init("OUT1", OUTPUT);
  
  const timer_config_t config = {
    .callback = chip_timer_callback,
    .user_data = chip_data,
  };

  timer_t timer_id = timer_init(&config);
  timer_start(timer_id, 1000, true); // Ejecutar cada segundo
}
