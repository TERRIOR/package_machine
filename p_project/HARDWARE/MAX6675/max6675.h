#ifndef _MAX6675__H
#define _MAX6675__H
#include "sys.h"
#define CS1 PAout(1)
#define TFILTER_A 2
#define TFILTER_N 4

#define Cs_L1  GPIO_ResetBits(GPIOF,GPIO_Pin_6);//片选低
#define Cs_H1  GPIO_SetBits(GPIOF,GPIO_Pin_6);//片选高
#define Clk_L1 GPIO_ResetBits(GPIOF,GPIO_Pin_7);//时钟低
#define Clk_H1 GPIO_SetBits(GPIOF,GPIO_Pin_7); //时钟高
#define So_H1 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_5)//so输入

#define Cs_L  GPIO_ResetBits(GPIOC,GPIO_Pin_3);//片选低
#define Cs_H  GPIO_SetBits(GPIOC,GPIO_Pin_3);//片选高
#define Clk_L GPIO_ResetBits(GPIOC,GPIO_Pin_4);//时钟低
#define Clk_H GPIO_SetBits(GPIOC,GPIO_Pin_4); //时钟高
#define So_H GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//so输入
extern u32 show;
typedef struct Thermometer thermometer;
struct  Thermometer{
	u32 flag;
	float tempreture;
	u32 offset;
	u32 step;
	float buffer[TFILTER_N];
	void (*init)(void);
	void (*read)(u32 m);
	u32 over_cnt;
};
void  max6675_read(u32 m);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
//u8 SPI1_ReadWriteByte(u8 TxData);
void SPI_MAX6675_Init(void);
float max6675_read_be(u32 m);
void pid_controlopen(u16 i);
extern thermometer thermometer1,thermometer2,thermometer3,thermometer4;
#endif
