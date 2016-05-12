

#include "Utilities.h"
#include <xc.h>

#define _XTAL_FREQ 32000000

unsigned char Timer0Flag=0;
unsigned int Load=0;


void TMR0_Glaube_ISR()
{
        Timer0Flag=1;
        PORTCbits.RC3 = !PORTCbits.RC3;

        Sekunde();
        WriteTimer0(Load);    //0.001 Seg. FOSC 32 MHz
        INTCONbits.TMR0IF = 0; //regresar a cero


}

void Delay_10ms(unsigned char NumDelays)
{
  for(unsigned char i=0; i<=NumDelays; i++)
    __delay_ms(10);
}
