#ifndef __RELAYS_H
#define __RELAYS_H
#include "sys.h"
u32 isopened(u32 i);
typedef struct relays{
	void (*init)();
	u32 (*isopen)(u32 i);
	void (*control)(u32 i,u32 j);
}RELAYS;
extern RELAYS Relays;
void relays_Init(void);//继电器初始化
void relays_control(u32 i,u32 j);//控制继电器

#endif

