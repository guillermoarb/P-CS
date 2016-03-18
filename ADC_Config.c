#include <plib/adc.h>


void Init_ADC()
{

    unsigned char config1=0x00,config2=0x00,config3=0x00;


    config1 = ADC_FOSC_32 & ADC_RIGHT_JUST & ADC_0_TAD ;
    config2 = ADC_CH11 & ADC_CH13 & ADC_INT_OFF ;
    config3 = ADC_REF_VDD_VSS ;

    OpenADC(config1,config2,config3);

}
