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

void e1000_init();

int e1000_send_packet(void* packet, size_t len);

#endif	// JOS_KERN_E1000_H
