#include "printer.h"
#include <stdio.h>
#include "egadc.h"
#include "mydefs.h"

void mcp356x_config_print_voltage(struct mcp356x_config * c)
{
	// In single channel mode, any channel will be stored in channel 0:
	int index = MCP356X_CH_CH0;
	int32_t v_iir = MCP356X_raw_to_volt(c->raw_iir[index], TIABT_VREF_MICRO_VOLT, c->gain_reg);
	int32_t v_min = MCP356X_raw_to_volt(c->raw_min[index], TIABT_VREF_MICRO_VOLT, c->gain_reg);
	int32_t v_max = MCP356X_raw_to_volt(c->raw_max[index], TIABT_VREF_MICRO_VOLT, c->gain_reg);
	int32_t v_pp = v_max - v_min;
	printk("IRQ:%-3i DRDY:%-3i avg:%-8i min:%-8i max:%-8i pp:%-8i\n", c->num_irq, c->num_drdy, v_iir, v_min, v_max, v_pp);
}


void mcp356x_config_print_temperature(struct mcp356x_config * c)
{
    int raw = c->raw_iir[MCP356X_CH_CH0];
    // TODO: I can't figure out why datasheet temperature transfer function does not work.
    //double celsius  = MCP356X_ADCDATA_to_temperature_o1(raw) * ((double)TIABT_VREF/(double)3300);
    //double celcius  = MCP356X_ADCDATA_to_temperature_o3(raw);

    // If we convert raw ADCDATA to millivolt then use datasheet celsius to mv function (we get correct temperature?)
    // https://www.eevblog.com/forum/microcontrollers/problems-with-internal-temperature-sensor-mcp3561/
    int32_t v = MCP356X_raw_to_volt(raw, TIABT_VREF_MICRO_VOLT, c->gain_reg);
    double celsius = ((double)v -79.32) / (0.2964*10000);
    printf("%08i %08i %20.10f C\n", raw, v, celsius);
}

/*
printk("%08X\n", c.lastdata);
printk("%8i %8i\n", c.num_irq, c.num_drdy);
printk("    " MCP356X_PRINTF_HEADER "\n");
printk("avg " MCP356X_PRINTF_PLUS "\n", MCP356X_ARGS(c.avg));
printk("min " MCP356X_PRINTF_PLUS "\n", MCP356X_ARGS(c.val_min));
printk("max " MCP356X_PRINTF_PLUS "\n", MCP356X_ARGS(c.val_max));
printk("n   " MCP356X_PRINTF_PLUS "\n", MCP356X_ARGS(c.n));
egadc_log_REG_IRQ(&c.bus, MCP356X_REG_IRQ);
*/




void app_print_adc_all(app_t * a)
{
	printk("%-8i %-8i %-8i %-8i %-8i %-8i %-8i %-8i\n", 
    a->values[MYID_ADC_CH0],
    a->values[MYID_ADC_CH1],
    a->values[MYID_ADC_CH2],
    a->values[MYID_ADC_CH3],
    a->values[MYID_ADC_CH4],
    a->values[MYID_ADC_CH5],
    a->values[MYID_ADC_CH6],
    a->values[MYID_ADC_CH7]
    );
}