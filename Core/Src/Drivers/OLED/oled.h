#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "main.h"   


extern I2C_HandleTypeDef hi2c1;
extern uint8_t oled_addr;
extern const uint8_t Font5x7[96][5];  


void OLED_WriteCmd(uint8_t cmd);
void OLED_WriteData(uint8_t *data,uint16_t len);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_WriteString(char *str);


#endif