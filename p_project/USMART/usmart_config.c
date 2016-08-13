#include "usmart.h"
#include "usmart_str.h"
////////////////////////////用户配置区///////////////////////////////////////////////
//这下面要包含所用到的函数所申明的头文件(用户自己添加) 
#include "delay.h"	 	
#include "sys.h"
#include "motorlei.h"
#include "PID.h"
#include "max6675.h"	
#include "dcmotor.h"
extern void led_set(u8 sta);
extern void test_fun(void(*ledset)(u8),u8 sta);
extern void setstart(u16 i);
//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
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
//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	1,		//参数显示类型,0,10进制;1,16进制
	0,		//参数类型.bitx:,0,数字;1,字符串	    
	0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	0,		//函数的参数,需要PARM_LEN个0初始化
};   



















