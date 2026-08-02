#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "main.h"   


extern ADC_HandleTypeDef hadc1;

// 函式宣告
uint32_t ADC_Read_Channel(uint32_t channel);
float Thermistor_GetTemperature(uint32_t adc_value);

#endif