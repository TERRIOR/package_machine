#ifndef __LIGHTSWITCH_H
#define __LIGHTSWITCH_H
#include "sys.h"
#define lswitch0 		GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_0) //PG1
typedef struct lightswitch{
	void (*init)();
	u32 (*isopen)();
	
}Lightswitch;
extern Lightswitch lswitch;
void lightswitch_Init(void);
u32 light_isopen(u32 i);

#endif

