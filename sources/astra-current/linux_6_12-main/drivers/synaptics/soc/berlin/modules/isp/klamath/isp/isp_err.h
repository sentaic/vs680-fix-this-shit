// SPDX-License-Identifier: GPL-2.0
/*
* Copyright (C) 2025 Synaptics Incorporated
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*/

// error code definitions
enum ISP_ERROR {
	ISP_OK			   = 0x0000,   /**< Success. */
	ISP_ENOTIMPL	   = 0x0001,   /**< Function/feature not implementated. */
	ISP_EBADPARAM	   = 0x0002,   /**< Function parameter error. */
	ISP_ENOMEM		   = 0x0003,   /**< Not enough memory. */
	ISP_ENOSHM		   = 0x0004,   /**< Not enough share memory. */
	ISP_ETIMEOUT	   = 0x0005,   /**< Operation timeout. */
	ISP_EERRSYSCALL    = 0x0006,   /**< Syscall error. */
	ISP_EIOFAIL		   = 0x0007,   /**< Perepheral IO fail. */
	ISP_EEVENTFULL	   = 0x0008,   /**< Event queue full. */
	ISP_EHARDWAREBUSY  = 0x0009,   /**< Hardware busy. */
	ISP_EHWFAIL		   = 0x000A,   /**< Hardware fail. */
	ISP_EOSALFAIL	   = 0x000B,   /**< OSAL fail. */
	ISP_ENOSWRSC	   = 0x000C,   /**< Not enough software resource. */
	ISP_ENOHWRSC	   = 0x000D,   /**< Not enough hardware resource. */
	ISP_ESWSTATEWRONG  = 0x000E,   /**< Don't permit in current software state. */
	ISP_EHWSTATEWRONG  = 0x000F,   /**< Don't permit in current hardware state. */
	ISP_ERCPERROR	   = 0x0010,   /**< RPC error. */
	ISP_ESWMODEWRONG   = 0x0011,   /**< Software mode wrong. */
	ISP_EHWMODEWRONG   = 0x0012,   /**< Hardware mode wrong. */
	ISP_ECONNCLEARED   = 0x0013,   /**< Connection has been cleared error. */
	ISP_ERANGEPARAM    = 0x0014,   /**< Parameter is out of range. */
	ISP_ECANCELLED	   = 0x0015,   /**< Operation is cancelled. */
	ISP_EBDERROR	   = 0x0016,   /**< BD data error. */

	ISP_EPRIVATE_BASE  = 0x1000,   /**< The module private error value base. */
	ISP_ENODEV		   = 0x1001,   /**< No device. */
	ISP_EBADCALL	   = 0x1002,   /**< channel not connected to DV */
	ISP_EUNSUPPORT	   = 0x1003,   /**< plane not connected in configuration*/
	ISP_EUNCONFIG	   = 0x1004,   /**< VPP not configured */
	ISP_ECMDQFULL	   = 0x1005,   /**< Command Queue is full */
	ISP_EFRAMEQFULL    = 0x1006,   /**< Frame Queue is full */
	ISP_EBCMBUFFULL    = 0x1007,   /**< BCM Buffer is full */
	ISP_EVBIBUFFULL    = 0x1008,   /**< VBI Buffer is full */
};

