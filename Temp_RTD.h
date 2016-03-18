/* 
 * File:   Temp_RTD.h
 * Author: GuillermoARB
 *
 * Created on 26 de septiembre de 2015, 17:33
 */

#ifndef TEMP_RTD_H
#define	TEMP_RTD_H

#ifdef	__cplusplus
extern "C" {
#endif

    int     GetTempADC(void);
    float   GetTemp(void);
    float   GetTempVol(void);
    extern float TempExt;


#ifdef	__cplusplus
}
#endif

#endif	/* TEMP_RTD_H */

