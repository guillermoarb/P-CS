/*
 * File:   main_fullProjectCloudSalud.c
 * Author: MiguelAlejandro
 *
 * Created on 3 de noviembre de 2015, 06:15 PM
 */
//Smart Shirt V0.0
//Env�o de se�al de ECG muestreada a 250 Hz
//Captura de temperatura
//Calculo de frecuencia cardiaca, deficiente.
//Identificacion de caidas
// Miguel Chavarin Hernandez
// Leonid
// Guillermo Barrag�n

#define _XTAL_FREQ 32000000
#define USE_AND_MASKS

#include <xc.h>
#include "config.h"
#include <stdio.h>
#include <math.h>
#include <plib/timers.h>
#include <plib/usart.h>
#include <plib/adc.h>
#include "XBeeAPI16Bits.h"
#include "ADC_Config.h"
#include "Temp_RTD.h"
#include "ECG.h"
#include "Utilities.h"
#include "Interrupt_manager.h"
#include "LariaProtV0.h"
#include "myI2C.h"
#include "adxl345.h"

// Declaraciones
#define USE_I2C 0

unsigned char Timer0Config;

void SetupInit(void);
void SetupClock(void);
void Setup_USART2XBAPI();
void SetupTimer0(unsigned char T,unsigned char M);
void putch(char data);
void SetupPorts();
void Sekunde(void);

//Tiempo
unsigned char Seg=0,Min=0;
unsigned int CuartoSeg=0;
unsigned int MilSeg=0;
unsigned int MilSegMin=0;



void main() {


    SetupInit();                        //Inicializar sensores y procesos.

    printf("Smart Shirt v1.0");


    while(1)
    {


    }
}


void SetupTimer0(unsigned char T,unsigned char N)  //Configuracion de interrupci0n del timer
{
    switch (T)
    {
        case 1:   //Opcion para 1 Segundo
            Timer0Config = TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_256 ;
            Load=0x85ED; //1 Seg. FOSC 32 MHz
        break;
        case 2:   //Opcion para 1 ms
            Timer0Config = TIMER_INT_ON & T0_8BIT & T0_SOURCE_INT & T0_PS_1_256 ;
            Load=0xE5; //E8;
        break;

    }

    if(T==0)
        CloseTimer0();
    else
    {
        OpenTimer0(Timer0Config);
        WriteTimer0(Load);
        INTCONbits.TMR0IF = 0;
    }

}


void SetupInit()  //Funci�n de inicializaci�n de funciones y procesos.
{
    SetupClock();
    SetupPorts();
    Setup_USART1XBAPI();
    SetupTimer0(2,0);
    INTERRUPT_Initialize();
    Init_ADC();
    setupI2C();
    unsigned char r = initADXL345();
    PEIE = 1;
    INTCONbits.GIE = 1;
}

void SetupClock() //Configuraci�n de reloj.
{
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
}


void SetupPorts()  //Configuraci�n de puertos
{
    ANSELCbits.ANSC6=0;
    ANSELCbits.ANSC7=0;
    TRISCbits.RC6=0;        //TX es salida
    TRISCbits.RC7=1;        //RX es entrada

    ANSELCbits.ANSC2=0;
    TRISCbits.RC2 = 0;
    PORTCbits.RC2=0;

    TRISAbits.RA3=0;
    PORTAbits.RA3=0;

    TRISBbits.RB4=1;
    ANSELBbits.ANSB4=1;

    TRISBbits.RB5=1;
    ANSELBbits.ANSB5=1;     //RB5 Entrada ADC para sensor de temperatura.

    TRISBbits.RB0=1;
    ANSELBbits.ANSB0=0;
    PORTBbits.RB0=0;
}


void Sekunde(void)  //Rutina de interrupcion para pulso de muestreo
{
    //Variables globales de tiempo
    MilSeg++;
    CuartoSeg++;
    //Conteo de Tiempo
    if(MilSeg>=1000) //Segundo
    {
        GetTemp();

        //printf("\n\t Temp: %f\tFC:%d\t Pos: %d ",TempExt,FC_Send,getPosition());
        //SendLarPackTFP(TempExt,FC_Send,getPosition());
        TFP_API16Send(TempExt,FC_Send,getPosition());

        Seg++;
        MilSeg=0;
    }
      //conteo para fecuencia de muestreo
    if(CuartoSeg == 4) // 4 Milisegundos
    {
        ECG();  //Cálculo de ECG
        CuartoSeg=0;
    }
    if(Seg==60)
    {
        Min++;
        Seg=0;
    }
}
