#include "pwm.h"

#include "usart.h"
#include "motorlei.h"
#include "posswitch.h"
#include "lightswitch.h"


//TIM14 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
 vu32 step_done=0;
 u16 step_flag=1;
 
void TIM14_TIME_Init(u32 psc)
{		 					 
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14时钟使能    
	

	    
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=5000;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//初始化定时器14

 
 
 // TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPE使能 
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //允许定时器14更新中断	
	TIM_Cmd(TIM14, ENABLE);  //使能TIM14
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn; //定时14中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	 

}  
void TIM13_TIME_Init(u32 psc)
{		 					 
	//此部分需手动修改IO口设置
	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  	//TIM13时钟使能    
  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=5000;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM13,&TIM_TimeBaseStructure);//初始化定时器13
	TIM_ARRPreloadConfig(TIM13, ENABLE);
   TIM_ClearITPendingBit(TIM13,TIM_IT_Update);
	TIM_ITConfig(TIM13,TIM_IT_Update,ENABLE); //允许定时器13更新中断	
	
	TIM_Cmd(TIM13, ENABLE);  //使能TIM13
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_UP_TIM13_IRQn; //定时13中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	 
}  
void TIM9_TIME_Init(u32 psc)
{		 					 

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  ///使能TIM3时钟
		
  TIM_TimeBaseInitStructure.TIM_Period = 500; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);//初始化TIM3
	
  TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	///////////////////////


	////////////////////////////
	TIM_Cmd(TIM9,ENABLE); //使能定时器3
/*	
		*/
NVIC_InitStructure.NVIC_IRQChannel=TIM1_BRK_TIM9_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	


 
}

double GetMotorstep(){

	return step;
}
void SetMotorstep(double i){

	 step=i;
}
/*第一层
completed*/
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	u32 step_all=motor1.private->allstep;
	u32 step_acc=motor1.private->accstep;
	u32 step_run=motor1.private->runstep;
	if(TIM_GetITStatus(TIM14,TIM_IT_Update)==SET) //溢出中断
	{
		
		PBout(7)=!PBout(7);
		motor1.private->nowstep++;
		if(motor1.private->sec_flag==1){
			
			if(motor1.private->nowstep>=motor1.private->sec_allstep){
				TIM_Cmd(TIM14,DISABLE); 
				motor1.private->sec_flag=0;
				motor1.private->isrunning=0;//没运动标签
			}
		}else if(motor1.private->sec_flag==2){
			if(Pswitch.isopen(0)==0){
				TIM_Cmd(TIM14,DISABLE);
				motor1.private->sec_flag=0;
				motor1.private->isrunning=0;
			}
		}
		else {
			
			step=motor1.private->nowstep;		
			if(step<=step_acc ){
				TIM14_Configuration(motor1.private->acc_arr[step]);
			}else if(step<=step_run+step_acc){
				if(step<step_acc+10){TIM14_Configuration(motor1.private->run_arr);}
				
			}else if(step<step_all){
				TIM14_Configuration(motor1.private->dec_arr[step-step_acc-step_run]);
			}else{
				TIM_Cmd(TIM14,DISABLE);  
				motor1.private->isrunning=0;
			}
			 //step_done = 1; 
		}
		
	}
	TIM_ClearITPendingBit(TIM14,TIM_IT_Update);  //清除中断标志位
}
/*第三层丝杆平台*/
void TIM8_UP_TIM13_IRQHandler(void)
{
	u32 step_all=motor3.private->allstep;
	u32 step_acc=motor3.private->accstep;
	u32 step_run=motor3.private->runstep;
	if(TIM_GetITStatus(TIM13,TIM_IT_Update)==SET) //溢出中断
	{
		//printf("2\r\n");
		PBout(5)=!PBout(5);
		motor3.private->nowstep++;
		if(motor3.private->sec_flag==1){                    //无加速度运动
			if(motor3.private->nowstep>=motor3.private->sec_allstep){
				TIM_Cmd(TIM13,DISABLE); 
				motor3.private->sec_flag=0;
				motor3.private->isrunning=0;//没运动标签
			}
		}else if(motor3.private->sec_flag==3){//移动到光电门之下
			if(lswitch.isopen(0)==1 ){//导通 没有障碍物
				TIM_Cmd(TIM13,DISABLE);
				nowpos+=motor3.private->nowstep;
				motor3.private->sec_flag=0;
				motor3.private->isrunning=0;
			}
		}else if(motor3.private->sec_flag==2){                      //可去掉
			if(motor3.private->nowstep>=motor3.private->sec_allstep){
				nowpos=0;
				TIM_Cmd(TIM13,DISABLE); 
				motor3.private->sec_flag=0;
				motor3.private->isrunning=0;//没运动标签
			}
		}else{
			step=motor3.private->nowstep;		
			if(step<=step_acc ){
				TIM13_Configuration(motor3.private->acc_arr[step]);
			}else if(step<=step_run+step_acc){
				if(step<step_acc+10){TIM13_Configuration(motor3.private->run_arr);}
				
			}else if(step<step_all){
				TIM13_Configuration(motor3.private->dec_arr[step-step_acc-step_run]);
			}else{
				TIM_Cmd(TIM13,DISABLE); 
				motor3.private->isrunning=0;
			}
		}
	}
	TIM_ClearITPendingBit(TIM13,TIM_IT_Update);  //清除中断标志位
}
/*第二电机定时器*/
void TIM1_BRK_TIM9_IRQHandler(void)
{

	u32 step_all=motor2.private->allstep;
	u32 step_acc=motor2.private->accstep;
	u32 step_run=motor2.private->runstep;

	if(TIM_GetITStatus(TIM9,TIM_IT_Update)==SET) //溢出中断
	{
	
		PBout(6)=!PBout(6);
		motor2.private->nowstep++;

		if(motor2.private->sec_flag==1){
			if(motor2.private->nowstep>=motor2.private->sec_allstep){
				TIM_Cmd(TIM9,DISABLE); 
				motor2.private->sec_flag=0;
				motor2.private->isrunning=0;//没运动标签
			}
		}else if(motor2.private->sec_flag==2){
			if(Pswitch.isopen(1)==0){
				TIM_Cmd(TIM9,DISABLE);
				motor2.private->sec_flag=0;
				motor2.private->isrunning=0;
			}
		}else {
			step=motor2.private->nowstep;		
			if(step<=step_acc ){
				TIM9_Configuration(motor2.private->acc_arr[step]);
			}else if(step<=step_run+step_acc){
				if(step<step_acc+10){
					TIM9_Configuration(motor2.private->run_arr);
				}
			}else if(step<step_all){
				TIM9_Configuration(motor2.private->dec_arr[step-step_acc-step_run]);
			}else{
				TIM_Cmd(TIM9,DISABLE);  
				motor2.private->isrunning=0;
			}	
		}	
	}
	TIM_ClearITPendingBit(TIM9,TIM_IT_Update);  //清除中断标志位
}
/*void TIM3_IRQHandler(void)
{
	//
	
	u32 step_all=motor2.private->allstep;
	u32 step_acc=motor2.private->accstep;
//	u32 step_dec=motor1.private->decstep;
	u32 step_run=motor2.private->runstep;
	//printf("1\r\n");
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		//printf("2\r\n");
		PBout(6)=!PBout(6);
		
		motor2.private->nowstep++;
		step=motor2.private->nowstep;		
		if(step<=step_acc ){
			TIM3_Configuration(motor2.private->acc_arr[step]);
		}else if(step<=step_run+step_acc){
			if(step<step_acc+10){TIM3_Configuration(motor2.private->run_arr);}
			
		}else if(step<step_all){
			TIM3_Configuration(motor2.private->dec_arr[step-step_acc-step_run]);
		}else{
			TIM_Cmd(TIM3,DISABLE);  
			motor2.private->isrunning=0;
		}	
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志�
	
	
	if(motor1.private->sec_flag==1){
			
			if(motor1.private->nowstep>=motor1.private->sec_allstep){
				TIM_Cmd(TIM14,DISABLE); 
				motor1.private->sec_flag=0;
				motor1.private->isrunning=0;//没运动标签
			}
		}else if(motor1.private->sec_flag==2){
			if(Pswitch.isopen(0)==0){
				TIM_Cmd(TIM14,DISABLE);
				motor1.private->sec_flag=0;
				motor1.private->isrunning=0;
			}
		}
		else {
			
			step=motor1.private->nowstep;		
			if(step<=step_acc ){
				TIM14_Configuration(motor1.private->acc_arr[step]);
			}else if(step<=step_run+step_acc){
				if(step<step_acc+10){TIM14_Configuration(motor1.private->run_arr);}
				
			}else if(step<step_all){
				TIM14_Configuration(motor1.private->dec_arr[step-step_acc-step_run]);
			}else{
				TIM_Cmd(TIM14,DISABLE);  
				motor1.private->isrunning=0;
			}
			 //step_done = 1; 
		}�
}*/



void TIM14_Configuration(u32 period)
{
 TIM14->ARR = period-1;
 TIM14->CCR2 = period >> 1;

}

void TIM13_Configuration(u32 period)
{
 TIM13->ARR = period-1;
 TIM13->CCR2 = period >> 1;

}
void TIM9_Configuration(u32 period)
{
 TIM9->ARR = period-1;
 TIM9->CCR2 = period >> 1;

}

