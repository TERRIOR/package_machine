#include "lightswitch.h"
#include "sys.h"
u32 lisopened[2]={0,0};
u32 lstate=0;

void lgpio_init(){
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2; //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIOG1
}
void lightswitch_Init(){
	lgpio_init();
}

u32 light_isopen(u32 i){
	switch(i){
		case 0:
			if(lswitch0==1){
				lisopened[i]=1;
			 
			}	else{lisopened[i]=0;} 
			lstate=lisopened[i];
		break;
		case 1:
				lstate=lisopened[i];
		break;
	}
	return lstate;
}
Lightswitch lswitch={
	lgpio_init,
	light_isopen

};

