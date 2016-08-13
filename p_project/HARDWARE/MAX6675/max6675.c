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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  	//TIM13ʱ��ʹ��    
  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseStructure);//��ʼ����ʱ��13
	TIM_ARRPreloadConfig(TIM12, ENABLE);
   TIM_ClearITPendingBit(TIM12,TIM_IT_Update);
	TIM_ITConfig(TIM12,TIM_IT_Update,ENABLE); //����ʱ��13�����ж�	
	
	TIM_Cmd(TIM12, ENABLE);  //ʹ��TIM13
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_BRK_TIM12_IRQn; //��ʱ13�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	 
	

} 
void TIM11_TIME_Init(u16 arr,u16 psc)
{		

NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);  	//TIM13ʱ��ʹ��    
  //////////////////////////ע��APB2!!
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM11,&TIM_TimeBaseStructure);//��ʼ����ʱ��13
	TIM_ARRPreloadConfig(TIM11, ENABLE);
   TIM_ClearITPendingBit(TIM11,TIM_IT_Update);
	TIM_ITConfig(TIM11,TIM_IT_Update,ENABLE); //����ʱ��13�����ж�	
	
	TIM_Cmd(TIM11, ENABLE);  //ʹ��TIM13
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_TRG_COM_TIM11_IRQn; //��ʱ13�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	 
	

} 
void SPI_MAX6675_Init(void)
{
  
	GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOCʱ��
 // RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//ʹ��SPI1ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//ʹ��SPI2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);//ʹ��SPI3ʱ��
	//////////////////SPI2����////////////////
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); //PB13����Ϊ SPI2 SCLK
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource2,GPIO_AF_SPI2);
	/////////////////SPI1����/////////////////
	/*	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); //PA5����Ϊ SPI1 SCLK
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); //PA6����Ϊ SPI1 SO
		
*//////////////////SPI3����/////////////////
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_SPI3); //PB3����Ϊ SPI1 SCLK
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_SPI3); //PB4����Ϊ SPI1 SO
  //GPIOFA5��6��ʼ������
	///////////////////////////SPI1-GPIO///////////////////
	/*  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;//PA5��6��7���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��*/

//////////////////SPI2_GPIO///////////////////////
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//PB13���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PC2���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	
	//////////////////SPI3_GPIO///////////////////////

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;//PC2���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	///////////////////ģ��SPI4_GPIO////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;//PC3,4         �ֱ�Ϊcs��clk
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PC5 SO
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	//////////////////////ģ��SPI1_GPIO////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;//PC3,4         �ֱ�Ϊcs��clk
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PC5 SO
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��	
	
/////////////////////////////CS��ʼ��////////////////////////////////////////

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//��Ϊcs1/CS2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//��Ϊcs1/CS2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	
	GPIO_SetBits(GPIOA, GPIO_Pin_1);						// �Ȱ�Ƭѡ���ߣ������õ�ʱ��������
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
	GPIO_SetBits(GPIOC, GPIO_Pin_12);
	//����ֻ���SPI�ڳ�ʼ��
////////////////////////////////////////////////////
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//��λSPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//ֹͣ��λSPI1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);//��λSPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);//ֹͣ��λSPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,ENABLE);//��λSPI3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,DISABLE);//ֹͣ��λSPI3
	
	///////////////////////////SPI1����/////////////////////////////
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_16;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	
	//SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	//SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	///////////////////////////SPI2����/////////////////////////////
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ��� 
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
		///////////////////////////SPI3����/////////////////////////////
	SPI_Init(SPI3, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	SPI_Cmd(SPI3, ENABLE); //ʹ��SPI����
	/////////////////////////////��ʱ��11��12 ��ʼ��//////////////////////////////////////
	TIM12_TIME_Init(4000,8400);
	TIM11_TIME_Init(1000,8400);//50ms�ж�һ�Σ�2.5sһ�����ڣ�50��
	}
//SPI1�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2ʱ��һ��Ϊ84Mhz��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
} 
//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�

u16 read3(void)//
{
  unsigned char i=0;
  unsigned long Temp_2=0;//ת�����¶�
  __nop();
  Cs_L;
	__nop();		
	Clk_L;
	__nop();
  Cs_L;//ѡ��max6675
  for(i=0;i<16;i++)//һ֡����16λ
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
  unsigned long Temp_2=0;//ת�����¶�
  __nop();
  Cs_L1;
	__nop();		
	Clk_L1;
	__nop();
  Cs_L1;//ѡ��max6675
  for(i=0;i<16;i++)//һ֡����16λ
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
			i = i|((unsigned int)c);			//i�Ƕ�������ԭʼ����
			flag = i&0x04;						//flag�������ȵ�ż������״̬
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
			i = i|((unsigned int)c);			//i�Ƕ�������ԭʼ����
			flag = i&0x04;						//flag�������ȵ�ż������״̬
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
			i = i|((unsigned int)c);			//i�Ƕ�������ԭʼ����
			flag = i&0x04;						//flag�������ȵ�ż������״̬
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
	
	  /////////////////////�˲�//////////////////////////////
		tp->buffer[TFILTER_N - 1] = tempreture;
  if(((tp->buffer[TFILTER_N - 1] - tp->buffer[TFILTER_N - 2]) > TFILTER_A) || ((tp->buffer[TFILTER_N - 2] - tp->buffer[TFILTER_N - 1]) > TFILTER_A)||(tp->buffer[TFILTER_N - 1]<10))
		{//������Χ
			tp->buffer[TFILTER_N - 1] = tp->buffer[TFILTER_N - 2];//�����������
			tp->over_cnt++;//ÿ�γ�����Χ�ͼ�һ
			if(tp->over_cnt>=6){
				tp->buffer[TFILTER_N - 1] = tempreture;//���Ĵζ�������Χ�����¸���buffer[TFILTER_N - 1],�ѵ�ǰ�¶���Ϊ��Чֵ
				tp->over_cnt=0;//��������0
			}
		}else {
			tp->over_cnt=0;//��������0
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
		if(m->step<50){//��Ϊ50��
				if(m->step <out/2){//ԭ����100�ݣ�������50�ݣ����Գ�2
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

	if(TIM_GetITStatus(TIM12,TIM_IT_Update)==SET) //����ж�
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
	TIM_ClearITPendingBit(TIM12,TIM_IT_Update);  //����жϱ�־λ
}

void TIM1_TRG_COM_TIM11_IRQHandler(void){

	if(TIM_GetITStatus(TIM11,TIM_IT_Update)==SET) //����ж�
	{
		/*if((motor1.private->isrunning==0)&&(motor3.private->isrunning==0)&&(motor2.private->isrunning==0)){
			 PIDcontrol(1);
			 PIDcontrol(2);
			 PIDcontrol(3);
			 PIDcontrol(4);
		 }else{
			Relays.control(1,0);//�������ʱ���̵����رգ�����һֱ����
			Relays.control(2,0);
			Relays.control(3,0);//�������ʱ���̵����رգ�����һֱ����
			Relays.control(4,0);
		 }*/
			 PIDcontrol(1);
			 PIDcontrol(2);
			 PIDcontrol(3);
			 PIDcontrol(4);
	} 
	TIM_ClearITPendingBit(TIM11,TIM_IT_Update);  //����жϱ�־λ
}

void pid_controlopen(u16 i){
	if (i==1){
		TIM_Cmd(TIM11,ENABLE);	//����tim11 ����pid�¿�	
	}else if(i==0){
		TIM_Cmd(TIM11,DISABLE); //�ر�tim11�����ü̵����Ͽ�����
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

