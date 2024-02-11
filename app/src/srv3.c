#include "srv3.h"
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


LOG_MODULE_REGISTER(srv3, CONFIG_APP_LOG_LEVEL);

#define MY_UUID(x) BT_UUID_INIT_128(BT_UUID_128_ENCODE(MYGATTW32_SRV3, UINT16_C(x), 0x4B9C, 0x9C69, 0x4975713E0FF2))




static ssize_t read_signed(struct bt_conn *conn, const struct bt_gatt_attr *attr,void *buf, uint16_t len, uint16_t offset)
{
	const char *value = attr->user_data;
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(int32_t));
}

static ssize_t write_signed(struct bt_conn *conn, const struct bt_gatt_attr *attr,const void *buf, uint16_t len, uint16_t offset,uint8_t flags)
{
	uint8_t *value = attr->user_data;
	if (offset + len > sizeof(int32_t)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}
	memcpy(value + offset, buf, len);
    printk("write_signed value: %i\n", values.voltage[MYID_DPOT0_WIPER]);
	return len;
}


static void ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);
BT_GATT_SERVICE_DEFINE(service3,
	BT_GATT_PRIMARY_SERVICE(&uuids[MYGATT_SRV3]),
	//BT_GATT_CHARACTERISTIC(&led_char_uuid, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, read_u16, NULL, &values.voltage[0]),
	BT_GATT_CHARACTERISTIC(&uuids[MYGATT_SRV3_DPOT0_CHRC0], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &values.voltage[MYID_DPOT0_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT0", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cha_format_value),
);

static void ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    printk("DPOT CCC %i\n", (value == BT_GATT_CCC_NOTIFY));
}



                   







void srv3_update(void)
{

}