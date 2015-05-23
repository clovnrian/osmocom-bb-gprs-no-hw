/*
 * File: btsToMobile.c
 * Date: 1.4.2014
 * Autor: Bc. Miroslav Babják, xbabjak@fiit.stuba.sk
 * Project: Emulator of GPRS modem
 * Description: Handling messages from BTS
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <l1ctl_proto.h>
#include <L1toL2int.h>
#include <osmocom/core/gsmtap.h>

/** Receive and switch messages from BTS **/
void switch_BTS_messages(int socket_l2, unsigned char *msg, int length){
	struct gsmtap_hdr *gsmtapHeader = malloc(sizeof(struct gsmtap_hdr));
	uint8_t channelType = chantype_gsmtap2rsl(gsmtapHeader->sub_type, 0);

	/* fill the gsmtap header from incoming msg */
	memcpy(gsmtapHeader, msg, sizeof(struct gsmtap_hdr));

	l1_to_l2_data_ind(socket_l2, msg, length, gsmtapHeader);

	free(gsmtapHeader);

}
