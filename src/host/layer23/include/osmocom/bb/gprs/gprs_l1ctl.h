#ifndef osmocom_gprs_l1ctl_h
#define osmocom_gprs_l1ctl_h

#include <osmocom/core/msgb.h>
#include <osmocom/bb/common/osmocom_data.h>

/** handle received GPRS data */
int rx_ph_gprs_data_ind(struct osmocom_ms *ms, struct msgb *msg);

#endif
