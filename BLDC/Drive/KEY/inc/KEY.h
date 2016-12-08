#ifndef _KEY_H
#define _KEY_H

#define KEY1	1
#define KEY2    2
#define SW2		3

extern void KEY_Init(void);
extern uint8_t KEY_Read(uint8_t key);
extern uint8_t SW__Read(void);

#endif
