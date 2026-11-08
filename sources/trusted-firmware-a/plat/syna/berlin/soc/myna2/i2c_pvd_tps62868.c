/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/debug.h>
#include "i2c.h"

// all defined under uv
#define TPS62868_STEPuV 5000
#define MIN_VOLTAGE 400000
#define MAX_VOLTAGE 1675000

#define SLAVE_ADDR 	0x49
#define RSVD		0xFFFFFFFF
#define REG_VOLTAGE	0x1
#define REG_CTRL3	0x3

#define VOUT_CPU_ID	(2)
#define VOUT_CORE_ID	(2)

static int curr_volt;

static int tps62868_pmic_volt2data(int volt, int *p_data)
{
	unsigned int data;

	if ((volt >= MIN_VOLTAGE) && (volt <= MAX_VOLTAGE))
		data = (volt - MIN_VOLTAGE) / TPS62868_STEPuV;
	else
		data = RSVD;

	if (data == RSVD)
		return -1;
	else {
		*p_data = data;
		return 0;
	}
}

static int tps62868_pmic_data2volt(int *p_volt, int vdata)
{
	*p_volt = MIN_VOLTAGE + (vdata * TPS62868_STEPuV);

	return 0;
}

static int tps62868_set_vol(int master_id, int slaveAddr, int volt)
{
	int target = volt;
	unsigned char buff[2];
	unsigned char data_reg0, data;
	int vdata = 0;
	unsigned int bulk_reg0 = REG_VOLTAGE;
	int ret = 1;

	i2c_master_init(master_id, 100, 0);  //int id, int speed, int b_10bit_addr
	ret = i2c_master_write_and_read(master_id, slaveAddr, (unsigned char*)&bulk_reg0, 1, (unsigned char*)&data_reg0, 1);
	if (ret) {
		NOTICE("PMIC i2c read fail !\n");
		return ret;
	}

	ret = tps62868_pmic_volt2data(target, &vdata);
	if (ret == 0) {
		data_reg0 = (unsigned char)vdata;
		INFO("data_reg0 = 0x%x \n", data_reg0);

		buff[0] = (unsigned char)REG_VOLTAGE;
		buff[1] = (unsigned char)data_reg0;
		ret = i2c_master_write_and_read(master_id, slaveAddr, buff, 2, (unsigned char*)0, 0);
		if (ret) {
			NOTICE("PMIC i2c write fail !\n");
			return ret;
		}

		buff[0] = (unsigned char)REG_VOLTAGE;
		ret = i2c_master_write_and_read(master_id, slaveAddr, buff, 1, &data, 1);
		if (ret) {
			NOTICE("PMIC i2c read fail !\n");
			return ret;
		}

		if (data_reg0 != data) {
			NOTICE("PMIC TPS62868: read:0x%02x != write:0x%02x\n", data, data_reg0);
			return -1;
		}
		udelay(20);
	}
	else {
		NOTICE("Illegal target voltage for PMIC TPS62868 !\n");
		return -1;
	}
	return ret;
}

static int tps62868_get_vol(int master_id, int slaveAddr)
{
	unsigned char read = 0xff;
	unsigned char buff[2];
	int volt;
	unsigned int bulk_reg = REG_VOLTAGE;
	unsigned int bulk_ctrl = REG_CTRL3;
	int ret = 1;

	i2c_master_init(master_id, 100, 0);  //int id, int speed, int b_10bit_addr

	ret = i2c_master_write_and_read(master_id, slaveAddr, (unsigned char*)&bulk_ctrl, 1, (unsigned char*)&read, 1);
	if (ret) {
		NOTICE("PMIC i2c read fail !\n");
		return ret;
	}

	buff[0] = (unsigned char)REG_CTRL3;
	buff[1] = (unsigned char)(read & 0xF7); //disable fast discharge
	ret = i2c_master_write_and_read(master_id, slaveAddr, buff, 2, (unsigned char*)0, 0);
	if (ret) {
		NOTICE("PMIC i2c write fail !\n");
		return ret;
	}

	ret = i2c_master_write_and_read(master_id, slaveAddr, (unsigned char*)&bulk_reg, 1, (unsigned char*)&read, 1);

	INFO("\nREG_VSEL0 = 0x%x \n", read);

	ret |= tps62868_pmic_data2volt(&volt, (int)read);
	if(ret) {
		NOTICE(" i2c read fail\n");
		return -1;
	}

	return volt;
}

int i2c_get_cpu_volt(void)
{
	int volt = tps62868_get_vol(VOUT_CPU_ID, SLAVE_ADDR);

	if(volt < 0)
		return 0;

	return volt;
}

int i2c_set_volt(int master_id, int from, int to)
{
	int volt = from;

	while (volt != to){
		if (volt > to){
			volt -= TPS62868_STEPuV;
			if (volt < to)
				volt = to;
		}else{
			volt += TPS62868_STEPuV;
			if (volt > to)
				volt = to;
		}

		if(0 != tps62868_set_vol(master_id, SLAVE_ADDR, volt))
			return 1;
	}

	return 0;
}

int i2c_set_vcpu_volt(int from, int to)
{
	return i2c_set_volt(VOUT_CPU_ID, from ,to);
}

int i2c_get_and_set_vcpu_volt(int to)
{
	int ret = 0;
	curr_volt = i2c_get_cpu_volt();

	if (!curr_volt) {
		NOTICE("can't get current vcpu voltage\n");
		return -1;
	}

	NOTICE("current volt = %d\n", curr_volt);

	ret = i2c_set_vcpu_volt(curr_volt, to);

	if (ret)
		NOTICE("fail to set vcpu voltage\n");
	else
		NOTICE("cpu voltage is set to %d\n", to);

	return ret;
}

int i2c_restore_vcpu_volt(void)
{
	return i2c_get_and_set_vcpu_volt(curr_volt);
}
