#include "usmart.h"
#include "usmart_str.h"
////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����) 
#include "delay.h"	 	
#include "sys.h"
#include "motorlei.h"
#include "PID.h"
#include "max6675.h"	
#include "dcmotor.h"
extern void led_set(u8 sta);
extern void test_fun(void(*ledset)(u8),u8 sta);
extern void setstart(u16 i);
//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif		
  (void*)MOTOR_SecondRun,"MOTOR_SecondRun(u32 dir,u32 j)",
	(void*)MOTOR_Sec_calculate,"MOTOR_Sec_calculate(u32 j,u32 speed,u32 distance);",
	(void*)MOTOR_run,"void MOTOR_run(u32 dir,u32 j)",
	(void*)MOTOR_calculate,"void MOTOR_calculate(u32 j)" ,
	(void*)motorinit,"void  motorinit(u32 FLAG,u32 SPEED,u32 DIRlei,u32 ACC,u32 DEC,u32 DIS)",
	(void*)setpid,"void setpid(u32 j,u32 p,u32 i,u32 d,u32 input)",
	(void*)showpid,"void showpid()",
	(void*)Dcmotor_set,"void Dcmotor_set(u16 i,u16 dir,u32 speed)",
	(void*)movetoinit,"void movetoinit(u32 j,u32 speed)",
	(void*)pid_controlopen,"void pid_controlopen(u16 i)",
	(void*)Dcmotor_run,"void Dcmotor_run(u16 i)",
	(void*)setstart,"void setstart(u16 i)",
	(void*)downtolight,"void downtolight(u32 speed)",
	(void*)uptostart,"void uptostart(u32 speed)"
};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   



















