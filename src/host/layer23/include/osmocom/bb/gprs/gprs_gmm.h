#ifndef osmocom_gprs_l1ctl_h
#define osmocom_gprs_l1ctl_h

#include <osmocom/core/msgb.h>
#include <osmocom/bb/common/osmocom_data.h>

/* GMM STATES */
#define GPRS_GMM_ST_IDLE 0x01;
#define GPRS_GMM_ST_READY 0x02;
#define GPRS_GMM_ST_STANDBY 0x03;

/* Table 10.4 / 10.4a, GPRS Mobility Management (GMM) */
#define GSM48_MT_GMM_ATTACH_REQ		0x01
#define GSM48_MT_GMM_ATTACH_ACK		0x02
#define GSM48_MT_GMM_ATTACH_COMPL	0x03
#define GSM48_MT_GMM_ATTACH_REJ		0x04
#define GSM48_MT_GMM_DETACH_REQ		0x05
#define GSM48_MT_GMM_DETACH_ACK		0x06

#define GSM48_MT_GMM_RA_UPD_REQ		0x08
#define GSM48_MT_GMM_RA_UPD_ACK		0x09
#define GSM48_MT_GMM_RA_UPD_COMPL	0x0a
#define GSM48_MT_GMM_RA_UPD_REJ		0x0b

#define GSM48_MT_GMM_PTMSI_REALL_CMD	0x10
#define GSM48_MT_GMM_PTMSI_REALL_COMPL	0x11
#define GSM48_MT_GMM_AUTH_CIPH_REQ	0x12
#define GSM48_MT_GMM_AUTH_CIPH_RESP	0x13
#define GSM48_MT_GMM_AUTH_CIPH_REJ	0x14
#define GSM48_MT_GMM_ID_REQ		0x15
#define GSM48_MT_GMM_ID_RESP		0x16
#define GSM48_MT_GMM_STATUS		0x20
#define GSM48_MT_GMM_INFO		0x21

/* GMM sublayer instance */
struct gprs_gmmlayer {
	struct osmocom_ms	*ms;
	int	state;

	uint32_t		tlli;
	uint16_t		bvci;
	uint16_t		nsei;
};

#define GSM48_ALLOC_SIZE	2048
#define GSM48_ALLOC_HEADROOM	256

static inline struct msgb *gsm48_msgb_alloc(void)
{
	return msgb_alloc_headroom(GSM48_ALLOC_SIZE, GSM48_ALLOC_HEADROOM,
				   "GSM 04.08");
}


int gmm_tx_channel_request(struct osmocom_ms *ms);
int gprs_gmm_sendmsg(struct msgb *msg);

#endif
