/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/debug.h>
#include "string.h"
#include <lib/utils_def.h>
#include "i2c.h"

// TI: INA220 High- or Low-Side, Bidirectional Current and Power Monitor

// assume current < 3A. 15 bits (32767), so define Current_LSB is 0.1mA
#define CURRENT_LSB_1mA_MULTI     10        // means Current_LSB = 1/10 mA
#define SAMPLE_DELAY_TIME_US      3500     // 35ms

typedef struct
{
    char  ch_name[18];
    int   r_sample; // mOhm
    int   current_lsb_1mA_div;
    int   i2c_id;
    char  i2c_addr;
	int  sensor_type;

    int   inited;

    // context data
    unsigned short saved_value;
    unsigned short old_value;

    unsigned short ManufacturerID;
} pc_mon_t;

/*
 * There are three types of current/power monitor sensor:
 * 1. INA220 (sensor_type = 0 ManufacturerID = 0x2000)
 * 2. SGM832A (sensor_type = 0, ManufacturerID = 0x5449)
 * 3. INA3221 (sensor_type = 1)
 * They can be distinguished by sensor type and ManufacturerID
 */


static int ti_monitor_write_cfg(pc_mon_t* p_cfg, int restore)
{
    unsigned char write_bytes[3];
    unsigned short value;

    if (!p_cfg || (p_cfg->i2c_addr == 0))
        return -1;

    value = restore ? p_cfg->old_value : p_cfg->saved_value;

    write_bytes[0] = 0x00;
    write_bytes[1] = (value >> 8) & 0xff;
    write_bytes[2] = value & 0xff;
    return i2c_master_write_and_read(p_cfg->i2c_id,
                                    p_cfg->i2c_addr,
                                    write_bytes,
                                    3,
                                    NULL,
                                    0);
}

static int ti_monitor_read_cfg(pc_mon_t* p_cfg)
{
    int ret;
    unsigned char write_byte;
    unsigned char read_bytes[2];

    if (!p_cfg || (p_cfg->i2c_addr == 0))
        return -1;

    write_byte = 0x00;
    ret = i2c_master_write_and_read(  p_cfg->i2c_id,
                                    p_cfg->i2c_addr,
                                    &write_byte,
                                    1,
                                    read_bytes,
                                    2);

    if (ret)
        return ret;

    p_cfg->old_value = ((unsigned short)read_bytes[0] << 8) + read_bytes[1];

    return ret;
}

static int ti_monitor_init(pc_mon_t* p_cfg)
{
    unsigned char write_bytes[3];
	unsigned char read_bytes[2];
    unsigned short u16_value = 0;
    int ret;

    if (!p_cfg || (p_cfg->i2c_addr == 0))
        return -1;

    if (0 == p_cfg->sensor_type) {
        // Manufacturer ID register addr: 0xFEh
        write_bytes[0] = 0xFE;
        ret = i2c_master_write_and_read(  p_cfg->i2c_id,
                                        p_cfg->i2c_addr,
                                        write_bytes,
                                        1,
                                        read_bytes,
                                        2);

        if (ret != 0) {
            NOTICE("failed to read manufacturer ID\n");
            return ret;
        }

        //ManufacturerID ina220: 0x2000; SGM832A: 0x5449
        p_cfg->ManufacturerID = ((unsigned short)read_bytes[0]<<8) + read_bytes[1];

        if(p_cfg->ManufacturerID == 0x2000)   //ina220
            INFO("INA220 IS DETECTED in one power channel\n");
        else if(p_cfg->ManufacturerID == 0x5449)   //SGM832A
            INFO("SGM832A IS DETECTED in one power channel\n");
    }

    if (0 != ti_monitor_read_cfg(p_cfg)) {
        NOTICE("failed to read configuration\n");
        return ret;
    }

    if (0 == p_cfg->sensor_type) {
        if(p_cfg->ManufacturerID == 0x2000) {  //ina220
            // Configuration Register addr:0x00h, default setting 399Fh
            //  bit[15]: Reset = 1'b1 (default=0)
            //  bit[14]: x
            //  bit[13]: Bus Vol Range =	1'b0 -> 16V
            // 								1'b1 -> 32V (default)
            //  bit[12:11]: Gain/Range =	2'b00 -> /1, +/-40mV
            // 								2'b01 -> /2, +/-80mV
            // 								2'b10 -> /4, +/-160mV
            // 								2'b11 -> /8, +/-320mV (default)
            //  bit[10:7]: Shunt ADC Resolu/Avg = 4'b0011 -> 12bit
            //  bit[6:3]: BUS ADC Resolu/Avg = 4'b0011 -> 12bit (default)
            //  bit[2:0]: MODE =	3/b000 -> PD
            // 										3'b100 -> ADC Off.
            // 										3'b101 -> Shunt Voltage, continuous.
            // 										3'b110 -> Bus Voltage, continuous.
            // 										3'b111 -> Shun and Bus, continuous. (default)

            u16_value = (0<<13)  |  // Bus Vol Range,   1'b0 -> 16V
                        (0<<11)  |  // Gain/Range,      2'b00 -> /1, +/-40mV
                        (0xd<<7) |  // Shunt ADC Resolu/Avg, 4'b1101 -> 32 samples, 17.02 ms
                        (0xd<<3) |  // BUS ADC Resolu/Avg,   4'b1101 -> 32 samples, 17.02 ms
                        (3<<0);     // MODE,   3'b011 -> Shunt and bus, triggered

    } else if(p_cfg->ManufacturerID == 0x5449) {
            // Configuration Register addr:0x00h, default setting 4127h
            //  bit[15]: Reset = 1'b1 (default=0)
            //  bit[14:12]: x
            //  bit[11:9]: Averaging Mode
            //  bit[8:6]: Bus Voltage Conversion Time
            //  bit[5:3]: Shunt Voltage Conversion Time
            //  bit[2:0]: MODE =	3/b000 -> PD
            // 										3'b100 -> ADC Off.
            // 										3'b101 -> Shunt Voltage, continuous.
            // 										3'b110 -> Bus Voltage, continuous.
            // 										3'b111 -> Shun and Bus, continuous. (default)


            u16_value = (3<<9)  |  // Averaging Mode,   3'b011 -> 64
                        (4<<6)  |  // BUS ADC Resolu/Avg,   3'b100 -> 16 samples, 1.036ms
                        (4<<3) |   // Shunt ADC Resolu/Avg, 3'b100 -> 16 samples, 1.036ms
                        (3<<0);     // MODE,   3'b011 -> Shunt and bus, triggered
        }
    } else {
        // Configuration Register addr:0x00h, default setting 7127h
        //Bit Field Type Reset Description
        //15 RST R/W 0h Reset bit. Set this bit = 1 to generate a system reset that is the
        //   same as a power-on reset (POR). This bit resets all registers to
        //   default values and self-clears.
        //14 CH1en R/W 7h Channel enable mode. These bits allow each channel to be
        //   independently enabled or disabled.
        //   0 = Channel disable
        //   1 = Channel enable (default)
        //13 CH2en
        //12 CH3en
        //11-9 AVG2-0 R/W 0h Averaging mode. These bits set the number of samples that are
        //   collected and averaged together.
        //   000 = 1 (default)
        //   001 = 4
        //   010 = 16
        //   011 = 64
        //   100 = 128
        //   101 = 256
        //   110 = 512
        //   111 = 1024
        //8-6 VBUSCT2-0 R/W 4h Bus-voltage conversion time. These bits set the conversion time
        //   for the bus-voltage measurement.
        //   000 = 140 μs
        //   001 = 204 μs
        //   010 = 332 μs
        //   011 = 588 μs
        //   100 = 1.1 ms (default)
        //   101 = 2.116 ms
        //   110 = 4.156 ms
        //   111 = 8.244 ms
        //5-3 VSHCT2-0 R/W 4h Shunt-voltage conversion time. These bits set the conversion
        //   time for the shunt-voltage measurement.
        //   The conversion-time bit settings for VSHCT2-0 are the same as
        //   VBUSCT2-0 (bits 8-6) listed in the previous row.
        //2-0 MODE3-1 R/W 7h Operating mode. These bits select continuous, single-shot
        //   (triggered), or power-down mode of operation. These bits default
        //   to continuous shunt and bus mode.
        //   000 = Power-down
        //   001 = Shunt voltage, single-shot (triggered)
        //   010 = Bus voltage, single-shot (triggered)
        //   011 = Shunt and bus, single-shot (triggered)
        //   100 = Power-down
        //   101 = Shunt voltage, continuous
        //   110 = Bus voltage, continuous
        //   111 = Shunt and bus, continuous (default)

        u16_value = (7<<12)  |  // CH 1/2/3 en
                    (4<<9)   |  // 128 samples that are collected and averaged together.
                    (4<<6)   |  // Bus-voltage conversion time. 1.1mS
                    (4<<3)   |  // Shunt-voltage conversion time. 1.1mS
                    (3<<0);     // MODE,   3'b011 -> Shunt and bus, single-shot (triggered)
    }

    p_cfg->saved_value = u16_value;

    ret = ti_monitor_write_cfg(p_cfg, 0);
    if (0 != ret)
        NOTICE("failed to set configuration\n");

    return ret;
}

static int ti_monitor_kick(pc_mon_t* p_cfg)
{
    int ret;

    if (!p_cfg || (p_cfg->i2c_addr == 0))
        return -1;

    ret = ti_monitor_write_cfg(p_cfg, 0);
    if (0 != ret)
        NOTICE("failed to set configuration\n");

    return ret;
}

static int ti_ina220_get_sample(pc_mon_t* p_cfg, int* p_v_shunt_uv, int* p_v_bus_uv, int* p_current_ua, int* p_power_uw)
{
    unsigned char write_bytes[3];
    unsigned char read_bytes[2];
    unsigned short u16_value;
    int v_shunt_uv;
    int v_bus_uv;
    int int_value;
    int ret = 0;

    if (!p_cfg || !p_v_shunt_uv || !p_v_bus_uv
        || !p_current_ua || !p_power_uw)
        return -1;

    //ina220
    // Shunt voltage register addr: 0x01h
    // bit[15]: Sign, 1'b1=Neg; 1'b0=Pos
    // bit[14:0]: voltage data. 320mV = 0x7D00, -320mV = 0x8300h
    // Vshount_LSB = 10 uV

    //SGM832A
    // Shunt voltage register addr: 0x01h
    // Vshount_LSB = 2.5 uV

    write_bytes[0] = 0x01;
    ret = i2c_master_write_and_read(  p_cfg->i2c_id,
                                        p_cfg->i2c_addr,
                                        write_bytes,
                                        1,
                                        read_bytes,
                                        2);

    if (ret != 0) {
        return ret;
    }

    u16_value = ((unsigned short)read_bytes[0]<<8) + read_bytes[1];
    INFO("%s, REG01 raw data: 0x%04x\n", p_cfg->ch_name, u16_value);

    int_value = (short)u16_value;
    if(p_cfg->ManufacturerID == 0x2000) {  //ina220
        int_value *= 10;
    } else if(p_cfg->ManufacturerID == 0x5449) {
        int_value *= 25;
        int_value /= 10;
    }

    //NOTICE("%s shunt voltage:%0.3fmv\n", p_cfg->ch_name, (double)int_value/1000.0);
    v_shunt_uv = int_value;
    if (p_v_shunt_uv) *p_v_shunt_uv = v_shunt_uv;


    //ina220
    // Bus Voltage register addr: 0x02h
    // bit[15:3]: Bus voltage data. full scale 32V-> 8000 steps. full scale 16V-> 4000 steps.
    // bit[2]: x
    // bit[1]: conversion ready = 1'b1
    // bit[0]: overflow = 1'b1
    // VBus_LSB = 4 mV

    //SGM832A
    // Bus Voltage register addr: 0x02h
    // VBus_LSB = 1.25 mV

    write_bytes[0] = 0x02;
    ret = i2c_master_write_and_read(  p_cfg->i2c_id,
                                        p_cfg->i2c_addr,
                                        write_bytes,
                                        1,
                                        read_bytes,
                                        2);

    if (ret != 0) {
        NOTICE("failed\n");
        return ret;
    }

    u16_value = ((unsigned short)read_bytes[0]<<8) + read_bytes[1];

    INFO("%s, REG02 raw data: 0x%04x\n", p_cfg->ch_name, u16_value);

    if (p_cfg->ManufacturerID == 0x2000) {  //ina220
        if (u16_value & 0x01) {
            NOTICE("overflow\n");
            return -1;
        }

        if (!(u16_value & 0x02)) {
            NOTICE("not ready\n");
            return -1;
        }

        u16_value >>= 3;
        int_value = u16_value * 4; // 4mv
    } else if (p_cfg->ManufacturerID == 0x5449)
        int_value = u16_value * 125 / 100; // 1.25mv

    //NOTICE("%s bus voltage:%dmv\n", p_cfg->ch_name, int_value);

    v_bus_uv = int_value*1000;
    if (p_v_bus_uv) *p_v_bus_uv = v_bus_uv;

    *p_current_ua = v_shunt_uv * 1000 / p_cfg->r_sample;

    *p_power_uw = v_bus_uv/100 * v_shunt_uv / p_cfg->r_sample / 10;

    return ret;
}

static int ti_ina3221_get_sample(pc_mon_t* p_cfg, int* p_v_shunt_uv, int* p_v_bus_uv, int* p_current_ua, int* p_power_uw)
{
    unsigned char write_bytes[3];
    unsigned char read_bytes[2];
    unsigned short u16_value;
    int v_shunt_uv;
    int v_bus_uv;
    int int_value;
    int i, ret = 0;

    if (!p_cfg || !p_v_shunt_uv || !p_v_bus_uv
        || !p_current_ua || !p_power_uw)
        return -1;

    for(i = 0; i < 3; i++) {
        // Shunt voltage register addr: 0x01h
        // 15 SIGN R 0h Sign bit.
        //   0 = positive number
        //   1 = negative number in twos complement format
        //14-3 SD11-0 R 0h Channel-1 shunt-voltage data bits
        //2-0 Reserved R 0h Reserved
        //Full-scale range = 163.8 mV (decimal = 7FF8); LSB (SD0): 40 μV.

        write_bytes[0] = 0x01 + i * 2;
        ret = i2c_master_write_and_read(  p_cfg->i2c_id,
                                            p_cfg->i2c_addr,
                                            write_bytes,
                                            1,
                                            read_bytes,
                                            2);

        if (ret != 0) {
            return ret;
        }

        u16_value = ((unsigned short)read_bytes[0]<<8) + read_bytes[1];
        INFO("%s, REG01 raw data: 0x%04x\n", p_cfg->ch_name, u16_value);

        u16_value >>= 3;   //bit[2:0] is reserved
        int_value = (short)u16_value;
        int_value *= 40;

        //NOTICE("%s shunt voltage:%0.3fmv\n", p_cfg->ch_name, (double)int_value/1000.0);
        v_shunt_uv = int_value;
        *(p_v_shunt_uv + i) = v_shunt_uv;

        // Bus Voltage register addr: 0x02h
        // 15 SIGN R 0h Sign bit.
        //     0 = positive number
        //     1 = negative number in twos complement format.
        //14-3 BD11-0 R 0h Channel-1 bus-voltage data bits
        //2-0 Reserved R 0h Reserved
        //Full-scale range = 32.76 V (decimal = 7FF8); LSB (BD0) = 8 mV.

        write_bytes[0] = 0x02+i*2;
        ret = i2c_master_write_and_read(  p_cfg->i2c_id,
                                            p_cfg->i2c_addr,
                                            write_bytes,
                                            1,
                                            read_bytes,
                                            2);

        if (ret != 0) {
            NOTICE("failed\n");
            return ret;
        }

        u16_value = ((unsigned short)read_bytes[0]<<8) + read_bytes[1];

        INFO("%s, REG02 raw data: 0x%04x\n", p_cfg->ch_name, u16_value);

        /*
        if (u16_value & 0x01) {
            NOTICE("overflow\n");
        }

        if (!(u16_value & 0x02)) {
            NOTICE("not ready\n");
        }
        */
        u16_value >>= 3;
        int_value = u16_value * 8; // 8mv

        //NOTICE("%s bus voltage:%dmv\n", p_cfg->ch_name, int_value);

        v_bus_uv = int_value*1000;
        *(p_v_bus_uv+i) = v_bus_uv;

        *(p_current_ua + i) = v_shunt_uv * 1000 / p_cfg->r_sample;

        *(p_power_uw + i) = v_bus_uv/100 * v_shunt_uv / p_cfg->r_sample / 10;
    }

    return ret;
}

#define PC_MON_MAX_NUM      5   //SL1620_RDK

typedef struct
{
    int pc_mon_init[PC_MON_MAX_NUM];
    pc_mon_t pc_mon[PC_MON_MAX_NUM];
} pc_mon_global_t;

pc_mon_global_t g_pc_mon;

static int pc_mon_set_cfg(int id, char* ch_name, int r_sample, int i2c_id, int i2c_addr, int sensor_type)
{
    strlcpy(g_pc_mon.pc_mon[id].ch_name, ch_name, sizeof(g_pc_mon.pc_mon[id].ch_name));
    g_pc_mon.pc_mon[id].r_sample = r_sample;
    g_pc_mon.pc_mon[id].i2c_id = i2c_id;
    g_pc_mon.pc_mon[id].i2c_addr = i2c_addr;
	g_pc_mon.pc_mon[id].sensor_type = sensor_type;
    g_pc_mon.pc_mon[id].inited = 0;

    return 0;
}

int pc_monitor_init(void)
{
    int i = 0;
    int ret = 0;
    pc_mon_t* p_cfg;

    pc_mon_set_cfg(0, "3V3", 10, 1, 0x40, 0);
    pc_mon_set_cfg(1, "1V8",  10, 1, 0x41, 0);
    pc_mon_set_cfg(2, "VCORE   ",   10, 0, 0x40, 1);
    pc_mon_set_cfg(3, "VDDM_1V2",   10, 0, 0x40, 1);
    pc_mon_set_cfg(4, "VDDM_2V5",   10, 0, 0x40, 1);

    i2c_master_init(0, 100, 0);
    i2c_master_init(1, 100, 0);

    for (i = 0; i < ARRAY_SIZE(g_pc_mon.pc_mon); i++) {
        p_cfg = &g_pc_mon.pc_mon[i];
        ret = ti_monitor_init(p_cfg);
        if (ret)
            return ret;
    }

    return ret;
}

int pc_monitor_restore(void)
{
    int i = 0;
    int ret = 0;
    pc_mon_t* p_cfg;

    for (i = 0; i < ARRAY_SIZE(g_pc_mon.pc_mon); i++) {
        p_cfg = &g_pc_mon.pc_mon[i];
        ret = ti_monitor_write_cfg(p_cfg, 1);
        if (ret)
            return ret;
    }

    return ret;
}

int pc_mon_sample_all(unsigned int mask)
{
    pc_mon_t* p_cfg;
    unsigned int sum = 0;
    int ret = 0;
    int i, j = 0;

    

    for (i = 0; i < ARRAY_SIZE(g_pc_mon.pc_mon); i++) {
        if (0 == (mask & (1<<i))) continue;

        p_cfg = &g_pc_mon.pc_mon[i];
        ret = ti_monitor_kick(p_cfg);
        if (ret != 0) {
            continue;
	    }
    }

    udelay(SAMPLE_DELAY_TIME_US);

    NOTICE("  ID:    CH    V_shunt(mV)        V_bus(mV)        I(mA)        Power(mW)\n");

    for (i = 0; i < ARRAY_SIZE(g_pc_mon.pc_mon); i++) {
        int v_shunt_uv[3], v_bus_uv[3], current_ua[3], power_uw[3];

        if (0 == (mask & (1<<i))) continue;

        p_cfg = &g_pc_mon.pc_mon[i];

		if (p_cfg->sensor_type == 0) {   //ina220
	        ret = ti_ina220_get_sample(p_cfg, &v_shunt_uv[0], &v_bus_uv[0], &current_ua[0], &power_uw[0]);
	        if (ret != 0) {
	            continue;
	        }

	        sum += power_uw[0];

            NOTICE(" [%d]: %s        %d.%d(mV)        %d.%d(mV)     %d.%d(mA)      %d.%d(mW)\n",
	                i,
	                p_cfg->ch_name,
                    v_shunt_uv[0] / 1000, v_shunt_uv[0] % 1000,
	                v_bus_uv[0] / 1000, v_bus_uv[0] % 1000,
	                current_ua[0] / 1000, current_ua[0] % 1000,
	                power_uw[0] / 1000, power_uw[0] % 1000);
		} else if (p_cfg->sensor_type == 1) {   //ina3221
            if(i == 2) {
                ret = ti_ina3221_get_sample(p_cfg, &v_shunt_uv[0], &v_bus_uv[0], &current_ua[0], &power_uw[0]);
                if (ret != 0) {
                    continue;
                }

	            sum +=  power_uw[0] + power_uw[1] + power_uw[2];

                // print
                for (j = 0; j < 3; j++)
                    NOTICE(" [%d]: %s   %d.%d(mV)        %d.%d(mV)     %d.%d(mA)       %d.%d(mW)\n",
                            i + j,
                            //p_cfg->ch_name,
                            g_pc_mon.pc_mon[i+j].ch_name,
                            v_shunt_uv[j] / 1000, v_shunt_uv[j] % 1000,
                            v_bus_uv[j] / 1000, v_bus_uv[j] % 1000,
                            current_ua[j] / 1000, current_ua[j] % 1000,
                            power_uw[j] / 1000, power_uw[j] % 1000);
		    }
		}
    }

    NOTICE(" Total Power: %d.%d(mW)\n", sum / 1000, sum % 1000);

    return 0;
}