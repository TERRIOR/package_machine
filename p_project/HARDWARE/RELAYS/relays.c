#include "relays.h"
#include "sys.h"
u32 isopen[4]={0,0,0,0};
u32 isopened(u32 i){//���ؼ̵����Ƿ��
	u32 j=0;
	j=isopen[i];
	return j;
}
void gpio_init(){//io�ڳ�ʼ��
  GPIO_InitTypeDef  GPIO_InitStructure;
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOGʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_5; //
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��

	GPIO_ResetBits(GPIOG,GPIO_Pin_2);
	GPIO_ResetBits(GPIOG,GPIO_Pin_3);
	GPIO_ResetBits(GPIOG,GPIO_Pin_5);
	GPIO_ResetBits(GPIOG,GPIO_Pin_6);
}
void relays_Init(){//�̵�����ʼ��
	gpio_init();
	
}
void relays_control(u32 i,u32 j){//���Ƽ̵��� i:����ڼ����̵�����j�����͹�
	if(j==1){
		isopen[i]=1;
		switch(i){
			case 1:
				GPIO_SetBits(GPIOG,GPIO_Pin_2);
			break;
			case 2:
				GPIO_SetBits(GPIOG,GPIO_Pin_3);
			break;
			case 3:
				GPIO_SetBits(GPIOG,GPIO_Pin_5);
			break;
			case 4:
				GPIO_SetBits(GPIOG,GPIO_Pin_6);
			break;
	}
	}else if (j==0){
		isopen[i]=0;
		switch(i){
			case 1:
				GPIO_ResetBits(GPIOG,GPIO_Pin_2);	
			break;
			case 2:
				GPIO_ResetBits(GPIOG,GPIO_Pin_3);	
			break;
			case 3:
				GPIO_ResetBits(GPIOG,GPIO_Pin_5);	
			break;
			case 4:
				GPIO_ResetBits(GPIOG,GPIO_Pin_6);		
			break;
	}
	}
}
RELAYS Relays={
	relays_Init,
	isopened,
	relays_control

};

