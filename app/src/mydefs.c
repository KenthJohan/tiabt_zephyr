#include "mydefs.h"

app_t app = {
	.app_print_mode = APP_PRINT_MODE_UNKNOWN,
	.values = {
		[MYID_APP_PRINT_MODE] = APP_PRINT_MODE_ADC_ALL
	}
};


char const * myid_t_tostr(myid_t id)
{
	switch (id)
	{
	case MYID_APP_PRINT_MODE: return "APP_PRINT_MODE";
	case MYID_ADC_CH0: return "ADC_CH0";
	case MYID_ADC_CH1: return "ADC_CH1";
	case MYID_ADC_CH2: return "ADC_CH2";
	case MYID_ADC_CH3: return "ADC_CH3";
	case MYID_ADC_CH4: return "ADC_CH4";
	case MYID_ADC_CH5: return "ADC_CH5";
	case MYID_ADC_CH6: return "ADC_CH6";
	case MYID_ADC_CH7: return "ADC_CH7";
	case MYID_CH0_PIKO_A: return "CH0_PIKO_A";
	case MYID_CH1_PIKO_A: return "CH1_PIKO_A";
	case MYID_CH2_PIKO_A: return "CH2_PIKO_A";
	case MYID_CH3_PIKO_A: return "CH3_PIKO_A";
	case MYID_CH4_PIKO_A: return "CH4_PIKO_A";
	case MYID_CH5_PIKO_A: return "CH5_PIKO_A";
	case MYID_CH6_PIKO_A: return "CH6_PIKO_A";
	case MYID_CH7_PIKO_A: return "CH7_PIKO_A";
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
