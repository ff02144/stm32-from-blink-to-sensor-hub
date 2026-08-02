#include "adc.h"
#include <math.h>   

uint32_t ADC_Read_Channel(uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig={0};

    // 設定要讀取的通道
    sConfig.Channel=channel;
    sConfig.Rank=1;
    sConfig.SamplingTime=ADC_SAMPLETIME_13CYCLES_5;

    if (HAL_ADC_ConfigChannel(&hadc1,&sConfig)!=HAL_OK)
    {
        return 0;
    }

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1,100)==HAL_OK)
    {
        return HAL_ADC_GetValue(&hadc1);
    }
    return 0;
}


/*  熱換算  */
float Thermistor_GetTemperature(uint32_t adc_value)
{
    // 1. 計算電壓
    float voltage = adc_value * 3.3f / 4096.0f;

    // 2. 計算熱敏電阻的電阻值（假設分壓電阻 = 10kΩ）
    float r_fixed = 10000.0f;          // 固定電阻（模組上的）
    float r_ntc = (voltage * r_fixed) / (3.3f - voltage);

    // 3. 用 Steinhart-Hart 計算溫度（B = 3950, R0 = 10000Ω @ 25°C）
    float t0 = 298.15f;                // 25°C = 298.15 K
    float r0 = 10000.0f;               // 25°C 時的電阻值
    float b_value = 3950.0f;           // NTC 的 B 值（常見）

    float temp_k = 1.0f / (1.0f/t0 + (1.0f/b_value) * logf(r_ntc / r0));
    float temp_c = temp_k - 273.15f;   // 轉換成攝氏
    temp_c=temp_c-19.4f;
    return temp_c;
}
/*  熱換算  */