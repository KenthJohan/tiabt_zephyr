#include "egadc.h"

#include <zephyr/drivers/spi.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>

#include "MCP356X.h"
#include "mydefs.h"


LOG_MODULE_REGISTER(egadc, LOG_LEVEL_DBG);


static void MCP356X_log_REG_IRQ(uint32_t value)
{
	LOG_INF("MCP356X_IRQ_MASK_EN_STP:        %i", !!(value & MCP356X_IRQ_MASK_EN_STP));
	LOG_INF("MCP356X_IRQ_MASK_EN_FASTCMD:    %i", !!(value & MCP356X_IRQ_MASK_EN_FASTCMD));
	LOG_INF("MCP356X_IRQ_MASK_MODE0:         %i", !!(value & MCP356X_IRQ_MASK_MODE0));
	LOG_INF("MCP356X_IRQ_MASK_MODE1:         %i", !!(value & MCP356X_IRQ_MASK_MODE1));
	LOG_INF("MCP356X_IRQ_MASK_POR_STATUS:    %i", !!(value & MCP356X_IRQ_MASK_POR_STATUS));
	LOG_INF("MCP356X_IRQ_MASK_CRCCFG_STATUS: %i", !!(value & MCP356X_IRQ_MASK_CRCCFG_STATUS));
	LOG_INF("MCP356X_IRQ_MASK_DR_STATUS:     %i", !!(value & MCP356X_IRQ_MASK_DR_STATUS));
	LOG_INF("MCP356X_IRQ_MASK_UNIMPLEMENTED: %i", !!(value & MCP356X_IRQ_MASK_UNIMPLEMENTED));
}

static void MCP356X_log_REG_MUX(uint32_t value)
{
	LOG_INF("MCP356X_REG_MUX_VIN_POS: %s", MCP356X_MUX_POS_to_str(value));
	LOG_INF("MCP356X_REG_MUX_VIN_NEG: %s", MCP356X_MUX_NEG_to_str(value));
}

static void MCP356X_log_REG(uint32_t reg, uint32_t value)
{
	switch (reg)
	{
	case MCP356X_REG_IRQ:
		MCP356X_log_REG_IRQ(value);
		break;
	case MCP356X_REG_MUX:
		MCP356X_log_REG_MUX(value);
		break;
	default:
		break;
	}
}




static int transceive(const struct spi_dt_spec *bus, uint8_t *tx, uint8_t *rx, uint8_t reg, uint8_t len, uint8_t cmd)
{
	struct spi_buf buf_tx[] = {{.buf = tx,.len = len+1}};
	struct spi_buf buf_rx[] = {{.buf = rx,.len = len+1}};
	struct spi_buf_set tx_buf = {.buffers = buf_tx, .count = 1};
	struct spi_buf_set rx_buf = {.buffers = buf_rx, .count = 1};
	tx[0] = MCP356X_COMMAND_BYTE(MCP356X_DEVICE_ADR, reg, cmd);
	int rv = spi_transceive_dt(bus, &tx_buf, &rx_buf);
	return rv;
}


static int get(const struct spi_dt_spec *bus, uint8_t reg, uint32_t * value)
{
	uint8_t len = MCP356X_get_len(reg);
	uint8_t tx[5] = {0};
	uint8_t rx[5] = {0};
	int rv = transceive(bus, tx, rx, reg, len, MCP356X_CMD_INC_READ);
	*value = MCP356X_get_value(rx, len);
	//printk("MCP356X_get_value: %2i : %02X : %02X %02X %02X %02X : %5i\n", len, rx[0], rx[1], rx[2], rx[3], rx[4], v);
	return rv;
}

/*
In MUX mode, channel is defaulted to 0.
Get ADC data as (dataformat 11), see datasheet for more info
*/
static int get_data_11(const struct spi_dt_spec *bus, int32_t * out_value, uint8_t * out_channel)
{
	uint8_t reg = MCP356X_REG_ADC_DATA;
	uint8_t len = 4;
	uint8_t tx[5] = {0};
	uint8_t rx[5] = {0};
	int rv = transceive(bus, tx, rx, reg, len, MCP356X_CMD_INC_READ);
	MCP356X_ADC_DATA_decode_11(rx, out_value, out_channel);
	return rv;
}


static int egadc_log_REG_IRQ(const struct spi_dt_spec *bus, uint8_t reg)
{
	uint32_t value;
	int rv = get(bus, reg, &value);
	if(reg == MCP356X_REG_IRQ)
	{
		MCP356X_log_REG_IRQ(value);
	}
	return rv;
}


#define MCP356X_LOG_SET_VALUE
static int set(const struct spi_dt_spec *bus, uint8_t reg, uint32_t value)
{
	uint8_t len = MCP356X_get_len(reg);
	uint8_t tx[5] = {0};
	uint8_t rx[5] = {0};
	MCP356X_set_value(tx, len, value);
	int rv = transceive(bus, tx, rx, reg, len, MCP356X_CMD_INC_WRITE);
#ifdef MCP356X_LOG_SET_VALUE
	uint32_t value2;
	rv = get(bus, reg, &value2);
	LOG_INF("%s: write:%08x read:%08x", MCP356X_REG_tostring(reg), value, value2);
	MCP356X_log_REG(reg, value2);
#endif
return rv;
}


typedef struct
{
	uint8_t key;
	uint32_t value;
} egadc_regval_pair_t;





void egadc_setup_adc(struct mcp356x_config * config)
{
	LOG_INF("Setting registers in MCP356X");
	/*
	egadc_regval_pair_t a[] = 
	{
		{MCP356X_REG_CFG_0, MCP356X_CFG_0_VREF_SEL_0 | MCP356X_CFG_0_CLK_SEL_2 | MCP356X_CFG_0_CS_SEL_0 | MCP356X_CFG_0_MODE_CONV},
		{MCP356X_REG_CFG_1, MCP356X_CFG_1_PRE_1 | MCP356X_CFG_1_OSR_98304 | MCP356X_CFG_1_DITHER_DEF},
		{MCP356X_REG_CFG_2, MCP356X_CFG_2_BOOST_X_1 | config->gain_reg | MCP356X_CFG_2_AZ_MUX_DIS},
		{MCP356X_REG_CFG_3, MCP356X_CFG_3_CONV_MODE_CONT |MCP356X_CFG_3_DATA_FORMAT_CH_ADC |MCP356X_CFG_3_CRC_FORMAT_16 |MCP356X_CFG_3_CRC_COM_DIS |MCP356X_CFG_3_CRC_OFF_CAL_EN |MCP356X_CFG_3_CRC_GAIN_CAL_EN},
		{MCP356X_REG_IRQ, MCP356X_IRQ_MODE_LOGIC_HIGH},
		{MCP356X_REG_MUX, 0},
		{MCP356X_REG_SCAN, 0},
		{MCP356X_REG_TIMER, 0},
		{MCP356X_REG_OFFSET_CAL, 0},
		{MCP356X_REG_GAIN_CAL, 0x00800000},
		{MCP356X_RSV_REG_W_A, 0x00900000},
		{MCP356X_REG_C, 0},
		{MCP356X_REG_LOCK, 0xA5},
		{MCP356X_RSV_REG, 0},
		{MCP356X_REG_CRC_CFG, 0},
	};
	*/

	set(&config->bus, MCP356X_REG_LOCK, 0xA5); //Unlock
	set(&config->bus, MCP356X_REG_CFG_0,
	MCP356X_CFG_0_VREF_SEL_0 |
	MCP356X_CFG_0_CLK_SEL_2 |
	MCP356X_CFG_0_CS_SEL_0 |
	MCP356X_CFG_0_MODE_CONV
	);
	set(&config->bus, MCP356X_REG_CFG_1,
	MCP356X_CFG_1_PRE_1 |
	//MCP356X_CFG_1_OSR_32 |
	//MCP356X_CFG_1_OSR_256 |
	MCP356X_CFG_1_OSR_98304|
	MCP356X_CFG_1_DITHER_DEF
	);
	set(&config->bus, MCP356X_REG_CFG_2,
	MCP356X_CFG_2_BOOST_X_1 |
	//MCP356X_CFG_2_GAIN_X_1 |
	config->gain_reg |
	MCP356X_CFG_2_AZ_MUX_DIS |
	//MCP356X_CFG_2_AZ_VREF_EN |
	//MCP356X_CFG_2_AZ_FREQ_HIGH |
	0
	);
	set(&config->bus, MCP356X_REG_CFG_3,
	MCP356X_CFG_3_CONV_MODE_CONT |
	//MCP356X_CFG_3_DATA_FORMAT_DEF |
	MCP356X_CFG_3_DATA_FORMAT_CH_ADC |
	MCP356X_CFG_3_CRC_FORMAT_16 |
	MCP356X_CFG_3_CRC_COM_DIS |
	MCP356X_CFG_3_CRC_OFF_CAL_EN |
	MCP356X_CFG_3_CRC_GAIN_CAL_EN
	);
	
	/*
	set(&config->bus, MCP356X_REG_MUX,
	//MCP356X_MUX_VIN_POS_CH0 | //10Mohm + 1µA offset
	//MCP356X_MUX_VIN_POS_CH1 | //10Mohm
	//MCP356X_MUX_VIN_POS_CH5 | //100Mohm + 1µA offset
	MCP356X_MUX_VIN_POS_CH4 | //100Mohm
	//MCP356X_MUX_VIN_POS_CH3 | //1Gohm
	//MCP356X_MUX_VIN_POS_CH1 | 
	//MCP356X_MUX_VIN_POS_CH2 | 
	//MCP356X_MUX_VIN_POS_CH3 | 
	//MCP356X_MUX_VIN_POS_TEMP |
	//MCP356X_MUX_VIN_POS_AVDD | 
	//MCP356X_MUX_VIN_POS_VREF_EXT_PLUS|
	//MCP356X_MUX_VIN_NEG_CH1 |
	//MCP356X_MUX_VIN_POS_CH0 | 
	//MCP356X_MUX_VIN_POS_CH5 | 
	MCP356X_MUX_VIN_NEG_AGND |
	0);
	*/
	
	//set(&config->bus, MCP356X_REG_SCAN, 0);
	
	set(&config->bus, MCP356X_REG_SCAN, 
	MCP356X_SCAN_CH0|
	MCP356X_SCAN_CH1|
	MCP356X_SCAN_CH2|
	MCP356X_SCAN_CH3|
	MCP356X_SCAN_CH4|
	MCP356X_SCAN_CH5|
	MCP356X_SCAN_CH6|
	MCP356X_SCAN_CH7|
	//MCP356X_SCAN_VREF|
	//MCP356X_SCAN_TEMP|
	//MCP356X_SCAN_AVDD|
	//MCP356X_SCAN_OFFSET|
	0);
	
	//set24_verbose(bus, MCP356X_REG_SCAN, MCP356X_SCAN_CH0);
	//set24_verbose(bus, MCP356X_REG_SCAN, MCP356X_SCAN_CH3);
	
	set(&config->bus, MCP356X_REG_IRQ, MCP356X_IRQ_MODE_LOGIC_HIGH);
	set(&config->bus, MCP356X_REG_OFFSET_CAL, 0);
	set(&config->bus, MCP356X_REG_GAIN_CAL, 0x00800000);
	//set(&config->bus, MCP356X_RSV_REG_W_A, 0x00900F00);
	set(&config->bus, MCP356X_RSV_REG_W_A, 0x00900000);

	/*
	{
		uint32_t value;
		uint32_t reg = MCP356X_REG_LOCK;
		get(&config->bus, reg, &value);
		LOG_INF("%s: %08x", MCP356X_REG_tostring(reg), value);
	}
	{
		uint32_t value;
		uint32_t reg = MCP356X_REG_CRC_CFG;
		get(&config->bus, reg, &value);
		LOG_INF("%s: %08x", MCP356X_REG_tostring(reg), value);
	}
	*/
}


void egadc_set_mux(struct mcp356x_config * config, uint8_t mux)
{
	set(&config->bus, MCP356X_REG_MUX, mux);
}


void egadc_set_ch(struct mcp356x_config * config, uint8_t ch)
{
	uint8_t mux = MCP356X_channel_to_mux(ch);
	set(&config->bus, MCP356X_REG_MUX, mux);
}














static void drdy_callback(const struct device *port, struct gpio_callback *cb, gpio_port_pins_t pins)
{
	struct mcp356x_config *config = CONTAINER_OF(cb, struct mcp356x_config, drdy_cb);
	//TODO: Why is num_irq inside drdy_callback much higher than num_drdy inside mcp356x_acquisition_thread?
	config->num_irq++;
	k_sem_give(&config->drdy_sem);
}


static void mcp356x_acquisition_thread(struct mcp356x_config * config)
{
	LOG_INF("mcp356x_acquisition_thread started!");
	config->status |= EGADC_THREAD_STARTED_BIT;
	while (true) {
		int rv = 0;

		// Wait for IRQ callback to give this semaphore:
		rv = k_sem_take(&config->drdy_sem, K_SECONDS(1));
		if(rv != 0) {
			//LOG_ERR("mcp356x_acquisition_thread: k_sem_take: %i\n", rv);
			config->status |= EGADC_TIMEOUT_BIT;
			continue;
		}
		
		config->num_drdy++;
		
		// Read ADC value from register MCP356X_REG_ADC_DATA as DATA_FORMAT=11:
		int32_t value = 0;
		uint8_t channel = 0;
		rv = get_data_11(&config->bus, &value, &channel);
		if (rv) {
			LOG_ERR("mcp356x_acquisition_thread error: get_data_11:%i\n", rv);
			continue;
		}

		// Protect array bounds
		if (channel >= MCP356X_CHANNEL_COUNT) {
			LOG_ERR("mcp356x_acquisition_thread error: channel out of bounds:%i\n", channel);
			continue;
		}

		uint8_t gain_reg = config->gain_reg;
		if(config->is_scan) {
			gain_reg = MCP356X_get_scan_channel_gain(channel);
		}

		// https://dsp.stackexchange.com/questions/66171/single-pole-iir-filter-fixed-point-design
		// efficient implementation of IIR1 alpha = 1/2:
		config->raw_iir[channel] = ((int64_t)config->raw_iir[channel] + (int64_t)value) >> 1;
		config->raw_max[channel] = MAX(config->raw_max[channel], value);
		config->raw_min[channel] = MIN(config->raw_min[channel], value);
	}
}


int egadc_setup_board(struct mcp356x_config * config)
{
	LOG_INF("Configuring port %s %i",config->irq.port->name, config->irq.pin);
	int err;
	err = gpio_pin_configure_dt(&config->irq, GPIO_INPUT);
	if (err) {
		LOG_ERR("gpio_pin_configure_dt error: %i\n", err);
		return err;
	}
	err = gpio_pin_interrupt_configure_dt(&config->irq, GPIO_INT_EDGE_TO_ACTIVE);
	if (err) {
		LOG_ERR("gpio_pin_interrupt_configure_dt error: %i\n", err);
		return err;
	}
	gpio_init_callback(&config->drdy_cb, drdy_callback, BIT(config->irq.pin));
	err = gpio_add_callback(config->irq.port, &config->drdy_cb);
	if (err) {
		LOG_ERR("gpio_add_callback error: %i\n", err);
		return err;
	}
	err = k_sem_init(&config->acq_sem, 0, 1);
	if (err) {
		LOG_ERR("k_sem_init error: %i\n", err);
		return err;
	}
	err = k_sem_init(&config->drdy_sem, 0, 1);
	if (err) {
		LOG_ERR("k_sem_init error: %i\n", err);
		return err;
	}
	k_thread_create(&config->thread, config->stack,
			ADC_MCP356X_ACQUISITION_THREAD_STACK_SIZE,
			(k_thread_entry_t)mcp356x_acquisition_thread,
			(void *)config, NULL, NULL,
			//K_PRIO_COOP(ADC_MCP356X_ACQUISITION_THREAD_PRIO),
			ADC_MCP356X_ACQUISITION_THREAD_PRIO,
			//0,
			0, K_NO_WAIT);
	err = k_thread_name_set(&config->thread, "mcp356x");
	if (err) {
		LOG_WRN("k_thread_name_set error: %i\n", err);
		return err;
	}
	return 0;
}



void egadc_adc_value_reset(struct mcp356x_config * config)
{
	config->num_drdy = 0;
	config->num_irq = 0;
	config->raw_max[MCP356X_CH_CH0] = INT32_MIN; // Reset max
	config->raw_max[MCP356X_CH_CH1] = INT32_MIN; // Reset max
	config->raw_max[MCP356X_CH_CH2] = INT32_MIN; // Reset max
	config->raw_max[MCP356X_CH_CH3] = INT32_MIN; // Reset max
	config->raw_max[MCP356X_CH_CH4] = INT32_MIN; // Reset max
	config->raw_max[MCP356X_CH_CH5] = INT32_MIN; // Reset max
	config->raw_max[MCP356X_CH_CH6] = INT32_MIN; // Reset max
	config->raw_max[MCP356X_CH_CH7] = INT32_MIN; // Reset max
	config->raw_min[MCP356X_CH_CH0] = INT32_MAX; // Reset min
	config->raw_min[MCP356X_CH_CH1] = INT32_MAX; // Reset min
	config->raw_min[MCP356X_CH_CH2] = INT32_MAX; // Reset min
	config->raw_min[MCP356X_CH_CH3] = INT32_MAX; // Reset min
	config->raw_min[MCP356X_CH_CH4] = INT32_MAX; // Reset min
	config->raw_min[MCP356X_CH_CH5] = INT32_MAX; // Reset min
	config->raw_min[MCP356X_CH_CH6] = INT32_MAX; // Reset min
	config->raw_min[MCP356X_CH_CH7] = INT32_MAX; // Reset min
}







void egadc_progress(struct mcp356x_config * config)
{
	switch (config->state) {
	case EGADC_STATE_START:
		LOG_INF("EGADC_STATE_START");
		egadc_setup_board(config);
		config->state = EGADC_STATE_INIT;
		break;

	case EGADC_STATE_INIT:
		LOG_INF("EGADC_STATE_INIT");
		egadc_setup_adc(config);
		//egadc_set_ch(config, MCP356X_CH_CH3);
		config->state = EGADC_STATE_WAIT0;
		config->time0 = 0;
		break;

	case EGADC_STATE_WAIT0:
		if(config->num_drdy > 0){
			config->state = EGADC_STATE_READY;
			LOG_INF("EGADC_STATE_READY");
		} else {
			config->time0++;
			if(config->time0 > 10) {
				LOG_INF("No respond from ADC");
				config->state = EGADC_STATE_INIT;
			}
			/*
			gpio_pin_set_dt(leds + MY_LEDS_WAITING, 1);
			k_sleep(K_MSEC(500));
			gpio_pin_set_dt(leds + MY_LEDS_WAITING, 0);
			k_sleep(K_MSEC(500));
			*/
		}
		break;

	case EGADC_STATE_READY:
		if(config->status & EGADC_TIMEOUT_BIT) {
			LOG_INF("ADC Timeout");
			config->status &= ~EGADC_TIMEOUT_BIT;
			config->state = EGADC_STATE_INIT;
		}
		//printk("raw_iir0: %i\n", config->raw_iir[0]);
		app.values[MYID_ADC_CH0] = MCP356X_raw_to_volt(config->raw_iir[0], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH1] = MCP356X_raw_to_volt(config->raw_iir[1], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH2] = MCP356X_raw_to_volt(config->raw_iir[2], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH3] = MCP356X_raw_to_volt(config->raw_iir[3], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH4] = MCP356X_raw_to_volt(config->raw_iir[4], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH5] = MCP356X_raw_to_volt(config->raw_iir[5], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH6] = MCP356X_raw_to_volt(config->raw_iir[6], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH7] = MCP356X_raw_to_volt(config->raw_iir[7], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH0_MIN] = MCP356X_raw_to_volt(config->raw_min[0], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH1_MIN] = MCP356X_raw_to_volt(config->raw_min[1], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH2_MIN] = MCP356X_raw_to_volt(config->raw_min[2], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH3_MIN] = MCP356X_raw_to_volt(config->raw_min[3], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH4_MIN] = MCP356X_raw_to_volt(config->raw_min[4], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH5_MIN] = MCP356X_raw_to_volt(config->raw_min[5], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH6_MIN] = MCP356X_raw_to_volt(config->raw_min[6], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH7_MIN] = MCP356X_raw_to_volt(config->raw_min[7], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH0_MAX] = MCP356X_raw_to_volt(config->raw_max[0], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH1_MAX] = MCP356X_raw_to_volt(config->raw_max[1], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH2_MAX] = MCP356X_raw_to_volt(config->raw_max[2], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH3_MAX] = MCP356X_raw_to_volt(config->raw_max[3], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH4_MAX] = MCP356X_raw_to_volt(config->raw_max[4], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH5_MAX] = MCP356X_raw_to_volt(config->raw_max[5], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH6_MAX] = MCP356X_raw_to_volt(config->raw_max[6], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		app.values[MYID_ADC_CH7_MAX] = MCP356X_raw_to_volt(config->raw_max[7], TIABT_VREF_MICRO_VOLT, config->gain_reg);
		int resistor = 1000000;
		app.values[MYID_CH0_PIKO_A] = app.values[MYID_ADC_CH0] * (1000000 / resistor);
		app.values[MYID_CH1_PIKO_A] = app.values[MYID_ADC_CH1] * (1000000 / resistor);
		app.values[MYID_CH2_PIKO_A] = app.values[MYID_ADC_CH2] * (1000000 / resistor);
		app.values[MYID_CH3_PIKO_A] = app.values[MYID_ADC_CH3] * (1000000 / resistor);
		app.values[MYID_CH4_PIKO_A] = app.values[MYID_ADC_CH4] * (1000000 / resistor);
		app.values[MYID_CH5_PIKO_A] = app.values[MYID_ADC_CH5] * (1000000 / resistor);
		app.values[MYID_CH6_PIKO_A] = app.values[MYID_ADC_CH6] * (1000000 / resistor);
		app.values[MYID_CH7_PIKO_A] = app.values[MYID_ADC_CH7] * (1000000 / resistor);
		egadc_adc_value_reset(config);
		break;

	default:
		break;
	}

	//printk("state: %i\n", config->state);
}