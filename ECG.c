#include <xc.h>
#include "config.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <plib/timers.h>
#include <plib/usart.h>
#include <plib/adc.h>
#include "XBeeAPI16Bits.h"
#include "ADC_Config.h"
#include "ECG.h"
#include "Utilities.h"
#include "Interrupt_manager.h"
#include "LariaProtV0.h"
#include "Temp_RTD.h"

#define _XTAL_FREQ 32000000
#define PorcUmbral 0.80
#define UmbralR 1.4
#define MaxMax 1.7
#define MinMin 1.3

//unsigned int ECGpack[104]={0};
//unsigned char ECGpack[ECGpackSize]={0};
unsigned char iECGsample=2;
unsigned char iPack=0;

float ECGmax=0;
float ECGmin=2.0;

float ECGsample=0;
unsigned char  FC,FC6;
unsigned int  R=0,R6=0;
unsigned char  FlagR=0;

//Inicialización de buffer para an�lisis de frecuencia cardiaca
unsigned int ECGsignal[BufferSignalSize]={0};
unsigned char Der_ECGsignal[BufferSignalSize]={0};
unsigned int iECGsignal=0;
unsigned int ECG_ThresholdMAX=0;
unsigned char AuxFC=0;
unsigned char FC_Send=0;  //Variable de almacenamiento de la frecuencia cardiaca

int GetECGADC()  //Captura de ECG en ADC
{
    unsigned int ECGRaw=0;

    SetChanADC(ADC_CH11);
    ConvertADC();
    while(BusyADC());
    ECGRaw = ReadADC();

    return ECGRaw;
}

float GetECG_Volt() //Conversi�n de ADC Raw a Voltaje
{
    float ECG_Volt;

    ECG_Volt=((float)GetECGADC()*VolBat)/1024;
    return ECG_Volt;
}

void ECG(void)
{
    unsigned int ECG_Sample;
    unsigned char iECG=0;

    //Discretizacion de señal
    ECG_Sample=GetECGADC();                         //Signal samppling
    ECGsignal[iECGsignal]=ECG_Sample;               //Adquisicion de señal

    // Adquisicion de frecuencia cardiaca al tener muestras suficientes
    if(iECGsignal>=BufferSignalSize)
    {
        AuxFC=GetFC();                                    //Funcion para calculo de Frecuencia cardiaca

        if(AuxFC!=0)
          FC_Send=AuxFC;
          iECGsignal=0;  // Reinicio de muestreo

          for(unsigned char i=0; i<=25;i++)  //Envío de los 25 fragmentos de la señal
            ECG_API16Send(i);
    }
    else
    {
        iECGsignal++;
    }

    //ECGpack[iECGsample]=Make8(ECG_Sample,1);        //Almacenamiento paquete para envío de se�al primer Byte
    //ECGpack[iECGsample+1]=Make8(ECG_Sample,2);      //Almacenamiento paquete para envío de se�al segundo Byte

    iECGsample=iECGsample+2;
    // printf("%d\n\r",ECG_Sample);  //Graficación matlab



    //------------------------     Envío de información
    if(iECGsample>=ECGpackSize)                 //Al completar 80 muestras se envía
    {
       //    ECG_API16Send();
    }
}


unsigned char GetFC(void)
{
    //Calculo de la primer derivada de se�al ECG
    unsigned int  HistR[10][10];
    unsigned int FC=0;
    unsigned char FCppm=0;
    unsigned char iNoise=0;
    unsigned int  RR[5]={0};
    unsigned int  RR_Mean=0;
    unsigned char iRR=0;
    unsigned char iHistR=0;
    unsigned int iDer=0;
    unsigned int DerMAX=0;
    unsigned char WindowOff=0;

    Der_ECGsignal[0]=0; //Primer dato en derivada es igualado a cero.

    for (iDer=1;iDer<BufferSignalSize;iDer++)
    {
        Der_ECGsignal[iDer]= abs(  ECGsignal[iDer+1] - ECGsignal[iDer-1] );
    }

    for(iDer=0;iDer <= NoiseWindowSize;iDer++)
        Der_ECGsignal[iDer]=0;

    //Obtenci�n de M�ximo
    for (int i = 0; i < BufferSignalSize; ++i)
    {
        if(Der_ECGsignal[i]>DerMAX)
            DerMAX=Der_ECGsignal[i];
    }

    //Threshold
    ECG_ThresholdMAX=ECG_Threshold*DerMAX;          //C�lculo de umbral para complejo R

    for (int i = 0; i <=BufferSignalSize; ++i)
    {
        if( (Der_ECGsignal[i] > ECG_ThresholdMAX) && WindowOff==0 )
        {
            HistR[1][iHistR]=Der_ECGsignal[i];
            HistR[2][iHistR]=i;
            //printf("\n\tR!%u\t%d",HistR[2][iHistR],iHistR);
            //printf("\nSAMPLE->WindowON i=%u",i);

            if(iHistR>0)
            {
                RR[iRR] = HistR[2][iHistR] - HistR[2][iHistR-1];
                iRR++;
            }


            iHistR++;
            WindowOff=1;
        }

        if(WindowOff==1)
        {
            //printf("\nWindowOff i=%u",i);
            if(iNoise > NoiseWindowSize)
            {
                //printf("\nWindowOff 2 ON i=%u",i);
                WindowOff=0;
                iNoise=0;
            }

            iNoise++;
        }


    }

    //C�lculo de media en intervalos RR
    for (unsigned char i = 0; i < iRR; ++i)
    {
        RR_Mean=RR_Mean+RR[i];
    }
    RR_Mean=(int)RR_Mean/(iRR);

    //C�lculo de frecuencia cardiaca
    FC=(unsigned int) (ECG_Fs*60)/RR_Mean;
    FCppm= (unsigned char) FC;

    return FC;
}
