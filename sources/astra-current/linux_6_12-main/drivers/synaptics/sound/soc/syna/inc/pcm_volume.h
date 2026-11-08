/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2025 PCM Volume Control Module - dB based */

#ifndef __PCM_VOLUME_H__
#define __PCM_VOLUME_H__

#include <linux/types.h>

/* Volume range in dB (hundredths of dB) */
#define PCM_VOLUME_DB_MIN    -6000  /* -60.00 dB */
#define PCM_VOLUME_DB_MAX        0  /*   0.00 dB */
#define PCM_VOLUME_DB_STEP      50  /*   0.50 dB */
#define PCM_VOLUME_DB_MUTE   -9999  /*   -∞ dB (mute) */
#define PCM_VOLUME_DB_DEFAULT    0  /*   0.00 dB (unity gain) */

/* Q14 fixed-point gain constants (1.0 = 16384) */
#define PCM_VOLUME_Q14_UNITY   16384  /* 1.0 in Q14, corresponds to 0dB */
#define PCM_VOLUME_Q14_SHIFT      14  /* Q14 fractional bits */
#define PCM_VOLUME_TABLE_SIZE    121  /* number of entries in db_to_linear_table */

/* PCM sample range limits */
#define PCM_SAMPLE_MAX_S16   32767
#define PCM_SAMPLE_MIN_S16  (-32768)
#define PCM_SAMPLE_MAX_S24   0x7FFFFF
#define PCM_SAMPLE_MIN_S24  (-0x800000)
#define PCM_SAMPLE_MAX_S32   2147483647LL
#define PCM_SAMPLE_MIN_S32  (-2147483648LL)

/* Convert dB to linear gain (Q14 fixed-point: 1.0 = 16384) */
int32_t pcm_volume_db_to_linear(int db_value);

/* Apply volume scaling to PCM data using dB value */
void pcm_volume_apply_s16_db(int16_t *dst, const int16_t *src,
			     size_t frames, int channels, int db_value);
void pcm_volume_apply_s24_db(int32_t *dst, const int32_t *src,
			     size_t frames, int channels, int db_value);
void pcm_volume_apply_s32_db(int32_t *dst, const int32_t *src,
			     size_t frames, int channels, int db_value);

#endif /* __PCM_VOLUME_H__ */
