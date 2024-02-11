#pragma once
#include <zephyr/bluetooth/gatt.h>

//2.4.1 GATT Format Types
#define FORMAT_FLOAT32 0x14
#define FORMAT_SINT32 0x10

// 3.5 Units
#define UNIT_ACCELERATION 0x2713
#define UNIT_VOLTAGE 0x2728
#define UNIT_SECOND 0x2703

static struct bt_gatt_cpf cha_format_value =
{
	FORMAT_SINT32, // 2.4.1 GATT Format Types
	0x00, // exponent
	UNIT_VOLTAGE, // 3.5.1 Units by Name
	0x01, // 2.4.2 GATT Characteristic Presentation Format Name Space
	0x010F // 2.4.2.1 Bluetooth SIG GATT Characteristic Presentation Format Description
};