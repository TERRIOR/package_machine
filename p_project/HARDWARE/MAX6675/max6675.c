#include "sys.h"
#include "max6675.h"
#include "usart.h"
#include "relays.h"
#include "PID.h"
#include "motorlei.h"
#include "delay.h"
/*
MAX6675_1.CS:PA1
MAX6675_1.(SI SO SCLK):PA5,6
MAX6675_2.CS:PA7
MAX6675_2.CLK:PB13
MAX6675_2.SO:PC2
*/
//double Tfilter_buf[TFILTER_N];
u32 show=0;
thermometer  thermometer1={
1,20,
0,0,
{0},
SPI_MAX6675_Init,
max6675_read,
0
};
thermometer  thermometer2={
2,20,
0,0,
{0},
SPI_MAX6675_Init,
max6675_read,
0
};

thermometer  thermometer3={
3,20,0,0,
{0},
SPI_MAX6675_Init,
max6675_read
};
thermometer  thermometer4={
4,20,0,0,
{0},
SPI_MAX6675_Init,
max6675_read
};
void TIM12_TIME_Init(u16 arr,u16 psc)
{		

NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  	//TIM13时钟使能    
  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseStructure);//初始化定时器13
	TIM_ARRPreloadConfig(TIM12, ENABLE);
   TIM_ClearITPendingBit(TIM12,TIM_IT_Update);
	TIM_ITConfig(TIM12,TIM_IT_Update,ENABLE); //允许定时器13更新中断	
	
	TIM_Cmd(TIM12, ENABLE);  //使能TIM13
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_BRK_TIM12_IRQn; //定时13中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	 
	

} 
void TIM11_TIME_Init(u16 arr,u16 psc)
{		

NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);  	//TIM13时钟使能    
  //////////////////////////注意APB2!!
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM11,&TIM_TimeBaseStructure);//初始化定时器13
	TIM_ARRPreloadConfig(TIM11, ENABLE);
   TIM_ClearITPendingBit(TIM11,TIM_IT_Update);
	TIM_ITConfig(TIM11,TIM_IT_Update,ENABLE); //允许定时器13更新中断	
	
	TIM_Cmd(TIM11, ENABLE);  //使能TIM13
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_TRG_COM_TIM11_IRQn; //定时13中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	 
	

} 
void SPI_MAX6675_Init(void)
{
  
	GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOC时钟
 // RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//使能SPI2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);//使能SPI3时钟
	//////////////////SPI2复用////////////////
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); //PB13复用为 SPI2 SCLK
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource2,GPIO_AF_SPI2);
	/////////////////SPI1复用/////////////////
	/*	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); //PA5复用为 SPI1 SCLK
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); //PA6复用为 SPI1 SO
		
*//////////////////SPI3复用/////////////////
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_SPI3); //PB3复用为 SPI1 SCLK
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_SPI3); //PB4复用为 SPI1 SO
  //GPIOFA5，6初始化设置
	///////////////////////////SPI1-GPIO///////////////////
	/*  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;//PA5，6，7复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化*/

//////////////////SPI2_GPIO///////////////////////
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//PB13复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PC2复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	
	//////////////////SPI3_GPIO///////////////////////

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;//PC2复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	///////////////////模拟SPI4_GPIO////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;//PC3,4         分别为cs和clk
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PC5 SO
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	//////////////////////模拟SPI1_GPIO////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;//PC3,4         分别为cs和clk
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PC5 SO
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化	
	
/////////////////////////////CS初始化////////////////////////////////////////

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//作为cs1/CS2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//作为cs1/CS2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	
	GPIO_SetBits(GPIOA, GPIO_Pin_1);						// 先把片选拉高，真正用的时候再拉低
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
	GPIO_SetBits(GPIOC, GPIO_Pin_12);
	//这里只针对SPI口初始化
////////////////////////////////////////////////////
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);//复位SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);//停止复位SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,ENABLE);//复位SPI3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,DISABLE);//停止复位SPI3
	
	///////////////////////////SPI1设置/////////////////////////////
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_16;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	
	//SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	//SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	///////////////////////////SPI2设置/////////////////////////////
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
		///////////////////////////SPI3设置/////////////////////////////
	SPI_Init(SPI3, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	SPI_Cmd(SPI3, ENABLE); //使能SPI外设
	/////////////////////////////定时器11，12 初始化//////////////////////////////////////
	TIM12_TIME_Init(4000,8400);
	TIM11_TIME_Init(1000,8400);//50ms中断一次，2.5s一个周期，50段
	}
//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
} 
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节

u16 read3(void)//
{
  unsigned char i=0;
  unsigned long Temp_2=0;//转化后温度
  __nop();
  Cs_L;
	__nop();		
	Clk_L;
	__nop();
  Cs_L;//选中max6675
  for(i=0;i<16;i++)//一帧数据16位
  {
    Clk_H;
    Temp_2<<=1;//??????
        if(So_H==1){Temp_2=Temp_2|0x01;}//???????????1,?temp_2??????????1
        else Temp_2=Temp_2|0x00;
        Clk_L;//SCK1=0;
__nop();
  }
  Cs_H;
  Temp_2=Temp_2<<1;//
  Temp_2=Temp_2>>4;//
  Temp_2=Temp_2*102375/4096-300;//
  return Temp_2/100;//
}
u16 read1(void)//
{
  unsigned char i=0;
  unsigned long Temp_2=0;//转化后温度
  __nop();
  Cs_L1;
	__nop();		
	Clk_L1;
	__nop();
  Cs_L1;//选中max6675
  for(i=0;i<16;i++)//一帧数据16位
  {
    Clk_H1;
    Temp_2<<=1;//??????
        if(So_H1==1){Temp_2=Temp_2|0x01;}//???????????1,?temp_2??????????1
        else Temp_2=Temp_2|0x00;
        Clk_L1;//SCK1=0;
__nop();
  }
  Cs_H1;
  Temp_2=Temp_2<<1;//
  Temp_2=Temp_2>>4;//
  Temp_2=Temp_2*102375/4096-300;//
  return Temp_2/100;//
}

u8 MAX6675_SPI1_ReadWriteByte(u8 TxData)
{		 			 
 
	/* Loop while DR register in not emplty */
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
	
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI1, 0xff);
	
	/* Wait to receive a byte */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1);
}
u8 MAX6675_SPI2_ReadWriteByte(u8 TxData)
{		 			 
 
	/* Loop while DR register in not emplty */
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET);
	
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI2, 0xff);
	
	/* Wait to receive a byte */
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI2);
}
u8 MAX6675_SPI3_ReadWriteByte(u8 TxData)
{		 			 
 
	/* Loop while DR register in not emplty */
	while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE) == RESET);
	
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI3, 0xff);
	
	/* Wait to receive a byte */
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
	
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI3);
}
float max6675_read_be(u32 m){

	unsigned int t=0,i=0;
	unsigned char c=0;
	unsigned char flag=0;
	float tempreture=0;

	switch (m){
		case 1: 

		/*			//CS1=0;
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);	
			c = MAX6675_SPI1_ReadWriteByte(0xff);
			i = c;
			i = i<<8;
			c = MAX6675_SPI1_ReadWriteByte(0xff);
			//CS1=1;
			GPIO_SetBits(GPIOA, GPIO_Pin_1);		
			i = i|((unsigned int)c);			//i是读出来的原始数据
			flag = i&0x04;						//flag保存了热电偶的连接状态
			t = i<<1;
			t = t>>4;
			tempreture = t*0.25;*/
			tempreture= read1();
		break;
		
		case 2: 
			GPIO_ResetBits(GPIOA, GPIO_Pin_7);	
			c = MAX6675_SPI2_ReadWriteByte(0xff);
			i = c;
			i = i<<8;
			c = MAX6675_SPI2_ReadWriteByte(0xff);
			//CS1=1;
			GPIO_SetBits(GPIOA, GPIO_Pin_7);	
			i = i|((unsigned int)c);			//i是读出来的原始数据
			flag = i&0x04;						//flag保存了热电偶的连接状态
			t = i<<1;
			t = t>>4;
			tempreture = t*0.25;
		break;
		case 3: 
			tempreture= read3();
		break;
		case 4: 
			GPIO_ResetBits(GPIOC, GPIO_Pin_12);	
			c = MAX6675_SPI3_ReadWriteByte(0xff);
			i = c;
			i = i<<8;
			c = MAX6675_SPI3_ReadWriteByte(0xff);
			//CS1=1;
			GPIO_SetBits(GPIOC, GPIO_Pin_12);	
			i = i|((unsigned int)c);			//i是读出来的原始数据
			flag = i&0x04;						//flag保存了热电偶的连接状态
			t = i<<1;
			t = t>>4;
			tempreture = t*0.25;
		break;
	}
	return tempreture;
}
void max6675_read(u32 m){
	//int error_sum;
	//u32 overbuffer[3];
	float filter_sum = 0;
	unsigned int i;
	float tempreture;
  thermometer *tp;	
	tempreture=max6675_read_be(m) ;
	switch (m){
		case 1: tp=&thermometer1;
		break;
		case 2: tp=&thermometer2;
		break;
		case 3: tp=&thermometer3;
		break;
		case 4: tp=&thermometer4;
		break;
	}
	
	  /////////////////////滤波//////////////////////////////
		tp->buffer[TFILTER_N - 1] = tempreture;
  if(((tp->buffer[TFILTER_N - 1] - tp->buffer[TFILTER_N - 2]) > TFILTER_A) || ((tp->buffer[TFILTER_N - 2] - tp->buffer[TFILTER_N - 1]) > TFILTER_A)||(tp->buffer[TFILTER_N - 1]<10))
		{//超出范围
			tp->buffer[TFILTER_N - 1] = tp->buffer[TFILTER_N - 2];//当作误差舍弃
			tp->over_cnt++;//每次超出范围就加一
			if(tp->over_cnt>=6){
				tp->buffer[TFILTER_N - 1] = tempreture;//当四次都超出范围后，重新更新buffer[TFILTER_N - 1],把当前温度作为有效值
				tp->over_cnt=0;//计数器清0
			}
		}else {
			tp->over_cnt=0;//计数器清0
		}
    for(i = 0; i < TFILTER_N - 1; i++) {
    tp->buffer[i] =tp->buffer[i + 1];
    filter_sum += tp->buffer[i]; 
  }
		//tp->buffer[TFILTER_N - 2]=filter_sum / (TFILTER_N - 1);
		tp->tempreture= filter_sum / (TFILTER_N - 1);
}


void PIDcontrol(u32 i){
	thermometer *m;	
	pid *P;
	int out;
	switch (i){
		case 1: m=&thermometer1;P=&pid1;;break;
		case 2: m=&thermometer2;P=&pid2;break;
		case 3: m=&thermometer3;P=&pid3;break;
		case 4: m=&thermometer4;P=&pid4;break;
	}
	P->outcal(i);
	out=P->output;
	if(out>0){
		
		if(out<=10)out=10;
		if(m->step<50){//分为50份
				if(m->step <out/2){//原来是100份，现在是50份，所以除2
					Relays.control(m->flag,1);
				}else{
					Relays.control(m->flag,0);
				}
				m->step++;
				
			}else if(m->step>= 50){
				m->step=0;
			}
	}else if(out<=0) {
		Relays.control(m->flag,0);
		//m->step=0;
	}
		
}
void TIM8_BRK_TIM12_IRQHandler(void){

	if(TIM_GetITStatus(TIM12,TIM_IT_Update)==SET) //溢出中断
	{	
		thermometer3.read(3);
		thermometer4.read(4);
		thermometer1.read(1);
		thermometer2.read(2);

		//thermometer3.read(3);
		/*if(show==1){
		printf("%f\n",thermometer1.tempreture);
		}else if (show==2){
		printf("%f\n",thermometer2.tempreture);
		}*/
		//showpid();
		printf("%.3f,%.3f,%.3f,%.3f\n",thermometer1.tempreture,thermometer2.tempreture,thermometer3.tempreture,thermometer4.tempreture);

	}
	TIM_ClearITPendingBit(TIM12,TIM_IT_Update);  //清除中断标志位
}

void TIM1_TRG_COM_TIM11_IRQHandler(void){

	if(TIM_GetITStatus(TIM11,TIM_IT_Update)==SET) //溢出中断
	{
		/*if((motor1.private->isrunning==0)&&(motor3.private->isrunning==0)&&(motor2.private->isrunning==0)){
			 PIDcontrol(1);
			 PIDcontrol(2);
			 PIDcontrol(3);
			 PIDcontrol(4);
		 }else{
			Relays.control(1,0);//电机运作时，继电器关闭，以免一直加热
			Relays.control(2,0);
			Relays.control(3,0);//电机运作时，继电器关闭，以免一直加热
			Relays.control(4,0);
		 }*/
			 PIDcontrol(1);
			 PIDcontrol(2);
			 PIDcontrol(3);
			 PIDcontrol(4);
	} 
	TIM_ClearITPendingBit(TIM11,TIM_IT_Update);  //清除中断标志位
}

void pid_controlopen(u16 i){
	if (i==1){
		TIM_Cmd(TIM11,ENABLE);	//开启tim11 进行pid温控	
	}else if(i==0){
		TIM_Cmd(TIM11,DISABLE); //关闭tim11，并用继电器断开加热
		pid1.iterm=0;
		pid2.iterm=0;
		pid1.derror=0;
		pid2.derror=0;
		pid3.iterm=0;
		pid4.iterm=0;
		pid3.derror=0;
		pid4.derror=0;
		Relays.control(1,0);
		Relays.control(2,0);
		Relays.control(3,0);
		Relays.control(4,0);
	}
}

