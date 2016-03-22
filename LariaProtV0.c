#include <xc.h>
#include "config.h"
#include <stdio.h>
#include <math.h>
#include <plib/timers.h>
#include <plib/usart.h>
#include <plib/adc.h>
#include "XBeeAT.h"
#include "ADC_Config.h"
#include "Temp_RTD.h"
#include "ECG.h"
#include "Utilities.h"
#include "Interrupt_manager.h"
#include "LariaProtV0.h"
#include "XBeeAPI16Bits.h"



unsigned char FallenFlag=0;

//Variables protocolo SISO
unsigned char AddressDestino[2]={0};
unsigned int NoPacket=0;

void SendLarPackTFP(float Temp, unsigned char FC, unsigned char POS)
{
    //printf("\t \nTemp C = %1.2f ", GetTemp());

    unsigned char PackTFP[5]={0};
    unsigned char iTFP;

    double TempAux;

    char TempINT = (unsigned char)Temp;
    //putch(TempINT);
    unsigned char TempDEC = (unsigned char) (modf(Temp,&TempAux)*100);

    PackTFP[0]=PackTFPID;
    PackTFP[1]=TempINT;
    PackTFP[2]=TempDEC;
    PackTFP[3]=FC;
    PackTFP[4]=POS;

    for (iTFP=0;iTFP<=4;iTFP++)
        putch(PackTFP[iTFP]);

}

void TFP_API16Send(float Temp, unsigned char FC, unsigned char Pos)
{
  unsigned char iTFP;
  double TempAux;
  unsigned char BufferTx[25]={0};

  char TempINT = (unsigned char)Temp;
  unsigned char TempDEC = (unsigned char) (modf(Temp,&TempAux)*100);

  AddressSend[0]=0xFF;
  AddressSend[1]=0xAB;

  AddressDestino[0]=0xCC;
  AddressDestino[1]=0xCC;

  AddresMy[0]=0xFF;
  AddresMy[1]=0xCB;

  // Llenado de paqeute TFPAPI 16 Bits
  BufferTx[0]=0x7E;
  BufferTx[1]=0x00;
  BufferTx[2]=0x15;
  BufferTx[3]=0x01;
  BufferTx[4]=0x00;
  BufferTx[5]=AddressSend[0];
  BufferTx[6]=AddressSend[1];
  BufferTx[7]=0x00;
  BufferTx[8]=DataPackID;
  BufferTx[9]=NodoMovilID;
  BufferTx[10]=TraspNoASK;
  BufferTx[11]=AddresMy[0];
  BufferTx[12]=AddresMy[1];
  BufferTx[13]=AddressDestino[0];
  BufferTx[14]=AddressDestino[1];
  BufferTx[15]=0x00;              //Sin saltos por ser origen
  BufferTx[16]=0x00;              //Contador de paquetes no mayor a 32 bits
  BufferTx[17]=Make8(NoPacket,1);
  BufferTx[18]=Make8(NoPacket,2);
  BufferTx[19]=PackTFPID;
  BufferTx[20]=TempINT;
  BufferTx[21]=TempDEC;
  BufferTx[22]=FC;
  BufferTx[23]=Pos;
  BufferTx[24]=ChecksumGen(BufferTx);

  for(unsigned char i=0;i<=25;i++)
  {
      while(!TXSTA1bits.TRMT);
        Write1USART(BufferTx[i]);
  }

  NoPacket++; //Incremento en el contador de paquetes

}
