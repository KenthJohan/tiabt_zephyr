#pragma once
#include <zephyr/bluetooth/gatt.h>

/*
https://www.bluetooth.com/wp-content/uploads/Files/Specification/HTML/Assigned_Numbers/out/en/Assigned_Numbers.pdf?v=1707735720897

*/

//2.4.1 GATT Format Types
#define FORMAT_FLOAT32 0x14
#define FORMAT_SINT32 0x10

// 3.5.1 Units by Name
#define UNIT_ACCELERATION 0x2713
#define UNIT_VOLTAGE 0x2728
#define UNIT_AMPERE 0x2704
#define UNIT_SECOND 0x2703
#define UNIT_UNKNOWN 0x0000



#define GATT_FORMAT_NAMESPACE 0x01 // 2.4.2 GATT Characteristic Presentation Format Name Space
#define GATT_FORMAT_DESCRIPTION 0x010F // 2.4.2.1 Bluetooth SIG GATT Characteristic Presentation Format Description

static struct bt_gatt_cpf cpf_i32 = {
	FORMAT_SINT32,
	0, // exponent
	UNIT_UNKNOWN,
	GATT_FORMAT_NAMESPACE, 
	GATT_FORMAT_DESCRIPTION
};

static struct bt_gatt_cpf cpf_dpot = {
	FORMAT_SINT32,
	0, // exponent
	UNIT_UNKNOWN,
	GATT_FORMAT_NAMESPACE, 
	GATT_FORMAT_DESCRIPTION
};


static struct bt_gatt_cpf cpf_micro_voltage = {
	FORMAT_SINT32,
	-6, // exponent
	UNIT_VOLTAGE,
	GATT_FORMAT_NAMESPACE, 
	GATT_FORMAT_DESCRIPTION
};

static struct bt_gatt_cpf cpf_piko_ampere = {
	FORMAT_SINT32,
	-12, // exponent
	UNIT_AMPERE,
	GATT_FORMAT_NAMESPACE, 
	GATT_FORMAT_DESCRIPTION
};