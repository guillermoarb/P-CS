#include <xc.h>
#include "XBeeAPI16Bits.h"
#include "LariaProtV0.h"
#include <stdio.h>
#include <plib/usart.h>

//VARIABLES GLOBALES
//Manejo de puerto
unsigned char BufferRxUART[127];
unsigned char FlagPaqRx2=0;
unsigned char iRx1XBAPI=0;
unsigned int PaqXBAPILen=0;
unsigned int NoPaqXBAPI=0;

//Enrrutamiento
unsigned char AddressSend[2];
unsigned char AddressMy[2];

void UART_XBeeAPI_ISR(void)
{
        BufferRxUART[iRx1XBAPI] = Read1USART();
      //  printf("Recibido: %d\n",BufferRxUART[0]);
        TokenSend(BufferRxUART[0]);
/*
        if (iRx1XBAPI == 2) // Se recive suficiente informaci�n para determinar la longitud del paquete
            if (BufferRxUART[0] == XBAPI_StrDel) //Identificacion de un paquete API XBee
                PaqXBAPILen = Make16(BufferRxUART[1], BufferRxUART[2]); //Se obtiene la longitud del paquete esperado

        //Terminaci�n por longitud de paquete esperado
        if (iRx1XBAPI >= PaqXBAPILen + 3) //Si se ha alcanzado el final del paquete esperado
        {
            iRx1XBAPI = 0; //Se cierra el paquete
            NoPaqXBAPI++; //Se aumenta el contador de paquetes recividos
            FlagPaqRx2 = 1; //Se habilita bandera de nuevo paquete


            //PIR3bits.RC2IF = 0;   //Experimental

            //NewPackUART(PaqXBAPILen);  //PaqXBAPILen = Longitud de paquete - Start Delimiter - Length Bytes - Check Sum
            //FlagBufferRx1Full=0;      //Se deshabilita bandera de buffer Rx lleno


            return ;
        }

        iRx1XBAPI++;
        */
      //  PIR1bits.RC1IF = 0; // clear rx flag  //Experimental

}

void SetAddressSend(unsigned char AddrsMSB, unsigned char AddrsLSB)
{
  AddressSend[0]=AddrsMSB;
  AddressSend[1]=AddrsLSB;
}

void SetAddressDestino(unsigned char AddrsMSB, unsigned char AddrsLSB)
{
  AddressDestino[0]=AddrsMSB;
  AddressDestino[1]=AddrsLSB;
}

void SetAddressMy(unsigned char AddrsMSB, unsigned char AddrsLSB)
{
  AddressMy[0]=AddrsMSB;
  AddressMy[1]=AddrsLSB;
}

char ChecksumGen(unsigned char Frame[])
{
    int Checksum=0,IndS=0,Len;
    unsigned int Sum=0;

    Len=Make16(Frame[1],Frame[2]);

    for(IndS=3;IndS<=Len+2;IndS++) //Checksum comienza despues de longitud y hasta el final.
    {
        Sum=Sum+Frame[IndS];
    }

    Checksum=Sum & 0xFF;
    Checksum=0xFF-Sum;

    return Checksum;
}


int Make16 (char MSB, char LSB)
{
    int Maked16=0;

    Maked16=(MSB<<8)+LSB;


    return Maked16;
}

char Make8(int Var, int NoByte)
{
    //Función para separar una variable de 16 bits en dos de 8 bits
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
//  while(!TX2IF)
//  continue;
//  TXREG2 = data;
    while(!TX1IF)
    continue;
    TXREG1 = data;

}



void Setup_USART1XBAPI(void)
{
    //Declaraci�n de Variables
    unsigned char USART1Config=0;
    unsigned int BaudRate=0;


    //Configuraci�n USART
    USART1Config=USART_TX_INT_OFF   //Interrupcion por Ttransmisi�n: Off
            &USART_RX_INT_ON       //Interrupcion por Recepci�n: Off
            &USART_ASYNCH_MODE      //Modo As�ncrono
            &USART_EIGHT_BIT        //Transmision de 8bits
            &USART_CONT_RX          //Recepci�n Continua
            &USART_BRGH_HIGH        //Baudios
            &USART_ADDEN_OFF;       //Detecci�n de Direcci�n OFF
    BaudRate=51;            //Valor que se carga a SPRBH para Definir BaudRate = 9600
    Close1USART();          //Cierra USART2 en caso de estar previamente abierto.
    Open1USART(USART1Config, BaudRate); //Abre USART1

    //printf("Puerto configurado...\n");
    //IPR1bits.RC1IP =1;      //Receive Interrupt: High Priority
    //PIE1bits.RC1IE=1;       //Receive Interrupt: Enabled
    //PIR1bits.RC1IF=0;       //Reset de EUSART2 Receive Interrupt Flag
}
