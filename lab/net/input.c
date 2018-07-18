#include "ns.h"

#define BUF_SIZE 2048

extern union Nsipc nsipcbuf;

void
input(envid_t ns_envid)
{
	binaryname = "ns_input";

	// LAB 6: Your code here:
	// 	- read a packet from the device driver
	//	- send it to the network server
	// Hint: When you IPC a page to the network server, it will be
	// reading from it for a while, so don't immediately receive
	// another packet in to the same physical page.
	struct jif_pkt* pkt = (struct jif_pkt*)&nsipcbuf;
	int r;

	r = sys_page_alloc(0, pkt, PTE_U|PTE_W|PTE_P);
	if (r < 0) {
		panic("sys_page_alloc: %e", r);
	}

	while (true) {
		r = sys_recv_packet(pkt->jp_data, BUF_SIZE);
		if (r < 0) {
			if (r != -E_RX_NO_BUF) {
				panic("sys_recv_packet: %e", r);
			}
			sys_yield();
			continue;
		}
		pkt->jp_len = r;

		ipc_send(ns_envid, NSREQ_INPUT, pkt, PTE_P|PTE_W|PTE_U);

		r = sys_page_unmap(0, (void*)pkt);
		if (r < 0) {
			panic("sys_page_unmap: %e", r);
		}
		r = sys_page_alloc(0, pkt, PTE_U|PTE_W|PTE_P);
		if (r < 0) {
			panic("sys_page_alloc: %e", r);
		}
	}
}
