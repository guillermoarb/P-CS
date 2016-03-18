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
