#include "motor.h"
#include "pwm.h"
#include "math.h"
#include "usart.h"
static  u16 _speed=0;
u16 _maxspeed=0;
extern  vu32 step_done;
vu32 run_state;
u16 _dis=0;
 int _DIS=10;
 u32 Autoreload=0;
u32 t_acc,t_dec,step_all=0,step_acc,step_dec,step_run;
u32 Acc_arr[2048];
u32 Run_arr=0;
u32 Dec_arr[2048];

 #define run_state_stop 0
#define run_state_acc 1
#define run_state_run 2
#define run_state_dec 3
void Motor_Init(){
	
		 GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟
	TIM14_PWM_Init();
  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;//控制方向f0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOF, GPIO_Pin_0);//GPIOF9,F10设置高，灯灭
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM14); //GPIOA7复用为定时器14
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOA7
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA7
}
void Motor_Setspeed(u16 speed){
	 
	_speed=speed;
}
u16 Motor_Getspeed(){
	return _speed;
}
void Motor_Setmaxspeed(u16 maxspeed){
	_maxspeed=maxspeed;
}
u16 Motor_Getmaxspeed(){
	return _maxspeed;
}
void Motor_Run(u16 dir){
	
	if(_speed==0){
			Autoreload=1000000*60/40/400 ;}
	else{
			Autoreload=1000000*60/_speed/400 ;//1000000/(_speed/60*4000)
			TIM_SetAutoreload(TIM14,Autoreload-1);
			TIM_SetCompare1(TIM14,Get_Autoreload()/2);	//修改比较值，修改占空比
			if(dir==1)DIR=1;
			else DIR=0;
	}

}
u32 Get_Autoreload(){
	return Autoreload;
}

void Motor_Stop(u32 pwmval){
	
	//TIM_CtrlPWMOutputs(TIM14, DISABLE);
	//TIM_CCxNCmd(TIM14,TIM_Channel_1, DISABLE);
	//TIM14 -> CCER &= 0XFEFF; //?
	//TIM_UpdateDisableConfig(TIM14,ENABLE);
	_speed=0;
	//Motor_Run(1);
	SetMotorstep(0);
	TIM_SetCompare1(TIM14,pwmval+20);	
}

void Motor_Moveto(u32 dis)//多少步
{
	
	_dis=dis;
}

int Motor_Distance(u16 current){
	if(_DIS==0||(Motor_Getspeed()==0))SetMotorstep(0);
	
	_DIS=_dis-current;
	if(_DIS<0)_DIS=0;
return _DIS;
}//还有多少步
void MOTOR_ACC_RUN(u16 dir){
	SetMotorstep(0);
	TIM_Cmd(TIM14,ENABLE);  
	if(dir==1)DIR=1;
			else DIR=0;
}

void MOTOR_RUN_calculate(u32 acc, u32 dec, u32 topspeed, u32 dis)
{
 
  u32 i,tim_cnt,tim_rest,tim_cnt_temp;
  
  step_all = (float)dis * N_MOTOR ;//N_MOTOR=200
  t_acc = topspeed * 1000 / acc; //unit: ms
  t_dec = topspeed * 1000 / dec; //unit: ms
  if(topspeed * (t_acc + t_dec) / 120000 > dis)  //??????? // topspeed/60/1000 * (t_acc + t_dec) / 2 > dis 
  {
    topspeed = sqrt(dis * acc * dec * 120 / (acc + dec));
    t_acc = topspeed * 1000 / acc; //unit: ms
    t_dec = topspeed * 1000 / dec; //unit: ms 
  }
  step_acc = N_MOTOR * ((float)topspeed*topspeed/(acc*120));
  step_dec = N_MOTOR * ((float)topspeed*topspeed/(dec*120));
  if(step_all > step_acc + step_dec)
   step_run = step_all - step_acc - step_dec;
  else
   step_run = 0;
  //tim_cnt = 5.2363 * ft / (sqrt(acc*N_MOTOR/2)); //(ft * sqrt(60)*0.676) / sqrt(acc*N_MOTOR/2);
  tim_cnt = 7.7460 * ft / (sqrt(acc*N_MOTOR/2))/2;//
  tim_rest = 0;
  i = 0;
  run_state = run_state_acc;
  while(i<=step_acc)
  {
    Acc_arr[i]=tim_cnt_temp ;
		i++;
    //tim_cnt_temp = tim_cnt;
    //tim_cnt= tim_cnt - (2*tim_cnt+tim_rest) / (4*i+1);
    //tim_rest = (2*tim_cnt_temp+tim_rest) % (4*i+1);
		
    tim_cnt_temp = tim_cnt / ( sqrt((float)(i+1)) + sqrt((float)(i)) );
		//TIM14_Configuration(tim_cnt_temp);

	  printf("\r\n\r\n");//插入换行
		printf("isaccing I:%d cnt:%d temp:%d accstep:%d",i,tim_cnt,tim_cnt_temp,step_acc);
  }
  
  if(step_run > 0)
  {
    run_state = run_state_run;
    tim_cnt = ft * 60 / (N_MOTOR_2*topspeed);
    i = 0;
		Run_arr=tim_cnt;
		printf("isrunning I:%d cnt:%d runstep:%d",i,tim_cnt,step_run);
   /* while(i<=step_run)
    {		
			
			printf("\r\n\r\n");//插入换行
			

    }*/
  }
  run_state = run_state_dec;
  tim_rest = 0;
  i=0;
  tim_cnt = tim_cnt + (2*tim_cnt+tim_rest) / (4*(step_dec-i)-1);
  while(i<=step_dec)
  {
		printf("\r\n\r\n");//插入换行
		printf("isdecing I:%d cnt:%d decstep:%d",i,tim_cnt,step_dec);
		Dec_arr[i]=tim_cnt;
    i++;
    tim_cnt_temp = tim_cnt;
    tim_cnt = tim_cnt + (2*tim_cnt+tim_rest) / (4*(step_dec-i)-1);
    tim_rest = (2*tim_cnt_temp+tim_rest) % (4*(step_dec-i)-1);
  }
  run_state = run_state_stop;
}




void MOTOR_RUN(u32 acc, u32 dec, u32 topspeed, u32 dis)
{
 
  u32 i,tim_cnt,tim_rest,tim_cnt_temp;
  
  step_all = (float)dis * N_MOTOR ;//N_MOTOR=200
  t_acc = topspeed * 1000 / acc; //unit: ms
  t_dec = topspeed * 1000 / dec; //unit: ms
  if(topspeed * (t_acc + t_dec) / 120000 > dis)  //??????? // topspeed/60/1000 * (t_acc + t_dec) / 2 > dis 
  {
    topspeed = sqrt(dis * acc * dec * 120 / (acc + dec));
    t_acc = topspeed * 1000 / acc; //unit: ms
    t_dec = topspeed * 1000 / dec; //unit: ms 
  }
  step_acc = N_MOTOR * ((float)topspeed*topspeed/(acc*120));
  step_dec = N_MOTOR * ((float)topspeed*topspeed/(dec*120));
  if(step_all > step_acc + step_dec)
   step_run = step_all - step_acc - step_dec;
  else
   step_run = 0;
  //tim_cnt = 5.2363 * ft / (sqrt(acc*N_MOTOR/2)); //(ft * sqrt(60)*0.676) / sqrt(acc*N_MOTOR/2);
  tim_cnt = 7.7460 * ft / (sqrt(acc*N_MOTOR/2))/2;//
  tim_rest = 0;
  i = 0;
  TIM14_Configuration(tim_cnt);
  run_state = run_state_acc;
  TIM_Cmd(TIM14,ENABLE);
  step_done = 0;
  while(step_done==0);
	
  while(i<=step_acc)
  {
    i++;
    //tim_cnt_temp = tim_cnt;
    tim_cnt = tim_cnt - (2*tim_cnt+tim_rest) / (4*i+1);
    tim_rest = (2*tim_cnt_temp+tim_rest) % (4*i+1);
    //tim_cnt_temp = tim_cnt / ( sqrt((float)(i+1)) + sqrt((float)(i)) );
		//TIM14_Configuration(tim_cnt_temp);
    TIM14_Configuration(tim_cnt);
		//printf("\r\n\r\n");//插入换行
		//printf("isaccing I:%d cnt:%d temp:%d accstep:%d",i,tim_cnt,tim_cnt_temp,step_acc);
    step_done = 0;
    while(step_done==0);
  }
  
  if(step_run > 0)
  {
    run_state = run_state_run;
    tim_cnt = ft * 60 / (N_MOTOR_2*topspeed);
    i = 0;
   TIM14_Configuration(tim_cnt);
    while(i<=step_run)
    {		
			
			printf("\r\n\r\n");//插入换行
			printf("isrunning I:%d cnt:%d runstep:%d",i,tim_cnt,step_run);
      step_done = 0;
      while(step_done==0);
      i++;
    }
  }
  run_state = run_state_dec;
  tim_rest = 0;
  i=0;
  tim_cnt = tim_cnt + (2*tim_cnt+tim_rest) / (4*(step_dec-i)-1);
  while(i<=step_dec)
  {
    TIM14_Configuration(tim_cnt);
    step_done = 0;
    while(step_done==0);
		printf("\r\n\r\n");//插入换行
		printf("isdecing I:%d cnt:%d decstep:%d",i,tim_cnt,step_dec);
    i++;
    tim_cnt_temp = tim_cnt;
    tim_cnt = tim_cnt + (2*tim_cnt+tim_rest) / (4*(step_dec-i)-1);
    tim_rest = (2*tim_cnt_temp+tim_rest) % (4*(step_dec-i)-1);
  }
  run_state = run_state_stop;
  TIM_Cmd(TIM14,DISABLE);
}





