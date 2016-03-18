/* 
 * File:   XBeeAT.h
 * Author: GuillermoARB
 *
 * Created on 26 de septiembre de 2015, 16:53
 */

#ifndef XBEEAT_H
#define	XBEEAT_H

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

#endif	/* XBEEAT_H */

