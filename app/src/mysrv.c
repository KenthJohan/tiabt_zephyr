#include "mysrv.h"
#include "mydefs.h"
#include "gatt_cpf.h"
#include "myuuids.h"

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/byteorder.h>



LOG_MODULE_REGISTER(srv2, CONFIG_APP_LOG_LEVEL);


//static void ccc_cfg_changed1(const struct bt_gatt_attr *attr, uint16_t value){}

BT_GATT_SERVICE_DEFINE(service2,
	BT_GATT_PRIMARY_SERVICE(&uuids[MYGATT_SRV2]),
	//BT_GATT_CHARACTERISTIC(&led_char_uuid, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, read_u16, NULL, &app.values[0]),
	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH0_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[0]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH0", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH1_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[1]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH1", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH2_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[2]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH2", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH3_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[3]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH3", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH4_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[4]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH4", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH5_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[5]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH5", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH6_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[6]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH6", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV2_CH7_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[7]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH7", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),
);




static ssize_t read_signed(struct bt_conn *conn, const struct bt_gatt_attr *attr,void *buf, uint16_t len, uint16_t offset)
{
	const char *value = attr->user_data;
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(int32_t));
}

static ssize_t write_signed(struct bt_conn *conn, const struct bt_gatt_attr *attr,const void *buf, uint16_t len, uint16_t offset,uint8_t flags)
{
    const struct bt_uuid *uuid = attr[0].uuid;
	print_uuid(uuid);
	uint8_t *value = attr->user_data;
	if (offset + len > sizeof(int32_t)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}
	memcpy(value + offset, buf, len);
    printk("write_signed value: %i\n", app.values[MYID_DPOT0_WIPER]);
	return len;
}


//static void ccc_cfg_changed1(const struct bt_gatt_attr *attr, uint16_t value){}

BT_GATT_SERVICE_DEFINE(service3,
	BT_GATT_PRIMARY_SERVICE(&uuids[MYGATT_SRV3]),
	//BT_GATT_CHARACTERISTIC(&led_char_uuid, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, read_u16, NULL, &values.values[0]),
	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV3_DPOT0_CHRC0], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT0_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT0", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV3_DPOT1_CHRC0], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT1_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT1", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV3_DPOT2_CHRC0], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT2_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT2", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV3_DPOT3_CHRC0], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT3_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT3", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV3_DPOT4_CHRC0], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT4_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT4", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV3_DPOT5_CHRC0], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT5_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT5", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV3_DPOT6_CHRC0], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT6_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT6", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),
);



typedef struct
{
	struct bt_gatt_service_static * srv;
	mygatt_t att;
	myid_t id;
} notify_t;

#define NOTFIER_COUNT 8
const notify_t notifier[NOTFIER_COUNT] = {
	{&service2, MYGATT_SRV2_CH0_CHRC0, MYID_ADC_CH0},
	{&service2, MYGATT_SRV2_CH1_CHRC0, MYID_ADC_CH1},
	{&service2, MYGATT_SRV2_CH2_CHRC0, MYID_ADC_CH2},
	{&service2, MYGATT_SRV2_CH3_CHRC0, MYID_ADC_CH3},
	{&service2, MYGATT_SRV2_CH4_CHRC0, MYID_ADC_CH4},
	{&service2, MYGATT_SRV2_CH5_CHRC0, MYID_ADC_CH5},
	{&service2, MYGATT_SRV2_CH6_CHRC0, MYID_ADC_CH6},
	{&service2, MYGATT_SRV2_CH7_CHRC0, MYID_ADC_CH7},
	{0, 0}
};

void srv2_update(void)
{
	
	for(int i = 0; i < NOTFIER_COUNT; ++i)
	{
		struct bt_gatt_service_static const * srv = notifier[i].srv;
		if(srv == NULL) {
			continue;
		}
		int myid = notifier[i].id;
		if((app.values_flags[myid] & GATT_FLAG_NOTIFY) == 0) {
			continue;
		}
		mygatt_t att = notifier[i].att;
		app.values[myid]++;
		printk("notifier %i\n", notifier[i].att);
		bt_gatt_notify(NULL, &(srv->attrs[att]), &app.values[myid], sizeof(int32_t));
		
	}

	//bas_notify();
	//app.values[0] += 1;
	//app.values[1] += 1;
	if(0) {
		printk("srv2_update\n");
		//bt_gatt_notify_uuid(NULL, &uuids[MYID_SRV2_CH3], )
		bt_gatt_notify(NULL, &service2.attrs[MYGATT_SRV2_CH0_CHRC0], &app.values[0], sizeof(app.values[0]));
		bt_gatt_notify(NULL, &service2.attrs[MYGATT_SRV2_CH1_CHRC0], &app.values[1], sizeof(app.values[1]));
		//bt_gatt_notify(NULL, &service1.attrs[MYID_SRV2_CH1+5], &app.values[1], sizeof(app.values[1]));
		//bt_gatt_notify(NULL, &service1.attrs[MYID_SRV2_CH1+6], &app.values[1], sizeof(app.values[1]));
		//bt_gatt_notify(NULL, &service1.attrs[MYID_SRV2_CH1+7], &app.values[1], sizeof(app.values[1]));
	}
	//bt_gatt_notify(NULL, attr, &value, sizeof(value));
}