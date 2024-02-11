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

int32_t gatt_flags[MYGATT_COUNT];

const struct bt_uuid_128 uuids[MYGATT_COUNT] = {
	[MYGATT_SRV2] = MY_UUID_SRV(MYGATT_SRV2),
	[MYGATT_SRV2_CH0_CHRC0] = MY_UUID_CHRC(MYID_ADC_CH0),
	[MYGATT_SRV2_CH1_CHRC0] = MY_UUID_CHRC(MYID_ADC_CH1),
	[MYGATT_SRV2_CH2_CHRC0] = MY_UUID_CHRC(MYID_ADC_CH2),
	[MYGATT_SRV2_CH3_CHRC0] = MY_UUID_CHRC(MYID_ADC_CH3),
	[MYGATT_SRV2_CH4_CHRC0] = MY_UUID_CHRC(MYID_ADC_CH4),
	[MYGATT_SRV2_CH5_CHRC0] = MY_UUID_CHRC(MYID_ADC_CH5),
	[MYGATT_SRV2_CH6_CHRC0] = MY_UUID_CHRC(MYID_ADC_CH6),
	[MYGATT_SRV2_CH7_CHRC0] = MY_UUID_CHRC(MYID_ADC_CH7),
	[MYGATT_SRV3] = MY_UUID_SRV(MYGATT_SRV3),
	[MYGATT_SRV3_DPOT0_CHRC0] = MY_UUID_CHRC(MYID_DPOT0_WIPER),
	[MYGATT_SRV3_DPOT1_CHRC0] = MY_UUID_CHRC(MYID_DPOT1_WIPER),
	[MYGATT_SRV3_DPOT2_CHRC0] = MY_UUID_CHRC(MYID_DPOT2_WIPER),
	[MYGATT_SRV3_DPOT3_CHRC0] = MY_UUID_CHRC(MYID_DPOT3_WIPER),
	[MYGATT_SRV3_DPOT4_CHRC0] = MY_UUID_CHRC(MYID_DPOT4_WIPER),
	[MYGATT_SRV3_DPOT5_CHRC0] = MY_UUID_CHRC(MYID_DPOT5_WIPER),
	[MYGATT_SRV3_DPOT6_CHRC0] = MY_UUID_CHRC(MYID_DPOT6_WIPER),
};





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
    uint16_t tmp4;
    memcpy(&tmp4, &BT_UUID_128(uuid)->val[10], sizeof(tmp4));
	//print_uuid(uuid);
	int a = sys_le16_to_cpu(tmp4);
	if(a >= 0 && a < MYID_COUNT) {
		app.values_flags[a] &= ~GATT_FLAG_NOTIFY;
		app.values_flags[a] |= (value == BT_GATT_CCC_NOTIFY) ? GATT_FLAG_NOTIFY : 0;
		printk("%04x %i\n", a, app.values_flags[a]);
	}
	

	//simulate_temp = value == BT_GATT_CCC_NOTIFY;
    //BT_UUID_16(attr->uuid, );
	//void * v3 = service2.attrs[MYGATT_SRV2_CH0_CHRC0].uuid;
	//void * v3 = service2.attrs[MYGATT_SRV2_CH0_CHRC1].user_data;
	//LOG_INF("ccc_cfg_changed v1:%p, v2:%p, v3:%p, voltage:%p", v1, v2, v3, &values.voltage[0]);
    /*
	if (v == &values.voltage[0]){
		printk("ccc_cfg_changed 0");
	}
	else if (v == &values.voltage[0]){
		printk("ccc_cfg_changed 1");
	}
    */
}



