#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <l1ctl_proto.h>
#include <osmocom/core/gsmtap.h>
#include <osmocom/core/gsmtap_util.h>
#include <link_info.h>

#define BTS_DST_IP '127.0.0.1'

const int BTS_DST_PORT = 12345;

/** write message to mobile socket **/
void write_to_bts(int socket_bts, struct msgb *msg){
	struct sockaddr_in bts;
	srand(time(NULL));

	memset((char *) &bts, 0, sizeof(bts));
	bts.sin_family = AF_INET;
	bts.sin_port = htons(BTS_DST_PORT);
        if (inet_aton(BTS_DST_IP, &bts.sin_addr)==0) {
          fprintf(stderr, "inet_aton() failed\n");
          exit(1);
        }

	/** send data to mobile **/
	if (sendto(socket_bts, msg->data, msg->len, 0, &bts, sizeof(bts))==-1)
           printf("Error on sending data to mobile\n");
	else printf("Data sended\n");

	msgb_free(msg);
}

/** transmit RACH REQ to BTS **/
void l1_to_bts_rach_req(int socket_bts, struct l1ctl_info_dl *ul){
	struct l1ctl_rach_req *rach_req = (struct l1ctl_rach_req *) ul->payload;
	struct msgb *msg;
	uint8_t data[2];
	uint16_t info[1];

	data[0] = 0;
	data[1] = rach_req->ra;

	info[0] = ((uint16_t)(data[0])) | ((uint16_t)(data[1])<<8);

	msg = gsmtap_makemsg(
		  globalArfcn,
		  0,
		  GSMTAP_CHANNEL_RACH,
		  0,
		  0,
		  0,
		  0,
		  info,
		  sizeof(uint16_t)
		);

	write_to_bts(socket_bts, msg);

}
