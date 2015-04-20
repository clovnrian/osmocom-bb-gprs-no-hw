/*
 * File: L2toL1int.c
 * Date: 10.11.2013
 * Autor: Bc. Miroslav Babják, xbabjak@fiit.stuba.sk
 * Project: Emulator of GPRS modem
 * Description: Handling messages from L2
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <l1ctl_proto.h>
#include <layer1.h>
#include <L2toL1int.h>
#include <L1toL2int.h>
#include <osmocom/core/msgb.h>

/** Receive and switch messages from L2 **/
void switch_L2_messages(int socket_l2, uint8_t *msg){
	struct l1ctl_hdr *hdr;

				hdr = (struct l1ctl_hdr *) msg;
			
				/** message switch **/
				switch(hdr->msg_type){

					case L1CTL_RESET_REQ:
						l1_to_l2_reset_conf(socket_l2,(struct l1ctl_reset *)hdr->data);
						break;
					case L1CTL_SIM_REQ:
						l1_to_l2_sim_conf(socket_l2);
						break;
					case L1CTL_PM_REQ:
						l1_to_l2_pm_conf(socket_l2, (struct l1ctl_pm_req *)hdr->data);
						break;
					case L1CTL_FBSB_REQ:
						l1_to_l2_fbsb_conf(socket_l2, (struct ll1ctl_fbsb_req *)hdr->data);
						break;
					case L1CTL_CCCH_MODE_REQ:
						l1_to_l2_ccch_mode_conf(socket_l2, (struct l1ctl_info_dl *)hdr->data);
						break;
					default:
					  printf("Unhandled message type number %u\n",hdr->msg_type);
					  break;
				}

			free(msg);
}


