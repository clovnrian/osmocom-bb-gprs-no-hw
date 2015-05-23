#ifndef osmocom_gprs_l1ctl_h
#define osmocom_gprs_l1ctl_h

#include <osmocom/core/msgb.h>

/** handle received GPRS data */
int rx_ph_gprs_data_ind(struct msgb *msg);

#endif
