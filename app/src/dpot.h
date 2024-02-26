#pragma once
#include <zephyr/drivers/i2c.h>
#include "mydefs.h"
// MCP45HVX1
struct mcp45hvx1_config {
	const struct i2c_dt_spec bus;
	myid_t myid;
};



int dpot_setup(struct mcp45hvx1_config * config);

void dpot_progress(struct mcp45hvx1_config * config);