#ifndef _MOTOR_H
#define _MOTOR_H
#include "sys.h"

#define N_MOTOR_2 400
#define N_MOTOR 200
#define DIV 16//16细分
#define stepdigree 1.8//1.8度一步 一圈200步 200个脉冲
#define DIR PFout(0)	// DS0
#define ft 1000000//1MHZ 
void Motor_Init(void);
void Motor_Run(u16 dir);
void Motor_Stop(u32 pwmval);
void Motor_Moveto(u32 dis);//多少步
void Motor_Setmaxspeed(u16 maxspeed);
u16 Motor_Getmaxspeed(void);
void Motor_Setspeed(u16 speed);
u16 Motor_Getspeed(void);
int Motor_Distance(u16 current);//还有多少�
u32 Get_Autoreload(void);
void MOTOR_RUN(u32 acc, u32 dec, u32 topspeed, u32 dis);
void MOTOR_RUN_calculate(u32 acc, u32 dec, u32 topspeed, u32 dis);
void MOTOR_ACC_RUN(u16 dir);
	#endif
