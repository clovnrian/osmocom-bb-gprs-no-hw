/* L1CTL protocol for GPRS on phone */

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

#include <l1ctl_proto.h>

#include <osmocom/core/msgb.h>

#include <osmocom/bb/common/l1l2_interface.h>
#include <osmocom/bb/gprs/gprs_l1ctl.h>

/** handle received GPRS data */
int rx_ph_gprs_data_ind(struct msgb *msg){

	return 1;
}
