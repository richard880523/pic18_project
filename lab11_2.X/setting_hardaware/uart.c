#include <xc.h>
    //setting TX/RX

char mystring[50];
int lenStr = 0;
int start = 0;
int count = 0;
int ws_index = 0, ad_index = 0;
char ws[20], ad[20];
void UART_Initialize() {
    TRISBbits.TRISB3 = 0;//set as output   
    PORTBbits.RB3 = 0;
    PORTBbits.RB3 = 1;
    /*portC pin6/7 is mult with usart setting their tris*/    
    TRISCbits.TRISC6 = 1;            // Setting by data sheet 
    TRISCbits.TRISC7 = 1;            //  
    
    /*
           Serial Setting      
     * 1.   Setting Baud rate
     * 2.   choose sync/async mode 
     * 3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
     * 3.5  enable Tx, Rx Interrupt(optional)
     * 4.   Enable Tx & RX
     */      
    //  Setting baud rate
    TXSTAbits.SYNC = 0;             //choose the async moode
    BAUDCONbits.BRG16 = 0;          //Read Baud rate table: 8bits baud rate
    TXSTAbits.BRGH = 1;
    SPBRG = 25; //SYNC=0, BRGH=1, BRG16=0 baud rate = 9600
    
   //   Serial enable
    RCSTAbits.SPEN = 1;              //open serial port
    PIR1bits.TXIF = 1;
    PIR1bits.RCIF = 0;
    TXSTAbits.TXEN = 1;             //Enable Tx
    RCSTAbits.CREN = 1;             //Enable Rx
    //setting TX/RX interrupt
    PIE1bits.TXIE = 0;              //disable Tx interrupt
    IPR1bits.TXIP = 0;              //Setting Tx as low priority interrupt
    PIE1bits.RCIE = 1;              //Enable Rx interrupt
    IPR1bits.RCIP = 0;              //Setting Rc as low priority interrupt
    }

void UART_Write(unsigned char data)  // Output on Terminal
{
    while(!TXSTAbits.TRMT);
    TXREG = data;         
    //write to TXREG will send data 
}

char *GetString(){ // 
    return mystring;
}
char *WSGetString(){ // 
    return ws;
}
char *ADGetString(){ // 
    return ad;
}

void UART_Write_Text(char* text) { // Output on Terminal, limit:10 chars
    for(int i=0;text[i]!='\0';i++)
        UART_Write(text[i]);
}

void ClearBuffer(){
    for(int i = 0; i < 20 ; i++){
        mystring[i] = '\0';
        ws[i] = '\0';
        ad[i] = '\0';
    } 
    count = 0;
    start = 0;
    ws_index = 0;
    ad_index = 0;
    lenStr = 0;
}

void MyusartRead()
{
    //if((RCREG >= '0' && RCREG <= '9') || (RCREG >= 'a' && RCREG <= 'z')){
        mystring[lenStr] = RCREG;
        
        if(mystring[lenStr] == 'A' || mystring[lenStr] == 'W'){
            start = 0;
            count = count + 1;
        }
        if(start == 1){
            //UART_Write(mystring[lenStr]);
            if(count == 1){
                ws[ws_index] = mystring[lenStr];
                ws_index++;
            }
            if(count == 2){
                ad[ad_index] = mystring[lenStr];
                ad_index++;
            }
        }
        if(mystring[lenStr] == ':'){
            start = 1;
        }
       
        lenStr++;
        lenStr %= 30;
    //}    
//    if(RCREG == '^') TRISC6 = 1;
//    if(RCREG == '$') TRISC6 = 0;
    return ;
}


// old version: 
// void interrupt low_priority Lo_ISR(void)
void __interrupt(low_priority)  Lo_ISR(void)
{
    if(RCIF)
    {
        if(RCSTAbits.OERR)
        {
            CREN = 0;
            Nop();
            CREN = 1;
        }
        PORTBbits.RB3 = 1;
        MyusartRead();
    }
    
   // process other interrupt sources here, if required
    return;
}