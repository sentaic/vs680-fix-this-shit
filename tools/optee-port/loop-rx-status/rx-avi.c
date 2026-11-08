// SPDX-License-Identifier: GPL-2.0
#include <linux/string.h>
#include "rx-phy-context.h"
#include "rx-avi.h"

int loop_rx_avi_read_packet(struct loop_rx_avi_packet *packet)
{
    u32 header[2], payload[2][4];
    unsigned int sample, word;
    int result;
    if (!packet) return -EINVAL;
    /* The header read latches the corresponding payload, as in hrx-hdmi.c.
     * Two samples reject a packet replacement while taking this snapshot. */
    for (sample = 0; sample < 2; sample++) {
        header[sample] = hrx_reg_read(&loop_receiver, HDMI_PKTDEC_AVIIF_PH);
        for (word = 0; word < 4; word++)
            payload[sample][word] = hrx_reg_read(&loop_receiver,
                                               HDMI_PKTDEC_AVIIF_PB(word));
    }
    /* Ignore bytes outside the PH version/length and 14 received PB bytes. */
    for (sample = 0; sample < 2; sample++) {
        header[sample] &= 0xffff;
        payload[sample][3] &= 0xffff;
    }
    if (header[0] != header[1] ||
        memcmp(payload[0], payload[1], sizeof(payload[0]))) return -EAGAIN;
    result = loop_rx_avi_validate(header[1], payload[1]);
    if (result) return result;
    packet->header = header[1];
    memcpy(packet->payload, payload[1], sizeof(packet->payload));
    return 0;
}

int loop_rx_avi_read_valid(void)
{
    struct loop_rx_avi_packet packet;
    return loop_rx_avi_read_packet(&packet);
}
