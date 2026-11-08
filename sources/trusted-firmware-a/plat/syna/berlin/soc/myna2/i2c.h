/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _I2C_API_H_
#define _I2C_API_H_

// error code definitions
#ifndef I2C_OK
#define I2C_OK			0
#endif
#ifndef I2C_ERROR
#define I2C_ERROR		1
#endif
#ifndef I2C_EBADPARAM
#define I2C_EBADPARAM		2
#endif
#ifndef I2C_EBUSRW
#define I2C_EBUSRW		3
#endif

enum I2C_INSTANCES
{
	SOC_I2C0,
	SOC_I2C1,
	SOC_I2C2,
	SOC_I2C3,
	I2C_MAX
};

// I2C standard bus speed
enum
{
	I2C_STANDARD_SPEED_100K = 100,
	I2C_STANDARD_SPEED_400K = 400,
};

// I2C speed mode registers, note our IP doesn't support high speed mode
enum
{
	I2C_STANDARD_SPEED_MODE = 1, // 100K is the default
	I2C_FAST_SPEED_MODE   = 2    // 400K is the default

};

// I2C slave address type
enum
{
	I2C_7BIT_SLAVE_ADDR	= 0,
	I2C_10BIT_SLAVE_ADDR = 1
};

#ifdef __cplusplus__
extern "C" {
#endif

/******************************************
 *  global function declarations
 *****************************************/
/*********************************************************
 * FUNCTION: init I2C master: set default bus speed
 * PARAMS: master_id - id of I2C master to init
 * PARAMS: spped - speed of i2c master
 * PARAMS: b_10bit_addr - 7 bit or 10 bit address
 * RETURN: I2C_OK - succeed
 *         I2C_EBADPARAM - invalid parameter
 ********************************************************/
int i2c_master_init(int id, int speed, int b_10bit_addr);

/***********************************************************************
 * FUNCTION: read bytes from slave device
 * PARAMS: master_id - id of I2C master to operate
 *         slave_addr - address of slave device
 *         addr_type - address type: I2C_7BIT_SLAVE_ADDR or I2C_10BIT_SLAVE_ADDR
 *         pwbuf - pointer of write buffer
 *         wnum - number of bytes to write
 *         prbuf - pointer of read buffer
 *         rnum - number of bytes to read
 * RETURN: I2C_OK - succeed  *         I2C_EBUSY - I2C module is enabled
 *         I2C_EBUSRW - read fail
 * NOTE: maximum write bytes is 260, maximum read bytes is 256 in a single
 *       transaction
 ***********************************************************************/
int i2c_master_write_and_read(int id, int target_addr, unsigned char* send_buff, int send_len, unsigned char* recv_buff, int recv_len);

void i2c_param_save(void);

void i2c_param_restore(void);

void udelay(unsigned int us);

int i2c_get_and_set_vcpu_volt(int to);

int i2c_restore_vcpu_volt(void);

#ifdef __cplusplus__
}
#endif

#endif
