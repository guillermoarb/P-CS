/*
 * File:   XBeeAPI16Bits.h
 * Author: guillermoadrianrodriguezbarragan
 *
 * Created on March 18, 2016, 6:06 PM
 */


#ifndef XBEEAPI16BITS_H
#define	XBEEAPI16BITS_H

#ifdef	__cplusplus
extern "C" {
#endif

//Identificadors recervados  por XBee API
#define XBAPI_StrDel        0x7E        //Delimitador de inicio en modo API de XBee
#define XBAPI_Rx64ID        0x80        //Rx API Identifier

//Definici�n de variables
//extern unsigned int 	Length;
//extern unsigned char 	ApiID;
//extern unsigned char 	ApiOp;
extern unsigned char 	iRx1XBAPI;
extern unsigned char 	PaqXBAPILen;
extern unsigned int 	NoPaqXBAPI;

//Variables manejo de puerto serial
extern unsigned char 	FlagPaqRx1;
extern unsigned char 	BufferRxUART[80];		//Vector donde es descargada la informaci�n cruda del puerto serial

//Variables para modo API
extern unsigned char AddressSend[2];
extern unsigned char AddressMy[2];



int  Make16 (char MSB, char LSB);
char Make8(int Var, int NoByte);
void Setup_USART1XBAPI(void);
void putch(char data);
char ChecksumGen(unsigned char Frame[]);
void SetAddressSend(unsigned char AddrsMSB, unsigned char AddrsLSB);
void SetAddressDestino(unsigned char AddrsMSB, unsigned char AddrsLSB);
void SetAddressMy(unsigned char AddrsMSB, unsigned char AddrsLSB);
void UART_XBeeAPI_ISR(void);
void PutByteUART1(unsigned char Byte2Send);

#ifdef	__cplusplus
}
#endif

#endif	/* XBEEAPI16BITS_H */
