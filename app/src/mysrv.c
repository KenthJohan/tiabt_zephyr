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



LOG_MODULE_REGISTER(mysrv, CONFIG_APP_LOG_LEVEL);


//static void ccc_cfg_changed1(const struct bt_gatt_attr *attr, uint16_t value){}

/*
BT_GATT_SERVICE_DEFINE(service2,

);
*/

static ssize_t read_signed(struct bt_conn *conn, const struct bt_gatt_attr *attr,void *buf, uint16_t len, uint16_t offset);
static ssize_t write_signed(struct bt_conn *conn, const struct bt_gatt_attr *attr,const void *buf, uint16_t len, uint16_t offset,uint8_t flags);




//static void ccc_cfg_changed1(const struct bt_gatt_attr *attr, uint16_t value){}

BT_GATT_SERVICE_DEFINE(gatt,
	BT_GATT_PRIMARY_SERVICE((const struct bt_uuid *)&uuids[MYGATT_SRV1]),
	BT_GATT_PRIMARY_SERVICE((const struct bt_uuid *)&uuids[MYGATT_SRV3]),
	//BT_GATT_CHARACTERISTIC(&led_char_uuid, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, read_u16, NULL, &values.values[0]),
	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids[MYGATT_SRV3_DPOT0_CHRC0], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT0_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT0", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids[MYGATT_SRV3_DPOT1_CHRC0], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT1_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT1", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids[MYGATT_SRV3_DPOT2_CHRC0], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT2_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT2", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids[MYGATT_SRV3_DPOT3_CHRC0], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT3_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT3", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids[MYGATT_SRV3_DPOT4_CHRC0], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT4_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT4", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids[MYGATT_SRV3_DPOT5_CHRC0], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT5_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT5", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids[MYGATT_SRV3_DPOT6_CHRC0], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT6_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT6", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),




	BT_GATT_PRIMARY_SERVICE(&uuids[MYGATT_SRV2]),
	//BT_GATT_CHARACTERISTIC(&led_char_uuid, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, read_u16, NULL, &app.values[0]),
	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids[MYGATT_SRV2_CH0_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[0]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH0", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids[MYGATT_SRV2_CH1_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[1]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH1", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids[MYGATT_SRV2_CH2_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[2]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH2", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids[MYGATT_SRV2_CH3_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[3]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH3", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids[MYGATT_SRV2_CH4_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[4]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH4", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids[MYGATT_SRV2_CH5_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[5]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH5", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids[MYGATT_SRV2_CH6_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[6]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH6", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids[MYGATT_SRV2_CH7_CHRC0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[7]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH7", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),
);



typedef struct
{
	struct bt_gatt_service_static const * srv;
	mygatt_t att;
	myid_t id;
} notify_t;

#define NOTFIER_COUNT 20
const notify_t notifier[NOTFIER_COUNT] = {
	{&gatt, MYGATT_SRV2_CH0_CHRC0, MYID_ADC_CH0},
	{&gatt, MYGATT_SRV2_CH1_CHRC0, MYID_ADC_CH1},
	{&gatt, MYGATT_SRV2_CH2_CHRC0, MYID_ADC_CH2},
	{&gatt, MYGATT_SRV2_CH3_CHRC0, MYID_ADC_CH3},
	{&gatt, MYGATT_SRV2_CH4_CHRC0, MYID_ADC_CH4},
	{&gatt, MYGATT_SRV2_CH5_CHRC0, MYID_ADC_CH5},
	{&gatt, MYGATT_SRV2_CH6_CHRC0, MYID_ADC_CH6},
	{&gatt, MYGATT_SRV2_CH7_CHRC0, MYID_ADC_CH7},
	{&gatt, MYGATT_SRV3_DPOT0_CHRC0, MYID_DPOT0_WIPER},
	{&gatt, MYGATT_SRV3_DPOT1_CHRC0, MYID_DPOT1_WIPER},
	{&gatt, MYGATT_SRV3_DPOT2_CHRC0, MYID_DPOT2_WIPER},
	{&gatt, MYGATT_SRV3_DPOT3_CHRC0, MYID_DPOT3_WIPER},
	{&gatt, MYGATT_SRV3_DPOT4_CHRC0, MYID_DPOT4_WIPER},
	{&gatt, MYGATT_SRV3_DPOT5_CHRC0, MYID_DPOT5_WIPER},
	{&gatt, MYGATT_SRV3_DPOT6_CHRC0, MYID_DPOT6_WIPER},
	{0, 0}
};

void mysrv_notifier(void)
{
	for(int i = 0; i < NOTFIER_COUNT; ++i) {
		struct bt_gatt_service_static const * srv = notifier[i].srv;
		if(srv == NULL) {
			break;
		}
		myid_t id = notifier[i].id;
		if((app.values_flags[id] & MYFLAG_NOTIFY) == 0) {
			continue;
		}
		mygatt_t att = notifier[i].att;
		app.values[id]++;
		LOG_INF("notifier: attribute:%i myid:%s", att, myid_t_tostr(id));
		bt_gatt_notify(NULL, &(srv->attrs[att]), &app.values[id], sizeof(int32_t));
	}
}


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

	uint16_t w1;
	uuid_extract(uuid, NULL, &w1, NULL, NULL, NULL);

	if(w1 < MYID_COUNT) {
		int32_t value = app.values[w1];
		app.values_flags[w1] |= MYFLAG_SETVAL;
    	//printk("write_signed value: myid:%s value:%i\n", myid_t_tostr(w1), value);
	} else {
		LOG_WRN("w1 outside array");
	}
	
	return len;
}