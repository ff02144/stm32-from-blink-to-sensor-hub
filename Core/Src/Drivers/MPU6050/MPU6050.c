#include "mpu6050.h"

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