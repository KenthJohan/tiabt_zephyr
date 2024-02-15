#include "mydefs.h"

app_t app = {
	.app_print_mode = APP_PRINT_MODE_UNKNOWN
};


char const * myid_t_tostr(myid_t id)
{
	switch (id)
	{
	case MYID_ADC_CH0: return "ADC_CH0";
	case MYID_ADC_CH1: return "ADC_CH1";
	case MYID_ADC_CH2: return "ADC_CH2";
	case MYID_ADC_CH3: return "ADC_CH3";
	case MYID_ADC_CH4: return "ADC_CH4";
	case MYID_ADC_CH5: return "ADC_CH5";
	case MYID_ADC_CH6: return "ADC_CH6";
	case MYID_ADC_CH7: return "ADC_CH7";
	case MYID_ADC_OFFSET: return "ADC_VREF";
	case MYID_ADC_VCM: return "ADC_VCM";
	case MYID_ADC_AVDD: return "ADC_AVDD";
	case MYID_ADC_TEMP: return "ADC_TEMP";

	case MYID_DPOT0_WIPER: return "DPOT0_WIPER";
	case MYID_DPOT1_WIPER: return "DPOT1_WIPER";
	case MYID_DPOT2_WIPER: return "DPOT2_WIPER";
	case MYID_DPOT3_WIPER: return "DPOT3_WIPER";
	case MYID_DPOT4_WIPER: return "DPOT4_WIPER";
	case MYID_DPOT5_WIPER: return "DPOT5_WIPER";
	case MYID_DPOT6_WIPER: return "DPOT6_WIPER";
	
	default:
		break;
	}
}
