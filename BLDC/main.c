#include "includes.h"
/********************CoOS变量**********************/
#define TASK_STK_SIZE 	128

#define TASK0_PRIO	2
#define TASK1_PRIO  3
#define TASK2_PRIO  4
#define TASK3_PRIO  5


OS_STK STK_TASK0[TASK_STK_SIZE];
OS_STK STK_TASK1[TASK_STK_SIZE];
OS_STK STK_TASK2[TASK_STK_SIZE];
OS_STK STK_TASK3[TASK_STK_SIZE];

void TASK0(void *param);
void TASK1(void *param);
void TASK2(void *param);
void TASK3(void *param);
/*************************************************/

/*********************一般变量********************/
extern uint32_t Speed_count;
uint8_t USART_Flag = 0;

/*************************************************/
int main(void)
{
	/* 片内外设初始化 */	
	Periph_Init();
	
	/* 操作系统初始化 */
    CoInitOS();

	CoCreateTask( TASK0, (void*)0, TASK0_PRIO, &STK_TASK0[TASK_STK_SIZE - 1], TASK_STK_SIZE);
	CoCreateTask( TASK1, (void*)0, TASK1_PRIO, &STK_TASK1[TASK_STK_SIZE - 1], TASK_STK_SIZE);
	CoCreateTask( TASK2, (void*)0, TASK2_PRIO, &STK_TASK2[TASK_STK_SIZE - 1], TASK_STK_SIZE);
	CoCreateTask( TASK3, (void*)0, TASK3_PRIO, &STK_TASK3[TASK_STK_SIZE - 1], TASK_STK_SIZE);
	CoStartOS();
    while(1);
}

void TASK0(void *param)
{
   int ADC_value_1= 2;
  uint8_t start_flag=0;
  uint8_t sw_state = 1;
  KEY_Init();
  Speed_PIDInit();			
  for(;;)
  {
  	//SW__Read()=1 	 Direction = SET
    if(SW__Read()^sw_state)
	{
	   if(start_flag)
	   {
		   BLDC_Stop();
	   }
	   sw_state = SW__Read();
	   if(sw_state)
	   {
	   		Direction = SET;
			
	   }
	   else
	   {
	   		Direction = RESET;
			
	   }
	   if(start_flag)
	   {
	   	  BLDC_Start();

	   }
	   
	} 
    if(KEY_Read(KEY1))
	{
		CoTickDelay(5);
		if(KEY_Read(KEY1))
		{
		   BLDC_Start();
		   start_flag = 1;	

		    if (ADC_value_1>=11)ADC_value_1=2;
		   ADC_value_1=ADC_value_1+1;
		  	Speed_Adjust(ADC_value_1); 
         
		}
	}
	if(KEY_Read(KEY2))
	{
		CoTickDelay(5);
		if(KEY_Read(KEY2))
		{
		   BLDC_Stop();
		   Flaghuanxiang();
		   
		   start_flag = 0;
		   if (ADC_value_1>=12)ADC_value_1=2;
		   Speed_Adjust(ADC_value_1); 

		}
	}
	CoTickDelay(10);
  }			
}
void TASK1(void *param)
{
  uint16_t adc_value= 0;
  uint8_t i = 0; 
  for(;;)
  {
  	   adc_value= 0;
       for(i=0;i<4;i++)
	   {
	   		ADC_SoftwareStartConvCmd(ADC1,ENABLE);    /* 使能转换开始 */
	   		while(ADC_GetSoftwareStartConvStatus(ADC1));
			adc_value += ADC_GetConversionValue(ADC1);
	   }
	   ADC_Speed = adc_value/4;
	   CoTickDelay(10);
  }
}
void TASK2(void *param)
{
  uint16_t data;
  for(;;)
  {
  	if(USART_Flag)
	{
    	data = 1000000/(6*Speed_count);
		USART_SendData( USART2, data);
		USART_Flag = 0;
	}
	CoTickDelay(1);
  }
}
void TASK3(void *param)
{
	LED_Off();
  for(;;)
  {
	   LED_On();
	   CoTickDelay(200);
	   LED_Off();
	   CoTickDelay(200);
  }
}
