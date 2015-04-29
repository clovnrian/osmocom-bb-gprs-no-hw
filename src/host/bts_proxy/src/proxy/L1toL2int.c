/*
 * File: L1toL2int.c
 * Date: 10.11.2013
 * Autor: Bc. Miroslav Babják, xbabjak@fiit.stuba.sk
 * Project: Emulator of GPRS modem
 * Description: Handling messages from L1
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <poll.h>
#include <l1ctl_proto.h>
#include <layer1.h>
#include <L2toL1int.h>
#include <L1toL2int.h>
#include <osmocom/core/gsmtap.h>
#include <osmocom/gsm/rsl.h>

uint16_t globalArfcn;		/* ARFCN (frequency) for actual BTS*/

/** write message to L2 socket **/
void write_to_L2(int socket_l2, struct msgb *msg){
	uint16_t *len;

	if(msg->l1h != msg->data)
		printf("msg l1 header != msg data");

	len = (uint16_t *) msgb_push(msg,sizeof(*len));
	*len = htons(msg->len - sizeof(*len));

	/** send data to L2 **/
	write(socket_l2,msg->data,msg->len);

	msgb_free(msg);
}

/** transmit L1CTL_RESET_CONF message **/
void l1_to_l2_reset_conf(int socket_l2, struct l1ctl_reset *reset_req){

	struct msgb *msg = l1ctl_msgb_alloc(L1CTL_RESET_CONF,0);
	struct l1ctl_reset *reset_resp;

	reset_resp = (struct l1ctl_reset *) msgb_put(msg, sizeof(*reset_resp));
	reset_resp->type = reset_req->type;
	reset_resp->pad[0] = 0;
   	reset_resp->pad[1] = 0;
   	reset_resp->pad[2] = 0;

	write_to_L2(socket_l2, msg);
}

/** transmit L1CTL_SIM_CONF message **/
void l1_to_l2_sim_conf(int socket_l2){
	struct l1ctl_hdr *l1h;
	uint8_t *response;
	struct msgb *msg;

	/* allocate space for expected response */
	msg = msgb_alloc_headroom(256, L3_MSG_HEAD + sizeof(struct l1ctl_hdr), "l1ctl1");
	response = msgb_put(msg, 0 + 2 + 1);

	msgb_pull(msg, msg->len - 2);
	msg->data[0] = 0;
	msg->data[1] = 0;

	l1h = (struct l1ctl_hdr *) msgb_push(msg, sizeof(*l1h));
	l1h->msg_type = L1CTL_SIM_CONF;
	l1h->flags = 0;
	msg->l1h = (uint8_t *)l1h;

	write_to_L2(socket_l2, msg);
}

/** transmit L1CTL_PM_CONF message **/
void l1_to_l2_pm_conf(int socket_l2, struct l1ctl_pm_req *pm_req){
	struct msgb *msg;
	struct l1ctl_pm_conf *pm_resp;
	srand(time(NULL));
	int i;
	uint8_t flag;

	if(ntohs(pm_req->range.band_arfcn_from) < 512 || ntohs(pm_req->range.band_arfcn_from) > 885)
		flag = ntohs(pm_req->range.band_arfcn_to) - ntohs(pm_req->range.band_arfcn_from) + 1;
	else flag = ntohs(pm_req->range.band_arfcn_to) - ntohs(pm_req->range.band_arfcn_from);
	
	msg = l1ctl_msgb_alloc(L1CTL_PM_CONF,flag);

	for(i = ntohs(pm_req->range.band_arfcn_from); i <= ntohs(pm_req->range.band_arfcn_from) + 49;i++){
		if(i >= ntohs(pm_req->range.band_arfcn_to) + 1)
			break;

		pm_resp = (struct l1ctl_pm_conf *) msgb_put(msg, sizeof(*pm_resp));

		pm_resp->band_arfcn = htons((uint16_t) i);
	  	pm_resp->pm[0] = (i == globalArfcn) ? 10 : 0;
	  	pm_resp->pm[1] = 0;
	}


	write_to_L2(socket_l2, msg);
}

/** transmit L1CTL_FBSB_CONF message **/
void l1_to_l2_fbsb_conf(int socket_l2, struct l1ctl_fbsb_req *fbsb_req){
	struct msgb *msg = l1ctl_msgb_alloc(L1CTL_FBSB_CONF,0);
	struct l1ctl_fbsb_conf *fbsb_resp;
	struct l1ctl_info_dl *info_dl = (struct l1ctl_info_dl *) msgb_put(msg, sizeof(*info_dl));
	int arfcn = ntohs(fbsb_req->band_arfcn);

	srand(time(NULL));

	info_dl->band_arfcn = fbsb_req->band_arfcn;
	info_dl->frame_nr = rand() % UINT32_MAX;
	info_dl->snr = 0;

	fbsb_resp = (struct l1ctl_fbsb_conf *) msgb_put(msg, sizeof(*fbsb_resp));

	fbsb_resp->initial_freq_err = htons((uint16_t) 0);
	fbsb_resp->result = (uint8_t) 255;
	fbsb_resp->bsic = (uint8_t) 0;
	
	if(arfcn == globalArfcn)
		fbsb_resp->result = (uint8_t) 0;

	write_to_L2(socket_l2, msg);
}

/** transmit L1CTL_CCCH_MODE_CONF message **/
void l1_to_l2_ccch_mode_conf(int socket_l2, struct l1ctl_info_dl *info_dl){
	struct msgb *msg = l1ctl_msgb_alloc(L1CTL_CCCH_MODE_CONF,0);
	struct l1ctl_ccch_mode_conf *ccch_conf;
	struct l1ctl_ccch_mode_req *ccch_req = info_dl->payload;

	ccch_conf = (struct l1ctl_ccch_mode_conf *) msgb_put(msg, sizeof(*ccch_conf));
	
	ccch_conf->ccch_mode = ccch_req->ccch_mode;
	ccch_conf->padding[0] = 0;
	ccch_conf->padding[1] = 0;
	ccch_conf->padding[2] = 0;

	write_to_L2(socket_l2, msg);
}

/* transmit L1CTL_DATA_IND message */
void l1_to_l2_data_ind(int socket_l2, unsigned char *data, int len){
	struct msgb *msg = l1ctl_msgb_alloc(L1CTL_DATA_IND, 0);
	struct l1ctl_data_ind *data_ind;
	struct l1ctl_info_dl *info_dl;
	struct gsmtap_hdr *gsmtapHeader = malloc(sizeof(struct gsmtap_hdr));

	/* fill the gsmtap header from incoming msg */
	memcpy(gsmtapHeader, data, sizeof(struct gsmtap_hdr));

	/* set global ARFCN */
	globalArfcn = ntohs(gsmtapHeader->arfcn);
	prinf("GLOBAL ARFCN SET TO: %d\n", globalArfcn);
	info_dl = fill_info_dl_structure(gsmtapHeader, msg);
	data_ind = (struct l1ctl_data_ind *) msgb_put(msg, sizeof(struct l1ctl_data_ind));

	memcpy(&data_ind->data, data + sizeof(struct gsmtap_hdr), len - sizeof(struct gsmtap_hdr));

	write_to_L2(socket_l2, msg);

	free(gsmtapHeader);
}

/** Alloc L1CTL message **/
struct msgb *l1ctl_msgb_alloc(uint8_t msg_type, uint8_t flag){
	struct msgb *msg;
	struct l1ctl_hdr *l1h;

	msg = msgb_alloc_headroom(L3_MSG_SIZE, L3_MSG_HEAD, "osmo_l1");
	if (!msg) {
		while (1) {
			puts("OOPS. Out of buffers...\n");
		}

		return NULL;
	}
	
	msg->l1h = msgb_put(msg, sizeof(*l1h));
	l1h = (struct l1ctl_hdr *) msg->l1h;
	l1h->msg_type = msg_type;
	l1h->flags = flag;

	return msg;
}

struct l1ctl_info_dl *fill_info_dl_structure(struct gsmtap_hdr *gsmtapHeader, struct msgb *msg){

	struct l1ctl_info_dl *info_dl = (struct l1ctl_info_dl *) msgb_put(msg, sizeof(struct l1ctl_info_dl));
	uint8_t parentChan = gsmtapHeader->sub_type & 0xF0;


	info_dl->link_id = parentChan == GSMTAP_CHANNEL_ACCH ? 0x40 : 0x0;
	info_dl->chan_nr = chantype_gsmtap2rsl(gsmtapHeader->sub_type, 0);
	info_dl->band_arfcn = gsmtapHeader->arfcn;
	info_dl->frame_nr = gsmtapHeader->frame_number;
	info_dl->rx_level = gsmtapHeader->signal_dbm;
	info_dl->snr = gsmtapHeader->snr_db;
	info_dl->num_biterr = (uint8_t) 0;
	info_dl->fire_crc = (uint8_t) 0;

	return info_dl;
}

/* convert GSMTAP channel number to RSL channel type
 *  \param[in] gt_chantype GSMTAP channel type
 *  \param[in] link_id RSL link identifier
 *  \returns RSL channel type
 */
uint8_t chantype_gsmtap2rsl(uint8_t gt_chantype, uint8_t link_id)
{
	uint8_t ret = 0;

	//use only lowest 4bits for channel type
	gt_chantype &= 0x0F;

	switch (gt_chantype) {
	case GSMTAP_CHANNEL_TCH_F:
		ret = RSL_CHAN_Bm_ACCHs;
		break;
	case GSMTAP_CHANNEL_TCH_H:
		ret = RSL_CHAN_Lm_ACCHs;
		break;
	case GSMTAP_CHANNEL_SDCCH4:
		ret = RSL_CHAN_SDCCH4_ACCH;
		break;
	case GSMTAP_CHANNEL_SDCCH8:
		ret = RSL_CHAN_SDCCH8_ACCH;
		break;
	case GSMTAP_CHANNEL_BCCH:
		ret = RSL_CHAN_BCCH;
		break;
	case GSMTAP_CHANNEL_RACH:
		ret = RSL_CHAN_RACH;
		break;
	case GSMTAP_CHANNEL_PCH:
		/* it could also be AGCH... */
		ret = RSL_CHAN_PCH_AGCH;
		break;
	}

	/*if (link_id & 0x40)
		ret |= GSMTAP_CHANNEL_ACCH;*/

	return ret;
}

