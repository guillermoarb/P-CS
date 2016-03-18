/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File:   adxl345.h
 * Author: MiguelAlejandro
 *
 * Created on 13 de octubre de 2015, 06:22 PM
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "adxl345.h"
#include "myI2C.h"
#include "misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <plib/usart.h>
#include <math.h>

/* Inicialización de variables */
    signed short rawADXL[3]={0,0,0};
    double biasADXL[3]={0,0,0};
    double GforceADXL[3]={0,0,0};

/******************************************************************************
 * int initADXL(void)
 * - Función de Inicialización y Configuración del acelerómetro de acuerdo a la
 * documentación abajo comentada. Retorna el valor del registro ADXL_DEV_ID (0xE5)
 ******************************************************************************/
unsigned char initADXL345(void)
{
    /* Device configuration */
        // * PWR_CTL: Sleep Mode * //
     //   writeTo(ADXL345,ADXL345_POWER_CTL,0x00);
         writeTo(ADXL345,ADXL345_POWER_CTL,POWER_CTL_SLEEP);

        // * DATA_FORMAT: Device into +/- 16G Range, Full Res = 13bits. Sensibilidad = 2^13 / 3.2 * //
     //   writeTo(ADXL345,ADXL345_DATA_FORMAT,0x0B);
        writeTo(ADXL345,ADXL345_DATA_FORMAT,DATA_FORMAT_RANGE_16|DATA_FORMAT_FULL_RES);

        // * BW_RATE: Device into Normal Power Operation, ODR = 100Hz * //
     //   writeTo(ADXL345,ADXL345_BW_RATE,0x0A);
        writeTo(ADXL345,ADXL345_BW_RATE,BW_50);

//        // * PWR_CTL: Measurement Mode * //
//     //   writeTo(ADXL345,ADXL345_POWER_CTL,0x08);
//        writeTo(ADXL345,ADXL345_POWER_CTL,POWER_CTL_MEASURE);

    /* Interrupt configuration */
        // * First Disable All Interrupts * //
        writeTo(ADXL345,ADXL345_INT_ENABLE,INT_DISABLE_ALL);
        
        // * THRESH_FF: 350mg (recommended values: 300mg - 600mg [0x05 - 0x09]); scale: (62.5mg/LSB) * //
        writeTo(ADXL345,ADXL345_THRESH_FF,0x05);    //312.5mg
//        writeTo(ADXL345,ADXL345_THRESH_FF,0xC); //750mg
   
        // * TIME_FF: 100ms (recommended values: 100ms - 350ms [0x14 - 0x46]); scale: (5ms/LSB) * //
        writeTo(ADXL345,ADXL345_TIME_FF,0x14);      //100mS
//        writeTo(ADXL345,ADXL345_TIME_FF,0x6);   //30mS

        // * INT_MAP: Free_Fall Interrupt INT1 * //
//        writeTo(ADXL345,ADXL345_INT_MAP,~INT_MAP_FREE_FALL);
        writeTo(ADXL345,ADXL345_INT_MAP,0x00);

        // * INT_ENABLE: Free_Fall Interrupt enabled * //
        writeTo(ADXL345,ADXL345_INT_ENABLE,INT_ENABLE_FREE_FALL);
        
        // * PWR_CTL: Measurement Mode * //
     //   writeTo(ADXL345,ADXL345_POWER_CTL,0x08);
        writeTo(ADXL345,ADXL345_POWER_CTL,POWER_CTL_MEASURE);

    /* DEVICE_ID: Communication Test -> 0xE5 */
        //   getBiasADXL();
       readFrom(ADXL345,ADXL_DEV_ID,1);
   
   return I2Cbuffer[0];
}

/******************************************************************************
 * void getRawADXL(void)
 * - Obtiene datos directos del acelerómetro.
 ******************************************************************************/
void getRawADXL(void)
{
  readFrom(ADXL345,ADXL345_DATA,6);
  // *El ADXL envía primero el L y luego el H, en complemento a dos * //
  rawADXL[0] = eval2comp(make16(I2Cbuffer[1],I2Cbuffer[0]),16);
  rawADXL[1] = eval2comp(make16(I2Cbuffer[3],I2Cbuffer[2]),16);
  rawADXL[2] = eval2comp(make16(I2Cbuffer[5],I2Cbuffer[4]),16);
}

/******************************************************************************
 * void getBiasADXL(void)
 * - Cálculo de bias (offsets) para corrección de lecturas del acelerómetro.
 ******************************************************************************/
void getBiasADXL(void)
{
   for(int i=0; i<ADXL345_SAMPLES; i++)
   {
      getRawADXL();
      biasADXL[0]+=rawADXL[0];
      biasADXL[1]+=rawADXL[1];
      biasADXL[2]+=rawADXL[2];
   }   
   biasADXL[0]/=ADXL345_SAMPLES;
   biasADXL[1]/=ADXL345_SAMPLES;
   biasADXL[2]/=ADXL345_SAMPLES;
   biasADXL[2]-=256;
}

/******************************************************************************
 * void getGforceADXL(void)
 * - Lectura de Fuerzas G del acelerómetro.
 ******************************************************************************/
void getGforceADXL(void)
{
   getRawADXL();
   GforceADXL[0] = (rawADXL[0] - biasADXL[0]) * ADXL345_SENS;
   GforceADXL[1] = (rawADXL[1] - biasADXL[1]) * ADXL345_SENS;
   GforceADXL[2] = (rawADXL[2] - biasADXL[2]) * ADXL345_SENS;
}

/******************************************************************************
 * unsigned char getPosition(void)
 * - Lectura de Posición del Acelerómetro:
 *                                      X       Y       Z       Returns:
 *      - Acostado Boca-Abajo           0       0      +1           (1)
 *      - Acostado Boca-Arriba          0       0      -1           (2)
 *      - Acostado Costado-Derecho      0      -1       0           (3)
 *      - Acostado Costado-Izquierdo    0      +1       0           (4)
 *      - Espalda Erguida              -1       0       0           (5)
 *      - No definida                   /       /       /           (0)
 ******************************************************************************/
unsigned char getPosition(void)
{
    float samples = 10;
    double pos[3] = {0,0,0};
    for(int k = 0; k<samples; k++)
    {
        getGforceADXL();
        pos[0] += GforceADXL[0];
        pos[1] += GforceADXL[1];
        pos[2] += GforceADXL[2];
    }
    
    for(int k = 0; k<=2; k++)
        pos[k] /= samples;
    
    double x = round(pos[0]);
    double y = round(pos[1]);
    double z = round(pos[2]);
    
    unsigned char posicion = 0;
         if( (x == 0) && (y == 0) && (z == 1) )
        posicion = 1;
    else if( (x == 0) && (y == 0) && (z == -1) )
        posicion = 2;
    else if( (x == 0) && (y == -1) && (z == 0) )
        posicion = 3;
    else if( (x == 0) && (y == 1) && (z == 0) )
        posicion = 4;
    else if( (x == -1) && (y == 0) && (z == 0) )
        posicion = 5;
    else
        posicion = 0;
    
    return posicion;
}

/******************************************************************************
 * unsigned char isFallen(void)
 * - Funcion que evalúa si existe interrupcion por free-falling
 ******************************************************************************/
unsigned char isFallen(void)
{
    unsigned char f = 0;
    readFrom(ADXL345,ADXL345_INT_SOURCE,1);
    f = (I2Cbuffer[0] & 0x04);
    return f;
}
