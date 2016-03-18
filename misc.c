/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File:   misc.c
 * Author: MiguelAlejandro
 *
 * Created on 28 de julio de 2015, 12:51
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "misc.h"

/* Función para armar dato de 16bits */
unsigned int make16(unsigned char H, unsigned char L)
{
    /* H: MS_Byte; L: LS_Byte */
    return ((H<<8)|L);
}

/* Función para realizar el complemento a dos de un valor n de x bits */
signed short eval2comp(unsigned short n, unsigned short x)
{
   if( ( 1<<(x-1) ) & n )
   {
      unsigned int m = 0;
      m = ~n;
      m = m + 0x1;
      return -m;
   }
   else
      return n;
}