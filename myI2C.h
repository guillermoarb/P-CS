/* 
 * File:   myI2C.h
 * Author: MiguelAlejandro
 *
 * Created on 28 de julio de 2015, 19:27
 */

#ifndef MYI2C_H
#define	MYI2C_H

#ifdef	__cplusplus
extern "C" {
#endif

    /* myI2C definitions */
    #define i2cBuffSize     6   //Tamaño del Buffer 6 Bytes.

    /* myI2C Functions */
    void writeTo(unsigned char dispositivo, unsigned char registro, unsigned char dato);
    void readFrom(unsigned char dispositivo, unsigned char registro, unsigned char size);
    void setupI2C(void);
    
    /* myI2C Variables */
    extern unsigned char I2Cbuffer[i2cBuffSize];

#ifdef	__cplusplus
}
#endif

#endif	/* MYI2C_H */

