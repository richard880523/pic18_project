/* PIC18F4550 ADC Header File 
 * http://www.electronicwings.com
 */

#ifndef ADC_HEADER_FILE_H
#define	ADC_HEADER_FILE_H

//#include <pic18f4550.h>         /* Include PIC18F4550 header file */
//#include "Configuration_header_file_1.h"  /* Include Configuration header file */
#include "setting_hardaware/setting.h"
void ADC_Init();                /* ADC Initialize function */
int ADC_Read1(char);             /* ADC Read function */
int ADC_Read2(char);

#endif	/* ADC_HEADER_FILE_H */

