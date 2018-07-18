#include <inc/lib.h>

#include "ns.h"

extern union Nsipc nsipcbuf;

void serve_output(struct jif_pkt* pkt) {
	int r;
	while ((r = sys_send_packet(pkt->jp_data, pkt->jp_len)) != 0) {
		if (r != -E_TX_NO_BUF) {
			panic("serve_output: %e", r);
		}
		sys_yield();
	}	
}

void
output(envid_t ns_envid)
{
	binaryname = "ns_output";

	// LAB 6: Your code here:
	// 	- read a packet from the network server
	//	- send the packet to the device driver
	uint32_t req, whom;
	int perm;

	while (true) {
		req = ipc_recv((int32_t*) &whom, &nsipcbuf, &perm);

		if (whom != ns_envid) {
			cprintf("Invalid request from %08x: not from network server\n",
				whom);
			continue;
		}	

		if (!(perm & PTE_P)) {
			cprintf("Invalid request from network server %08x: no argument page\n",
				whom);
			continue;
		}

		if (req != NSREQ_OUTPUT) {
			cprintf("Invalid request from network server %08x: unknown request %d", 
				whom, req);
			continue;
		}

		serve_output((struct jif_pkt*)&nsipcbuf);
	}
}
