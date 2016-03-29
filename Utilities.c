

#include "Utilities.h"
#include <xc.h>

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
