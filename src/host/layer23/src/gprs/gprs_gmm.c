/* GMM layer of GPRS on phone */

/* (C) 2015 by Miroslav Babjak
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <osmocom/core/msgb.h>
#include <osmocom/bb/common/osmocom_data.h>
#include <osmocom/bb/common/logging.h>
#include <osmocom/bb/gprs/gprs_gmm.h>
#include <osmocom/gsm/protocol/gsm_04_08.h>
#include <osmocom/bb/gprs/gprs_llc.h>

/** Send channel request to network */
int gmm_tx_channel_request(struct osmocom_ms *ms){
	struct msgb *msg = gsm48_msgb_alloc();
	struct gsm48_hdr *gh;

	gh->proto_discr = GSM48_PDISC_MM_GPRS;
	gh->msg_type = GSM48_MT_GMM_ATTACH_REQ;
	gh->data[0] = 0x70;

	return gprs_gmm_sendmsg(msg);
}

/* Send a message through the underlying layer */
int gprs_gmm_sendmsg(struct msgb *msg)
{
	/* caller needs to provide TLLI, BVCI and NSEI */
	return gprs_llc_tx_ui(msg, GPRS_SAPI_GMM);
}

/* initialize GPRS Mobility Management process */
int gsm48_gmm_init(struct osmocom_ms *ms)
{
	struct gprs_gmmlayer *gprsGmm = &ms->gprsGmmLayer;

	memset(gprsGmm, 0, sizeof(*gprsGmm));
	gprsGmm->ms = ms;

	LOGP(DMM, LOGL_INFO, "init GPRS Mobility Management process\n");
}
