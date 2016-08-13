#include "bluebooth.h"
#include "sys.h"
#include "usart3.h" 	
#include "lcd.h"
  

u8* receiver(){
	u8 reclen=0;
	if(USART3_RX_STA&0X8000)			//接收到一次数据了
		{
			LCD_Fill(30,200,240,320,WHITE);	//清除显示
 			reclen=USART3_RX_STA&0X7FFF;	//得到数据长度
		  	USART3_RX_BUF[reclen]=0;	 	//加入结束符
					LCD_ShowString(30,200,209,119,16,USART3_RX_BUF);//显示接收到的数据
 			USART3_RX_STA=0;	 
	  		
				return USART3_RX_BUF;
		}
}