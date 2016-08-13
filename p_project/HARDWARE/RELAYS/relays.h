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
void relays_Init(void);//�̵�����ʼ��
void relays_control(u32 i,u32 j);//���Ƽ̵���

#endif

