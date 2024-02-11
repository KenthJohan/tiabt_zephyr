#pragma once
#include "mydefs.h"
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#define GATT_FLAG_NOTIFY UINT32_C(0x00000001)

extern const struct bt_uuid_128 uuids[MYGATT_COUNT];
extern int32_t gatt_flags[MYGATT_COUNT];


void ccc_cfg_changed1(const struct bt_gatt_attr *attr, uint16_t value);