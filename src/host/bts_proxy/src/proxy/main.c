/*
 * File: main.c
 * Date: 10.11.2013
 * Autor: Bc. Miroslav Babják, xbabjak@fiit.stuba.sk
 * Project: Emulator of GPRS modem
 * Description: Interface between osmocomBB layer23 and socket BTS
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <poll.h>
#include <stropts.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <L1toL2int.h>
#include <L2toL1int.h>
#include <btsToMobile.h>
#include <link_info.h>

#define SOCK_PATH_TO_L2 "/tmp/osmocom_l2"
#define BTS_ADDRESS "127.0.0.1"
#define MAIN_FILE

const int BTS_SRC_PORT = 19789;

uint16_t globalArfcn = 51;		/* ARFCN (frequency) for actual BTS*/
uint8_t globalRxLevel = 55;
uint8_t globalSnrLevel = 15;

int closeApp = 0;

int create_socket_l2(const char* path){
  int len,s;
  struct sockaddr_un local, remote;
  socklen_t remote_len;

  /* create socket */
  if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("Error creating socket");
    exit(1);
  }

  /* bind socket */
  local.sun_family = AF_UNIX;
  strcpy(local.sun_path, path);
  unlink(local.sun_path);
  len = strlen(local.sun_path) + sizeof(local.sun_family);
  if (bind(s, (struct sockaddr *)&local, len) == -1) {
    perror("Error binding socket");
    exit(1);
  }

  /* listen on socket */
  if (listen(s, 1) == -1) {
    perror("Error listening on socket");
    exit(1);
  }    

  /* wait for connection on socket */
  printf("Waiting for a connection on socket ...\n");
  remote_len = sizeof(remote);
  if ((s = accept(s, (struct sockaddr *)&remote, &remote_len)) == -1) {
    perror("Error accepting connection on socket");
    exit(1);
  }

  printf("Socket connected.\n\n");

  return s;
}

int create_socket_bts(){
  int s;
  struct sockaddr_in myaddr;

  /* create socket */
  if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    perror("Error creating socket");
    exit(1);
  }

  memset((char *)&myaddr, 0, sizeof(myaddr)); 
  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myaddr.sin_port = htons(BTS_SRC_PORT);

  if (bind(s, &myaddr, sizeof(myaddr)) == -1) {
    perror("Error binding socket");
    exit(1);
  }  

  printf("Socket created.\n\n");

  return s;
}

void signalHandler(int signal){
	printf("Closing ....\n\n");
	closeApp = 1;
}

void socket_messages_handling(int socket_l2, int socket_bts) {
	unsigned int poll_timeout = 500;
	uint8_t *msg;
	uint16_t msg_len;
	unsigned char buffer[500];	//buffer for BTS messages
	struct l1ctl_hdr *hdr;
  	struct pollfd fds[2];
	int p_ret,len,i;

	fds[0].fd = socket_bts;
	fds[1].fd = socket_l2;
  	fds[0].events = POLLIN|POLLPRI;
	fds[1].events = POLLIN|POLLPRI;

	p_ret = poll(fds,2,poll_timeout);

	if(p_ret >= 0){
		for (i=0; i<2; i++) {
			
			if(((fds[i].revents&POLLIN) == POLLIN) || ((fds[i].revents&POLLPRI) == POLLPRI)){
			   if(i == 0){
				//receive msg from BTS

				/** receive data **/
				if ((len=recvfrom(fds[i].fd, buffer, 100, 0, NULL, NULL)) > 0) {
					printf("RECEIVE DATA FROM BTS with size %d\n",len);
					switch_BTS_messages(socket_l2, &buffer, len);	//message from BTS
				}
			   } else {
				//receive msg from layer2
				if ((len=recv(fds[i].fd, &msg_len, sizeof(msg_len), 0)) <= 0) {
					if (len < 0) 
						perror("Error receiving header on socket");
					else {
						printf("Closed connection to socket\n");
						closeApp = 1;
					}
				}


				msg = malloc(sizeof(uint8_t)*msg_len);
		
				/** receive data **/
				if ((len=recv(fds[i].fd, msg, htons(msg_len), 0)) > 0) {
					switch_L2_messages(socket_l2, socket_bts, msg);	//message from L2
				}
			   }
			}
		}
	}
}

/** ================================================================
================== MAIN START ====================================== 
==================================================================== **/
int main(void){
	int sock_layer2, sock_bts;

	signal(SIGINT,signalHandler); 

	sock_bts = create_socket_bts();
	sock_layer2 = create_socket_l2(SOCK_PATH_TO_L2);

	printf("layer1 has conencted to layer2 and BTS. It's ready to work!\n\n");

	while(!closeApp){
		socket_messages_handling(sock_layer2, sock_bts);
	}

	/* close sockets */
	close(sock_layer2);
	close(sock_bts);

	return 0;
}
