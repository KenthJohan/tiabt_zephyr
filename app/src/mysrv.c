#include "mysrv.h"
#include "mydefs.h"
#include "gatt_cpf.h"
#include "myuuids.h"
#include "btmisc.h"

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/byteorder.h>


LOG_MODULE_REGISTER(mysrv, CONFIG_APP_LOG_LEVEL);


BT_GATT_SERVICE_DEFINE(gatt,
	BT_GATT_PRIMARY_SERVICE((const struct bt_uuid *)&uuids_srv[MYSRV_UUID_SRV1]),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_APP_PRINT_MODE], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_APP_PRINT_MODE]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("APP_PRINT_MODE", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_dpot),



	BT_GATT_PRIMARY_SERVICE(&uuids_srv[MYSRV_UUID_SRV2]),
	
	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_ADC_CH0], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[0]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH0", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_micro_voltage),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_ADC_CH1], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[1]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH1", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_micro_voltage),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_ADC_CH2], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[2]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH2", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_micro_voltage),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_ADC_CH3], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[3]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH3", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_micro_voltage),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_ADC_CH4], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[4]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH4", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_micro_voltage),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_ADC_CH5], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[5]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH5", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_micro_voltage),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_ADC_CH6], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[6]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH6", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_micro_voltage),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_ADC_CH7], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[7]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH7", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_micro_voltage),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_ADC_OFFSET], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[0]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("OFFSET", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_micro_voltage),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_ADC_VCM], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[0]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("VCM", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_micro_voltage),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_ADC_AVDD], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[0]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("AVDD", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_micro_voltage),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_ADC_TEMP], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[0]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("TEMP", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_micro_voltage),



	BT_GATT_PRIMARY_SERVICE((const struct bt_uuid *)&uuids_srv[MYSRV_UUID_SRV3]),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_DPOT0_WIPER], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT0_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT0", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_dpot),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_DPOT1_WIPER], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT1_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT1", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_dpot),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_DPOT2_WIPER], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT2_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT2", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_dpot),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_DPOT3_WIPER], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT3_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT3", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_dpot),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_DPOT4_WIPER], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT4_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT4", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_dpot),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_DPOT5_WIPER], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT5_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT5", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_dpot),

	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_DPOT6_WIPER], BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_signed, write_signed, &app.values[MYID_DPOT6_WIPER]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("DPOT6", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_dpot),



	BT_GATT_PRIMARY_SERVICE(&uuids_srv[MYSRV_UUID_SRV4]),
	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_CH0_PIKO_A], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[MYID_CH0_PIKO_A]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH0_PA", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_piko_ampere),
	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_CH1_PIKO_A], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[MYID_CH1_PIKO_A]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH1_PA", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_piko_ampere),
	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_CH2_PIKO_A], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[MYID_CH2_PIKO_A]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH2_PA", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_piko_ampere),
	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_CH3_PIKO_A], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[MYID_CH3_PIKO_A]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH3_PA", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_piko_ampere),
	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_CH4_PIKO_A], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[MYID_CH4_PIKO_A]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH4_PA", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_piko_ampere),
	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_CH5_PIKO_A], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[MYID_CH5_PIKO_A]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH5_PA", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_piko_ampere),
	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_CH6_PIKO_A], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[MYID_CH6_PIKO_A]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH6_PA", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_piko_ampere),
	BT_GATT_CHARACTERISTIC((const struct bt_uuid *)&uuids_chrc[MYID_CH7_PIKO_A], BT_GATT_CHRC_NOTIFY, 0, NULL, NULL, &app.values[MYID_CH7_PIKO_A]),
	BT_GATT_CCC(ccc_cfg_changed1, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CUD("CH7_PA", BT_GATT_PERM_READ),
	BT_GATT_CPF(&cpf_piko_ampere),



);



#define NOTFIER_COUNT 28
static const notify_t notifier[NOTFIER_COUNT] = {
	{&gatt, MYGATT_SRV2_CH0_CHRC0, MYID_ADC_CH0},
	{&gatt, MYGATT_SRV2_CH1_CHRC0, MYID_ADC_CH1},
	{&gatt, MYGATT_SRV2_CH2_CHRC0, MYID_ADC_CH2},
	{&gatt, MYGATT_SRV2_CH3_CHRC0, MYID_ADC_CH3},
	{&gatt, MYGATT_SRV2_CH4_CHRC0, MYID_ADC_CH4},
	{&gatt, MYGATT_SRV2_CH5_CHRC0, MYID_ADC_CH5},
	{&gatt, MYGATT_SRV2_CH6_CHRC0, MYID_ADC_CH6},
	{&gatt, MYGATT_SRV2_CH7_CHRC0, MYID_ADC_CH7},
	{&gatt, MYGATT_SRV2_ADC_OFFSET_CHRC0, MYID_ADC_OFFSET},
	{&gatt, MYGATT_SRV2_ADC_VCM_CHRC0, MYID_ADC_VCM},
	{&gatt, MYGATT_SRV2_ADC_AVDD_CHRC0, MYID_ADC_AVDD},
	{&gatt, MYGATT_SRV2_ADC_TEMP_CHRC0, MYID_ADC_TEMP},
	{&gatt, MYGATT_SRV3_DPOT0_CHRC0, MYID_DPOT0_WIPER},
	{&gatt, MYGATT_SRV3_DPOT1_CHRC0, MYID_DPOT1_WIPER},
	{&gatt, MYGATT_SRV3_DPOT2_CHRC0, MYID_DPOT2_WIPER},
	{&gatt, MYGATT_SRV3_DPOT3_CHRC0, MYID_DPOT3_WIPER},
	{&gatt, MYGATT_SRV3_DPOT4_CHRC0, MYID_DPOT4_WIPER},
	{&gatt, MYGATT_SRV3_DPOT5_CHRC0, MYID_DPOT5_WIPER},
	{&gatt, MYGATT_SRV3_DPOT6_CHRC0, MYID_DPOT6_WIPER},
	{&gatt, MYGATT_SRV4_CH0_CHRC0, MYID_CH0_PIKO_A},
	{&gatt, MYGATT_SRV4_CH1_CHRC0, MYID_CH1_PIKO_A},
	{&gatt, MYGATT_SRV4_CH2_CHRC0, MYID_CH2_PIKO_A},
	{&gatt, MYGATT_SRV4_CH3_CHRC0, MYID_CH3_PIKO_A},
	{&gatt, MYGATT_SRV4_CH4_CHRC0, MYID_CH4_PIKO_A},
	{&gatt, MYGATT_SRV4_CH5_CHRC0, MYID_CH5_PIKO_A},
	{&gatt, MYGATT_SRV4_CH6_CHRC0, MYID_CH6_PIKO_A},
	{&gatt, MYGATT_SRV4_CH7_CHRC0, MYID_CH7_PIKO_A},
	{0, 0}
};


int mysrv_notify(app_t * a)
{
	btmisc_notifier(a->values, a->values_flags, MYID_COUNT, notifier, NOTFIER_COUNT, gatt.attrs);
	return 0;
}