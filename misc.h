/* 
 * File:   misc.h
 * Author: MiguelAlejandro
 *
 * Created on 29 de julio de 2015, 10:53
 */

#ifndef MISC_H
#define	MISC_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    /* misc definitions */   
    
    /* misc functions */
        unsigned int make16(unsigned char H, unsigned char L);
        signed short eval2comp(unsigned short n, unsigned short x);
        
    /* misc variables */

#ifdef	__cplusplus
}
#endif

#endif	/* MISC_H */

