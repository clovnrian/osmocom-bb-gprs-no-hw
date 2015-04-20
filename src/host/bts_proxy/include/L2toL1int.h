/*
 * File: L2toL1int.h
 * Date: 10.11.2013
 * Autor: Bc. Miroslav Babják, xbabjak@fiit.stuba.sk
 * Project: Emulator of GPRS modem
 * Description: Handling messages from L2
 */

#include <l1ctl_proto.h>

#define L3_MSG_HEAD 4
#define L3_MSG_DATA 256
#define L3_MSG_SIZE (L3_MSG_HEAD + sizeof(struct l1ctl_hdr) + L3_MSG_DATA)

/** write message to BTS socket **/
void write_to_L1(int socket_bts);

/** Receive and switch messages from L2 **/
void switch_L2_messages(int socket_l2, uint8_t *msg);


