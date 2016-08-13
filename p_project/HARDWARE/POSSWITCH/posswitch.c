#include "posswitch.h"
#include "sys.h"
u32 pisopened[4]={0,0,0,0};
u32 state=0;
pswitch Pswitch={
	posswitch_Init,
	pisopen
};
void swgpio_init(){
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4; //
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIOG1
}
void posswitch_Init(){
	swgpio_init();
}
u32 pisopen(u32 i){
	switch(i){
		case 0:
			if(switch0==1){
				pisopened[i]=1;
			 
			}	else{pisopened[i]=0;} 
			state=pisopened[i];
		break;
		case 1:
			if(switch1==1){
				pisopened[i]=1;
			 
			}else{pisopened[i]=0;} 
				state=pisopened[i];
		break;
		case 2:
				state=pisopened[i];
		break;
	}
	return state;
}


