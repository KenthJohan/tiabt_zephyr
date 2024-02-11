#include "srv2.h"
#include "mydefs.h"
#include "gatt_cpf.h"

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/byteorder.h>



LOG_MODULE_REGISTER(srv2, CONFIG_APP_LOG_LEVEL);

int simulate_temp = 0;
static void ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);


#define MY_UUID(x) BT_UUID_INIT_128(BT_UUID_128_ENCODE(MYGATTW32_SRV2, UINT16_C(x), 0x4B9C, 0x9C69, 0x4975713E0FF2))

static const struct bt_uuid_128 uuids[] = {
	[MYGATT_SRV2] = MY_UUID(MYGATT_SRV2),
	[MYGATT_SRV2_CH0_CHRC0] = MY_UUID(MYGATT_SRV2_CH0_CHRC0),
	[MYGATT_SRV2_CH1_CHRC0] = MY_UUID(MYGATT_SRV2_CH1_CHRC0),
	[MYGATT_SRV2_CH2_CHRC0] = MY_UUID(MYGATT_SRV2_CH2_CHRC0),
	[MYGATT_SRV2_CH3_CHRC0] = MY_UUID(MYGATT_SRV2_CH3_CHRC0),
	[MYGATT_SRV2_CH4_CHRC0] = MY_UUID(MYGATT_SRV2_CH4_CHRC0),
	[MYGATT_SRV2_CH5_CHRC0] = MY_UUID(MYGATT_SRV2_CH5_CHRC0),
	[MYGATT_SRV2_CH6_CHRC0] = MY_UUID(MYGATT_SRV2_CH6_CHRC0),
	[MYGATT_SRV2_CH7_CHRC0] = MY_UUID(MYGATT_SRV2_CH7_CHRC0),
};

BT_GATT_SERVICE_DEFINE(service2,
	BT_GATT_PRIMARY_SERVICE(&uuids[MYGATT_SRV2]),
	//BT_GATT_CHARACTERISTIC(&led_char_uuid, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, read_u16, NULL, &values.voltage[0]),
	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH0_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &values.voltage[0]),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH0", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH1_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &values.voltage[1]),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH1", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH2_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &values.voltage[2]),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH2", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH3_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &values.voltage[3]),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH3", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH4_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &values.voltage[4]),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH4", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH5_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &values.voltage[5]),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH5", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH6_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &values.voltage[6]),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH6", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH7_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &values.voltage[7]),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH7", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),
);

// https://github.com/zephyrproject-rtos/zephyr/blob/b4e66299cdf687003ad6d7e010328f468fda1fbd/subsys/bluetooth/host/uuid.c#L41
static void ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    const struct bt_uuid *uuid = attr[-1].uuid;
    uint16_t tmp4;
    memcpy(&tmp4, &BT_UUID_128(uuid)->val[10], sizeof(tmp4));
    char str[256];
    bt_uuid_to_str(uuid, str, 256);
	printk("%s %i\n", str, sys_le16_to_cpu(tmp4));
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








void srv2_update(void)
{
	//bas_notify();
	values.voltage[0] += 1;
	values.voltage[1] += 1;
	if(simulate_temp) {
		printk("srv2_update\n");
		//bt_gatt_notify_uuid(NULL, &uuids[MYID_SRV2_CH3], )
		bt_gatt_notify(NULL, &service2.attrs[MYGATT_SRV2_CH0_CHRC0], &values.voltage[0], sizeof(values.voltage[0]));
		bt_gatt_notify(NULL, &service2.attrs[MYGATT_SRV2_CH1_CHRC0], &values.voltage[1], sizeof(values.voltage[1]));
		//bt_gatt_notify(NULL, &service1.attrs[MYID_SRV2_CH1+5], &values.voltage[1], sizeof(values.voltage[1]));
		//bt_gatt_notify(NULL, &service1.attrs[MYID_SRV2_CH1+6], &values.voltage[1], sizeof(values.voltage[1]));
		//bt_gatt_notify(NULL, &service1.attrs[MYID_SRV2_CH1+7], &values.voltage[1], sizeof(values.voltage[1]));
	}
	//bt_gatt_notify(NULL, attr, &value, sizeof(value));
}