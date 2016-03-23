/*
 * File:   LariaProtV0.h
 * Author: GuillermoARB
 *
 * Created on 27 de septiembre de 2015, 13:31
 */

#ifndef LARIAPROTV0_H
#define	LARIAPROTV0_H

#ifdef	__cplusplus
extern "C" {
#endif

#define PackTFPID 0xC7
#define PackECGID 0x4D

// Identificadores protocolo SISO
#define DataPackID 0x44
#define TockenPackID 0x54

#define NodoMovilID 0x00
#define TraspNoASK 0x00
#define TraspASK 0x01

//Variables protocolo Laria
extern unsigned char FallenFlag;

//Variables protocolo SISO
extern unsigned char AddressDestino[2];
extern unsigned int NoPacket;
extern unsigned char BufferTx[100];

void SendLarPackTFP(float Temp, unsigned char FC, unsigned char Pos);
void TFP_API16Send(float Temp, unsigned char FC, unsigned char Pos);
void ECG_API16Send(void);


#ifdef	__cplusplus
}
#endif

#endif	/* LARIAPROTV0_H */
