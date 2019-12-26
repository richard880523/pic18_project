/*
 * File:   PIC_DC_Motor.c
 * Author: Suraj
 *
 * Created on May 15, 2017, 7:45 PM
 */
#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

char str[20];
double ws_double, ad_double;
//from lab11 file

#include <math.h>
//#include <stdio.h>
//#include <pic18f4550.h>
#include "ADC_Header_File_1.h"
//#include "Configuration_header_file_1.h"

#define MINTHR              8000
#define RESOLUTION          488

#define InternalOsc_8MHz    8000000
#define InternalOsc_4MHz    4000000
#define InternalOsc_2MHz    2000000
#define InternalOsc_1MHz    1000000
#define InternalOsc_500KHz  500000
#define InternalOsc_250KHz  250000
#define InternalOsc_125KHz  125000
#define InternalOsc_31KHz   31000

#define Timer2Prescale_1    1
#define Timer2Prescale_4    4
#define Timer2Prescale_16   16

void MSdelay(unsigned int val)
{
 unsigned int i,j;
 for (i=0; i<=val; i++)
     for (j=0; j<165; j++);	/* Delay count for 1ms for 8MHz freq. */
}

void PWM_Init()             /* Initialize PWM */
{
    TRISCbits.TRISC1 = 0;   /* Set CCP2 pin as output for PWM out */
    CCP2CON = 0x0C;         /* Set PWM mode */
    TRISCbits.TRISC2 = 0;   /* Set CCP1 pin as output for PWM out */
    CCP1CON = 0x0C;         /* Set PWM mode */
}

int setPeriodTo(unsigned long FPWM)/* Set period */
{
    int clockSelectBits, TimerPrescaleBits;
    int TimerPrescaleValue;
    float period;
    unsigned long FOSC, _resolution = RESOLUTION;

    if (FPWM < MINTHR)    {TimerPrescaleBits = 2; TimerPrescaleValue = Timer2Prescale_16;}
    else                  {TimerPrescaleBits = 0; TimerPrescaleValue = Timer2Prescale_1;}

//    if (FPWM > _resolution)               {clockSelectBits = 7; FOSC = InternalOsc_8MHz;}
//    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 6; FOSC = InternalOsc_4MHz;}
//    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 5; FOSC = InternalOsc_2MHz;}
//    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 4; FOSC = InternalOsc_1MHz;}
//    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 3; FOSC = InternalOsc_500KHz;}
//    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 2; FOSC = InternalOsc_250KHz;}
//    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 1; FOSC = InternalOsc_125KHz;}
//    else                                  {clockSelectBits = 0; FOSC = InternalOsc_31KHz;}
    _resolution >>= 1;
    clockSelectBits = 6;
    FOSC = InternalOsc_4MHz;
    period = ((float)FOSC / (4.0 * (float)TimerPrescaleValue * (float)FPWM)) - 1.0;
    period = round(period);

    OSCCON = ((clockSelectBits & 0x07) << 4) | 0x02;
    PR2 = (int)period;
    T2CON = TimerPrescaleBits;
    TMR2 = 0;
    T2CONbits.TMR2ON = 1;  /* Turn ON Timer2 */
    return (int)period;
}

void SetDutyCycleTo(float Duty_cycle, int Period)/* Set Duty cycle for given period */
{
    int PWM10BitValue;
    
    PWM10BitValue = 4.0 * ((float)Period + 1.0) * (Duty_cycle/100.0);
    CCPR2L = (PWM10BitValue >> 2);
    CCP2CON = ((PWM10BitValue & 0x03) << 4) | 0x0C;
}
void SetDutyCycleTo2(float Duty_cycle, int Period)/* Set Duty cycle for given period */
{
    int PWM10BitValue;
    
    PWM10BitValue = 4.0 * ((float)Period + 1.0) * (Duty_cycle/100.0);
    CCPR1L = (PWM10BitValue >> 2);
    CCP1CON = ((PWM10BitValue & 0x03) << 4) | 0x0C;
}
void External_Interrupt_Init()
{
    TRISBbits.TRISB0=1;		/* Make INT0 pin as an input pin*/

    /* Also make PBADEN off in Configuration file or
    clear ADON in ADCON0 so as to set analog pin as digital*/
  
    INTCON2 = 0x40;         /* Set Interrupt on Rising Edge*/
    INTCONbits.INT0IF = 0;	/* Clear INT0IF flag*/
    INTCONbits.INT0IE = 1;	/* Enable INT0 external interrupt*/
    INTCONbits.GIE = 1;		/* Enable Global Interrupt*/
}

void main(void)
{
    SYSTEM_Initialize() ;//by lab11 file
    float Duty_Scale;
    int Period;
    
    TRISD = 0x00;           /* PORTD as output */
    LATD = 0b00000000;
    
    ADC_Init();
    PWM_Init();             /* Initialize PWM */
    Period = setPeriodTo(500);/* 500Hz PWM frequency */
    /* Note that period step size will gradually increase with PWM frequency */
    
    ClearBuffer();
    while(1)
    {
        strcpy(str, GetString());
        for(int i=0; i<5000; i++);
//        UART_Write_Text("ws:");
//        UART_Write_Text(WSGetString());
//        UART_Write_Text("ad:");
//        UART_Write_Text(ADGetString());
        
        sscanf(WSGetString(), "%lf", &ws_double);
        sscanf(ADGetString(), "%lf", &ad_double);
        if(ws_double >= 0){
            LATD = 0b00000101; //move forward <0:3>1010
        }
        else if(ws_double < 0){
            LATD = 0b00001010; //move backward <0:3>0101
        }

        //UART_Write_Text(WSGetString());
        ClearBuffer();
        
        //Duty_Scale = (((float)(ADC_Read1(0)/4.0))/2.55); /* Scale Duty Cycle */
        Duty_Scale = (((float)(abs(ws_double*1023)/4.0))/2.55);
        SetDutyCycleTo(Duty_Scale, Period);
        //Duty_Scale = (((float)(ADC_Read2(0)/4.0))/2.55); /* Scale Duty Cycle */
        Duty_Scale = (((float)(abs(ws_double*1023)/4.0))/2.55);
        SetDutyCycleTo2(Duty_Scale, Period);
    }
}
void __interrupt(high_priority) Hi_ISR(void)
{
    if(PIR1bits.CCP1IF == 1) {
        RC2 ^= 1;
        PIR1bits.CCP1IF = 0;
        TMR3 = 0;
    }
    return ;
}