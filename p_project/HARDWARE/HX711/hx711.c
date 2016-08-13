#include "hx711.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"

hx711 hx711_1={
	0.0,
	21686,
	0.00229333,
	{0},
	0,0,0.0,	
	hxinit,
	Read_HX711_fin,
	hx_set
};


void hxinit(){

	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	hx711_1.buffer[WFILTER_N - 2] =0;

}
unsigned long Read_HX711(void){

	unsigned long val = 0;
	unsigned char i = 0;
	
	GPIO_SetBits(GPIOF,GPIO_Pin_3); //DOUT
	//DT=1;
	GPIO_ResetBits(GPIOF,GPIO_Pin_4);
	//SCK=0;
	while(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3));
	delay_us(1);
	
	for(i=0;i<24;i++)
	{
		GPIO_SetBits(GPIOF,GPIO_Pin_4);
		//SCK=1;
		val = val << 1;
		delay_us(1);
		GPIO_ResetBits(GPIOF,GPIO_Pin_4);
		//SCK=0;
		if(	GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3))
		val++;		
		delay_us(2);	
	}
	GPIO_SetBits(GPIOF,GPIO_Pin_4);
	//SCK=1;
	delay_us(2);
	GPIO_ResetBits(GPIOF,GPIO_Pin_4);
	//SCK=0;
	delay_us(2);
	return val;

}

void Read_HX711_fin(u32 i){
	hx711 *w;
	float fin,filter_sum ;
	u32 be;
	switch(i){
		case 1:w=&hx711_1;break ;
	} 
	be=Read_HX711();
	//printf(" %d  %f\n",w->offset,w->cofficient); 
	if(be>w->offset){
		fin=(be - w->offset)* w->cofficient;
	}else {
		fin =0;
	}
		w->buffer[WFILTER_N - 1] =fin;
	//printf ("%f",w->buffer[WFILTER_N - 1]);
	//�˴���֪Ϊ�β��ܼӽ�ȥ
		/*if(((w->buffer[WFILTER_N - 1] - w->buffer[WFILTER_N - 2]) < 0.5) || ((w->buffer[WFILTER_N - 2] - w->buffer[WFILTER_N - 1])< 0.5))
		{
				w->buffer[WFILTER_N - 1] = w->buffer[WFILTER_N - 2];//�����������
		}*/
    for(i = 0; i < WFILTER_N - 1; i++) {
    w->buffer[i] =w->buffer[i + 1];
    filter_sum += w->buffer[i]; 
  }
	
		w->lastweight=w->weight;
		//w->buffer[WFILTER_N - 2]=filter_sum / (WFILTER_N - 1);
		w->weight=filter_sum / (WFILTER_N - 1);
	//weighfilter(m,fin);
	//printf("%d  %.2f weight:%.2f  %.1f  %.1f\n",be,fin,w->weight,filter_sum ,w->buffer[WFILTER_N - 1]);
	//printf("weight:%.1f  ",w->weight);
}

void weighfilter(hx711 *w, float j){
	
	int i;
	double  filter_sum = 0;
		w->buffer[WFILTER_N - 1] = (double )j;
		if(((w->buffer[WFILTER_N - 1] - w->buffer[WFILTER_N - 2]) < WFILTER_A) || ((w->buffer[WFILTER_N - 2] - w->buffer[WFILTER_N - 1])< WFILTER_A))
		{
				w->buffer[WFILTER_N - 1] = w->buffer[WFILTER_N - 2];//�����������
		}
		
    for(i = 0; i < WFILTER_N - 1; i++) {
    w->buffer[i] =w->buffer[i + 1];
    filter_sum += w->buffer[i]; 
  }
		w->lastweight=w->weight;
		//w->buffer[WFILTER_N - 2]=filter_sum / (WFILTER_N - 1);
		w->weight= (float)filter_sum / (WFILTER_N - 1);
}

void isstabled(u32 i){//�ж��Ƿ��ȶ�
	hx711 *w;
	switch(i){
		case 1:w=&hx711_1;break;
	} 
		if(((w->weight - w->lastweight) <2) || ((w->weight - w->lastweight) <2)){//��ֵС��2����
			if(w->over_cnt>=10){//�����������10����˵���ȶ� ���Ҳ���������
				w->stable=1;//�ȶ�
			}else {
				w->over_cnt++;//
			}
		}else{
			w->over_cnt=0;
			w->stable=0;//���ȶ�
		}	
}


void hx_set(u32 i,u32 off,float coff){
	hx711 *w;
	switch(i){
		case 1:w=&hx711_1;break;
	}
	w->cofficient=coff;
	w->offset=off;
	printf("offset:%d  coff:%f\n",w->offset,w->cofficient);
}
void hx_cal(){
	int n;
	float i;
	u32 j;
	for(n=0;n<10;n++){
		j=Read_HX711();
		i+=100/(j-hx711_1.offset);
	}
	hx711_1.cofficient=i/10;
	printf("���¼��������:%f\n",hx711_1.cofficient);
}
void returnzero(){
//	u32 n;
//	u32 m;

	hx711_1.offset=Read_HX711();
	printf("ȥƤ��offset��%d\n",hx711_1.offset);
}

