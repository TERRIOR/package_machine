#ifndef _DC_H
#define _DC_H
#include "sys.h"
typedef struct DCMOTOR dcmotor;
void Dcmotor_Gpioinit(void);
void Dcmotor_init(void);
void Dcmotor_run(u16 i);
void Dcmotor_stop(u16 i);
void Dcmotor_set(u16 i,u16 dir,u32 speed);
struct DCMOTOR{
	u32 speed;
	u32 dir;
	u32 isrunning;
	void (*init)();
	void (*stop)(u16 i);
	void (*run)(u16 i);
	void (*set)(u16 i,u16 dir,u32 speed);
};
extern dcmotor dcmotor1,dcmotor2,dcmotor3,dcmotor4;
#endif

