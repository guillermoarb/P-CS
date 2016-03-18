/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File:   adxl345.h
 * Author: MiguelAlejandro
 *
 * Created on 13 de octubre de 2015, 06:22 PM
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ADXL345_H
#define	ADXL345_H

#ifdef	__cplusplus
extern "C" {
#endif

    /* Accelerometer definitions */
        #define ADXL345                 0x53    //Dirección 7bits
        #define ADXL345_SENS            0.00380859375  // 3.9/[2^10bits] = 0.00380859375 [Tabla 1]
        #define ADXL345_SAMPLES         100
       
    /* Register names */
        #define ADXL_DEV_ID             0x00    //Device ID, should return 0xE5
                        //  0x01 to 0x1C Reserved
        #define ADXL345_THRESH_TAP      0x1D    //Tap Threshold
        #define ADXL345_OFSX            0x1E    //X-axis offset
        #define ADXL345_OFSY            0x1F    //Y-axis offset
        #define ADXL345_OFSZ            0x20    //Z-axis offset
        #define ADXL345_DUR             0x21    //Tap duration
        #define ADXL345_LATENT          0x22    //Tap latency
        #define ADXL345_WINDOW          0x23    //Tap window
        #define ADXL345_THRESH_ACT      0x24    //Activity Threshold
        #define ADXL345_THRESH_INACT    0x25    //Inactivity Threshold
        #define ADXL345_TIME_INACT      0x26    //Inactivity Time
        #define ADXL345_ACT_INACT_CTL   0x27    //Axis enable control for activity and inactivity detection
        #define ADXL345_THRESH_FF       0x28    //Free-fall threshold
        #define ADXL345_TIME_FF         0x29    //Free-fall time
        #define ADXL345_TAP_AXES        0x2A    //Axis control for single tap/double tap
        #define ADXL345_ACT_TAP_STATUS  0x2B    //Source of single tap/double tap
        #define ADXL345_BW_RATE         0x2C    //Data rate and power mode control
        #define ADXL345_POWER_CTL       0x2D    //Power-saving features control
        #define ADXL345_INT_ENABLE      0x2E    //Interrupt enable control
        #define ADXL345_INT_MAP         0x2F    //Interrupt mapping control
        #define ADXL345_INT_SOURCE      0x30    //Source of interrupts
        #define ADXL345_DATA_FORMAT     0x31    //Data format control
        #define ADXL345_DATA            0x32    //LSB from X axis... it starts data reception
        #define ADXL345_DATAX0          0x32    //X-Axis Data 0
        #define ADXL345_DATAX1          0x33    //X-Axis Data 1
        #define ADXL345_DATAY0          0x34    //Y-Axis Data 0
        #define ADXL345_DATAY1          0x35    //Y-Axis Data 1
        #define ADXL345_DATAZ0          0x36    //Z-Axis Data 0
        #define ADXL345_DATAZ1          0x37    //Z-Axis Data 1
        #define ADXL345_FIFO_CTL        0x38    //FIFO Control
        #define ADXL345_FIFO_STATUS     0x39    //FIFO Status
        
    /* Activity/Inactivity Control options */
    
    
    /* Tap Axes options */
        #define TAP_AXES_SUPRESS        0x8     //00001000
        #define TAP_AXES_X_ENABLE       0x4     //00000100
        #define TAP_AXES_Y_ENABLE       0x2     //00000010
        #define TAP_AXES_Z_ENABLE       0x1     //00000001
    
    /* Bandwidth options */
        #define BW_1600                 0x0F    // 1111
        #define BW_800                  0x0E    // 1110
        #define BW_400                  0x0D    // 1101  
        #define BW_200                  0x0C    // 1100
        #define BW_100                  0x0B    // 1011  
        #define BW_50                   0x0A    // 1010 
        #define BW_25                   0x09    // 1001 
        #define BW_12_5                 0x08    // 1000 
        #define BW_6_25                 0x07    // 0111
        #define BW_3_125                0x06    // 0110
        #define BW_LOW_POWER            0x10    // A setting of 1 selects reduced power operation, which has somewhat higher noise.

    /* Power Control options */
        #define POWER_CTL_LINK          0x20    // 00100000
        #define POWER_CTL_AUTO_SLEEP    0x10    // 00010000
        #define POWER_CTL_MEASURE       0x8     // 00001000
        #define POWER_CTL_SLEEP         0x4     // 00000100
        #define POWER_CTL_WAKEUP_1      0x3     // 00000011
        #define POWER_CTL_WAKEUP_2      0x2     // 00000010
        #define POWER_CTL_WAKEUP_4      0x1     // 00000001
        #define POWER_CTL_WAKEUP_8      0x0     // 00000000
    
    /* Interrupt Enable options. IT IS RECOMMENDED THAT INTS BE 
        CONFIGURED BEFORE ENABLING THEIR OUTPUTS */
        #define INT_ENABLE_DATA_READY   0x80    // 10000000
        #define INT_ENABLE_SINGLE_TAP   0x40    // 01000000
        #define INT_ENABLE_DOUBLE_TAP   0x20    // 00100000
        #define INT_ENABLE_ACTIVITY     0x10    // 00010000
        #define INT_ENABLE_INACTIVITY   0x08    // 00001000
        #define INT_ENABLE_FREE_FALL    0x04    // 00000100
        #define INT_ENABLE_WATERMARK    0x02    // 00000010
        #define INT_ENABLE_OVERRUN      0x01    // 00000001
        #define INT_DISABLE_ALL         0x00    // 00000001

    /* Interrupt Map options. When 1, the interrupt is sent to INT2 pin */
        #define INT_MAP_DATA_READY      0x80    // 10000000
        #define INT_MAP_SINGLE_TAP      0x40    // 01000000
        #define INT_MAP_DOUBLE_TAP      0x20    // 00100000
        #define INT_MAP_ACTIVITY        0x10    // 00010000
        #define INT_MAP_INACTIVITY      0x08    // 00001000
        #define INT_MAP_FREE_FALL       0x04    // 00000100
        #define INT_MAP_WATERMARK       0x02    // 00000010
        #define INT_MAP_OVERRUN         0x01    // 00000001

    /* Interrupt Source options */
        #define INT_SOURCE_DATA_READY   0x80    // 10000000
        #define INT_SOURCE_SINGLE_TAP   0x40    // 01000000
        #define INT_SOURCE_DOUBLE_TAP   0x20    // 00100000
        #define INT_SOURCE_ACTIVITY     0x10    // 00010000
        #define INT_SOURCE_INACTIVITY   0x08    // 00001000
        #define INT_SOURCE_FREE_FALL    0x04    // 00000100
        #define INT_SOURCE_WATERMARK    0x02    // 00000010
        #define INT_SOURCE_OVERRUN      0x01    // 00000001
        
    /* Data format options */
        #define DATA_FORMAT_SELF_TEST   0x80
        #define DATA_FORMAT_SPI         0x40
        #define DATA_FORMAT_INT_INVERT  0x20
        #define DATA_FORMAT_RESERVED    0x10
        #define DATA_FORMAT_FULL_RES    0x08
        #define DATA_FORMAT_JUSTIFY     0x04
        #define DATA_FORMAT_RANGE_16    0x03
        #define DATA_FORMAT_RANGE_8     0x02
        #define DATA_FORMAT_RANGE_4     0x01
        #define DATA_FORMAT_RANGE_2     0x00

    /* Accelerometer Functions */
        void getRawADXL(void);
        void getGforceADXL(void);
        void getBiasADXL(void);
        unsigned char initADXL345(void);
        unsigned char getPosition(void);
        unsigned char isFallen(void);
    
    /* utilities Functions */
    unsigned int make16(unsigned char H, unsigned char L);
    signed short eval2comp(unsigned short n, unsigned short x);
    
    /* Accelerometer Variables */
    extern signed short rawADXL[3];     //Datos RAW del ADXL345
    extern double biasADXL[3];          //Bias del ADXL
    extern double GforceADXL[3];        //Fuerzas G del Acelerómetro


#ifdef	__cplusplus
}
#endif

#endif	/* ADXL345_H */

