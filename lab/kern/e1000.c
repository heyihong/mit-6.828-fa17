#include <inc/assert.h>
#include <inc/string.h>
#include <inc/error.h>
#include <kern/e1000.h>
#include <kern/pmap.h>

// LAB 6: Your driver code here

#define TX_DESCRIPTOR_LEN 32 /* Must be multiple of 8*/
#define ETHERNET_MAX_SIZE 1518

struct tx_desc
{
	uint64_t addr;
	uint16_t length;
	uint8_t cso;
	uint8_t cmd;
	uint8_t status;
	uint8_t css;
	uint16_t special;
};

volatile uint32_t *e1000 = NULL;

struct tx_desc tx_descs[TX_DESCRIPTOR_LEN];
char pkt_bufs[TX_DESCRIPTOR_LEN][ETHERNET_MAX_SIZE];
uint32_t tx_desc_tail;

static int prev_idx(int idx) {
    return (idx + TX_DESCRIPTOR_LEN - 1) % TX_DESCRIPTOR_LEN;
}

static int next_idx(int idx) {
    return (idx + 1) % TX_DESCRIPTOR_LEN;
}

int e1000_send_packet(void* packet, size_t len) {
    if (len == 0) {
        return 0;
    }
    if (TX_DESCRIPTOR_LEN * ETHERNET_MAX_SIZE < len) {
        return -E_TOO_LARGE;
    }

    int num_bufs = (len + ETHERNET_MAX_SIZE - 1) / ETHERNET_MAX_SIZE;
    int next_tail = (tx_desc_tail + num_bufs) % TX_DESCRIPTOR_LEN;
    if (!(tx_descs[prev_idx(next_tail)].status & E1000_TXD_STAT_DD)) {
        return -E_NO_BUF;
    }

    struct tx_desc *desc;
    void *buf;
    size_t desc_len;
    do {
        desc = tx_descs + tx_desc_tail;
        assert(desc->status & E1000_TXD_STAT_DD);
        buf = pkt_bufs + tx_desc_tail;
        desc_len = len < ETHERNET_MAX_SIZE ? len : ETHERNET_MAX_SIZE;
        memcpy(buf, packet, desc_len); 
        packet += desc_len;
        len -= desc_len;

        desc->cmd = E1000_TXD_CMD_RS;
        desc->status = 0;
        desc->addr = PADDR(buf);
        desc->length = desc_len;

        tx_desc_tail = next_idx(tx_desc_tail);
    } while(tx_desc_tail != next_tail);

    tx_descs[prev_idx(tx_desc_tail)].cmd |= E1000_TXD_CMD_EOP;

    e1000[E1000_TDT] = tx_desc_tail;
    return 0;
}

void e1000_init() {
    uint32_t i;
    for (i = 0; i < TX_DESCRIPTOR_LEN; i++) {
        tx_descs[i].cmd = 0;
        tx_descs[i].status = E1000_TXD_STAT_DD;
    }

    assert(e1000 != NULL);
    e1000[E1000_TDBAL] = PADDR(tx_descs);
    e1000[E1000_TDLEN] = TX_DESCRIPTOR_LEN * sizeof(struct tx_desc);
    assert(e1000[E1000_TDLEN] % 128 == 0);

    e1000[E1000_TDH] = 0;
    e1000[E1000_TDT] = tx_desc_tail = 0;

    e1000[E1000_TCTL] = E1000_TCTL_EN | E1000_TCTL_PSP | E1000_TCTL_CT | E1000_TCTL_COLD;
    e1000[E1000_TIPG] = E1000_TIPG_IPGT | E1000_TIPG_IPGR1 | E1000_TIPG_IPGR2;
}