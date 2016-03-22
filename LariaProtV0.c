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

#define PackTFPid 0xC7

unsigned char FallenFlag=0;

void SendLarPackTFP(float Temp, unsigned char FC, unsigned char POS)
{
    //printf("\t \nTemp C = %1.2f ", GetTemp());

    unsigned char PackTFP[5]={0};
    unsigned char iTFP;

    double TempAux;

    char TempINT = (unsigned char)Temp;
    //putch(TempINT);
    unsigned char TempDEC = (unsigned char) (modf(Temp,&TempAux)*100);

    PackTFP[0]=PackTFPid;
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
  unsigned char BufferTx[14]={0};

  char TempINT = (unsigned char)Temp;
  unsigned char TempDEC = (unsigned char) (modf(Temp,&TempAux)*100);


  BufferTx[0]=0x7E;
  BufferTx[1]=0x00;
  BufferTx[2]=0x0A;
  BufferTx[3]=0x01;
  BufferTx[4]=0x00;
  BufferTx[5]=AddressSend[0];
  BufferTx[6]=AddressSend[1];
  BufferTx[7]=0x00;
  BufferTx[8]=PackTFPid;
  BufferTx[9]=TempINT;
  BufferTx[10]=TempDEC;
  BufferTx[11]=FC;
  BufferTx[12]=Pos;
  BufferTx[13]=ChecksumGen(BufferTx);

  for(unsigned char i=0;i<=13;i++)
  {
      while(!TXSTA1bits.TRMT);
        Write1USART(BufferTx[i]);
  }

}
