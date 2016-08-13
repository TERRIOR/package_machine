#include "bluebooth.h"
#include "sys.h"
#include "usart3.h" 	
#include "lcd.h"
  

u8* receiver(){
	u8 reclen=0;
	if(USART3_RX_STA&0X8000)			//���յ�һ��������
		{
			LCD_Fill(30,200,240,320,WHITE);	//�����ʾ
 			reclen=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
		  	USART3_RX_BUF[reclen]=0;	 	//���������
					LCD_ShowString(30,200,209,119,16,USART3_RX_BUF);//��ʾ���յ�������
 			USART3_RX_STA=0;	 
	  		
				return USART3_RX_BUF;
		}
}