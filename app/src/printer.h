#pragma once
#include "egadc.h"
#include "mydefs.h"

void mcp356x_config_print_voltage(struct mcp356x_config * c);
void mcp356x_config_print_temperature(struct mcp356x_config * c);

void app_print_adc_all(app_t * a);