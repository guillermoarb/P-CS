

#include <xc.h>
#include "XBeeAPI16Bits.h"
#include "Utilities.h"
#include "myI2C.h"
#include "adxl345.h"
#include "ECG.h"
#include "LariaProtV0.h"
#include <stdio.h>
#include <plib/usart.h>

void INTERRUPT_Initialize(void) {

    /* Interrup flags for Timer */
    INTCON2bits.TMR0IP = 1; // 1-> Habilitado 0-> Deshabilitado

    /*Interrupt flags for UART CONTROL*/
    RCONbits.IPEN = 1;  //Habilitación de vector de prioridad
    IPR1bits.RC1IP = 1; //Receive Interrupt: High Priority
    PIE1bits.RC1IE = 1; //Receive Interrupt: Enabled
    PIR1bits.RC1IF = 0; //Reset de EUSART1 Receive Interrupt Flag

    /* External Interrupt for FreeFall detection */
    INTCONbits.INT0E = 1;       // Enable INT0 interrupt
    INTCON2bits.INTEDG0 = 1;    // Raising edge on INT0
    INTCON2bits.RBPU = 0;       // PORTB Pull-up Enable bit

    PEIE = 1;               //Global interrupts
    INTCONbits.GIE = 1;
    //ei();
}

void interrupt ISR() //Rutinas de interrupci�n
{
    //Timer 0 Interrupci�n
    if (INTCONbits.TMR0IF == 1 && INTCON2bits.TMR0IP ==1 ) {
        TMR0_Glaube_ISR();
    }

    //UART1 Interrupci�n
    //if (PIR1bits.RC1IF && PIE1bits.RC1IE)
    if (PIR1bits.RC1IF && PIE1bits.RC1IE)
    {
        if(RC1STAbits.OERR == 1)
        {
          RC1STAbits.CREN =0;
          RC1STAbits.CREN = 1;
          //Reset();
        }

        UART_XBeeAPI_ISR();
        PIR1bits.RC1IF = 0;
    }

    if(INTCONbits.INT0IF)
    {
//        PORTAbits.RA3 = !PORTAbits.RA3;             //Bandera visual de Interrupci�n RB0
//        SendLarPackTFP(GetTemp(),FC_Send,getPosition(),0xCA);
        //SendLarPackTFP(GetTemp(),FC_Send,0xCA);
        readFrom(ADXL345,ADXL345_INT_SOURCE,1);     //Lectura para limpiar registro INT_SOURCE
        INTCONbits.INT0IF = 0;                      //Clear de flag de interrupci�n RB0
    }
//ei();
  //  return;
}





/**
 End of File
 */
