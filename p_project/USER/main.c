#include "sys.h"
#include "delay.h"  
#include "usart.h"  
//#include "lcd.h"
#include "stdlib.h" 
#include "usmart.h"		
#include "pwm.h"
#include "motorlei.h"
#include "usart3.h" 	
#include "key.h" 	 
#include "string.h"	 
#include "relays.h"
#include "posswitch.h"
#include "lightswitch.h"
//#include "adc.h"
#include "max6675.h"
#include "pid.h"
#include "dcmotor.h"
#include "hx711.h"
/*
���ߣ�֣׿��
ʱ�䣺2016/4
��Ŀ����������Ĥ��װ��
*/
/*
���ȷ����������305mm����һ��������motorinit(1,300,1,1000,1000,12) �պ�ѹ��  ���г�120mm 305mm��69mm ���ͬ���ְ뾶9.83
���ã�
1.0:
TIM4 usmart
TIM7 ��ʱ��usart3ɨ�����
TIM13,14,3 �������
��time13 ˿�� time14��3Ƥ��������
TIME 12 �¶ȼ�ⶨʱ��
TIME 11 PID �¿�
PB5,6,7���������
//PC6,PC7,PC8:DIR
PA1��max6675Ƭѡ
PA7��max6675Ƭѡ
PA5,6:SPI1
PB13,PC2��SPI2

PC0:ADC����ѹ
PG0:���������
PG1:�г̿���1
PG2:�̵���1
PG3:�̵���2
PG4:�г̿���2
PG5:�̵���3
PG6:�̵���4
MOTOR_run(1,3)��


1.1:
TIM13,14,9 �������
��time13 ˿�� time14��9Ƥ��������
TIM3 ��·ͨ�����pwm ����DCmotor pc6,7,8,9
pc0,1,2 ��Ϊ�����������
4/30�� ��
	pid1.set(1,65,3,0,137);
	pid2.set(2,63,1,0,140);//40
	pid3.set(3,57,1,0,140);//45̫��
	pid4.set(4,57,1,0,140);
	Ч������
	
	5/9:
	last day
*/

	u16 start=0;
	u8 flag=0;
	u8 speed[4];
	u8 acc[4];
	u8 temp[3];
	u8 reclen=0;  
	u16 htemp;
	void allrun(void);
	void u3_processing(void);
	void setstart(u16 i);
int main(void)
{ 
	//u8 t;

	//u8 sendmask=1;
	//u8 sendcnt=0;
	//u8 sendbuf[20];	  
  //u8 dir=1;	
	u8 key;
  //u16 adcx;
	//float temp;

	pid1.set(1,65,4,0,147);
	pid2.set(2,65,1,0,145);//40
	pid3.set(3,60,2,0,145);//45̫��
	pid4.set(4,57,1,0,140);
	
	
	
	SPI_MAX6675_Init();	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);      //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	usmart_dev.init(84); 		//��ʼ��USMART		
	KEY_Init();					//��ʼ������
 	//LCD_Init();		 			//��ʼ��LCD	  
	TIM14_TIME_Init(84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.     
	TIM13_TIME_Init(84-1);//���3��ʱ����ʼ��
	TIM9_TIME_Init(168-1);//���2��ʱ����ʼ��
	Motors_Init();
	//Adc_Init();         //��ʼ��ADC
	motor1.init(1,360,1,2000,2000,996);//�����ʼ�� ��ǩ1 �ٶ�120  ���ٶ�500��-500
	motor2.init(2,360,1,2000,2000,1110);//�����ʼ�� ��ǩ2 �ٶ�120 ��ת ���ٶ�500��-500 
	//motor3.init(3,360,1,2000,2000,1000);//�����ʼ�� ��ǩ2 �ٶ�120 ��ת ���ٶ�500��-500 
	motor1.calculate(1);
	motor2.calculate(2);
	//motor3.calculate(3);
	MOTOR_Sec_calculate(2,35,65);
	MOTOR_Sec_calculate(1,35,60);
	//MOTOR_Sec_calculate(3,400,3500);
	Pswitch.init();
	lswitch.init();
	Relays.init();
	Dcmotor_init();
	hxinit();
	returnzero();
	TIM_SetCompare1(TIM3,500);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	TIM_SetCompare2(TIM3,500);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	TIM_SetCompare3(TIM3,500);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	TIM_SetCompare4(TIM3,500);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	//POINT_COLOR=RED;
	//LCD_ShowString(30,30,200,16,16,"JUNZI STM32F4 ^_^");	
	//LCD_ShowString(30,50,200,16,16,"PACKAGE");	
	//LCD_ShowString(30,70,200,16,16,"ROASTER");
	thermometer1.buffer[TFILTER_N - 2]=max6675_read_be(1);
	thermometer2.buffer[TFILTER_N - 2]=max6675_read_be(2);
	thermometer3.buffer[TFILTER_N - 2]=max6675_read_be(3);
	thermometer4.buffer[TFILTER_N - 2]=max6675_read_be(4);
	delay_ms(1000);			//�ȴ�����ģ���ϵ��ȶ�
	usart3_init(9600);	//��ʼ������3Ϊ:9600,������.								   	   
	//LCD_ShowString(30,90,210,16,16,"KEY_UP:ROLE KEY0:SEND/STOP");  
	//LCD_ShowString(30,110,200,16,16,"package Standby!");  
  //LCD_ShowString(30,160,200,16,16,"Send:");	
	//LCD_ShowString(30,180,200,16,16,"Receive:"); 
	//LCD_ShowString(30,200,200,16,16,"state:"); 
	//POINT_COLOR=BLUE;
	delay_ms(100);
	USART3_RX_STA=0;
	thermometer1.buffer[TFILTER_N - 2]=max6675_read_be(1);
	thermometer2.buffer[TFILTER_N - 2]=max6675_read_be(2);
	thermometer3.buffer[TFILTER_N - 2]=max6675_read_be(3);
	thermometer4.buffer[TFILTER_N - 2]=max6675_read_be(4);
	pid_controlopen(0);
while(1) 
	{		
		u3_processing();
		//adcx=Get_Adc_Average(ADC_Channel_0,20);//��ȡͨ��5��ת��ֵ��20��ȡƽ��
		//temp=(float)adcx*(3.3/4096);   
		//printf("temp:%f",temp);//���뻻��
		hx711_1.read(1);
		isstabled(1);
		/*	if(show==1){
			if(motor1.private->allstep!=0){
				printf("motor:%d stepall:%d accstep:%d decstep:%d runstep:%d speed:%d dir:%d run_arr:%d",motor1.private->flag,motor1.private->allstep,motor1.private->accstep,motor1.private->decstep,motor1.private->runstep,motor1.private->speed,motor1.private->dir,motor1.private->run_arr); 
				printf("\r\n");//���뻻��
			}
			if(motor2.private->allstep!=0){
				printf("motor:%d stepall:%d accstep:%d decstep:%d runstep:%d speed:%d dir:%d",motor2.private->flag,motor2.private->allstep,motor2.private->accstep,motor2.private->decstep,motor2.private->runstep,motor2.private->speed,motor2.private->dir);
				printf("\r\n");//���뻻��
			}
			if(motor3.private->allstep!=0){
				printf("motor:%d stepall:%d accstep:%d decstep:%d runstep:%d speed:%d dir:%d",motor3.private->flag,motor3.private->allstep,motor3.private->accstep,motor3.private->decstep,motor3.private->runstep,motor3.private->speed,motor3.private->dir);
				printf("\r\n");//���뻻��
			}
			if(Pswitch.isopen(0)==0){
				printf("have touched\n");
			}
		}
		if(lswitch.isopen(0)==1){
			printf("��ͨ ���ڵ�\n");
		}else {
			printf("�Ͽ� ���ڵ�\n");
		}*/
	

		delay_ms(100);

		if(start==3){
			MOTOR_run(0,1);//in
			while(motor1.private->isrunning==1);
			delay_ms(6000);
			MOTOR_SecondRun(0,1);		
			while(motor1.private->isrunning==1);
			delay_ms(400);
			movetoinit(1,300);
			start =0;
		}
		if(start==4){
			MOTOR_run(0,2);//in
			delay_ms(100);
			while(motor2.private->isrunning==1);
			delay_ms(5000);
			MOTOR_SecondRun(0,2);
			while(motor2.private->isrunning==1);
			delay_ms(300);
			movetoinit(2,300);	
			start =0;
		}
		if((hx711_1.stable==1)&&(hx711_1.weight>30)&&(start==1)&&(lswitch.isopen(0)==0)){
			allrun();
			start=2;
		}
		if((start==2)&&(hx711_1.stable==1)&&(hx711_1.weight<20)&&(lswitch.isopen(0)==1)){
			uptostart(300);
			start=1;
		}

		/*
						if(Relays.isopen(0)==1){
			u3_printf(" relays have opened");	
		}else {
			u3_printf(" relays unopen");	
		}
		u3_printf("\r\n\r\n");//���뻻��
		if(Pswitch.isopen(0)==1){
			u3_printf(" �������ͨ�У����赲��");	
			//LCD_ShowString(30+50,240,200,16,16,"connect");	//��ʾ��������	
		}else {
			u3_printf("���赲��");
			//LCD_ShowString(30+50,240,200,16,16,"unconnect");	//��ʾ��������	
		}
		u3_printf("\r\n");//���뻻��
		
		if(flag==1){
				
			}*/
		////////////////////////////////////////////////
		key=KEY_Scan(0);		//�õ���ֵ
	   	if(key)
		{						   
			switch(key)
			{				 
				case WKUP_PRES:	//���Ʒ�����
					//show=0;
					returnzero();
					//showpid();
					break;
				case KEY0_PRES:	//����LED0��ת
					secstep(4000);
					//show=1;
					break;
				case KEY1_PRES:	//����LED1��ת	 
					firststep(4000);
					break;
				case KEY2_PRES:	//ͬʱ����LED0,LED1��ת 
					down(4500,1);
				
					//motor2.calculate(2);
				 //MOTOR_Sec_calculate(1,240,10);
				break;
			}
		}else delay_ms(10); 
	}		
		
}


void setstart(u16 i){
	start =i;
}

void allrun(){
	
	downtolight(400);
	printf(" ���ڹ����,nowpos:%d \n",nowpos);
	while(motor3.private->isrunning==1);
	delay_ms(100);
	down(5100,1);
	printf(" �½�\n");
	delay_ms(500);
	firststep(5000);
	down(5500,2);
	secstep(5000);
	MOTOR_Sec_calculate(3,450,4000);
	MOTOR_SecondRun(1,3);	
}

void u3_processing(){
	if(USART3_RX_STA&0X8000)			//���յ�һ��������
			{
	
				reclen=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
				USART3_RX_BUF[reclen]=0;	 	//���������
				u3_printf((char*)USART3_RX_BUF);
				u3_printf("%d",reclen);
				//LCD_Fill(30+100,180,240,320,WHITE);	//�����ʾ
				//LCD_ShowString(30+100,180,200,16,16,USART3_RX_BUF	); 
				if(USART3_RX_BUF[0]=='0'){
					Relays.control(0,1);
				}else if(USART3_RX_BUF[0]=='9'){
					Relays.control(0,0);
				}
				if(USART3_RX_BUF[0]=='1'&&((reclen==3)||(reclen==1)||(reclen==2))){
					//LCD_Fill(30+50,200,240,320,WHITE);	//�����ʾ
					
					flag=1;
					u3_printf("start");
				}else if(USART3_RX_BUF[0]=='0'&&((reclen==3)||(reclen==1)||(reclen==2))){
					flag=0;
					u3_printf("end");
					//LCD_Fill(30+50,200,240,320,WHITE);	//�����ʾ
					//LCD_ShowString(30+50,200,200,16,16,"end");	//��ʾ��������	
				}
				if(reclen>3){
					int i;
					int j=(int)USART3_RX_BUF[0]-1-48;//asciiת0
					u3_printf(" %d",j);
					switch(USART3_RX_BUF[1]){
							case '1':
								for(i=2;i<reclen;i++){
								speed[i-2]=USART3_RX_BUF[i];
							}
								switch(j){
									case 1:motor1.private->speed=atoi((char*)speed);
									case 2:motor2.private->speed=atoi((char*)speed);
									case 3:motor3.private->speed=atoi((char*)speed);
								}
								//tspeed[j]=atoi((char*)speed);
									break;
								case '2':
									for(i=2;i<reclen;i++){
								acc[i-2]=USART3_RX_BUF[i];
							}
								switch(j){
									case 1:motor1.private->acc=atoi((char*)acc);
									case 2:motor2.private->acc=atoi((char*)acc);
									case 3:motor3.private->acc=atoi((char*)acc);
				
								}
								//tacc[j]=atoi((char*)acc);
									break;
								case '3':
									for(i=2;i<reclen;i++){
								temp[i-2]=USART3_RX_BUF[i];
							}
									htemp=atoi((char*)temp);
									break;
					}
				}	
				u3_printf("\r\n");//���뻻��
				USART3_RX_STA=0;	 
			}	 
}
