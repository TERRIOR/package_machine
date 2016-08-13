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

/*���Ƶ������ת������*/
void Dcmotor_Gpioinit(){
	
}
/*pwm��ʼ��*/
void TIM3_PWM_Init(u32 arr,u32 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM14ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//ʹ��PORTFʱ��	
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOF9����Ϊ��ʱ��14
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3); //GPIOF9����Ϊ��ʱ��14
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3); //GPIOF9����Ϊ��ʱ��14
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3); //GPIOF9����Ϊ��ʱ��14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOC,&GPIO_InitStructure);              //��ʼ��PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ����ʱ��14
	
	//��ʼ��TIM14 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
	
  TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ�� 

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM14

}  
/*ֱ������ĳ�ʼ��*/
void Dcmotor_init(){
	TIM3_PWM_Init(500-1,84-1);
}
/*ֱ������˶�*/
void Dcmotor_run(u16 i){
	dcmotor *m;
	switch(i){
		case 1:
			m=&dcmotor1;
			TIM3->CCR1=m->speed;//����ccr1 �Դﵽ����ռ�ձ�
		break;
		case 2:
		  m=&dcmotor2;
			TIM3->CCR2=m->speed;//����ccr1 �Դﵽ����ռ�ձ�break;
		break;
		case 3:
			m=&dcmotor3;
			TIM3->CCR3=m->speed;//����ccr1 �Դﵽ����ռ�ձ�break;
		break;
		case 4:
			m=&dcmotor4;
			TIM3->CCR4=m->speed;//����ccr1 �Դﵽ����ռ�ձ�break;
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

