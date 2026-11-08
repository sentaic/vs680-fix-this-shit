// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2025 PCM Volume Control Module - dB based */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include "pcm_volume.h"

/*
 * dB to linear gain lookup table
 * Range: -60dB to 0dB, step 0.5dB (121 entries)
 * Format: Q14 fixed-point (1.0 = 16384)
 * Formula: gain = round(pow(10, dB/20.0) * 16384)
 * All values are attenuation only (gain <= 1.0), max value is 16384
 */
static const uint16_t db_to_linear_table[121] = {
        /* -60.0 to -55.0 dB (index 0-10) */
        16, 17, 18, 19, 21, 22, 23, 25, 26, 28, 29,
        /* -54.5 to -49.5 dB (index 11-21) */
        31, 33, 35, 37, 39, 41, 44, 46, 49, 52, 55,
        /* -49.0 to -44.0 dB (index 22-32) */
        58, 62, 65, 69, 73, 78, 82, 87, 92, 98, 103,
        /* -43.5 to -38.5 dB (index 33-43) */
        110, 116, 123, 130, 138, 146, 155, 164, 174, 184, 195,
        /* -38.0 to -33.0 dB (index 44-54) */
        206, 218, 231, 245, 260, 275, 291, 309, 327, 346, 367,
        /* -32.5 to -27.5 dB (index 55-65) */
        389, 412, 436, 462, 489, 518, 549, 581, 616, 652, 691,
        /* -27.0 to -22.0 dB (index 66-76) */
        732, 775, 821, 870, 921, 976, 1034, 1095, 1160, 1229, 1301,
        /* -21.5 to -16.5 dB (index 77-87) */
        1379, 1460, 1547, 1638, 1735, 1838, 1947, 2063, 2185, 2314, 2451,
        /* -16.0 to -11.0 dB (index 88-98) */
        2597, 2751, 2914, 3086, 3269, 3463, 3668, 3885, 4115, 4359, 4618,
        /* -10.5 to  -5.5 dB (index 99-109) */
        4891, 5181, 5488, 5813, 6158, 6523, 6909, 7318, 7752, 8211, 8698,
        /*  -5.0 to   0.0 dB (index 110-120) */
        9213, 9759, 10338, 10950, 11599, 12286, 13014, 13785, 14602, 15467, 16384
};


/**
 * pcm_volume_db_to_linear - Convert dB to linear gain
 * @db_value: Volume in hundredths of dB (e.g., -1250 = -12.50dB)
 *
 * Returns: Linear gain in Q14 format (16384 = 1.0)
 */
int32_t pcm_volume_db_to_linear(int db_value)
{
	int index;

	/* Mute */
	if (db_value <= PCM_VOLUME_DB_MUTE)
		return 0;

	/* 0dB or above */
	if (db_value >= PCM_VOLUME_DB_MAX)
		return PCM_VOLUME_Q14_UNITY;

	/* Clamp to valid range */
	if (db_value < PCM_VOLUME_DB_MIN)
		db_value = PCM_VOLUME_DB_MIN;

	/* Convert dB to table index */
	index = (db_value - PCM_VOLUME_DB_MIN) / PCM_VOLUME_DB_STEP;

	if (index < 0)
		index = 0;
	if (index >= PCM_VOLUME_TABLE_SIZE)
		index = PCM_VOLUME_TABLE_SIZE - 1;

	return db_to_linear_table[index];
}
EXPORT_SYMBOL_GPL(pcm_volume_db_to_linear);

/**
 * pcm_volume_apply_s16_db - Apply volume to S16_LE PCM data using dB
 */
void pcm_volume_apply_s16_db(int16_t *dst, const int16_t *src,
			     size_t frames, int channels, int db_value)
{
	int32_t gain = pcm_volume_db_to_linear(db_value);
	size_t i, total_samples = frames * channels;
	int32_t sample;

	if (!dst || !src || frames == 0 || channels == 0)
		return;

	/* Mute */
	if (gain == 0) {
		memset(dst, 0, total_samples * sizeof(int16_t));
		return;
	}

	/* Unity gain (0dB) */
	if (gain == PCM_VOLUME_Q14_UNITY) {
		if (dst != src)
			memcpy(dst, src, total_samples * sizeof(int16_t));
		return;
	}

	/* Apply gain (Q14 fixed-point) */
	for (i = 0; i < total_samples; i++) {
		sample = ((int32_t)src[i] * gain) >> PCM_VOLUME_Q14_SHIFT;

		/* Clamp */
		if (sample > PCM_SAMPLE_MAX_S16)
			sample = PCM_SAMPLE_MAX_S16;
		else if (sample < PCM_SAMPLE_MIN_S16)
			sample = PCM_SAMPLE_MIN_S16;

		dst[i] = (int16_t)sample;
	}
}
EXPORT_SYMBOL_GPL(pcm_volume_apply_s16_db);

/**
 * pcm_volume_apply_s24_db - Apply volume to S24_LE PCM data using dB
 */
void pcm_volume_apply_s24_db(int32_t *dst, const int32_t *src,
			     size_t frames, int channels, int db_value)
{
	int32_t gain = pcm_volume_db_to_linear(db_value);
	size_t i, total_samples = frames * channels;
	int64_t sample;

	if (!dst || !src || frames == 0 || channels == 0)
		return;

	/* Mute */
	if (gain == 0) {
		memset(dst, 0, total_samples * sizeof(int32_t));
		return;
	}

	/* Unity gain (0dB) */
	if (gain == PCM_VOLUME_Q14_UNITY) {
		if (dst != src)
			memcpy(dst, src, total_samples * sizeof(int32_t));
		return;
	}

	/* Apply gain (Q14 fixed-point) */
	for (i = 0; i < total_samples; i++) {
		/* Sign-extend 24-bit to 32-bit */
		sample = src[i];
		if (sample & 0x800000)
			sample |= 0xFF000000;

		sample = (sample * gain) >> PCM_VOLUME_Q14_SHIFT;

		/* Clamp to 24-bit */
		if (sample > PCM_SAMPLE_MAX_S24)
			sample = PCM_SAMPLE_MAX_S24;
		else if (sample < PCM_SAMPLE_MIN_S24)
			sample = PCM_SAMPLE_MIN_S24;

		dst[i] = (int32_t)(sample & 0xFFFFFF);
	}
}
EXPORT_SYMBOL_GPL(pcm_volume_apply_s24_db);

/**
 * pcm_volume_apply_s32_db - Apply volume to S32_LE PCM data using dB
 */
void pcm_volume_apply_s32_db(int32_t *dst, const int32_t *src,
			     size_t frames, int channels, int db_value)
{
	int32_t gain = pcm_volume_db_to_linear(db_value);
	size_t i, total_samples = frames * channels;
	int64_t sample;

	if (!dst || !src || frames == 0 || channels == 0)
		return;

	/* Mute */
	if (gain == 0) {
		memset(dst, 0, total_samples * sizeof(int32_t));
		return;
	}

	/* Unity gain (0dB) */
	if (gain == PCM_VOLUME_Q14_UNITY) {
		if (dst != src)
			memcpy(dst, src, total_samples * sizeof(int32_t));
		return;
	}

	/* Apply gain (Q14 fixed-point) */
	for (i = 0; i < total_samples; i++) {
		sample = ((int64_t)src[i] * gain) >> PCM_VOLUME_Q14_SHIFT;

		/* Clamp to 32-bit */
		if (sample > PCM_SAMPLE_MAX_S32)
			sample = PCM_SAMPLE_MAX_S32;
		else if (sample < PCM_SAMPLE_MIN_S32)
			sample = PCM_SAMPLE_MIN_S32;

		dst[i] = (int32_t)sample;
	}
}
EXPORT_SYMBOL_GPL(pcm_volume_apply_s32_db);

MODULE_DESCRIPTION("PCM Volume Control Module - dB based");
MODULE_LICENSE("GPL v2");
