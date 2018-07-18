#ifndef JOS_KERN_E1000_H
#define JOS_KERN_E1000_H

#include <inc/types.h>

#define PCI_VENDOR_E1000 0x8086
#define PCI_PRODUCT_E1000 0x100e

// Divided by 4 for use as uint32_t[] indices
#define E1000_TDBAL     (0x03800/4)  /* TX Descriptor Base Address Low - RW */
#define E1000_TDBAH     (0x03804/4)  /* TX Descriptor Base Address High - RW */
#define E1000_TDLEN     (0x03808/4)  /* TX Descriptor Length - RW */
#define E1000_TDH       (0x03810/4)  /* TX Descriptor Head - RW */
#define E1000_TDT       (0x03818/4)  /* TX Descripotr Tail - RW */

#define E1000_TCTL      (0x00400/4)  /* TX Control - RW */
#define E1000_TCTL_EN   (1 << 1)    /* enable tx */
#define E1000_TCTL_PSP  (1 << 3)    /* pad short packets */
#define E1000_TCTL_CT   (0x10 << 4)    /* collision threshold */
#define E1000_TCTL_COLD (0x40 << 12)    /* collision distance */

#define E1000_TIPG      (0x00410/4)  /* TX Inter-packet gap -RW */
#define E1000_TIPG_IPGT (10)
#define E1000_TIPG_IPGR1    (4 << 10) 
#define E1000_TIPG_IPGR2    (6 << 20)

#define E1000_TXD_CMD_EOP    0x01 /* End of Packet */
#define E1000_TXD_CMD_RS     0x08 /* Report Status */
#define E1000_TXD_STAT_DD    0x01 /* Descriptor Done */

#define E1000_MTA       (0x05200/4)  /* Multicast Table Array - RW Array */

#define E1000_RDBAL     (0x02800/4)  /* RX Descriptor Base Address Low - RW */
#define E1000_RDBAH     (0x02804/4)  /* RX Descriptor Base Address High - RW */
#define E1000_RDLEN     (0x02808/4)  /* RX Descriptor Length - RW */
#define E1000_RDH       (0x02810/4)  /* RX Descriptor Head - RW */
#define E1000_RDT       (0x02818/4)  /* RX Descriptor Tail - RW */
#define E1000_RAL       (0x05400/4)  /* Receive Address Low */ 
#define E1000_RAH       (0x05404/4)  /* Receive Address High */
#define E1000_RAH_AV    (1 << 31)    /* Receive Address High address valid bit */

#define E1000_RCTL      (0x00100/4)  /* RX Control - RW */
#define E1000_RCTL_EN       0x00000002    /* enable */
#define E1000_RCTL_LBM_NO   0x00000000    /* no loopback mode */
#define E1000_RCTL_BAM      0x00008000    /* broadcast enable */
#define E1000_RCTL_SZ_2048  0x00000000    /* rx buffer size 2048 */
#define E1000_RCTL_SECRC    0x04000000    /* Strip Ethernet CRC */

#define E1000_RXD_STAT_DD       0x01    /* Descriptor Done */
#define E1000_RXD_STAT_EOP      0x02    /* End of Packet */

void e1000_init();

int e1000_send_packet(void* packet, size_t len);

int e1000_recv_packet(void* packet, size_t len);

#endif	// JOS_KERN_E1000_H
