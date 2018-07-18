#include <inc/assert.h>
#include <inc/string.h>
#include <inc/error.h>
#include <kern/e1000.h>
#include <kern/pmap.h>

// LAB 6: Your driver code here

#define TX_DESCRIPTOR_LEN 32 /* Must be multiple of 8*/
#define RX_DESCRIPTOR_LEN 128
#define ETHERNET_MAX_SIZE 1518
#define TX_BUF_SIZE ETHERNET_MAX_SIZE
#define RX_BUF_SIZE 2048

/* Transmit Descriptor */
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

/* Receive Descriptor */
struct rx_desc {
    uint64_t addr;      
    uint16_t length;     
    uint16_t csum;       
    uint8_t status;      
    uint8_t errors;      
    uint16_t special;
};

volatile uint32_t *e1000 = NULL;

struct tx_desc tx_descs[TX_DESCRIPTOR_LEN];
char tx_pkt_bufs[TX_DESCRIPTOR_LEN][TX_BUF_SIZE];

struct rx_desc rx_descs[RX_DESCRIPTOR_LEN];
char rx_pkt_bufs[RX_DESCRIPTOR_LEN][RX_BUF_SIZE];

static int tx_prev_idx(int idx) {
    return (idx + TX_DESCRIPTOR_LEN - 1) % TX_DESCRIPTOR_LEN;
}

static int tx_next_idx(int idx) {
    return (idx + 1) % TX_DESCRIPTOR_LEN;
}

static int rx_next_idx(int idx) {
    return (idx + 1) % RX_DESCRIPTOR_LEN;
}

int e1000_send_packet(void* packet, size_t len) {
    if (len == 0) {
        return 0;
    }
    if (TX_DESCRIPTOR_LEN * TX_BUF_SIZE < len) {
        return -E_TX_TOO_LARGE;
    }

    int tx_desc_tail = e1000[E1000_TDT];
    int num_bufs = (len + TX_BUF_SIZE - 1) / TX_BUF_SIZE;
    int next_tail = (tx_desc_tail + num_bufs) % TX_DESCRIPTOR_LEN;
    if (!(tx_descs[tx_prev_idx(next_tail)].status & E1000_TXD_STAT_DD)) {
        return -E_TX_NO_BUF;
    }

    struct tx_desc *desc;
    void *buf;
    size_t desc_len;
    do {
        desc = tx_descs + tx_desc_tail;
        assert(desc->status & E1000_TXD_STAT_DD);
        buf = tx_pkt_bufs + tx_desc_tail;
        desc_len = len < TX_BUF_SIZE ? len : TX_BUF_SIZE;
        memcpy(buf, packet, desc_len); 
        packet += desc_len;
        len -= desc_len;

        desc->cmd = E1000_TXD_CMD_RS;
        desc->status = 0;
        desc->addr = PADDR(buf);
        desc->length = desc_len;

        tx_desc_tail = tx_next_idx(tx_desc_tail);
    } while(tx_desc_tail != next_tail);

    tx_descs[tx_prev_idx(tx_desc_tail)].cmd |= E1000_TXD_CMD_EOP;

    e1000[E1000_TDT] = tx_desc_tail;
    return 0;
}

int e1000_recv_packet(void* packet, size_t len) {
    if (len < RX_BUF_SIZE) {
        return -E_RX_TOO_SMALL;
    }

    int next_idx = rx_next_idx(e1000[E1000_RDT]);
    struct rx_desc* desc = rx_descs + next_idx;
    if (!(desc->status & E1000_RXD_STAT_DD)) {
        return -E_RX_NO_BUF;
    }
    assert(desc->status & E1000_RXD_STAT_EOP);

    memcpy(packet, rx_pkt_bufs + next_idx, desc->length);
    desc->status = 0;

    e1000[E1000_RDT] = next_idx;
    return desc->length;
}

static void sender_init() {
    uint32_t i;
    for (i = 0; i < TX_DESCRIPTOR_LEN; i++) {
        tx_descs[i].cmd = 0;
        tx_descs[i].status = E1000_TXD_STAT_DD;
    }

    e1000[E1000_TDBAL] = PADDR(tx_descs);
    e1000[E1000_TDLEN] = TX_DESCRIPTOR_LEN * sizeof(struct tx_desc);
    assert(e1000[E1000_TDLEN] % 128 == 0);

    e1000[E1000_TDH] = 0;
    e1000[E1000_TDT] = 0;

    e1000[E1000_TCTL] = E1000_TCTL_EN | E1000_TCTL_PSP | E1000_TCTL_CT | E1000_TCTL_COLD;
    e1000[E1000_TIPG] = E1000_TIPG_IPGT | E1000_TIPG_IPGR1 | E1000_TIPG_IPGR2;
}

static void receiver_init() {
    uint32_t i;
    for (i = 0; i < RX_DESCRIPTOR_LEN; i++) {
        rx_descs[i].addr = PADDR(rx_pkt_bufs[i]);
        rx_descs[i].status = 0;
    }

    e1000[E1000_RAL] = 0x12005452;
    e1000[E1000_RAH] = 0x5634 | E1000_RAH_AV;

    e1000[E1000_MTA] = 0;

    e1000[E1000_RDBAL] = PADDR(rx_descs);
    e1000[E1000_RDLEN] = RX_DESCRIPTOR_LEN * sizeof(struct rx_desc); 
    assert(e1000[E1000_RDLEN] % 128 == 0);

    e1000[E1000_RDH] = 0;
    e1000[E1000_RDT] = RX_DESCRIPTOR_LEN - 1;

    // The buffer size should be larger than ETHERNET_MAX_SIZE so that
    // no need to handle multiple buffers for a packet.
    // Enabled receiver after software is ready to process received packets
    e1000[E1000_RCTL] = 
        E1000_RCTL_EN | E1000_RCTL_LBM_NO | E1000_RCTL_BAM |
        E1000_RCTL_SZ_2048 | E1000_RCTL_SECRC;
}

void e1000_init() {
    assert(e1000 != NULL);
    receiver_init();
    sender_init();
}