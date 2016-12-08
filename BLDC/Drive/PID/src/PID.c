/*==================================================================================================== 
						增量式PID
The PID (比例、积分、微分) function is used in mainly 
control applications. PIDCalc performs one iteration of the PID 
algorithm. 
While the PID function works, main is just a dummy program showing 
a typical usage. 
=====================================================================================================*/
#define ADC_Speed_Max   4096	 //800 3500
#define ADC_Speed_Min   100
#define PWM_Max			1400
#define PWM_Min     	50

int ADC_Speed = 0;
   
typedef struct PI 
{ 
	int Target;     //设定目标 Desired Value
	int Uk;			//Uk
	int Udk;		//Udk增量
	int Uk_1; 		//Uk-1
	double P; 		//比例常数 Proportional Const 
	double I; 		//积分常数 Integral Const
	int    b; 
	int ek_0;		//ek 
	int ek_1; 		//ek-1 
	int ek_2; 		//ek-2 
}PI;
static PI Speed_PID; 
static PI *Speed_Point = &Speed_PID; 

static int ADC_Speed_1 = 0;
/*==================================================================================================== 
Initialize PID Structure PID参数初始化
=====================================================================================================*/ 
void Speed_PIDInit(void) 
{ 
	Speed_Point->Target = 0;
	Speed_Point->Uk		= 0;
	Speed_Point->Udk    = 0;
	Speed_Point->Uk_1   = PWM_Min;
	Speed_Point->ek_0 	= 0;	 //ek=0
	Speed_Point->ek_1 	= 0;	 //ek-1=0
	Speed_Point->ek_2 	= 0; 	 //ek-2=0
	Speed_Point->P 		= 0.65;  //比例常数 Proportional Const 
	Speed_Point->I 		= 0.002; 	 //积分常数Integral Const
	Speed_Point->b      = 0; 
}
/*==================================================================================================== 
增量式PID计算部分 
=====================================================================================================*/ 

 int Speed_Adjust(int Next_Point) 
{
	
	 
	  
	   ADC_Speed_1= ADC_Speed_1+60;
	   if(ADC_Speed_1>=1300)ADC_Speed_1 =200;

   return(ADC_Speed_1);

}

 int Speed_Adjust_1() 
{
	  return(ADC_Speed_1);

}


int Speed_PIDAdjust(int Next_Point) 
{
	if(ADC_Speed < ADC_Speed_Min)
	{
	 	ADC_Speed = ADC_Speed_Min;		
	}
	else if(ADC_Speed > ADC_Speed_Max)
	{
		ADC_Speed = ADC_Speed_Max;
	}
	Speed_Point->Target = ADC_Speed; 
	Speed_Point->ek_0= Speed_Point->Target - Next_Point; 	//增量计算
	if(((Speed_Point->Uk_1>=PWM_Max)&&(Speed_Point->ek_0>=0))||((Speed_Point->Uk_1<=PWM_Min)&&(Speed_Point->ek_0<=0)))
	{
	    Speed_Point->b=0;
	} 
	else
	{
		Speed_Point->b=1;
	} 
	Speed_Point->Udk=Speed_Point->P*(Speed_Point->ek_0-Speed_Point->ek_1) + Speed_Point->b*Speed_Point->I*Speed_Point->ek_0 ;
    /* 存储误差，用于下次计算 */
	Speed_Point->Uk = Speed_Point->Uk_1 + Speed_Point->Udk;
	Speed_Point->ek_2 =	Speed_Point->ek_1;
	Speed_Point->ek_1 = Speed_Point->ek_0;
	Speed_Point->Uk_1 = Speed_Point->Uk; 
    if(Speed_Point->Uk >= PWM_Max)
	{
		return PWM_Max;
	}
	else if(Speed_Point->Uk <= PWM_Min)
	{
		return PWM_Min;
	} 
	return(Speed_Point->Uk); 
}
