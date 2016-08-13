#ifndef PID__H
#define PID__H
#include "sys.h"
#define T 0.05
typedef struct PID pid;
struct PID {
	
	float kp;
	float ki;
	float kd;
	int input;
	int error;
	int iterm;
	int derror;
	int lasterror;
	int output; 
	
	void (*set)(u32 j,u32 p,u32 i,u32 d,u32 input);
	void (*outcal)(u32 j);
};
extern pid pid1,pid2,pid3,pid4;
void showpid(void);
void setpid(u32 j,u32 p,u32 i,u32 d,u32 input);
void pidoutput(u32 j);
#endif

