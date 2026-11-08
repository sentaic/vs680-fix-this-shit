// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2026, Synaptics Incorporated.
 *
 * Xincun SPI NAND driver
 */

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/mtd/spinand.h>

#define SPINAND_MFR_XINCUN		0x6c

/*
 * Status Register-3 (C0h)
 *
 * bit[5:4] ECC status:
 *   bit 5: ECC_S[1]
 *   bit 4: ECC_S[0]
 *
 *   ECC_S[1:0] = 00b: no bit errors
 *              = 01b: bit errors corrected, less than threshold
 *              = 10b: uncorrectable bit errors
 *              = 11b: bit errors corrected, equal to or greater than threshold
 */
#define XINCUN_STATUS_ECC_MASK		GENMASK(5, 4)
#define XINCUN_STATUS_ECC_NO_BITFLIPS	(0 << 4)
#define XINCUN_STATUS_ECC_HAS_BITFLIPS	(1 << 4)
#define XINCUN_STATUS_ECC_UNCOR_ERROR	(2 << 4)
#define XINCUN_STATUS_ECC_THRESHOLD	(3 << 4)

static SPINAND_OP_VARIANTS(read_cache_variants,
		SPINAND_PAGE_READ_FROM_CACHE_X4_OP(0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_X2_OP(0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_OP(true, 0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_OP(false, 0, 1, NULL, 0));

static SPINAND_OP_VARIANTS(write_cache_variants,
		SPINAND_PROG_LOAD_X4(true, 0, NULL, 0),
		SPINAND_PROG_LOAD(true, 0, NULL, 0));

static SPINAND_OP_VARIANTS(update_cache_variants,
		SPINAND_PROG_LOAD_X4(false, 0, NULL, 0),
		SPINAND_PROG_LOAD(false, 0, NULL, 0));


static int xcsp_ooblayout_ecc(struct mtd_info *mtd, int section,
			      struct mtd_oob_region *region)
{
	return -ERANGE;
}

static int xcsp_ooblayout_free(struct mtd_info *mtd, int section,
			       struct mtd_oob_region *region)
{
	if (section)
		return -ERANGE;

	region->offset = 1;
	region->length = 63;

	return 0;
}

static const struct mtd_ooblayout_ops xcsp_ooblayout = {
	.ecc = xcsp_ooblayout_ecc,
	.free = xcsp_ooblayout_free,
};

static int xcsp_ecc_get_status(struct spinand_device *spinand, u8 status)
{
	struct nand_device *nand = spinand_to_nand(spinand);

	switch (status & XINCUN_STATUS_ECC_MASK) {
	case XINCUN_STATUS_ECC_NO_BITFLIPS:
		return 0;

	case XINCUN_STATUS_ECC_HAS_BITFLIPS:
		return 1;

	case XINCUN_STATUS_ECC_THRESHOLD:
		return nanddev_get_ecc_conf(nand)->strength;

	case XINCUN_STATUS_ECC_UNCOR_ERROR:
		return -EBADMSG;

	default:
		break;
	}

	return -EINVAL;
}

static const struct spinand_info xincun_spinand_table[] = {
	/*
	* Table 19 MID & DID:
	*
	* Read ID sequence:
	*   9Fh + Dummy + MID + DID1 + DID2
	*
	* Byte 0, MID:
	*   0x6c: Manufacturer ID, XinCun
	*
	* Byte 1, DID1:
	*   0x01: 1Gb
	*   0xa1: 2Gb
	*   0xb1: 4Gb
	*   0xb2: 8Gb
	*
	* Byte 2, DID2:
	*   0x0a: 3.3V
	*   0x0c: 1.8V
	*
	*/
	SPINAND_INFO("XCSP1xXPK-IT",
		     SPINAND_ID(SPINAND_READID_METHOD_OPCODE_DUMMY, 0x01, 0x0a),
		     NAND_MEMORG(1, 2048, 64, 64, 1024, 40, 1, 1, 1),
		     NAND_ECCREQ(9, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     SPINAND_HAS_QE_BIT,
		     SPINAND_ECCINFO(&xcsp_ooblayout,
				      xcsp_ecc_get_status)),

	SPINAND_INFO("XCSP2xXPK-IT",
		     SPINAND_ID(SPINAND_READID_METHOD_OPCODE_DUMMY, 0xa1, 0x0c),
		     NAND_MEMORG(1, 2048, 64, 64, 2048, 40, 1, 1, 1),
		     NAND_ECCREQ(9, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     SPINAND_HAS_QE_BIT,
		     SPINAND_ECCINFO(&xcsp_ooblayout,
				      xcsp_ecc_get_status)),
};

static const struct spinand_manufacturer_ops xincun_spinand_manuf_ops = {
};

const struct spinand_manufacturer xincun_spinand_manufacturer = {
	.id = SPINAND_MFR_XINCUN,
	.name = "XINCUN",
	.chips = xincun_spinand_table,
	.nchips = ARRAY_SIZE(xincun_spinand_table),
	.ops = &xincun_spinand_manuf_ops,
};

