## 專案簡介
## 硬體需求
- **開發板**:
- 	MCU:STM32F103C8T6
- **感測器與顯示**:
- 	OLED螢幕顯示器(I2C)
- 	MPU-6050模塊三軸速度陀螺儀(I2C)
- 	光敏電阻模組(類比輸出)
- **其他**:
- 	TTL-USB 轉接器
- 	ST-LINK V2 燒錄器
## 開發環境/搭配工具
- 	STM32CubeIDE
- 	STM32CubeMX
- 	PuTTY
## 版本紀錄
- [v1.0-blink](https://github.com/ff02144/LED_Blink/releases/tag/v1.0-blink) - 基礎 LED 閃爍
- [v2.0-blink](https://github.com/ff02144/LED_Blink/releases/tag/v2.0) - 按鍵中斷
- [v3.0-blink](https://github.com/ff02144/LED_Blink/releases/tag/v3.0)-I2C 掃描器
- [v3.1-blink](https://github.com/ff02144/LED_Blink/releases/tag/v3.1)-OLED顯示Hello STM32
- [v3.2-blink](https://github.com/ff02144/LED_Blink/releases/tag/v3.2)-手持式照度計：OLED + ADC 光敏讀取
- [v3.3-blink](https://github.com/ff02144/stm32-from-blink-to-sensor-hub/releases/tag/v3.3-blink)-STM32 光敏感測器 OLED 顯示 + UART 電腦監控
- [v3.4-blink](https://github.com/ff02144/stm32-from-blink-to-sensor-hub/releases/tag/v3.4)-OLED + ADC + UART中斷Echo
- [v4.0-blink](https://github.com/ff02144/stm32-from-blink-to-sensor-hub/releases/tag/v4.0-blink)-加速度和陀螺儀原始數據讀取
- [v4.1-blink](https://github.com/ff02144/stm32-from-blink-to-sensor-hub/releases/tag/v4.1)-加速度和陀螺儀互補濾波
- [v4.2-blink](https://github.com/ff02144/stm32-from-blink-to-sensor-hub/releases/tag/v4.2-blink))-陀螺儀校準+oled整合
- [v4.3-blink]()-STM32 Blink 三合一感測器整合 - 光敏 + MPU6050姿態(互補濾波) + 熱敏溫度，OLED顯示 DEMO


## 展示影片: 
LED 閃爍:
- [影片](https://youtu.be/KM8ID3KEyd8)

按鍵中斷:
- [影片](https://youtu.be/mmpadcEgBps)

I2C 掃描器(掃到則慢速閃爍，未掃到則快速閃爍):
- [影片](https://youtu.be/VB3gYqTwDcg)

OLED顯示Hello STM32(I2C + 5x7 字型):
- [影片](https://youtu.be/_IZaFVZSwjc)

手持式照度計：OLED + ADC 光敏讀取:
- [影片](https://youtu.be/Dku-pLG0sW4)

STM32 光敏感測器 OLED 顯示 + UART 電腦監控:
- [影片](https://youtu.be/JIIunCFIMEw)

LED Blink UART中斷式接收+ring buffer echo DEMO:
- [影片](https://youtu.be/tAIhz0fM90Y)

STM32 Blink 加速度陀螺儀原始數據讀取 DEMO:
- [影片](https://youtu.be/nnsl0YgT2Us)

STM32 Blink 加速度陀螺儀 互補濾波 DEMO
- [影片](https://youtu.be/hlgaDxX-A7Y?si=JMBmCjouf2lho6x8)

STM32 Blink 陀螺儀校準+oled整合 DEMO
- [影片](https://youtu.be/oMBYzHZLTh0)

STM32 Blink 三合一感測器整合 - 光敏 + MPU6050姿態(互補濾波) + 熱敏溫度，OLED顯示 DEMO
- [影片](https://youtu.be/cZmKSajcams)