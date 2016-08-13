#ifndef __POSSWITCH_H
#define __POSSWITCH_H
#include "sys.h"
#define switch0 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_1) //PG1
#define switch1 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_4) //PG1
typedef struct positionswitch{
	void (*init)();
	u32 (*isopen)();
	
}pswitch;
extern pswitch Pswitch;
void posswitch_Init(void);
u32 pisopen(u32 i);

#endif

