/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Drivers/OLED/oled.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <math.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t oled_addr=0x3C; //OLED位址
#define RB_SIZE 64
volatile uint8_t rb_buffer[RB_SIZE];
volatile uint16_t rb_head=0;
volatile uint16_t rb_tail=0;
volatile uint16_t rb_count=0;
uint8_t rx_buffer=0;
int16_t gyro_bias_x=0;
int16_t gyro_bias_y=0;
int16_t gyro_bias_z=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void OLED_Clear(void);
void MPU6050_ReadAccel(int16_t *ax, int16_t *ay, int16_t *az);
void MPU6050_ReadGyro(int16_t *gx, int16_t *gy, int16_t *gz);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define PI 3.14159265f
/*互補濾波*/
#define DT 0.01f
#define ALPHA 0.98f
float roll=0.0f;
float pitch=0.0f;
/*互補濾波*/




volatile uint8_t uart_tx_done = 1;
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	 if (huart->Instance == USART1)
	    {
	        uart_tx_done = 1;  // 傳送完成，標記為空閒
	    }
}



/*  ADC彙整  */
// 讀取指定的ADC通道
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
/*  ADC彙整  */
void UART_SendString(char *str)
	    {
	if (uart_tx_done)  // 只有空閒時才送
	{
		uart_tx_done=0;
	    HAL_UART_Transmit_IT(&huart1, (uint8_t*)str, strlen(str));

	}
	    }


void RB_Push(uint8_t data){
	if(rb_count<RB_SIZE){
		rb_buffer[rb_tail]=data;
		rb_tail=(rb_tail+1)&(RB_SIZE-1);
		rb_count++;
	}
}

int RB_Pop(uint8_t *data){

	if(rb_count>0){
		*data=rb_buffer[rb_head];
		rb_head=(rb_head+1)&(RB_SIZE-1);
		rb_count--;
		return 1;
	}
	return 0;
}


/*   MPU6050   */
void MPU6050_ReadAccel(int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t data[6];
    HAL_I2C_Mem_Read(&hi2c1,0x68<<1,0x3B,I2C_MEMADD_SIZE_8BIT,data,6,100);

    *ax=(int16_t)((data[0]<<8)|data[1]);
    *ay=(int16_t)((data[2]<<8)|data[3]);
    *az=(int16_t)((data[4]<<8)|data[5]);
}

void MPU6050_ReadGyro(int16_t *gx, int16_t *gy, int16_t *gz)
{
    uint8_t data[6];
    HAL_I2C_Mem_Read(&hi2c1,0x68<<1,0x43,I2C_MEMADD_SIZE_8BIT,data,6,100);

    *gx=(int16_t)((data[0]<<8)|data[1]);
    *gy=(int16_t)((data[2]<<8)|data[3]);
    *gz=(int16_t)((data[4]<<8)|data[5]);
}


/*	互補濾波  */
void MPU6050_InitAngles(void)
{
    int16_t ax,ay,az;
    MPU6050_ReadAccel(&ax,&ay,&az);

    // 將原始值換算成重力加速度（g）
    float ax_g=ax/16384.0f;
    float ay_g=ay/16384.0f;
    float az_g=az/16384.0f;

    // 計算初始 Roll 和 Pitch（靜止時使用加速度計）
    roll=atan2(ay_g,az_g)*180.0f/PI;
    pitch=atan2(-ax_g,sqrt(ay_g*ay_g+az_g*az_g))*180.0f/PI;

    char msg[60];
    sprintf(msg,"Init Roll: %.2f, Pitch: %.2f\r\n",roll,pitch);
    UART_SendString(msg);
}
/*	互補濾波  */
/*	陀螺儀校準  */
void MPU6050_CalibrateGyro(void){
	int32_t sum_x=0,sum_y=0,sum_z=0;
	int16_t gx,gy,gz;
	 for(int i=0;i<200;i++)
	    {
	        MPU6050_ReadGyro(&gx,&gy,&gz);
	        sum_x+=gx;
	        sum_y+=gy;
	        sum_z+=gz;
	        HAL_Delay(5);
	    }
	 //計算平均值
	 gyro_bias_x=sum_x/200;
	 gyro_bias_y=sum_y/200;
	 gyro_bias_z=sum_z/200;

	 char msg[60];
	    sprintf(msg,"Gyro Bias: X=%d, Y=%d, Z=%d\r\n",gyro_bias_x,gyro_bias_y,gyro_bias_z);
	    UART_SendString(msg);
}
/*	陀螺儀校準  */
/*   MPU6050   */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */


  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  if (oled_addr != 0xFF)
  {
      OLED_Init();           // 初始化 OLED



   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        HAL_Delay(100);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        HAL_Delay(100);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        HAL_Delay(100);
   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

        // ★ 清除後閃3下代表清除完成
        for (int i=0; i<3; i++)
        {
      //      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
            HAL_Delay(200);
          //  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
            HAL_Delay(200);
        }


      OLED_Clear();          //清除螢幕全暗
  }


  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);   // 啟用接收中斷
  HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);       // 設定優先級（1，低於 EXTI0）
  HAL_NVIC_EnableIRQ(USART1_IRQn);               // 在 NVIC 中啟用 USART1 中斷
  HAL_UART_Receive_IT(&huart1, &rx_buffer, 1);   // 啟動接收


  // ===  I2C 掃描器（UART 版） ===
  char scan_msg[50];
  UART_SendString("\r\n=== I2C Scanner Start ===\r\n");

  for (uint8_t addr=0x08;addr<=0x77;addr++)
  {
      // HAL 的地址要左移 1 位（7-bit 轉 8-bit）
      if (HAL_I2C_IsDeviceReady(&hi2c1,(addr<<1),1,100)==HAL_OK)
      {
          sprintf(scan_msg,"Device found at 0x%02X\r\n",addr);
          UART_SendString(scan_msg);
      }
  }
  //   I2C 掃描器（結束） //
  //   讀取who am I 喚醒 MPU6050  //

  uint8_t who_am_i = 0;

  HAL_I2C_Mem_Read(&hi2c1, 0x68 << 1, 0x75, I2C_MEMADD_SIZE_8BIT, &who_am_i, 1, 100);

  sprintf(scan_msg, "WHO_AM_I = 0x%02X\r\n", who_am_i);
  UART_SendString(scan_msg);

  if (who_am_i == 0x68)
  {
      UART_SendString("MPU6050 found! (WHO_AM_I correct)\r\n");

      //  加入延遲，讓晶片穩定
      HAL_Delay(100);

      // 喚醒 MPU6050
      uint8_t wake_cmd = 0x00;
      HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1, 0x68 << 1, 0x6B, I2C_MEMADD_SIZE_8BIT, &wake_cmd, 1, 100);

      // 檢查寫入是否成功
      if (status == HAL_OK)
      {
          UART_SendString("MPU6050 woken up! (write success)\r\n");
      }
      else
      {
          UART_SendString("MPU6050 wake up FAILED! (I2C error)\r\n");
      }
  }
  else
  {
      UART_SendString("MPU6050 NOT found! (WHO_AM_I error)\r\n");
  }
//  讀取who am I 喚醒 MPU6050結束 //
  //  原本的 OLED 初始化 //
  if (oled_addr != 0xFF)
  {
      OLED_Init();    // 初始化 OLED
  }

/* 陀螺儀校準 */
  UART_SendString("Calibrating Gyro...\r\n");
  MPU6050_CalibrateGyro();
  UART_SendString("Calibration Done!\r\n");
  /* 陀螺儀校準 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
/* ADC彙整 */
	  uint32_t light_adc=ADC_Read_Channel(ADC_CHANNEL_0);
	    float light_voltage =light_adc*3.3f/4096.0f;
/*     光敏/熱敏      */
	    uint32_t temp_adc=ADC_Read_Channel(ADC_CHANNEL_1);
	      float temperature=Thermistor_GetTemperature(temp_adc);

  /* ADC彙整 */
  /* TTL  */


 uint8_t received_byte;
   if (RB_Pop(&received_byte))
   {
       // ★ 加入這 4 行：成功取出資料時，LED 閃爍 2 下
       for (int i = 0; i < 2; i++)
       {
           HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
           HAL_Delay(100);
       }

       // 把資料傳回去（Echo）
       HAL_UART_Transmit(&huart1, &received_byte, 1, 100);
   }
   /* TTL  */
   /*   MPU6050   */
   int16_t ax,ay,az,gx,gy,gz;

   MPU6050_ReadAccel(&ax,&ay,&az);
   MPU6050_ReadGyro(&gx,&gy,&gz);

   char msg[80];

  /*   MPU6050互補濾波   */






  //換算
float ax_g=ax/16384.0f;
float ay_g=ay/16384.0f;
float az_g=az/16384.0f;

  //陀螺儀
float gyro_x=(gx-gyro_bias_x)/131.0f;
float gyro_y=(gy-gyro_bias_y)/131.0f;
float gyro_z=(gz-gyro_bias_z)/131.0f;
(void)gyro_z;
//加速度計算角度
float accel_roll=atan2(ay_g,az_g)*180.0f/PI;
float accel_pitch=atan2(-ax_g,sqrt(ay_g*ay_g+az_g*az_g))*180.0f/PI;

  //陀螺儀積分
roll+=gyro_x*DT;
pitch+=gyro_y*DT;

    //互補濾波融合
roll=ALPHA*roll+(1.0f-ALPHA)*accel_roll;
pitch=ALPHA*pitch+(1.0f-ALPHA)*accel_pitch;

//輸出
/*//PuTTY測試
  sprintf(msg,"Roll: %7.2f  Pitch: %7.2f\r\n",roll,pitch);

   UART_SendString(msg);

*///PuTTY測試
  /*   MPU6050互補濾波   */
  /*   MPU6050   */



/*OLED*/


/*//PuTTY測試
  float temper = Thermistor_GetTemperature(temp_adc);
float voltage_t = temp_adc * 3.3f / 4096.0f;
char dbg[50];
sprintf(dbg, "ADC: %d,V: %.2fV, Temp: %.1fC\r\n", temp_adc,voltage_t, temper);
UART_SendString(dbg);
*///PuTTY測試



   // 把 roll 和 pitch 拆成「整數部分」和「小數部分」
   char oled_buf[80];
   float brightness=(100-(light_adc/4096.0f)*100.0f);
   sprintf(oled_buf,"Roll: %7.2f\nPitch: %7.2f\nL:%.2f%\nT:%.1fC",roll,pitch,brightness,temperature);
   OLED_Clear();
   OLED_WriteString(oled_buf);

   HAL_Delay(100);
  }//while結尾
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        // ★ 翻轉 LED
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

        // ★ 將收到的資料放入 Ring Buffer
        RB_Push(rx_buffer);

        // ★ 先「中止」先前的接收（如果有）
        HAL_UART_AbortReceive(&huart1);

        // ★ 重新啟動接收
        HAL_UART_Receive_IT(&huart1, &rx_buffer, 1);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
