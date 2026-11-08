/* SPDX-License-Identifier: GPL-2.0 */
#ifndef VS680_RX_TRANSACTION_H
#define VS680_RX_TRANSACTION_H
#include <linux/types.h>
/* 获取公共寄存器互斥锁，调用后由调用方解锁；事务持有期间返回 EACCES。 */
int loop_rx_register_acquire(void);
void loop_rx_register_release(void);
int loop_rx_transaction_prelink(void);
int loop_rx_transaction_prelink_rate(bool rate6g);
int loop_rx_transaction_begin(u64 attempt);
int loop_rx_transaction_poll(u64 attempt);
int loop_rx_transaction_release(u64 attempt);
ssize_t loop_rx_transaction_status(char *buffer);
/* 取消失败的配置事务并保留失败记录。 */
int loop_rx_maintenance_init(void);
void loop_rx_maintenance_detach(void);
int loop_rx_discard_failed(u64 generation, u64 attempt);
ssize_t loop_rx_maintenance_status(char *buffer);
struct kobject;
int loop_rx_transaction_attach(struct kobject *object);
#endif
