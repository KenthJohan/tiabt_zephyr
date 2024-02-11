#include "myuuids.h"
#include "mydefs.h"

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/byteorder.h>

#define MY_UUID(x) BT_UUID_INIT_128(BT_UUID_128_ENCODE(MYGATTW32_SRV2, UINT16_C(x), 0x4B9C, 0x9C69, 0x4975713E0FF2))

const struct bt_uuid_128 uuids[MYGATT_COUNT] = {
	[MYGATT_SRV2] = MY_UUID(MYGATT_SRV2),
	[MYGATT_SRV2_CH0_CHRC0] = MY_UUID(MYGATT_SRV2_CH0_CHRC0),
	[MYGATT_SRV2_CH1_CHRC0] = MY_UUID(MYGATT_SRV2_CH1_CHRC0),
	[MYGATT_SRV2_CH2_CHRC0] = MY_UUID(MYGATT_SRV2_CH2_CHRC0),
	[MYGATT_SRV2_CH3_CHRC0] = MY_UUID(MYGATT_SRV2_CH3_CHRC0),
	[MYGATT_SRV2_CH4_CHRC0] = MY_UUID(MYGATT_SRV2_CH4_CHRC0),
	[MYGATT_SRV2_CH5_CHRC0] = MY_UUID(MYGATT_SRV2_CH5_CHRC0),
	[MYGATT_SRV2_CH6_CHRC0] = MY_UUID(MYGATT_SRV2_CH6_CHRC0),
	[MYGATT_SRV2_CH7_CHRC0] = MY_UUID(MYGATT_SRV2_CH7_CHRC0),
	[MYGATT_SRV3] = MY_UUID(MYGATT_SRV3),
	[MYGATT_SRV3_DPOT0_CHRC0] = MY_UUID(MYGATT_SRV3_DPOT0_CHRC0),
};