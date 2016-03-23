/*
 * File:   ECGv2_0.h
 * Author: GuillermoARB
 *
 * Created on 27 de Octubre de 2015, 17:46
 */

#ifndef ECG_H
#define ECG_H

#ifdef  __cplusplus
extern "C" {
#endif

    //Definiciones frecuencia cardiaca
    #define ECG_Fs 250                      //Frecuancia de muestreo para ECG
    #define SearchWindowSize 40             //Tamaño de la ventana de busqueda
    #define NoiseWindowSize 30              //Tamaño de la ventana de pertubaciones en derivada de ECG
    #define ECG_Threshold 0.7               //Límite para identificación de complejo R en ECG
    #define BufferSignalSize 1000           //Tamaño de buffer para análisis de muestras
    //#define HistFC_Size 100                 //Tamaño para el buffer de historicos de frecuencia cardiaca
    #define ECGpackSize 80

    //Electrocardiograma
    //extern unsigned int  ECGpack[104];
    //extern unsigned char  ECGpack[ECGpackSize];
    extern unsigned char  iECGsample;



    //Variables de control de tiempo
    extern unsigned char Seg,Min;
    extern unsigned int CuartoSeg;
    extern unsigned int MilSeg;
    extern unsigned int MilSegMin;

    //Variables para cálculo de Frecuencia cardiaca
    extern unsigned int ECGsignal[BufferSignalSize];
    unsigned char Der_ECGsignal[BufferSignalSize];
    unsigned int iECGsignal;
    unsigned int ECG_ThresholdMAX;
    unsigned char AuxFC;
    extern unsigned char FC_Send;

    int     GetECGADC(void);
    float   GetECG_Volt(void);
    void    ECG(void);
    unsigned char GetFC(void);



#ifdef  __cplusplus
}
#endif

#endif  /* ECG_H */
