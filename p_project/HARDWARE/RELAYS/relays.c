#include "relays.h"
#include "sys.h"
u32 isopen[4]={0,0,0,0};
u32 isopened(u32 i){//返回继电器是否打开
	u32 j=0;
	j=isopen[i];
	return j;
}
void gpio_init(){//io口初始化
  GPIO_InitTypeDef  GPIO_InitStructure;
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_5; //
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化

	GPIO_ResetBits(GPIOG,GPIO_Pin_2);
	GPIO_ResetBits(GPIOG,GPIO_Pin_3);
	GPIO_ResetBits(GPIOG,GPIO_Pin_5);
	GPIO_ResetBits(GPIOG,GPIO_Pin_6);
}
void relays_Init(){//继电器初始化
	gpio_init();
	
}
void relays_control(u32 i,u32 j){//控制继电器 i:代表第几个继电器，j代表开和关
	if(j==1){
		isopen[i]=1;
		switch(i){
			case 1:
				GPIO_SetBits(GPIOG,GPIO_Pin_2);
			break;
			case 2:
				GPIO_SetBits(GPIOG,GPIO_Pin_3);
			break;
			case 3:
				GPIO_SetBits(GPIOG,GPIO_Pin_5);
			break;
			case 4:
				GPIO_SetBits(GPIOG,GPIO_Pin_6);
			break;
	}
	}else if (j==0){
		isopen[i]=0;
		switch(i){
			case 1:
				GPIO_ResetBits(GPIOG,GPIO_Pin_2);	
			break;
			case 2:
				GPIO_ResetBits(GPIOG,GPIO_Pin_3);	
			break;
			case 3:
				GPIO_ResetBits(GPIOG,GPIO_Pin_5);	
			break;
			case 4:
				GPIO_ResetBits(GPIOG,GPIO_Pin_6);		
			break;
	}
	}
}
RELAYS Relays={
	relays_Init,
	isopened,
	relays_control

};

