#include <xc.h>
#include "XBeeAPI16Bits.h"
#include <stdio.h>
#include <plib/usart.h>


int Make16 (char MSB, char LSB)
{
    int Maked16=0;

    Maked16=(MSB<<8)+LSB;


    return Maked16;
}

char Make8(int Var, int NoByte)
{
    //Funci�n para separar una variable de 16 bits en dos de 8 bits
    //Var= Variable de 16 bits a ser separada
    //NoByte = 1,2 Selecciona el byte que ser� retornado, 1=MSB, 2=LSB

    unsigned char Maked8=0;

    if(NoByte==2)
    Maked8=Var & 0xFF;
    if(NoByte==1)
    Maked8=Var>>8;

    return Maked8;
}

void putch(char data)
{
//    while(!TX2IF)
//    continue;
//    TXREG2 = data;
//
    while(!TX1IF)
    continue;
    TXREG1 = data;

}

//
//void Setup_USART2XBAPI()
//{
//    //Declaraci�n de Variables
//    unsigned char   USART2Config=0;
//    unsigned int    BaudRate=0;
//
//    TRISBbits.RB6=0;        //TX es salida   //cambiar estos son de USAR1
//    TRISBbits.RB7=1;        //RX es entrada
//
//    //Configuraci�n USART
//    USART2Config=USART_TX_INT_OFF   //Interrupcion por Ttransmisi�n: Off
//            &USART_RX_INT_OFF       //Interrupcion por Recepci�n: Off
//            &USART_ASYNCH_MODE      //Modo As�ncrono
//            &USART_EIGHT_BIT        //Transmision de 8bits
//            &USART_CONT_RX          //Recepci�n Continua
//            &USART_BRGH_HIGH        //Baudios
//            &USART_ADDEN_OFF;       //Detecci�n de Direcci�n OFF
//    //BaudRate=51;            //Valor que se carga a SPRBH para Definir BaudRate = 9600
//    //BaudRate=25;      //19200 8 mHZ
//    //BaudRate=103;     //19200 32 mHz
//    BaudRate=51;        //38400 32 MHz
//    Close2USART();          //Cierra USART2 en caso de estar previamente abierto.
//    Open2USART(USART2Config, BaudRate); //Abre USART2
//    IPR3bits.RC2IP=1;       //Receive Interrupt: High Priority
//    PIE3bits.RC2IE=1;       //Receive Interrupt: Enabled
//    PIR3bits.RC2IF=0;       //Reset de EUSART2 Receive Interrupt Flag
//}


void Setup_USART1XBAPI(void)
{
    //Declaraci�n de Variables
    unsigned char USART1Config=0;
    unsigned int BaudRate=0;

    TRISCbits.RC6=0;        //TX es salida
    TRISCbits.RC7=1;        //RX es entrada

    //Configuraci�n USART
    USART1Config=USART_TX_INT_OFF   //Interrupcion por Ttransmisi�n: Off
            &USART_RX_INT_OFF       //Interrupcion por Recepci�n: Off
            &USART_ASYNCH_MODE      //Modo As�ncrono
            &USART_EIGHT_BIT        //Transmision de 8bits
            &USART_CONT_RX          //Recepci�n Continua
            &USART_BRGH_HIGH        //Baudios
            &USART_ADDEN_OFF;       //Detecci�n de Direcci�n OFF
    BaudRate=51;            //Valor que se carga a SPRBH para Definir BaudRate = 9600
    Close1USART();          //Cierra USART2 en caso de estar previamente abierto.
    Open1USART(USART1Config, BaudRate); //Abre USART1

    IPR1bits.RC1IP =1;      //Receive Interrupt: High Priority
    PIE1bits.RC1IE=1;       //Receive Interrupt: Enabled
    PIR1bits.RC1IF=0;       //Reset de EUSART2 Receive Interrupt Flag
}
