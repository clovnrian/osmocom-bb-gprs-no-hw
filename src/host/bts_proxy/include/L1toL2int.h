/*
 * File: L2toL1int.h
 * Date: 10.11.2013
 * Autor: Bc. Miroslav Babják, xbabjak@fiit.stuba.sk
 * Project: Emulator of GPRS modem
 * Description: Handling messages from L1
 */

#include <l1ctl_proto.h>
#include <osmocom/core/gsmtap.h>
#include <osmocom/core/msgb.h>


/** transmit L1CTL_RESET_CONF message **/
void l1_to_l2_reset_conf(int socket_l2, struct l1ctl_reset *reset_req);

/** transmit L1CTL_SIM_CONF message **/
void l1_to_l2_sim_conf(int socket_l2);

/** transmit L1CTL_PM_CONF message **/
void l1_to_l2_pm_conf(int socket_l2, struct l1ctl_pm_req *pm_req);

/** transmit L1CTL_FBSB_CONF message **/
void l1_to_l2_fbsb_conf(int socket_l2, struct l1ctl_fbsb_req *fbsb_req);

/** transmit L1CTL_CCCH_MODE_CONF message **/
void l1_to_l2_ccch_mode_conf(int socket_l2, struct l1ctl_ccch_mode_req *ccch_req);

/** Fill info_dl structure with data **/
struct l1ctl_info_dl *fill_info_dl_structure(struct gsmtap_hdr *gsmtapHeader, struct msgb *msg);

/* transmit L1CTL_DATA_IND message */
void l1_to_l2_data_ind(int socket_l2, unsigned char *data, int len, struct gsmtap_hdr *gsmtapHeader);

/** alloc L1CTL to msgb structure **/
struct msgb *l1ctl_msgb_alloc(uint8_t msg_type, uint8_t flag);

/** convert GSMTAP channel number to RSL channel type **/
uint8_t chantype_gsmtap2rsl(uint8_t gt_chantype, uint8_t link_id);

/** transmit L1CTL_NEIGH_PM_CONF message **/
void l1_to_l2_neight_pm_conf(int socket_l2, struct l1ctl_neigh_pm_req *pm_req);
