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
#include "bt.h"
#include "printer.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);
//struct spi_dt_spec bus = SPI_DT_SPEC_GET(DT_NODELABEL(mcp3204), SPI_WORD_SET(8) | SPI_MODE_GET(0), 1);






void app_print_voltage_ref(struct mcp356x_config * c)
{
	LOG_INF("Checking voltage reference");
	egadc_adc_value_reset(c);
	egadc_set_mux(c, MCP356X_MUX_VIN_NEG_AGND | MCP356X_MUX_VIN_POS_VREF_EXT_PLUS);
	int n = 10;
	while (n--)
	{
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
	while (n--)
	{
		//printf("%04i %f C\n", voltage, celcius * (c->vref_mv / 3.3));
		k_sleep(K_MSEC(500));
	}
}




struct mcp356x_config c = {
	.state = EGADC_STATE_START,
	.bus = SPI_DT_SPEC_GET(DT_NODELABEL(examplesensor0), SPI_WORD_SET(8) | SPI_MODE_GET(0), 1),
	.irq = GPIO_DT_SPEC_GET(DT_NODELABEL(examplesensor0), irq_gpios),
	.is_scan = false,
	//.gain_reg = MCP356X_CFG_2_GAIN_X_033,
	.gain_reg = MCP356X_CFG_2_GAIN_X_1,
};


struct mcp45hvx1_config c1 = {
	.bus = I2C_DT_SPEC_GET(DT_NODELABEL(dpot0))
};




#define LEDS_COUNT 3
static const struct gpio_dt_spec leds[LEDS_COUNT] = 
{
	GPIO_DT_SPEC_GET(DT_NODELABEL(blue_led_1), gpios), //Blue
	GPIO_DT_SPEC_GET(DT_NODELABEL(green_led_2), gpios), //Green
	GPIO_DT_SPEC_GET(DT_NODELABEL(green_led_3), gpios), //Red
};





static int dpot_inc = 0;
static uint8_t pot = 0;



int main(void)
{
	printk("Color  \x1b[31;4mtext test\n");
	LOG_INF("examplesensor0 %s", "" DT_NODE_PATH(DT_NODELABEL(examplesensor0)));
	LOG_INF("dpot0 %s", "" DT_NODE_PATH(DT_NODELABEL(dpot0)));

	if (!spi_is_ready_dt(&c.bus)) {
		LOG_ERR("SPI bus is not ready %i", 0);
		return 0;
	}

	if (!i2c_is_ready_dt(&c1.bus)) {
		LOG_ERR("Failed to get pointer to %s device!", c1.bus.bus->name);
		return -EINVAL;
	}
	
	mybt_init();
	

	while(1) {
		mybt_progress();
		egadc_progress(&c);
		k_sleep(K_SECONDS(1));
	}















	/*
	dpot_setup(&c1);
	
	counter_start(counter_dev);
	alarm_cfg.flags = 0;
	alarm_cfg.ticks = counter_us_to_ticks(counter_dev, DELAY);
	alarm_cfg.callback = test_counter_interrupt_fn;
	alarm_cfg.user_data = &alarm_cfg;
	printk("Set alarm in %u sec (%u ticks)\n",
	       (uint32_t)(counter_ticks_to_us(counter_dev,
					   alarm_cfg.ticks) / USEC_PER_SEC),
	       alarm_cfg.ticks);
		   
	{
		int err = counter_set_channel_alarm(counter_dev, ALARM_CHANNEL_ID, &alarm_cfg);
		if (-EINVAL == err) {
			printk("Alarm settings invalid\n");
		} else if (-ENOTSUP == err) {
			printk("Alarm setting request not supported\n");
		} else if (err != 0) {
			printk("Error\n");
		}
	}
	
	while(1) {
		if(dpot_inc){
			dpot_set(&c1, pot++);
			dpot_inc = 0;
		};
		mybt_progress();
		k_sleep(K_MSEC(1));
	}
	//mybt_init();
	//while (1){k_sleep(K_MSEC(5000));}

	


	for(int i = 0; i < LEDS_COUNT; ++i)
	{
		int ret;
		if (!gpio_is_ready_dt(leds+i)) {return 0;}
		ret = gpio_pin_configure_dt(leds+i, GPIO_OUTPUT_ACTIVE);
		if (ret < 0) {return 0;}
		k_sleep(K_MSEC(500));
		ret = gpio_pin_set_dt(leds+i,1);
		if (ret < 0) {return 0;}
	}
	k_sleep(K_MSEC(500));
	for(int i = 0; i < LEDS_COUNT; ++i)
	{
		int ret = gpio_pin_set_dt(leds+i,0);
		if (ret < 0) {return 0;}
	}



	int appstate = APP_START;

	while (1)
	{
		{
			int ret = gpio_pin_toggle_dt(leds+MY_LEDS_BEATS);
			if (ret < 0) {return 0;}
		}

		if(c.status & EGADC_TIMEOUT_BIT)
		{
			appstate = APP_INIT_ADC;
			c.status &= ~EGADC_TIMEOUT_BIT;
		}


		switch (appstate)
		{
		case APP_WAITING:
			if(c.num_drdy > 0)
			{
				//app_print_voltage_ref(&c);
				//app_print_temperature(&c);
				egadc_set_ch(&c, MCP356X_CH_CH3);
				appstate = APP_PRINT_ADC;
			}
			else
			{
				LOG_INF("APP_WAITING No respond from ADC");
				gpio_pin_set_dt(leds + MY_LEDS_WAITING, 1);
				k_sleep(K_MSEC(500));
				gpio_pin_set_dt(leds + MY_LEDS_WAITING, 0);
				k_sleep(K_MSEC(500));
			}
			break;

		case APP_START:
			LOG_INF("APP_START");
			egadc_setup_board(&c);
			appstate = APP_INIT_ADC;
			break;

		case APP_INIT_ADC:
			LOG_INF("APP_INIT_ADC");
			egadc_setup_adc(&c);
			appstate = APP_WAITING;
			break;

		case APP_PRINT_ADC:{
			k_sleep(K_MSEC(1000));
			app_print_voltage(&c);
			egadc_adc_value_reset(&c);
			break;}

		default:
			break;
		}




		//printk("%08X\n", c.lastdata);
		
		
		//printk("%8i %8i\n", c.num_irq, c.num_drdy);
		//printk("    " MCP356X_PRINTF_HEADER "\n");
		//printk("avg " MCP356X_PRINTF_PLUS "\n", MCP356X_ARGS(c.avg));
		//printk("min " MCP356X_PRINTF_PLUS "\n", MCP356X_ARGS(c.val_min));
		//printk("max " MCP356X_PRINTF_PLUS "\n", MCP356X_ARGS(c.val_max));
		//printk("n   " MCP356X_PRINTF_PLUS "\n", MCP356X_ARGS(c.n));
		
		//egadc_log_REG_IRQ(&c.bus, MCP356X_REG_IRQ);

		
		
		//mybt_progress();
		//bt_bas_set_battery_level(i++);

		//k_sem_give(&c.acq_sem);
	}
	*/
}


