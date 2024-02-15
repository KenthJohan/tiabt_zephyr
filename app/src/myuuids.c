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

#define MY_UUID_CHRC(x) BT_UUID_INIT_128(BT_UUID_128_ENCODE(0x2A5A20B9, UINT16_C(x), 0x4B9C, 0x9C69, 0x4975713E0FF2))
#define MY_UUID_SRV(x) BT_UUID_INIT_128(BT_UUID_128_ENCODE(0x2A5A20F9, UINT16_C(x), 0x4B9C, 0x9C69, 0x4975713E0FF2))



const struct bt_uuid_128 uuids_srv[MYSRV_UUID_COUNT] = {
MY_UUID_SRV(MYSRV_UUID_SRV0),
MY_UUID_SRV(MYSRV_UUID_SRV1),
MY_UUID_SRV(MYSRV_UUID_SRV2),
MY_UUID_SRV(MYSRV_UUID_SRV3),
};

const struct bt_uuid_128 uuids_chrc[] = {
[MYID_ADC_CH0] = MY_UUID_CHRC(MYID_ADC_CH0),
[MYID_ADC_CH1] = MY_UUID_CHRC(MYID_ADC_CH1),
[MYID_ADC_CH2] = MY_UUID_CHRC(MYID_ADC_CH2),
[MYID_ADC_CH3] = MY_UUID_CHRC(MYID_ADC_CH3),
[MYID_ADC_CH4] = MY_UUID_CHRC(MYID_ADC_CH4),
[MYID_ADC_CH5] = MY_UUID_CHRC(MYID_ADC_CH5),
[MYID_ADC_CH6] = MY_UUID_CHRC(MYID_ADC_CH6),
[MYID_ADC_CH7] = MY_UUID_CHRC(MYID_ADC_CH7),
[MYID_ADC_OFFSET] = MY_UUID_CHRC(MYID_ADC_OFFSET),
[MYID_ADC_VCM] = MY_UUID_CHRC(MYID_ADC_VCM),
[MYID_ADC_AVDD] = MY_UUID_CHRC(MYID_ADC_AVDD),
[MYID_ADC_TEMP] = MY_UUID_CHRC(MYID_ADC_TEMP),
[MYID_DPOT0_WIPER] = MY_UUID_CHRC(MYID_DPOT0_WIPER),
[MYID_DPOT1_WIPER] = MY_UUID_CHRC(MYID_DPOT1_WIPER),
[MYID_DPOT2_WIPER] = MY_UUID_CHRC(MYID_DPOT2_WIPER),
[MYID_DPOT3_WIPER] = MY_UUID_CHRC(MYID_DPOT3_WIPER),
[MYID_DPOT4_WIPER] = MY_UUID_CHRC(MYID_DPOT4_WIPER),
[MYID_DPOT5_WIPER] = MY_UUID_CHRC(MYID_DPOT5_WIPER),
[MYID_DPOT6_WIPER] = MY_UUID_CHRC(MYID_DPOT6_WIPER),
};


void uuid_extract(const struct bt_uuid *uuid, uint64_t * w32, uint16_t * w1, uint16_t * w2, uint16_t * w3, uint32_t * w64)
{
	if(w1) {
    	uint16_t tmp;
    	memcpy(&tmp, &BT_UUID_128(uuid)->val[10], sizeof(tmp));
		(*w1) = sys_le16_to_cpu(tmp);
	}
}


void print_uuid(const struct bt_uuid *uuid)
{
    char str[256];
    bt_uuid_to_str(uuid, str, 256);
	printk("%s\n", str);
}

// https://github.com/zephyrproject-rtos/zephyr/blob/b4e66299cdf687003ad6d7e010328f468fda1fbd/subsys/bluetooth/host/uuid.c#L41
void ccc_cfg_changed1(const struct bt_gatt_attr *attr, uint16_t value)
{
    const struct bt_uuid *uuid = attr[-1].uuid;
	uint16_t w1;
	uuid_extract(uuid, NULL, &w1, NULL, NULL, NULL);
	if(w1 >= 0 && w1 < MYID_COUNT) {
		app.values_flags[w1] &= ~MYFLAG_NOTIFY;
		app.values_flags[w1] |= (value == BT_GATT_CCC_NOTIFY) ? MYFLAG_NOTIFY : 0;
		printk("ccc_cfg_changed1: w1:%04X flags:%08X\n", w1, app.values_flags[w1]);
	}
}



