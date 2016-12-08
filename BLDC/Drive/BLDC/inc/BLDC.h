#ifndef BLDC_H
#define BLDC_H
extern FlagStatus Direction;

extern void BLDC_GPIOConfig(void);
extern void BLDC_TIM1Config(void);
extern void BLDC_TIM3Config(void);
extern void huanxiang(void);
extern void BLDC_Start(void);
extern void BLDC_Stop(void);
#endif
