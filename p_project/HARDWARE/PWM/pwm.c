#include "pwm.h"

#include "usart.h"
#include "motorlei.h"
#include "posswitch.h"
#include "lightswitch.h"


//TIM14 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
 vu32 step_done=0;
 u16 step_flag=1;
 
void TIM14_TIME_Init(u32 psc)
{		 					 
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14ʱ��ʹ��    
	

	    
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=5000;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//��ʼ����ʱ��14

 
 
 // TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPEʹ�� 
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //����ʱ��14�����ж�	
	TIM_Cmd(TIM14, ENABLE);  //ʹ��TIM14
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn; //��ʱ14�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	 

}  
void TIM13_TIME_Init(u32 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  	//TIM13ʱ��ʹ��    
  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=5000;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM13,&TIM_TimeBaseStructure);//��ʼ����ʱ��13
	TIM_ARRPreloadConfig(TIM13, ENABLE);
   TIM_ClearITPendingBit(TIM13,TIM_IT_Update);
	TIM_ITConfig(TIM13,TIM_IT_Update,ENABLE); //����ʱ��13�����ж�	
	
	TIM_Cmd(TIM13, ENABLE);  //ʹ��TIM13
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_UP_TIM13_IRQn; //��ʱ13�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	 
}  
void TIM9_TIME_Init(u32 psc)
{		 					 

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  ///ʹ��TIM3ʱ��
		
  TIM_TimeBaseInitStructure.TIM_Period = 500; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
  TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	///////////////////////


	////////////////////////////
	TIM_Cmd(TIM9,ENABLE); //ʹ�ܶ�ʱ��3
/*	
		*/
NVIC_InitStructure.NVIC_IRQChannel=TIM1_BRK_TIM9_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	


 
}

double GetMotorstep(){

	return step;
}
void SetMotorstep(double i){

	 step=i;
}
/*��һ��
completed*/
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	u32 step_all=motor1.private->allstep;
	u32 step_acc=motor1.private->accstep;
	u32 step_run=motor1.private->runstep;
	if(TIM_GetITStatus(TIM14,TIM_IT_Update)==SET) //����ж�
	{
		
		PBout(7)=!PBout(7);
		motor1.private->nowstep++;
		if(motor1.private->sec_flag==1){
			
			if(motor1.private->nowstep>=motor1.private->sec_allstep){
				TIM_Cmd(TIM14,DISABLE); 
				motor1.private->sec_flag=0;
				motor1.private->isrunning=0;//û�˶���ǩ
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
	TIM_ClearITPendingBit(TIM14,TIM_IT_Update);  //����жϱ�־λ
}
/*������˿��ƽ̨*/
void TIM8_UP_TIM13_IRQHandler(void)
{
	u32 step_all=motor3.private->allstep;
	u32 step_acc=motor3.private->accstep;
	u32 step_run=motor3.private->runstep;
	if(TIM_GetITStatus(TIM13,TIM_IT_Update)==SET) //����ж�
	{
		//printf("2\r\n");
		PBout(5)=!PBout(5);
		motor3.private->nowstep++;
		if(motor3.private->sec_flag==1){                    //�޼��ٶ��˶�
			if(motor3.private->nowstep>=motor3.private->sec_allstep){
				TIM_Cmd(TIM13,DISABLE); 
				motor3.private->sec_flag=0;
				motor3.private->isrunning=0;//û�˶���ǩ
			}
		}else if(motor3.private->sec_flag==3){//�ƶ��������֮��
			if(lswitch.isopen(0)==1 ){//��ͨ û���ϰ���
				TIM_Cmd(TIM13,DISABLE);
				nowpos+=motor3.private->nowstep;
				motor3.private->sec_flag=0;
				motor3.private->isrunning=0;
			}
		}else if(motor3.private->sec_flag==2){                      //��ȥ��
			if(motor3.private->nowstep>=motor3.private->sec_allstep){
				nowpos=0;
				TIM_Cmd(TIM13,DISABLE); 
				motor3.private->sec_flag=0;
				motor3.private->isrunning=0;//û�˶���ǩ
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
	TIM_ClearITPendingBit(TIM13,TIM_IT_Update);  //����жϱ�־λ
}
/*�ڶ������ʱ��*/
void TIM1_BRK_TIM9_IRQHandler(void)
{

	u32 step_all=motor2.private->allstep;
	u32 step_acc=motor2.private->accstep;
	u32 step_run=motor2.private->runstep;

	if(TIM_GetITStatus(TIM9,TIM_IT_Update)==SET) //����ж�
	{
	
		PBout(6)=!PBout(6);
		motor2.private->nowstep++;

		if(motor2.private->sec_flag==1){
			if(motor2.private->nowstep>=motor2.private->sec_allstep){
				TIM_Cmd(TIM9,DISABLE); 
				motor2.private->sec_flag=0;
				motor2.private->isrunning=0;//û�˶���ǩ
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
	TIM_ClearITPendingBit(TIM9,TIM_IT_Update);  //����жϱ�־λ
}
/*void TIM3_IRQHandler(void)
{
	//
	
	u32 step_all=motor2.private->allstep;
	u32 step_acc=motor2.private->accstep;
//	u32 step_dec=motor1.private->decstep;
	u32 step_run=motor2.private->runstep;
	//printf("1\r\n");
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
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
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־�
	
	
	if(motor1.private->sec_flag==1){
			
			if(motor1.private->nowstep>=motor1.private->sec_allstep){
				TIM_Cmd(TIM14,DISABLE); 
				motor1.private->sec_flag=0;
				motor1.private->isrunning=0;//û�˶���ǩ
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

