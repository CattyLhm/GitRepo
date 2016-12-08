#ifndef _PID_H
#define _PID_H

extern int ADC_Speed;

extern void Speed_PIDInit(void);
extern int Speed_PIDAdjust(int NextPoint);

#endif
