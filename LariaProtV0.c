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
unsigned char BufferTx[105]={0};
unsigned char TknCnt=0;

//Tiempo
unsigned char SegCntr1=0;
unsigned char SegCntr2=0;

//Estados SISO banderas
unsigned char FlagSts= 0; //Bandera de estado del protocolo
unsigned char StsVrgn=0;  //Validación de entrada a estado virgen
unsigned char FlagAdrsACK=0;  //Bancera de nueva dirección recibida

void StsChng(void)   //Cambiador de estatus
{
  switch(FlagSts)
  {
    case 0:

      if (StsVrgn == 0)
      {
        SegCntr1=0;
        StsVrgn=1;
      }

      if(SegCntr1 >= Wait4AdrsReq) //Si se alcanza el tiempo límite se cambia a estado 1
      {
        FlagSts=1;
        StsVrgn=0;
      }

    break;

    case 1:
      if(StsVrgn == 0)
      {
        AdrsReq();
      }

      if(FlagAdrsACK==1)
      {
        FlagSts=3; //Cambio de estado a prueba de dirección recibida
        FlagAdrsACK=0; //Limpieza de bandera de nueva dirección recibida
      }

    break;

    default:

    break;
  }
}

void SISOProtInit(void)
{
  FlagSts=0; //Estatus de inicio sin dirección.
  SetAddressSend(0x00,0x00);
  SetAddressDestino(0x00,0x00);
  SetAddressMy(0xFF,0xCB);
}

void AdrsReq(void)
{
  SetAddressSend(0xFF,0xFF);
  TokenSend(TknCnt,AddressSend);
  TknCnt++;
}

void TokenSend(unsigned char TknID, unsigned char Address[])
{
  //SetAddressSend(0xFF,0xAB);
  SetAddressSend(Address[0],Address[1]);

  BufferTx[0]=0x7E;
  BufferTx[1]=0x00;
  BufferTx[2]=0x08;
  BufferTx[3]=0x01;
  BufferTx[4]=0x00;
  BufferTx[5]=AddressSend[0];
  BufferTx[6]=AddressSend[1];

  if(Address[0]==0xFF && Address[1]==0xFF) {BufferTx[7]=0x04; }
  else {BufferTx[7]=0x00;  }           //Fin de trama API

  BufferTx[8]=TockenPackID;
  BufferTx[9]=NodoMovilID;
  BufferTx[10]=TknID;
  BufferTx[11]=ChecksumGen(BufferTx);

  for(unsigned char i=0;i<=12;i++)
  {
      while(!TXSTA1bits.TRMT);
        Write1USART(BufferTx[i]);
  }
}

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
  //unsigned char BufferTx[25]={0};

  char TempINT = (unsigned char)Temp;
  unsigned char TempDEC = (unsigned char) (modf(Temp,&TempAux)*100);

  SetAddressSend(0xFF,0xAB);
  // AddressSend[0]=0xFF;
  // AddressSend[1]=0xAB;

  SetAddressDestino(0xCC,0xCC);
  // AddressDestino[0]=0xCC;
  // AddressDestino[1]=0xCC;

  SetAddressMy(0xFF,0xCB);
  // AddressMy[0]=0xFF;
  // AddressMy[1]=0xCB;

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
  BufferTx[11]=AddressMy[0];
  BufferTx[12]=AddressMy[1];
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

void ECG_API16Send(unsigned char NoFragmento)
{
  //unsigned char BufferTx[100]={0};

  SetAddressSend(0xFF,0xAB);
  // AddressSend[0]=0xFF;
  // AddressSend[1]=0xAB;

  SetAddressDestino(0xCC,0xCC);
  // AddressDestino[0]=0xCC;
  // AddressDestino[1]=0xCC;

  SetAddressMy(0xFF,0xCB);
  // AddressMy[0]=0xFF;
  // AddressMy[1]=0xCB;

  // Llenado de paqeute TFPAPI 16 Bits
  BufferTx[0]=0x7E;
  BufferTx[1]=0x00;
  BufferTx[2]=0x61;             // LONGITUD !!!!!
  BufferTx[3]=0x01;
  BufferTx[4]=0x00;
  BufferTx[5]=AddressSend[0];
  BufferTx[6]=AddressSend[1];
  BufferTx[7]=0x00;
  BufferTx[8]=DataPackID;           //Comienzo paquete SISO
  BufferTx[9]=NodoMovilID;
  BufferTx[10]=TraspNoASK;
  BufferTx[11]=AddressMy[0];
  BufferTx[12]=AddressMy[1];
  BufferTx[13]=AddressDestino[0];
  BufferTx[14]=AddressDestino[1];
  BufferTx[15]=0x00;              //Sin saltos por ser origen
  BufferTx[16]=0x00;              //Contador de paquetes no mayor a 32 bits
  BufferTx[17]=Make8(NoPacket,1);
  BufferTx[18]=Make8(NoPacket,2);
  BufferTx[19]=PackECGID;

  //Para envío de muestras de 16 bits
  // for (unsigned char i=0;i<=40;i++)
  // {
  //   BufferTx[20+(i*2)]   = Make8(ECGsignal[(40*NoFragmento)+i],1);
  //   BufferTx[20+(i*2)+1] = Make8(ECGsignal[(40*NoFragmento)+i],2);
  // }

  //Para envíos de muestras de 8 bits
  for (unsigned char i=0;i<=80;i++)
  {
    BufferTx[20+i] = Make8(ECGsignal[(80*NoFragmento)+i],2);
  }

  BufferTx[100]=ChecksumGen(BufferTx);

  for(unsigned char i=0;i<=100;i++)
  {
      while(!TXSTA1bits.TRMT);
        Write1USART(BufferTx[i]);
  }

  NoPacket++;

}
