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

	free(data);
	msgb_free(msg);
}

/** Create and send BCCH message (system type information 3) to mobile **/
void send_bcch_sys_info_3_msg(int sock_mobile){
	struct gsm48_system_information_type_3 *sys_info = (struct gsm48_system_information_type_3 *) malloc(sizeof(struct gsm48_system_information_type_3));
	int header = 0x49061b;
	int chan_desc = 0xe0033c;
	int cell_opt = 0x57;
	int cell_sel = 0x650B;
	int rach_cont = 0x790000;
	int res_oct = 0x8000400b;

	//char hex[23] = {0x49,0x06,0x1b,0x2d,0xb5,0x32,0xf1,0x10,0x00,0x0a,0xe0,0x03,0x3c,0x57,0x65,0x0b,0x79,0x00,0x00,0x80,0x00,0x40,0x0b};
	
	memset(&sys_info->header,header, sizeof(header));
	
	sys_info->cell_identity = 0x2DB5;
	sys_info->lai.digits[1] = 0x32;
	sys_info->lai.digits[2] = 0xF1;
	sys_info->lai.digits[3] = 0x10;
	sys_info->lai.lac = 10;
	
	memset(&sys_info->control_channel_desc,chan_desc, sizeof(sys_info->control_channel_desc));
	memset(&sys_info->cell_options,cell_opt, sizeof(sys_info->cell_options));
	memset(&sys_info->cell_sel_par,cell_sel, sizeof(sys_info->cell_sel_par));
	memset(&sys_info->rach_control,rach_cont, sizeof(sys_info->rach_control));
	memset(&sys_info->rest_octets,res_oct, sizeof(sys_info->rest_octets));

	write_to_mobile(sock_mobile, sys_info, sizeof(*sys_info), GSMTAP_CHANNEL_BCCH);
}

/** Create and send BCCH message (system type information 4) to mobile **/
void send_bcch_sys_info_4_msg(int sock_mobile){
	struct gsm48_system_information_type_4 *sys_info = (struct gsm48_system_information_type_4 *) malloc(sizeof(struct gsm48_system_information_type_4));
	int header = 0x31061c;
	int cell_sel = 0x650B;
	int rach_cont = 0x790000;

	sys_info->lai.digits[1] = 0x32;
	sys_info->lai.digits[2] = 0xF1;
	sys_info->lai.digits[3] = 0x10;
	sys_info->lai.lac = 10;

	memcpy(&sys_info->header,&header, sizeof(sys_info->header));
	memcpy(&sys_info->cell_sel_par,&cell_sel, sizeof(sys_info->cell_sel_par));
	memcpy(&sys_info->rach_control,&rach_cont, sizeof(sys_info->rach_control));

	write_to_mobile(sock_mobile, sys_info, sizeof(*sys_info), GSMTAP_CHANNEL_BCCH);
}

/** Create and send BCCH message (system type information 2) to mobile **/
void send_bcch_sys_info_2_msg(int sock_mobile){
	struct gsm48_system_information_type_2 *sys_info = (struct gsm48_system_information_type_2 *) malloc(sizeof(struct gsm48_system_information_type_2));
	int header = 0x59061A;
	int frequencyList = 0x0000000000000000800000003ae46487;
	int rach_cont = 0x790000;

	sys_info->ncc_permitted = 0x08;
	memset(&sys_info->header, 0x59061A, sizeof(0x59061A));

	memset(&sys_info->bcch_frequency_list,frequencyList, sizeof(frequencyList));
	memset(&sys_info->rach_control,rach_cont, sizeof(rach_cont));

	write_to_mobile(sock_mobile, sys_info, sizeof(*sys_info), GSMTAP_CHANNEL_BCCH);
}

/** Create and send BCCH message (system type information 2ter) to mobile **/
void send_bcch_sys_info_2ter_msg(int sock_mobile){
	struct gsm48_system_information_type_2ter *sys_info = (struct gsm48_system_information_type_2ter *) malloc(sizeof(struct gsm48_system_information_type_2ter));

	memset(sys_info,0x010603cb130214, sizeof(*sys_info));
	//memset(&sys_info->ext_bcch_frequency_list,0xcb130214000000000000000000000000, sizeof(0xcb130214000000000000000000000000));

	//sys_info->header->l2_plen =
	write_to_mobile(sock_mobile, sys_info, sizeof(*sys_info), GSMTAP_CHANNEL_BCCH);
}

/** Create and send PCH message (Paging request) to mobile **/
void send_pch_paging_request_msg(int sock_mobile){
	struct gsm48_paging1 *paging = (struct gsm48_paging1 *) malloc(sizeof(struct gsm48_paging1));

	paging->l2_plen = 0x15;
	paging->proto_discr = 0x06;
	paging->msg_type = 0x21;
	paging->pag_mode = 0x0;

	memset(&paging->data, 0x00100, sizeof(0x00100));

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
