#pragma once
#include "mydefs.h"
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>


extern const struct bt_uuid_128 uuids[MYGATT_COUNT];


void ccc_cfg_changed1(const struct bt_gatt_attr *attr, uint16_t value);

void print_uuid(const struct bt_uuid *uuid);