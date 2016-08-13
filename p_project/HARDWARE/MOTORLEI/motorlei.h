#ifndef __MOTORLEI_H
#define __MOTORLEI_H

#include "sys.h"
#define N_MOTORlei 200  //
#define ftlei 1000000//1MHZ 
#define N_MOTOR_2lei 400
#define DIR1 PFout(0)	// DS0
#define DIR2 PFout(1)
#define DIR3 PFout(2)
typedef struct MOTOR motor;
typedef struct MOTOR_PRIVATE motor_private;

struct MOTOR_PRIVATE{
	u32 isrunning;
	u32 speed;
	u32 dir;
	u32 acc;
	u32 dec;
	u32 dis;
	u32 autoload;
	u32 acc_arr[600];
	u32 dec_arr[600];
	u32 run_arr;
	u32 flag;
	u32 allstep;
	u32 accstep;
	u32 decstep;
	u32 runstep;
	u32 nowstep;
	u32 sec_allstep;
	u32 sec_arr;
	u32 sec_flag;
	u32 re_arr;
};
struct MOTOR{
	motor_private *private;
	void (*run)(u32 dir,u32 j);
	void (*init)(u32 FLAG,u32 SPEED,u32 DIRlei,u32 ACC,u32 DEC,u32 DIS);
	void (*calculate)(u32 j);
	void (*sec_calculate)(u32 j,u32 speed,u32 distance);
	void (*sec_run)(u32 dir,u32 j);
	void (*runtostart)(u32 j,u32 speed);
};
	void secstep(u16 i);
	void down(u32 dis,u32 i);
	void MOTOR_SecondRun(u32 dir,u32 j);
  void MOTOR_Sec_calculate(u32 j,u32 speed,u32 distance);
	void Motors_Init(void);
	void MOTOR_run(u32 dir,u32 j);
	void motorinit(u32 FLAG,u32 SPEED,u32 DIRlei,u32 ACC,u32 DEC,u32 DIS);
	void MOTOR_calculate(u32 j);
	void movetoinit(u32 j,u32 speed);
	void firststep(u16 i);
	void downtolight(u32 speed);
	void uptostart(u32 speed);
	extern motor motor1,motor2,motor3;
  extern u32 nowpos; 
//extern motor motor4;
#endif 

