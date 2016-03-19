#include <xc.h>
#include "config.h"
#include <stdio.h>
#include <math.h>
#include <plib/timers.h>
#include <plib/usart.h>
#include <plib/adc.h>
#include "XBeeAPI16Bits.h"
#include "ADC_Config.h"
#include "ECG.h"
#include "Utilities.h"
#include "Interrupt_manager.h"

float TempExt=0;


int GetTempADC()
{
    unsigned int TempRaw=0;

    SetChanADC(ADC_CH13);

    ConvertADC();

    while(BusyADC());
    TempRaw = ReadADC();

    //CloseADC();


    return TempRaw;


}

float GetTemp()
{

    //Temp=((float)GetTempADC()*VolBat)/1024;


    //Local variables
    float Temperature; //Corporal Temperature
    float Resistance;  //Resistance of sensor.
    float ganancia=5.0;
    float Vcc=VolBat;
    float RefTension=2.999; // Voltage Reference of Wheatstone bridge.
    float Ra=4600.0; //Wheatstone bridge resistance.
    float Rc=4562.0; //Wheatstone bridge resistance.
    float Rb=820.0; //Wheatstone bridge resistance.
    //int sensorValue = GetTempADC();

    float voltage2=((float)GetTempADC()*3.3)/1024; // binary to voltage conversion

    // Wheatstone bridge output voltage.
    voltage2=voltage2/ganancia;

    // Resistance sensor calculate
    float aux=(voltage2/RefTension)+Rb/(Rb+Ra);

    Resistance=Rc*aux/(1-aux);


    if (Resistance >=1822.8) {
            // if temperature between 25�C and 29.9�C. R(t�)=6638.20457*(0.95768)^t
            Temperature = (log10( Resistance/6638.20457))*(-1)  / (log10(0.95768) )*(-1);


    } else {
            if (Resistance >=1477.1){
                            // if temperature between 30�C and 34.9�C. R(t�)=6403.49306*(0.95883)^t
                            Temperature= (log10(Resistance/6403.49306))*(-1) / log10(0.95883)*(-1) ;
            } else {
                    if (Resistance >=1204.8){
                            // if temperature between 35�C and 39.9�C. R(t�)=6118.01620*(0.96008)^t
                            Temperature= (log10(Resistance/6118.01620))*(-1) / log10(0.96008)*(-1);
                    }
                    else{
                            if (Resistance >=988.1){
                                    // if temperature between 40�C and 44.9�C. R(t�)=5859.06368*(0.96112)^t
                                    Temperature= (log10(Resistance/5859.06368))*(-1)/log10(0.96112)*(-1);
                            }
                            else {
                                    if (Resistance >=811.7){
                                            // if temperature between 45�C and 50�C. R(t�)=5575.94572*(0.96218)^t
                                            Temperature= (log10(Resistance/5575.94572))*(-1)/log10(0.96218)*(-1);
                                    }
                            }
                    }
            }
    }

    //printf("\t%4.2f  \n\n",Temperature);
    TempExt=Temperature;
    //return Temperature;
    return 1.0;
}


float   GetTempVol(void)
{
    float Temp;

    Temp=((float)GetTempADC()*VolBat)/1024;

    return Temp;
}
