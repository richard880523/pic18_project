#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
// using namespace std;
char str[20];
double ws_double, ad_double;

void main(void) 
{
  
    SYSTEM_Initialize();
    ClearBuffer();
    while(1) {
        sscanf(WSGetString(), "%lf", &ws_double);
        sscanf(ADGetString(), "%lf", &ad_double);
        
        UART_Write_Text(WSGetString());
        //UART_Write_Text(ADGetString());
//        strcpy(str, GetString());
//        if(str[0] == '0'){
//            ClearBuffer();
//            //UART_Write_Text("Hi");
//            ClearBuffer();
//        }

        // TODO
    }
    return;
    
}

// old version: 
// void interrupt high_priority Hi_ISR(void)
void __interrupt(high_priority) Hi_ISR(void)
{
    if(PIR1bits.CCP1IF == 1) {
        RC2 ^= 1;
        PIR1bits.CCP1IF = 0;
        TMR3 = 0;
    }
    return ;
}