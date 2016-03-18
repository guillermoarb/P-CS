/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File:   myI2C.c
 * Author: MiguelAlejandro
 *
 * Created on 28 de julio de 2015, 12:51
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "myI2C.h"
#include <plib/i2c.h>

/* Variables Initialization */
unsigned char I2Cbuffer[i2cBuffSize] = {0,0,0,0,0,0};

/* Funcion de Escritura */
void writeTo(unsigned char Dispositivo, unsigned char Registro, unsigned char Dato)
{
    //Declaración de Variables.
    unsigned char Borrar_Data=0;    //Para limpiar registro
    signed char status;             //Para detectar errores

    /* 1: Start */
    StartI2C1();

    /* 2: Dirección del Dispositivo y R/W=0 (W) */
    Dispositivo=Dispositivo<<1;     //Se hace un corrimiento de los 7Lsb hacia la Izquierda.
    Borrar_Data=SSP1BUF;            //Lectura de cualquier dato almacenado en el buffer para limpiarlo.
    do
    {
        status=WriteI2C1(Dispositivo&0b11111110); //Escribe la dirección del esclavo con todo y R/W=0;
        if(status == -1)            //Verifica se existió colisión de datos.
        {
            Borrar_Data=SSP1BUF;    //Después de colisión, limpiar Buffer.
            SSPCON1bits.WCOL=0;     //Clear el bit de colisión.
        }
    }
    while(status!=0);   //Continúa escribiendo hasta comunicar exitosamente.
    status=0;           //Por si las moscas.

    /* 3: Recibir ACK */
    IdleI2C1();

    /* 4: Enviar Registro */
    do
    {
        status=WriteI2C1(Registro);  //Escritura de Comando
        if(status == -1)            //Verifica se existió colisión de datos.
        {
            Borrar_Data=SSP1BUF;    //Después de colisión, limpiar Buffer.
            SSPCON1bits.WCOL=0;     //Clear el bit de colisión.
        }
    }
    while(status!=0);
    status=0;           //Por si las moscas.

    /* 5: Recibir ACK */
    IdleI2C1();

    /* 6: Enviar Dato */
    Borrar_Data=SSP1BUF;    //Lectura de cualquier dato almacenado en el buffer para limpiarlo.
    do
    {
        status=WriteI2C1(Dato);     //Escribe al esclavo.
        if(status == -1)            //Verifica se existió colisión de datos.
        {
            Borrar_Data=SSP1BUF;    //Después de colisión, limpiar Buffer.
            SSPCON1bits.WCOL=0;     //Clear el bit de colisión.
        }
    }
    while(status!=0);   //Continúa escribiendo hasta comunicar exitosamente.
    status=0;           //Por si las moscas.

    /* 7: Recibir ACK */
    IdleI2C1();

    /* 8: Enviar NACK  */
    NotAckI2C1();                   //Send the end of transmission signal through nack
    while(SSPCON2bits.ACKEN!=0);    //Espera a término de secuencia ACK

    /* 9. Stop */
    StopI2C1();

    return;

}

/* Obtencion de Datos */
void readFrom(unsigned char Dispositivo, unsigned char Registro, unsigned char Size)
{
    //Declaración de Variables.
    unsigned char Borrar_Data=0;    //Para limpiar registro
    signed char status;             //Para detectar errores

    /* 1: Start */
    StartI2C1();

    /* 2: Dirección del Dispositivo y R/W=0 (W) */
    Dispositivo=Dispositivo<<1;     //Se hace un corrimiento de los 7Lsb hacia la Izquierda.
    Borrar_Data=SSP1BUF;            //Lectura de cualquier dato almacenado en el buffer para limpiarlo.
    do
    {
        status=WriteI2C1(Dispositivo&0b11111110); //Escribe la dirección del esclavo con todo y R/W=0;
        if(status == -1)            //Verifica se existió colisión de datos.
        {
            Borrar_Data=SSP1BUF;    //Después de colisión, limpiar Buffer.
            SSPCON1bits.WCOL=0;     //Clear el bit de colisión.
        }
    }
    while(status!=0);   //Continúa escribiendo hasta comunicar exitosamente.
    status=0;           //Por si las moscas.

    /* 3: Recibir ACK */
    IdleI2C1();

    /* 4: Enviar Registro */
    do
    {
        status=WriteI2C1(Registro);  //Escritura de Comando
        if(status == -1)            //Verifica se existió colisión de datos.
        {
            Borrar_Data=SSP1BUF;    //Después de colisión, limpiar Buffer.
            SSPCON1bits.WCOL=0;     //Clear el bit de colisión.
        }
    }
    while(status!=0);
    status=0;           //Por si las moscas.

    /* 5: Recibir ACK */
    IdleI2C1();

    /* 6. Re-Start*/
    RestartI2C1();

    /* 7: Dirección del Esclavo y R/W=1 */
    Borrar_Data=SSP1BUF;    //Lectura de cualquier dato almacenado en el buffer para limpiarlo.
    do
    {
        status=WriteI2C1(Dispositivo|0b00000001); //Escribe la dirección del esclavo con todo y R/W=1;
        if(status == -1)            //Verifica se existió colisión de datos.
        {
            Borrar_Data=SSP1BUF;    //Después de colisión, limpiar Buffer.
            SSPCON1bits.WCOL=0;     //Clear el bit de colisión.
        }
    }
    while(status!=0);   //Continúa escribiendo hasta comunicar exitosamente.
    status=0;           //Por si las moscas.

    /* 8: Recibir ACK */
    IdleI2C1();

    /* 9: Recibir Byte */
    unsigned char i=0;    //Contador
    
    for(i=0;i<Size;i++)
    {
        //while(DataRdyI2C1()==0);    //Espera a que existan datos en el Bufer
        I2Cbuffer[i]=ReadI2C1();
        /* 10. Enviar ACK para seguir recibiendo */
        if(i<Size-1)
        {
            AckI2C1();
        }
        else    /* 11: Enviar NACK para terminar.*/
        {
            NotAckI2C1();                   //Send the end of transmission signal through nack
            while(SSPCON2bits.ACKEN!=0);    //Espera a término de secuencia ACK
        }
        
    }
    
    /* 12. Stop */
    StopI2C1();

    return;
}

/* Configuración del I2C || pg.222 */
void setupI2C(void)
{
    /* i2c pins */
    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC4 = 0;
    TRISCbits.RC3 = 1;    //RC3 SCL1
    TRISCbits.RC4 = 1;    //RC4 SDA1
    
    /*Configuración del Módulo I2C1 */
    CloseI2C1();                //Cierra I2C en caso de operación previa.
    OpenI2C(MASTER, SLEW_ON);   //Inicializa el Módulo I2C| Slew rate control enabled for high speed mode (400 kHz)
    SSP1ADD=0x13;   //BRG Value: 13h @Fosc 32MHz @Fcy 8MHz Fclock: 400kHz
    IdleI2C1();     //Espera condición Idle
    return;
}