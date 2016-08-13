#include "dcmotor.h"
#include "sys.h"
#include "usart.h"

dcmotor dcmotor1={
	500,0,0,Dcmotor_init,Dcmotor_stop,Dcmotor_run,Dcmotor_set
};
dcmotor dcmotor2={
	500,0,0,Dcmotor_init,Dcmotor_stop,Dcmotor_run,Dcmotor_set
};
dcmotor dcmotor3={
	500,0,0,Dcmotor_init,Dcmotor_stop,Dcmotor_run,Dcmotor_set
};
dcmotor dcmotor4={
	500,0,0,Dcmotor_init,Dcmotor_stop,Dcmotor_run,Dcmotor_set
};

/*控制电机正反转的引脚*/
void Dcmotor_Gpioinit(){
	
}
/*pwm初始化*/
void TIM3_PWM_Init(u32 arr,u32 psc)
{		 					 
	//此部分需手动修改IO口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM14时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOF9复用为定时器14
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3); //GPIOF9复用为定时器14
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3); //GPIOF9复用为定时器14
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3); //GPIOF9复用为定时器14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure);              //初始化PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化定时器14
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
	
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
	
  TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 

	TIM_Cmd(TIM3, ENABLE);  //使能TIM14

}  
/*直流电机的初始化*/
void Dcmotor_init(){
	TIM3_PWM_Init(500-1,84-1);
}
/*直流电机运动*/
void Dcmotor_run(u16 i){
	dcmotor *m;
	switch(i){
		case 1:
			m=&dcmotor1;
			TIM3->CCR1=m->speed;//调整ccr1 以达到调整占空比
		break;
		case 2:
		  m=&dcmotor2;
			TIM3->CCR2=m->speed;//调整ccr1 以达到调整占空比break;
		break;
		case 3:
			m=&dcmotor3;
			TIM3->CCR3=m->speed;//调整ccr1 以达到调整占空比break;
		break;
		case 4:
			m=&dcmotor4;
			TIM3->CCR4=m->speed;//调整ccr1 以达到调整占空比break;
		break;
	}
	m->isrunning=1;
}

void Dcmotor_stop(u16 i){
	dcmotor *m;
		switch(i){
			case 1:
				m=&dcmotor1;
				TIM3->CCR1=500;

			break;
			case 2:
				m=&dcmotor2;
				TIM3->CCR2=500;
			break;
			case 3:
				m=&dcmotor3;
				TIM3->CCR3=500;
			break;
			case 4:
				m=&dcmotor4;
				TIM3->CCR4=500;	
			break;
		}
	m->isrunning=0;
}

void Dcmotor_set(u16 i,u16 dir,u32 speed){
	dcmotor *m;
	switch(i){
		case 1:
			m=&dcmotor1;
		break;
		case 2:
			m=&dcmotor2;
		break;
		case 3:
			m=&dcmotor3;
		break;
		case 4:
			m=&dcmotor4;
		break;
	}
	if(speed>=500)speed=499;
	m->speed=500-speed;
	m->dir=dir;
}

