#pragma once
#include "mydefs.h"
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/sys/byteorder.h>


typedef struct
{
	struct bt_gatt_service_static const * srv;
	mygatt_t att;
	myid_t id;
} notify_t;

ssize_t read_signed(struct bt_conn *conn, const struct bt_gatt_attr *attr,void *buf, uint16_t len, uint16_t offset);
ssize_t write_signed(struct bt_conn *conn, const struct bt_gatt_attr *attr,const void *buf, uint16_t len, uint16_t offset,uint8_t flags);


int btmisc_notifier(int32_t * values, uint32_t * flags, int vcount, notify_t * notifier, int count, const struct bt_gatt_attr *attrs);
void ccc_cfg_changed1(const struct bt_gatt_attr *attr, uint16_t value);