#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "main.h"   // 提供 I2C_HandleTypeDef 和 HAL 函式

// 常數定義
#define PI 3.14159265f

// 外部變數宣告（這些變數定義在 main.c 中）
extern I2C_HandleTypeDef hi2c1;
extern float roll;
extern float pitch;
extern int16_t gyro_bias_x;
extern int16_t gyro_bias_y;
extern int16_t gyro_bias_z;

// 函式宣告
void MPU6050_ReadAccel(int16_t *ax, int16_t *ay, int16_t *az);
void MPU6050_ReadGyro(int16_t *gx, int16_t *gy, int16_t *gz);
void MPU6050_InitAngles(void);
void MPU6050_CalibrateGyro(void);

// UART_SendString 在 main.c 或 utils 中定義，這裡宣告為 extern
extern void UART_SendString(char *str);

#endif