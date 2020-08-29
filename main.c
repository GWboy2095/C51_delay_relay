
/* --- STC12C5Axx Series 16-bit Timer 1 Demo -------------------------*/


#include "reg51.h"
//#include <stdbool.h>
//#include <stdint.h>
#include "intrins.h"
typedef unsigned char BYTE;
typedef unsigned int WORD;

//-----------------------------------------------

/* define constants */
#define FOSC 11059200L
#define MODE1T                      //Timer clock mode, comment this line is 12T mode, uncomment is 1T mode

#ifdef MODE1T
 #define T1MS (65536-FOSC/1000)      //1ms timer calculation method in 1T mode
#else
 #define T1MS (65536-FOSC/12/1000)   //1ms timer calculation method in 12T mode
#endif

/* define SFR */
sfr AUXR = 0x8e;                    //Auxiliary register
sbit IN_CH_0 = P0^0;               //work LED, flash once per second
sbit IN_CH_1 = P0^1;

sbit OUT_CH_0 = P1^0;
sbit OUT_CH_1 = P1^2;


sbit Key_1 = P0^4;
sbit Key_2 = P0^5;
sbit Key_3 = P0^6;
/* define variables */
WORD delay_X0;                         //1000 times counter
WORD delay_X1;
BYTE OUT_0_Close=0;
BYTE OUT_1_Close=0;
WORD delay = 2000;
//-----------------------------------------------

/* Timer0 interrupt routine */
void tm1_isr() interrupt 3 using 1
{
    TL1 = T1MS;                     //reload timer1 low byte
    TH1 = T1MS >> 8;                //reload timer1 high byte
	if(delay_X0>0)delay_X0--;
    if (delay_X0 == 1)               //1ms * 1000 -> 1s
    {
			OUT_0_Close ++;
			//OUT_1_Close = 0;
    }
	if(delay_X1>0)delay_X1--;	
		if (delay_X1 == 1)
		{
//			OUT_1_Close = 1;
			OUT_0_Close = 2;
			delay_X0 = delay;
		}
}

//-----------------------------------------------

/* main program */
void main()
{
	static BYTE Last_X0 = 1,Last_X1 = 1;
#ifdef MODE1T
    AUXR = 0x40;                    //timer1 work in 1T mode
#endif
    TMOD = 0x10;                    //set timer1 as mode1 (16-bit)
    TL1 = T1MS;                     //initial timer1 low byte
    TH1 = T1MS >> 8;                //initial timer1 high byte
    TR1 = 1;                        //timer1 start running
    ET1 = 1;                        //enable timer1 interrupt
    EA = 1;                         //open global interrupt switch
//    count = 0;                      //initial counter

    while (1)                      //loop
		{
		if(!Key_1)
			{
				delay = 500;
			}
			if(!Key_2)
			{
				delay = 1000;
			}
			if(!Key_3)
			{
				delay = 2000;
			}

			if(!IN_CH_0 && IN_CH_0 != Last_X0 && IN_CH_1)
			{
				delay_X0 = delay;
				OUT_0_Close = 0;
			}
			else if(!IN_CH_0 && IN_CH_0 != Last_X0 && !IN_CH_1)
			{
				delay_X1 = delay;				
			}
			else if(!IN_CH_1 && IN_CH_1 != Last_X1 && !IN_CH_0)
			{
				delay_X1 = delay;
			}
			else if(IN_CH_1 && IN_CH_1 != Last_X1 && !IN_CH_0)
			{
				OUT_0_Close = 1;
			}
			Last_X0 = IN_CH_0;
			Last_X1 = IN_CH_1;	
			
			
		if(IN_CH_0)
		{
			OUT_CH_0 = 1;
			OUT_CH_1 = 1;
			OUT_0_Close = 0;
		}
		if(IN_CH_1)
		{
			OUT_CH_1 = 1;
		}
		
	
			if(!IN_CH_0&&OUT_0_Close == 1)
			{
				OUT_CH_0 = 0;
			} 
			if (!IN_CH_0 && !IN_CH_1 && OUT_0_Close == 2 )
			{
				OUT_CH_0 = 1;
				OUT_CH_1 = 0;
				//OUT_1_Close = 0;
			}
		 if (!IN_CH_0 && !IN_CH_1 && OUT_0_Close == 3 )
			{
				OUT_CH_0 = 0;
				OUT_CH_1 = 0;
			}	
			
//			if(!IN_CH_1)
//			{
//				OUT_CH_1 = 0;
//			}
//			else if(IN_CH_1&&OUT_1_Close)
//			{
//				OUT_CH_1 = 1;
//				//OUT_1_Close = 0;
//			}
//			
//			if(IN_CH_0 && IN_CH_0 != Last_X0)
//			{
//				delay_X0 = delay*3;
//				OUT_0_Close = 0;
//			}
//			if(IN_CH_1 && IN_CH_1 != Last_X1)
//			{
//				delay_X1 = delay;
//				OUT_1_Close = 0;
//			}			
//			Last_X0 = IN_CH_0;
//			Last_X1 = IN_CH_1;
		}
}

