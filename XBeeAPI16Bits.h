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

int  Make16 (char MSB, char LSB);
char Make8(int Var, int NoByte);
void Setup_USART1XBAPI(void);
void putch(char data);


#ifdef	__cplusplus
}
#endif

#endif	/* XBEEAPI16BITS_H */
