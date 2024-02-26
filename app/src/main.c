/*
west build -p -b nucleo_wb55rg
west build -p -b esp32c3_devkitm

Timer for nucleo_wb55rg:
https://github.com/zephyrproject-rtos/zephyr/discussions/47033
https://github.com/zephyrproject-rtos/zephyr/blob/main/samples/drivers/counter/alarm/src/main.c

*/


#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/sys/util.h>
#include <stdint.h>
#include <stdio.h>


#include "app_version.h"
#include "egadc.h"
#include "dpot.h"
#include "mybt.h"
#include "printer.h"
#include "mydefs.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);
//struct spi_dt_spec bus = SPI_DT_SPEC_GET(DT_NODELABEL(mcp3204), SPI_WORD_SET(8) | SPI_MODE_GET(0), 1);






void app_print_voltage_ref(struct mcp356x_config * c)
{
	LOG_INF("Checking voltage reference");
	egadc_adc_value_reset(c);
	egadc_set_mux(c, MCP356X_MUX_VIN_NEG_AGND | MCP356X_MUX_VIN_POS_VREF_EXT_PLUS);
	int n = 10;
	while (n--) {
		mcp356x_config_print_voltage(c);
		egadc_adc_value_reset(c);
		k_sleep(K_MSEC(500));
	}
}

void app_print_temperature(struct mcp356x_config * c)
{
	egadc_adc_value_reset(c);
	LOG_INF("Checking temperature MCP356X_MUX_VIN_POS_TEMP");
	egadc_set_ch(c, MCP356X_CH_TEMP);
	int n = 10;
	while (n--) {
		//printf("%04i %f C\n", voltage, celcius * (c->vref_mv / 3.3));
		k_sleep(K_MSEC(500));
	}
}




struct mcp356x_config myadc = {
	.state = EGADC_STATE_START,
	.bus = SPI_DT_SPEC_GET(DT_NODELABEL(examplesensor0), SPI_WORD_SET(8) | SPI_MODE_GET(0), 1),
	.irq = GPIO_DT_SPEC_GET(DT_NODELABEL(examplesensor0), irq_gpios),
	.is_scan = false,
	//.gain_reg = MCP356X_CFG_2_GAIN_X_033,
	.gain_reg = MCP356X_CFG_2_GAIN_X_1,
};

struct mcp45hvx1_config dpots[7] = {
	{.bus = I2C_DT_SPEC_GET(DT_NODELABEL(dpot0)), .myid = MYID_DPOT0_WIPER},
	{.bus = I2C_DT_SPEC_GET(DT_NODELABEL(dpot1)), .myid = MYID_DPOT1_WIPER},
	{.bus = I2C_DT_SPEC_GET(DT_NODELABEL(dpot2)), .myid = MYID_DPOT2_WIPER},
	{.bus = I2C_DT_SPEC_GET(DT_NODELABEL(dpot3)), .myid = MYID_DPOT3_WIPER},
	{.bus = I2C_DT_SPEC_GET(DT_NODELABEL(dpot4)), .myid = MYID_DPOT4_WIPER},
	{.bus = I2C_DT_SPEC_GET(DT_NODELABEL(dpot5)), .myid = MYID_DPOT5_WIPER},
	{.bus = I2C_DT_SPEC_GET(DT_NODELABEL(dpot6)), .myid = MYID_DPOT6_WIPER},
};

static const struct gpio_dt_spec load_switch1 = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(load_switch1), gpios, {0});
static const struct gpio_dt_spec load_switch2 = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(load_switch2), gpios, {0});


static const struct gpio_dt_spec leds[MY_LEDS_COUNT] = 
{
	GPIO_DT_SPEC_GET(DT_NODELABEL(blue_led_1), gpios), //Blue
	GPIO_DT_SPEC_GET(DT_NODELABEL(green_led_2), gpios), //Green
	GPIO_DT_SPEC_GET(DT_NODELABEL(green_led_3), gpios), //Red
};


int test_leds()
{
	for(int i = 0; i < MY_LEDS_COUNT; ++i) {
		int ret;
		if (!gpio_is_ready_dt(leds+i)) {return 0;}
		ret = gpio_pin_configure_dt(leds+i, GPIO_OUTPUT_ACTIVE);
		if (ret < 0) {return 0;}
		k_sleep(K_MSEC(500));
		ret = gpio_pin_set_dt(leds+i,1);
		if (ret < 0) {return 0;}
	}
	k_sleep(K_MSEC(500));
	for(int i = 0; i < MY_LEDS_COUNT; ++i) {
		int ret = gpio_pin_set_dt(leds+i,0);
		if (ret < 0) {return 0;}
	}
	return 0;
}


int main(void)
{
	//printk("Color  \x1b[31;4mtext test\n");
	LOG_INF("examplesensor0 %s", "" DT_NODE_PATH(DT_NODELABEL(examplesensor0)));
	LOG_INF("dpot0 %s, %02X", "" DT_NODE_PATH(DT_NODELABEL(dpot0)), dpots[0].bus.addr);
	LOG_INF("dpot1 %s, %02X", "" DT_NODE_PATH(DT_NODELABEL(dpot1)), dpots[1].bus.addr);
	LOG_INF("dpot2 %s, %02X", "" DT_NODE_PATH(DT_NODELABEL(dpot2)), dpots[2].bus.addr);
	LOG_INF("dpot3 %s, %02X", "" DT_NODE_PATH(DT_NODELABEL(dpot3)), dpots[3].bus.addr);
	LOG_INF("dpot4 %s, %02X", "" DT_NODE_PATH(DT_NODELABEL(dpot4)), dpots[4].bus.addr);
	LOG_INF("dpot5 %s, %02X", "" DT_NODE_PATH(DT_NODELABEL(dpot5)), dpots[5].bus.addr);
	LOG_INF("dpot6 %s, %02X", "" DT_NODE_PATH(DT_NODELABEL(dpot6)), dpots[6].bus.addr);

	LOG_INF("Checking loadswitch1");
	if (!gpio_is_ready_dt(&load_switch1)) {
		LOG_ERR("The load switch pin GPIO port is not ready");
		return 0;
	}

	LOG_INF("Checking loadswitch2");
	if (!gpio_is_ready_dt(&load_switch1)) {
		LOG_ERR("The load switch pin GPIO port is not ready");
		return 0;
	}

	{
		int err = gpio_pin_configure_dt(&load_switch1, GPIO_OUTPUT_INACTIVE);
		if (err != 0) {
			LOG_ERR("Configuring GPIO pin failed: %d\n", err);
			return 0;
		}
	}

	{
		int err = gpio_pin_configure_dt(&load_switch2, GPIO_OUTPUT_INACTIVE);
		if (err != 0) {
			LOG_ERR("Configuring GPIO pin failed: %d\n", err);
			return 0;
		}
	}

	LOG_INF("Checking SPI BUS");
	if (!spi_is_ready_dt(&myadc.bus)) {
		LOG_ERR("SPI bus is not ready %i", 0);
		return 0;
	}
	LOG_INF("Checking SPI OK");
	
	LOG_INF("Checking I2C BUS");
	for(int i = 0; i < 7; ++i) {
		if (!i2c_is_ready_dt(&dpots[i].bus)) {
			LOG_ERR("Failed to get pointer to %s device!", dpots[i].bus.bus->name);
			return -EINVAL;
		}
	}
	LOG_INF("Checking I2C OK");


	LOG_INF("Init bluetooth");
	mybt_init();

	LOG_INF("Testing LEDS");
	test_leds();

	
	LOG_INF("Setup ADC");
	egadc_setup_adc(&myadc);
	

	while(1) {
		mybt_progress(&app);
		//egadc_progress(&myadc);
		dpot_progress(&dpots[1]);
		dpot_progress(&dpots[0]);
		dpot_progress(&dpots[2]);
		dpot_progress(&dpots[3]);
		dpot_progress(&dpots[4]);
		dpot_progress(&dpots[5]);
		dpot_progress(&dpots[6]);

		if(app.values_flags[MYID_APP_VREG1_EN] & MYFLAG_SETVAL) {
			app.values_flags[MYID_APP_VREG1_EN] &= ~MYFLAG_SETVAL;
			LOG_INF("Vreg enable: %i", !!app.values[MYID_APP_VREG1_EN]);
			int err = gpio_pin_set_dt(&load_switch1, !!app.values[MYID_APP_VREG2_EN]);
			if (err != 0) {
				printf("Setting GPIO pin level failed: %d\n", err);
			}
		}

		if(app.values_flags[MYID_APP_VREG2_EN] & MYFLAG_SETVAL) {
			app.values_flags[MYID_APP_VREG2_EN] &= ~MYFLAG_SETVAL;
			LOG_INF("Vreg enable: %i", !!app.values[MYID_APP_VREG2_EN]);
			int err = gpio_pin_set_dt(&load_switch2, !!app.values[MYID_APP_VREG2_EN]);
			if (err != 0) {
				printf("Setting GPIO pin level failed: %d\n", err);
			}
		}

		switch (app.values[MYID_APP_PRINT_MODE])
		{
		case APP_PRINT_MODE_ADC_ALL:
			if(myadc.state == EGADC_STATE_READY) {
				app_print_adc_all(&app);
			}
			break;
		
		default:
			break;
		}

		k_sleep(K_SECONDS(1));
	}



	return 0;
}


