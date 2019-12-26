/* PIC18F4550 ADC Source File 
 * http://www.electronicwings.com
 */

#include "ADC_Header_File_1.h"    /* Include ADC header file */

void ADC_Init()                 /* ADC initialization function */
{    
    TRISA = 0xFF;               /* Set as input port */
    ADCON1 = 0x07;              /* Set AN0-7 as Analog & other as digital */
    ADCON2 = 0x92;              /* Right Justified, 4Tad and Fosc/32. */
    ADRESH = 0;                 /* Flush ADC output Register */
    ADRESL = 0;   
}

int ADC_Read1(char channel)      /* ADC channel read function */
{
    //ADCON0 =(ADCON0 & 0b11000011)|((channel<<2) & 0b00111100); /* Channel is selected i.e (CHS3:CHS0) */
    ADCON0bits.CHS = 0b0000;
    ADCON0 |= ((1<<ADON)|(1<<GO)); /* Enable ADC and start conversion */
    while(ADCON0bits.GO_nDONE==1); /* wait for End of conversion i.e. Go/done'=0 conversion completed */        
    return((ADRESH*256) | (ADRESL));/* Return 10-bit ADC result */
}

int ADC_Read2(char channel)      /* ADC channel read function */
{
    //ADCON0 =(ADCON0 & 0b11000011)|((channel<<2) & 0b00111100); /* Channel is selected i.e (CHS3:CHS0) */
    ADCON0bits.CHS = 0b0001;
    ADCON0 |= ((1<<ADON)|(1<<GO)); /* Enable ADC and start conversion */
    while(ADCON0bits.GO_nDONE==1); /* wait for End of conversion i.e. Go/done'=0 conversion completed */        
    return((ADRESH*256) | (ADRESL));/* Return 10-bit ADC result */
}