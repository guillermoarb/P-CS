/*
 * File:   Utilities.h
 * Author: GuillermoARB
 *
 * Created on 21 de septiembre de 2015, 11:38
 */

#ifndef UTILITIES_H
#define	UTILITIES_H

#ifdef	__cplusplus
extern "C" {
#endif

extern unsigned char Timer0Flag;
extern unsigned int Load;

void TMR0_Glaube_ISR(void);
void Delay_10ms(unsigned char);


#ifdef	__cplusplus
}
#endif

#endif	/* UTILITIES_H */
