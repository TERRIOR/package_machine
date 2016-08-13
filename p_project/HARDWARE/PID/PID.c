#include "PID.h"
#include "sys.h"
#include "max6675.h"
#include "usart.h"

pid pid1={
	0,0,0,0,0,0,0,0,0,
	setpid,
	pidoutput
};
pid pid2={
	0,0,0,0,0,0,0,0,0,
	setpid,
	pidoutput
};
pid pid3={
	0,0,0,0,0,0,0,0,0,
	setpid,
	pidoutput
};
pid pid4={
	0,0,0,0,0,0,0,0,0,
	setpid,
	pidoutput
};
void setpid(u32 j,u32 p,u32 i,u32 d,u32 input){
	pid *m;
	switch(j){
		case 1:m=&pid1;break;
		case 2:m=&pid2;break;
		case 3:m=&pid3;break;
		case 4:m=&pid4;break;
	}
	m->kp=(float)p/100;
	m->ki=(float)i/10000;
	m->kd=(float)d/10000;
	m->input=input;
}
int limit(int i,int max,int min){
	if(i>max)i=max;
	if(i<min)i=min;
	return i;
}

void pidoutput(u32 j){
	pid *m;
	thermometer *t;
	switch(j){
		case 1:m=&pid1;t=&thermometer1;break;
		case 2:m=&pid2;t=&thermometer2;break;
		case 3:m=&pid3;t=&thermometer3;break;
		case 4:m=&pid4;t=&thermometer4;break;
	}
	m->error=m->input -(int)t->tempreture;//²î
	//printf("error:%d\n",m->error);
	m->iterm+=m->error*T;//»ý·
	//printf("iterm:%d\n",m->iterm);
	m->derror=(m->error-m->lasterror)/T;//Îó²îÎ¢·Ö
	//printf("derror:%d\n",m->derror);
	m->lasterror=m->error;
	
	m->output =m->kp*m->error + m->ki*m->iterm - m->kd*m->derror;
	//printf("out: %d\n",m->output);
  m->output=limit(m->output,70,-70);
}
void showpid(){
	printf(" 1p:%.2f i:%.4f d:%.4f out:%d in:%d\n",pid1.kp,pid1.ki,pid1.kd,pid1.output,pid1.input);
	printf(" 2p:%.2f i:%.4f d:%.4f out:%d in:%d\n",pid2.kp,pid2.ki,pid2.kd,pid2.output,pid2.input);
	printf(" 3p:%.2f i:%.4f d:%.4f out:%d in:%d\n",pid3.kp,pid3.ki,pid3.kd,pid3.output,pid3.input);
	printf(" 4p:%.2f i:%.4f d:%.4f out:%d in:%d\n",pid4.kp,pid4.ki,pid4.kd,pid4.output,pid4.input);
}

