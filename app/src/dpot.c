#include "dpot.h"
#include "MCP45HVX1.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(dpot, CONFIG_APP_LOG_LEVEL);

void transfer(struct mcp45hvx1_config * config, int cmd, int value)
{
	uint8_t tx[2] = {cmd, value};
	uint8_t rx[2] = {0};
	struct i2c_msg msgs[2] = {
		{.buf = tx},
		{.buf = rx},
	};
	int rc = 0;
	int addr = config->bus.addr;

	switch (cmd & 0x0F) {
	case MCP45HVX1_COM_READ:
		msgs[0].flags = I2C_MSG_WRITE;
		msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;
		msgs[0].len = 1;
		msgs[1].len = 2;
		rc = i2c_transfer_dt(&config->bus, msgs + 0, 1);
		if (rc) {
			LOG_ERR("Error i2c_transfer: %d", rc);
			return;
		}
		rc = i2c_transfer_dt(&config->bus, msgs + 1, 1);
		if (rc) {
			LOG_ERR("Error i2c_transfer: %d", rc);
			return;
		}
		break;
	
	case MCP45HVX1_COM_WRITE:
		msgs[0].flags = I2C_MSG_WRITE | I2C_MSG_STOP;
		msgs[0].len = 2;
		rc = i2c_transfer_dt(&config->bus, msgs + 0, 1);
		if (rc) {
			LOG_ERR("Error i2c_transfer: %d", rc);
			return;
		}
		break;
	case MCP45HVX1_COM_WIPERINC:
	case MCP45HVX1_COM_WIPERDEC:
		msgs[0].flags = I2C_MSG_WRITE | I2C_MSG_STOP;
		msgs[0].len = 1;
		rc = i2c_transfer_dt(&config->bus, msgs + 0, 1);
		if (rc) {
			LOG_ERR("Error i2c_transfer: %d", rc);
			return;
		}
		break;
	default:
		break;
	}

	switch (cmd & 0x0F) {
	case MCP45HVX1_COM_WIPERINC:
		LOG_INF("MCP45HVX1 WIPERINC: %02X/%s", addr, MCP45HVX1_REG_tostring(cmd & 0xF0));
		break;
	case MCP45HVX1_COM_WIPERDEC:
		LOG_INF("MCP45HVX1 WIPERDEC: %02X/%s", addr, MCP45HVX1_REG_tostring(cmd & 0xF0));
		break;
	case MCP45HVX1_COM_READ:
		LOG_INF("MCP45HVX1 READ: %02X/%s: %02X %02X", addr, MCP45HVX1_REG_tostring(cmd & 0xF0), rx[0], rx[1]);
		break;
	case MCP45HVX1_COM_WRITE:
		LOG_INF("MCP45HVX1 WRITE: %02X/%s: %02X", addr, MCP45HVX1_REG_tostring(cmd & 0xF0), value);
		break;
	}

}







int dpot_setup(struct mcp45hvx1_config * config)
{
	transfer(config, MCP45HVX1_MEM_TCON | MCP45HVX1_COM_WRITE, MCP45HVX1_TCON_R0HW | MCP45HVX1_TCON_R0A | MCP45HVX1_TCON_R0W);
	transfer(config, MCP45HVX1_MEM_WIPER | MCP45HVX1_COM_WRITE, 0);
	return 0;
}




void dpot_progress(struct mcp45hvx1_config * config)
{
	myid_t id = config->myid;
	if(id < MYID_COUNT) {
		if(app.values_flags[id] & MYFLAG_SETVAL) {
			app.values_flags[id] &= ~MYFLAG_SETVAL;
			int32_t value = app.values[id];
			if(value >= 0) {
				transfer(config, MCP45HVX1_MEM_TCON | MCP45HVX1_COM_WRITE, MCP45HVX1_TCON_R0HW | MCP45HVX1_TCON_R0A | MCP45HVX1_TCON_R0W);
				k_sleep(K_MSEC(10));
				transfer(config, MCP45HVX1_MEM_WIPER | MCP45HVX1_COM_WRITE, value);
				LOG_INF("Set dpot %s wiper: %i", myid_t_tostr(id), value);
			} else {
				transfer(config, MCP45HVX1_MEM_TCON | MCP45HVX1_COM_WRITE, MCP45HVX1_TCON_R0HW | MCP45HVX1_TCON_R0A);
			}
		}
	} else {
		LOG_WRN("id outside array");
	}
}