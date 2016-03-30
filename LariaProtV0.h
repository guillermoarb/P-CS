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

//Tiempo SISO
#define Wait4TknAck 10
#define Wait4AdrsReq 30

//Variables protocolo Laria
extern unsigned char FallenFlag;

//Variables protocolo SISO
extern unsigned char AddressDestino[2];
extern unsigned int NoPacket;
extern unsigned char BufferTx[105];
extern unsigned char TknCnt;
//Tiempor SISO
extern unsigned char SegCntr1;
extern unsigned char SegCntr2;

//Estados SISO
extern unsigned char FlagSts;
extern unsigned char StsVrgn;  //Validación de entrada a estado virgen
extern unsigned char FlagAdrsACK;  //Bancera de nueva dirección recibida
/*
Estados:
  0 = NO ADRS, No dirección de destino asignada
  1 = ADRS REQ & WAIT, Petición y espera de dirección de destino, no dirección de envío previa, MONITORIZACIÓN DESACTIVADA
  2 = OK, Monitorización normal, dirección de envío asignada
  3 = LINK TEST, Verficación de dirección
  4 = LINK ALIVE, En espera de dirección de destino, dirección de envío previa SI MONITORIZACIÓN y envío
*/

void SendLarPackTFP(float Temp, unsigned char FC, unsigned char Pos);
void TFP_API16Send(float Temp, unsigned char FC, unsigned char Pos);
void ECG_API16Send(unsigned char NoFragmento);
void TokenSend(unsigned char TknID, unsigned char Address[]);
void TokenACK(void);
void AdrsReq(void);
void SISOProtInit(void);
void StsChng(void);


#ifdef	__cplusplus
}
#endif

#endif	/* LARIAPROTV0_H */
