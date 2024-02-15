#include "btmisc.h"
#include "mydefs.h"
#include <zephyr/logging/log.h>


LOG_MODULE_REGISTER(btmisc, CONFIG_APP_LOG_LEVEL);

ssize_t read_signed(struct bt_conn *conn, const struct bt_gatt_attr *attr,void *buf, uint16_t len, uint16_t offset)
{
	const char *value = attr->user_data;
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(int32_t));
}

ssize_t write_signed(struct bt_conn *conn, const struct bt_gatt_attr *attr,const void *buf, uint16_t len, uint16_t offset,uint8_t flags)
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
		LOG_INF("ccc_cfg_changed1: w1:%04X flags:%08X\n", w1, app.values_flags[w1]);
	}
}


int btmisc_notifier(notify_t * notifier, int count, const struct bt_gatt_attr *attrs)
{
	int err = 0;
	for(int i = 0; i < count; ++i) {
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
		int err = bt_gatt_notify(NULL, &(attrs[att]), &app.values[id], sizeof(int32_t));
		if (err) {
			LOG_ERR("bt_gatt_notify error: %i\n", err);
		}
	}
	return err;
}

