#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <l1ctl_proto.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <osmocom/core/msgb.h>
#include <osmocom/gsm/protocol/gsm_04_08.h>
#include <osmocom/core/gsmtap.h>
#include <osmocom/core/gsmtap_util.h>
#include "bts.h"

#define MOBILE_ADDR "127.0.0.1"

int lastFrameNumber = 1780600;
uint16_t BTS_ARFCN = 17;

int create_socket(){
  int s;
  struct sockaddr_in myaddr;

  /* create socket */
  if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    perror("Error creating socket");
    exit(1);
  }

  /* bind socket */
  memset((char *)&myaddr, 0, sizeof(myaddr)); 
  myaddr.sin_family = AF_INET; 
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
  myaddr.sin_port = htons(19876);
  if (bind(s, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1) {
    perror("Error binding socket");
    exit(1);
  }  

  printf("Socket connected.\n\n");

  return s;
}

/** write message to mobile socket **/
void write_to_mobile(int socket_mobile, uint8_t *data, int size, uint8_t channelType){
	struct sockaddr_in mobile;
	struct msgb *msg;
	srand(time(NULL));

	msg = gsmtap_makemsg(
	  BTS_ARFCN,
	  0,
	  channelType,
	  0,
	  (lastFrameNumber + (rand() % 10)) % UINT32_MAX,
	  0,
	  170,
	  data,
	  size
	);

	memset((char *) &mobile, 0, sizeof(mobile));
        mobile.sin_family = AF_INET;
        mobile.sin_port = htons(4729);
        if (inet_aton(MOBILE_ADDR, &mobile.sin_addr)==0) {
          fprintf(stderr, "inet_aton() failed\n");
          exit(1);
        }

	/** send data to mobile **/
	if (sendto(socket_mobile, msg->data, msg->len, 0, &mobile, sizeof(mobile))==-1)
           printf("Error on sending data to mobile\n");
	else printf("Data sended\n");

	msgb_free(msg);
}

/** Create and send BCCH message (system type information 3) to mobile **/
void send_bcch_sys_info_3_msg(int sock_mobile){
	uint8_t data[] = {0x4B,0x06,0x1b,0x2d,0xb5,0x32,0xf1,0x10,0x00,0x0a,0xe0,0x03,0x3c,0x57,0x65,0x0b,0x79,0x00,0x00,0x80,0x00,0x40,0x0b};

	write_to_mobile(sock_mobile, &data, sizeof(data), GSMTAP_CHANNEL_BCCH);
}

/** Create and send BCCH message (system type information 4) to mobile **/
void send_bcch_sys_info_4_msg(int sock_mobile){
	uint8_t data[] = {0x31,0x06,0x1c,0x32,0xf1,0x10,0x00,0x0a,0x65,0x0b,0x79,0x00,0x00,0x80,0x00,0x43,0x2b,0x2b,0x2b,0x2b,0x2b,0x2b,0x2b};

	write_to_mobile(sock_mobile, &data, sizeof(data), GSMTAP_CHANNEL_BCCH);
}

/** Create and send BCCH message (system type information 2) to mobile **/
void send_bcch_sys_info_2_msg(int sock_mobile){
	uint8_t data[] = {0x59,0x06,0x1a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x3a,0xe4,0x64,0x87,0x08,0x79,0x00,0x00};

	write_to_mobile(sock_mobile, &data, sizeof(data), GSMTAP_CHANNEL_BCCH);
}

/** Create and send BCCH message (system type information 2ter) to mobile **/
void send_bcch_sys_info_2ter_msg(int sock_mobile){
	uint8_t data[] = {0x01,0x06,0x03,0xcb,0x13,0x02,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2b,0x2b,0x2b,0x2b};

	write_to_mobile(sock_mobile, &data, sizeof(data), GSMTAP_CHANNEL_BCCH);
}

/** Create and send PCH message (Paging request) to mobile **/
void send_pch_paging_request_msg(int sock_mobile){
	struct gsm48_paging1 *paging = (struct gsm48_paging1 *) malloc(sizeof(struct gsm48_paging1));
	int data = 0x00100;

	paging->l2_plen = 0x15;
	paging->proto_discr = 0x06;
	paging->msg_type = 0x21;
	paging->pag_mode = 0x0;

	memcpy(&paging->data, &data, sizeof(data));

	write_to_mobile(sock_mobile, paging, sizeof(*paging), GSMTAP_CHANNEL_PCH);
}

int main(void){
	int sock_mobile;

	sock_mobile = create_socket();

	while(1){
		send_bcch_sys_info_2_msg(sock_mobile);
		sleep(1);
		send_bcch_sys_info_3_msg(sock_mobile);
		sleep(1);
		send_bcch_sys_info_4_msg(sock_mobile);
		sleep(1);
		send_pch_paging_request_msg(sock_mobile);
		sleep(1);
		send_bcch_sys_info_2ter_msg(sock_mobile);
		sleep(1);
	}

	return 0;
}
