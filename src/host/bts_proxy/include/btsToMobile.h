/*
 * File: btsToMobile.h
 * Date: 1.4.2014
 * Autor: Bc. Miroslav Babják, xbabjak@fiit.stuba.sk
 * Project: Emulator of GPRS modem
 * Description: Handling messages from BTS
 */

#include <l1ctl_proto.h>

/** Receive and switch messages from BTS **/
void switch_BTS_messages(int socket_l2, unsigned char *msg, int length);
