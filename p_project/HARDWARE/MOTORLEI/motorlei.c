#include "motorlei.h"
#include "sys.h"
#include "pwm.h"
#include "math.h"
#include "usart.h"
#include "relays.h"
#include "delay.h"
#include "dcmotor.h"

/*motor1,motor2 run(0)ÍùÖÐÐÄ×
	motor3 run(0)ÏòÉÏ×ß  run(1)ÏÂ  ÏÂÎªÕý·½Ïò
*/
u32 nowpos=0;
void motorinit(u32 FLAG,u32 SPEED,u32 DIRlei,u32 ACC,u32 DEC,u32 DIS){
	motor *m;	
	switch (FLAG){
		case 1: m=&motor1;break;
		case 2: m=&motor2;break;
		case 3: m=&motor3;break;
	//	case 4: m=&motor4;break;
	}
	m->private->flag=FLAG;  
	m->private->speed=SPEED;
	m->private->dir=DIRlei;
	m->private->acc=ACC;
	m->private->dec=DEC;
	m->private->dis=DIS;
}

void MOTOR_calculate(u32 j)
{
	motor *m;
	u32 step_run;
  u32 step_all;
	u32 step_acc;
	u32 step_dec;
	u32 t_acc,t_dec;
  u32 i,tim_cnt,tim_rest,tim_cnt_temp;
	u32 acc,dec,topspeed;
u32 dis;
	switch (j){
		case 1: m=&motor1;break;
		case 2: m=&motor2;break;
		case 3: m=&motor3;break;
	}  
	dis=m->private->dis;
	acc=m->private->acc;
	dec=m->private->dec;
	topspeed=m->private->speed;		
  step_all = (float)dis * 2;//Ò»È¦ÐèÒª200Âö³å ÓÉÓÚ·Å´óÐÐ³Ì¾«¶È£¬ËùÒÔ³ý100 µÈÓÚ2
	
  t_acc = topspeed * 1000 / acc; //unit: ms
  t_dec = topspeed * 1000 / dec; //unit: ms
  if(topspeed * (t_acc + t_dec) / 120000 > dis)  //??????? // topspeed/60/1000 * (t_acc + t_dec) / 2 > dis 
  {
    topspeed = sqrt(dis * acc * dec * 120 / (acc + dec));
    t_acc = topspeed * 1000 / acc; //unit: ms
    t_dec = topspeed * 1000 / dec; //unit: ms 
  }
  step_acc = N_MOTORlei * ((float)topspeed*topspeed/(acc*120));
  step_dec = N_MOTORlei * ((float)topspeed*topspeed/(dec*120));
  if(step_all > step_acc + step_dec)
   step_run = step_all - step_acc - step_dec;
  else
   step_run = 0;
  //tim_cnt = 5.2363 * ft / (sqrt(acc*N_MOTOR/2)); //(ft * sqrt(60)*0.676) / sqrt(acc*N_MOTOR/2);
  tim_cnt = 7.7460 * ftlei / (sqrt(acc*N_MOTORlei/2))/2;//
  tim_rest = 0;
  i = 0;
  //run_state = run_state_acc;
  while(i<=step_acc)
  {
		m->private->acc_arr[i]=tim_cnt_temp;
 
		i++;
    //tim_cnt_temp = tim_cnt;
    //tim_cnt= tim_cnt - (2*tim_cnt+tim_rest) / (4*i+1);
    //tim_rest = (2*tim_cnt_temp+tim_rest) % (4*i+1);
		
    tim_cnt_temp = tim_cnt / ( sqrt((float)(i+1)) + sqrt((float)(i)) );
  }
  
  if(step_run > 0)
  {
    //run_state = run_state_run;
    tim_cnt = ftlei* 60 / (N_MOTOR_2lei*topspeed);
    i = 0;
		m->private->run_arr=tim_cnt;
  }
  //run_state = run_state_dec;
  tim_rest = 0;
  i=0;
  tim_cnt = tim_cnt + (2*tim_cnt+tim_rest) / (4*(step_dec-i)-1);
  while(i<=step_dec)
  {
	
		m->private->dec_arr[i]=tim_cnt;
    i++;
    tim_cnt_temp = tim_cnt;
    tim_cnt = tim_cnt + (2*tim_cnt+tim_rest) / (4*(step_dec-i)-1);
    tim_rest = (2*tim_cnt_temp+tim_rest) % (4*(step_dec-i)-1);
  }
 // run_state = run_state_stop;
  m->private->allstep=step_all;
	m->private->accstep= step_acc;
	m->private->decstep= step_dec;
	m->private->runstep=step_run;
	
}

void MOTOR_Sec_calculate(u32 j,u32 speed,u32 distance){
	motor *m;	
		switch (j){
			case 1: m=&motor1;break;
			case 2: m=&motor2;break;
			case 3: m=&motor3;break;
		}
		
		//m->private->isrunning=1;
		m->private->sec_arr = ftlei* 60 / (N_MOTOR_2lei*speed);
		m->private->sec_allstep=(float)distance *2;//Ò»È¦ÐèÒª200Âö³å ÓÉÓÚ·Å´óÐÐ³Ì¾«¶È£¬ËùÒÔ³ý100 µÈÓÚ2
		//printf("arr:%d,step:%d\n",m->private->sec_arr,m->private->sec_allstep);
}
/*completely*/
void MOTOR_SecondRun(u32 dir,u32 j){
	motor *m;	
		switch (j){
			case 1: m=&motor1;break;
			case 2: m=&motor2;break;
			case 3: m=&motor3;break;
		}
		m->private->sec_flag=1;
		m->private->nowstep=0;
		m->private->dir=dir;
		m->private->isrunning=1;
			switch(j){
				case 1:DIR1=dir;
							TIM_Cmd(TIM14,ENABLE); //µÚÒ»²ã
					    TIM14_Configuration(m->private->sec_arr);
							printf(" ´ò¿ªtim14\n");
				break;
				case 2:DIR2=dir;
							TIM_Cmd(TIM9,ENABLE);
				      TIM9_Configuration(m->private->sec_arr);
							printf(" ´ò¿ªtim9\n");
				break;
				case 3:DIR3=dir;
							if(dir==1){
								nowpos+=m->private->sec_allstep;
							}else{
								if(nowpos <= m->private->sec_allstep){
									nowpos=0;
								}else{
									nowpos=nowpos - m->private->sec_allstep;
								}
							}
							TIM_Cmd(TIM13,ENABLE);
				      TIM13_Configuration(m->private->sec_arr);
							printf(" ´ò¿ªtim13 Î»ÖÃ:%d\n",nowpos);
				break;
	}
}		
/*completely*/
void MOTOR_run(u32 dir,u32 j){
	//SetMotorstep(0);
		motor *m;	
	switch (j){
		case 1: m=&motor1;break;
		case 2: m=&motor2;break;
		case 3: m=&motor3;break;
	}
	m->private->isrunning=1;
	m->private->nowstep=0;
	m->private->dir=dir;
	switch(j){
			case 1:DIR1=dir;
						TIM_Cmd(TIM14,ENABLE); //µÚÒ»²ã
					
						printf(" ´ò¿ªtim14\n");
			break;
		
			case 2:DIR2=dir;
						TIM_Cmd(TIM9,ENABLE);
						printf(" ´ò¿ªtim9\n");
			break;
		case 3:DIR3=dir;
						TIM_Cmd(TIM13,ENABLE);//Éý½µµç»ú
						printf(" ´ò¿ªtim13\n");
		break;
		
	}
	
}
motor_private private1={0};
motor_private private2={0};
motor_private private3={0};

motor motor1={
	&private1,
	MOTOR_run,
	motorinit,
	MOTOR_calculate,
	MOTOR_Sec_calculate,
	MOTOR_SecondRun,
	movetoinit
};
motor motor2={
	&private2,
	MOTOR_run,
	motorinit,
	MOTOR_calculate,
	MOTOR_Sec_calculate,
	MOTOR_SecondRun,
	movetoinit
};
motor motor3={
	&private3,
	MOTOR_run,
	motorinit,
	MOTOR_calculate,
	MOTOR_Sec_calculate,
	MOTOR_SecondRun,
};
/*completely*/
void Motors_Init(void){
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	//pwm io
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//Ê¹ÄÜPORTAÊ±ÖÓ	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//Ê¹ÄÜPORTAÊ±ÖÓ	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_7;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //ÉÏÀ­
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //³õÊ¼»¯PF9
	//dir
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;           //GPIOA7
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //ÉÏÀ­
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //³õÊ¼»¯PA7

}
/*completely*/
void movetoinit(u32 j,u32 speed){
	u32 n;
	motor *m;	
	switch (j){
		case 1: m=&motor1;
		break;
		case 2: m=&motor2;
		break;

		//case 4: m=&motor4;break;
	}
		m->private->dir=1;
		m->private->sec_flag=2;
		m->private->nowstep=0;
		m->private->isrunning=1;
		m->private->re_arr = ftlei* 60 / (N_MOTOR_2lei*speed);
			switch(j){
				case 1:DIR1=1;
							TIM_Cmd(TIM14,ENABLE); //µÚÒ»²ã
					    TIM14_Configuration(m->private->re_arr);
							printf(" ´ò¿ªtim14\n");
				break;
			
				case 2:DIR2=1;
							TIM_Cmd(TIM9,ENABLE);
				      TIM9_Configuration(m->private->re_arr);
							printf(" ´ò¿ªtim9\n");
				break;
				case 3:
							n=nowpos/2;
							MOTOR_Sec_calculate(3,speed,n);
							MOTOR_SecondRun(0,3);
				break;
	}
}
void uptostart(u32 speed){
	motor3.private->sec_arr = ftlei* 60 / (N_MOTOR_2lei*speed);
	DIR3=0;
	motor3.private->sec_flag=2;
	motor3.private->nowstep=0;
	motor3.private->dir=0;	
	motor3.private->isrunning=1;
	motor3.private->sec_allstep=(float)nowpos;//Ò»È¦ÐèÒª200Âö³å ÓÉÓÚ·Å´óÐÐ³Ì¾«¶È£¬ËùÒÔ³ý100 µÈÓÚ2
	TIM13_Configuration(motor3.private->sec_arr);
	TIM_Cmd(TIM13,ENABLE);
}
void downtolight(u32 speed){
	dcmotor1.speed=150;
	dcmotor2.speed=150;
	dcmotor3.speed=150;
	dcmotor4.speed=150;
	dcmotor2.run(2);
	dcmotor1.run(1);
	dcmotor3.run(3);
	dcmotor4.run(4);
	motor3.private->sec_arr = ftlei* 60 / (N_MOTOR_2lei*speed);
	DIR3=1;
	motor3.private->sec_flag=3;
	motor3.private->nowstep=0;
	motor3.private->dir=1;
	motor3.private->isrunning=1;
	TIM13_Configuration(motor3.private->sec_arr);
	TIM_Cmd(TIM13,ENABLE);
}

void down(u32 dis,u32 i){
	
	motor3.sec_calculate(3,400,dis);
	MOTOR_SecondRun(1,3);
	if(i==1){
		dcmotor1.speed=150;
		dcmotor2.speed=150;
		dcmotor3.speed=150;
		dcmotor4.speed=150;
		dcmotor2.run(2);
		dcmotor1.run(1);
		dcmotor3.run(3);
		dcmotor4.run(4);
	}else {
		dcmotor3.speed=150;
		dcmotor4.speed=150;
		dcmotor3.run(3);
		dcmotor4.run(4);
	}
	
	while(motor3.private->isrunning==1);
	dcmotor1.speed=500;
	dcmotor2.speed=500;
	dcmotor3.speed=500;
	dcmotor4.speed=500;
	dcmotor2.run(2);
	dcmotor1.run(1);
	dcmotor3.run(3);
	dcmotor4.run(4);
}
//µÚ¶þ²ã ÈÈ·âÔË¶¯
void secstep(u16 i){
	MOTOR_run(0,2);//in
	dcmotor3.speed=100;
	dcmotor4.speed=100;
	dcmotor3.run(3);
	dcmotor4.run(4);
	delay_ms(100);
	MOTOR_Sec_calculate(3,350,1400);
	MOTOR_SecondRun(0,3);
	while(motor2.private->isrunning==1);
	dcmotor3.speed=500;
	dcmotor4.speed=500;
	dcmotor3.run(3);
	dcmotor4.run(4);
	delay_ms(i);
	MOTOR_SecondRun(0,2);
	dcmotor3.speed=100;
	dcmotor4.speed=100;
	dcmotor3.run(3);
	dcmotor4.run(4);
	while(motor2.private->isrunning==1);
	dcmotor3.speed=500;
	dcmotor4.speed=500;
	dcmotor3.run(3);
	dcmotor4.run(4);
	delay_ms(400);
	movetoinit(2,300);
}
//µÚÒ»²ã ÈÈ·âÔË¶¯
void firststep(u16 i){
	MOTOR_run(0,1);//in
	dcmotor1.speed=100;
	dcmotor2.speed=100;
	dcmotor2.run(2);
	dcmotor1.run(1);
	MOTOR_Sec_calculate(3,350,1500);
	MOTOR_SecondRun(0,3);
	while(motor1.private->isrunning==1);
	dcmotor1.speed=500;
	dcmotor2.speed=500;
	dcmotor2.run(2);
	dcmotor1.run(1);
	delay_ms(i);
	MOTOR_SecondRun(0,1);
	dcmotor1.speed=100;
	dcmotor2.speed=100;
	dcmotor2.run(2);
	dcmotor1.run(1);
	while(motor1.private->isrunning==1);
	dcmotor1.speed=500;
	dcmotor2.speed=500;
	dcmotor2.run(2);
	dcmotor1.run(1);
	delay_ms(500);
	movetoinit(1,300);
}


