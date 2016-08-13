#ifndef __HX711_H
#define __HX711_H
#include "sys.h"
#define DT PFout(3)	// DS0
#define SCK PFout(4)	// DS1	
#define WFILTER_A 1
#define WFILTER_N 5
typedef struct HX711 hx711;
struct 	HX711{
	float weight;
	u32 offset;
	float cofficient;
	float buffer[WFILTER_N];
	u32 over_cnt;
	u16 stable;
	float lastweight;
	void (*init)();
	void (*read)(u32 i);
	void (*set)(u32 i,u32 offset,float coff);
};
void hx_set(u32 i,u32 offset,float coff);
void weighfilter(hx711 *w, float j);
void hxinit(void);
void hx_set(u32 i,u32 offset,float coff);
void hx_cal(void);
void returnzero(void);
void isstabled(u32 i);
unsigned long Read_HX711(void);
void Read_HX711_fin(u32 i);
extern hx711 hx711_1;
#endif

