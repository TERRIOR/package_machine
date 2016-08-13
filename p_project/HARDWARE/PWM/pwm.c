#include "pwm.h"

#include "usart.h"
#include "motorlei.h"
#include "posswitch.h"
#include "lightswitch.h"


//TIM14 PWM²¿·Ö³õÊ¼»¯ 
//PWMÊä³ö³õÊ¼»¯
//arr£º×Ô¶¯ÖØ×°Öµ
//psc£ºÊ±ÖÓÔ¤·ÖÆµÊı
 vu32 step_done=0;
 u16 step_flag=1;
 
void TIM14_TIME_Init(u32 psc)
{		 					 
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14Ê±ÖÓÊ¹ÄÜ    
	

	    
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseStructure.TIM_Period=5000;   //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//³õÊ¼»¯¶¨Ê±Æ÷14

 
 
 // TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPEÊ¹ÄÜ 
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //ÔÊĞí¶¨Ê±Æ÷14¸üĞÂÖĞ¶Ï	
	TIM_Cmd(TIM14, ENABLE);  //Ê¹ÄÜTIM14
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn; //¶¨Ê±14ÖĞ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //ÇÀÕ¼ÓÅÏÈ¼¶1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	 

}  
void TIM13_TIME_Init(u32 psc)
{		 					 
	//´Ë²¿·ÖĞèÊÖ¶¯ĞŞ¸ÄIO¿ÚÉèÖÃ
	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  	//TIM13Ê±ÖÓÊ¹ÄÜ    
  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseStructure.TIM_Period=5000;   //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM13,&TIM_TimeBaseStructure);//³õÊ¼»¯¶¨Ê±Æ÷13
	TIM_ARRPreloadConfig(TIM13, ENABLE);
   TIM_ClearITPendingBit(TIM13,TIM_IT_Update);
	TIM_ITConfig(TIM13,TIM_IT_Update,ENABLE); //ÔÊĞí¶¨Ê±Æ÷13¸üĞÂÖĞ¶Ï	
	
	TIM_Cmd(TIM13, ENABLE);  //Ê¹ÄÜTIM13
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_UP_TIM13_IRQn; //¶¨Ê±13ÖĞ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //×ÓÓÅÏÈ¼¶2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	 
}  
void TIM9_TIME_Init(u32 psc)
{		 					 

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  ///Ê¹ÄÜTIM3Ê±ÖÓ
		
  TIM_TimeBaseInitStructure.TIM_Period = 500; 	//×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);//³õÊ¼»¯TIM3
	
  TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE); //ÔÊĞí¶¨Ê±Æ÷3¸üĞÂÖĞ¶Ï
	///////////////////////


	////////////////////////////
	TIM_Cmd(TIM9,ENABLE); //Ê¹ÄÜ¶¨Ê±Æ÷3
/*	
		*/
NVIC_InitStructure.NVIC_IRQChannel=TIM1_BRK_TIM9_IRQn; //¶¨Ê±Æ÷3ÖĞ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //×ÓÓÅÏÈ¼¶1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	


 
}

double GetMotorstep(){

	return step;
}
void SetMotorstep(double i){

	 step=i;
}
/*µÚÒ»²ã
completed*/
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	u32 step_all=motor1.private->allstep;
	u32 step_acc=motor1.private->accstep;
	u32 step_run=motor1.private->runstep;
	if(TIM_GetITStatus(TIM14,TIM_IT_Update)==SET) //Òç³öÖĞ¶Ï
	{
		
		PBout(7)=!PBout(7);
		motor1.private->nowstep++;
		if(motor1.private->sec_flag==1){
			
			if(motor1.private->nowstep>=motor1.private->sec_allstep){
				TIM_Cmd(TIM14,DISABLE); 
				motor1.private->sec_flag=0;
				motor1.private->isrunning=0;//Ã»ÔË¶¯±êÇ©
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
	TIM_ClearITPendingBit(TIM14,TIM_IT_Update);  //Çå³ıÖĞ¶Ï±êÖ¾Î»
}
/*µÚÈı²ãË¿¸ËÆ½Ì¨*/
void TIM8_UP_TIM13_IRQHandler(void)
{
	u32 step_all=motor3.private->allstep;
	u32 step_acc=motor3.private->accstep;
	u32 step_run=motor3.private->runstep;
	if(TIM_GetITStatus(TIM13,TIM_IT_Update)==SET) //Òç³öÖĞ¶Ï
	{
		//printf("2\r\n");
		PBout(5)=!PBout(5);
		motor3.private->nowstep++;
		if(motor3.private->sec_flag==1){                    //ÎŞ¼ÓËÙ¶ÈÔË¶¯
			if(motor3.private->nowstep>=motor3.private->sec_allstep){
				TIM_Cmd(TIM13,DISABLE); 
				motor3.private->sec_flag=0;
				motor3.private->isrunning=0;//Ã»ÔË¶¯±êÇ©
			}
		}else if(motor3.private->sec_flag==3){//ÒÆ¶¯µ½¹âµçÃÅÖ®ÏÂ
			if(lswitch.isopen(0)==1 ){//µ¼Í¨ Ã»ÓĞÕÏ°­Îï
				TIM_Cmd(TIM13,DISABLE);
				nowpos+=motor3.private->nowstep;
				motor3.private->sec_flag=0;
				motor3.private->isrunning=0;
			}
		}else if(motor3.private->sec_flag==2){                      //¿ÉÈ¥µô
			if(motor3.private->nowstep>=motor3.private->sec_allstep){
				nowpos=0;
				TIM_Cmd(TIM13,DISABLE); 
				motor3.private->sec_flag=0;
				motor3.private->isrunning=0;//Ã»ÔË¶¯±êÇ©
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
	TIM_ClearITPendingBit(TIM13,TIM_IT_Update);  //Çå³ıÖĞ¶Ï±êÖ¾Î»
}
/*µÚ¶şµç»ú¶¨Ê±Æ÷*/
void TIM1_BRK_TIM9_IRQHandler(void)
{

	u32 step_all=motor2.private->allstep;
	u32 step_acc=motor2.private->accstep;
	u32 step_run=motor2.private->runstep;

	if(TIM_GetITStatus(TIM9,TIM_IT_Update)==SET) //Òç³öÖĞ¶Ï
	{
	
		PBout(6)=!PBout(6);
		motor2.private->nowstep++;

		if(motor2.private->sec_flag==1){
			if(motor2.private->nowstep>=motor2.private->sec_allstep){
				TIM_Cmd(TIM9,DISABLE); 
				motor2.private->sec_flag=0;
				motor2.private->isrunning=0;//Ã»ÔË¶¯±êÇ©
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
	TIM_ClearITPendingBit(TIM9,TIM_IT_Update);  //Çå³ıÖĞ¶Ï±êÖ¾Î»
}
/*void TIM3_IRQHandler(void)
{
	//
	
	u32 step_all=motor2.private->allstep;
	u32 step_acc=motor2.private->accstep;
//	u32 step_dec=motor1.private->decstep;
	u32 step_run=motor2.private->runstep;
	//printf("1\r\n");
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //Òç³öÖĞ¶Ï
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
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //Çå³ıÖĞ¶Ï±êÖ¾Î
	
	
	if(motor1.private->sec_flag==1){
			
			if(motor1.private->nowstep>=motor1.private->sec_allstep){
				TIM_Cmd(TIM14,DISABLE); 
				motor1.private->sec_flag=0;
				motor1.private->isrunning=0;//Ã»ÔË¶¯±êÇ©
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
		}»
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

