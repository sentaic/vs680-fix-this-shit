/* SPDX-License-Identifier: GPL-2.0 */
#ifndef VS680_RX_AVI_H
#define VS680_RX_AVI_H
#ifdef __KERNEL__
#include <linux/errno.h>
#else
#include <errno.h>
#endif

/* AVI 类型为 0x82；PH 包含版本和长度，PB 按小端排列，首字节为收到的校验和。 */
static inline int loop_rx_avi_validate(unsigned int header,
                                        const unsigned int payload[4])
{
    unsigned int version = header & 255;
    unsigned int length = (header >> 8) & 255;
    unsigned int sum, i;
    if (!payload) return -EINVAL;
    if (version != 2) return -EOPNOTSUPP;
    if (length != 13) return -EPROTO;
    sum = 0x82 + version + length;
    for (i = 0; i <= length; i++)
        sum += (payload[i / 4] >> ((i % 4) * 8)) & 255;
    return (sum & 255) ? -EBADMSG : 0;
}

/* Caller owns RX configuration and establishes a fresh AVI event separately.
 * This validates stable packet bytes; it neither starts DMA nor reports stop. */
struct loop_rx_avi_packet {
    unsigned int header;
    unsigned int payload[4];
};
int loop_rx_avi_read_packet(struct loop_rx_avi_packet *packet);
int loop_rx_avi_read_valid(void);
#endif
