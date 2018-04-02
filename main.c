/**
  ******************************************************************************
  * @file    main.c
  * @author  Melchor Varela - EA4FRB
  * @version V1.0
  * @date    13-March-2018
  * @brief   SARK-110 Server for Linux
  ******************************************************************************
  * @copy
  *
  *  This file is a part of the "SARK110 Antenna Vector Impedance Analyzer" software
  *
  *  "SARK110 Antenna Vector Impedance Analyzer software" is free software: you can redistribute it
  *  and/or modify it under the terms of the GNU General Public License as
  *  published by the Free Software Foundation, either version 3 of the License,
  *  or (at your option) any later version.
  *
  *  "SARK110 Antenna Vector Impedance Analyzer" software is distributed in the hope that it will be
  *  useful,  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with "SARK110 Antenna Vector Impedance Analyzer" software.  If not,
  *  see <http://www.gnu.org/licenses/>.
  *
  * <h2><center>&copy; COPYRIGHT 2011-2018 Melchor Varela - EA4FRB </center></h2>
  *  Melchor Varela, Madrid, Spain.
  *  melchor.varela@gmail.com
  */

/** @addtogroup SARK110
  * @{
  */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include "hidapi.h"
#include "sark_rem_client.h"
#include "sark_cmd_defs.h"

#define BUFSIZE 	18
#define PORT_NB		8888

static void print_ifconfig (void);

/*
 * error - wrapper for perror
 */
void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char **argv) {
	int sockfd; /* socket */
	int clientlen; /* byte size of client's address */
	struct sockaddr_in serveraddr; /* server's addr */
	struct sockaddr_in clientaddr; /* client addr */
	struct hostent *hostp; /* client host info */
	char *hostaddrp; /* dotted decimal host addr string */
	int optval; /* flag value for setsockopt */
	int n; /* message byte size */
	int msg_cnt = 0;

	print_ifconfig();

	// socket address used for the server
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;

	// htons: host to network short: transforms a value in host byte
	// ordering format to a short value in network byte ordering format
	server_address.sin_port = htons(PORT_NB);

	// htonl: host to network long: same as htons but to long
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	// create a TCP socket, creation returns -1 on failure
	int listen_sock;
	if ((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		error("could not create listen socket");
	}

	// bind it to listen to the incoming connections on the created server
	// address, will return -1 on error
	if ((bind(listen_sock, (struct sockaddr *)&server_address,
	          sizeof(server_address))) < 0) {
		error("could not bind socket");
	}
	int wait_size = 16;  // maximum number of waiting clients, after which
						 // dropping begins
	if (listen(listen_sock, wait_size) < 0) {
		error("could not open socket for listening");
	}

	// Server process
	for (;;)
	{
		uint16_t u16Ver;
		uint8_t tu8FW[25];
		uint8_t tu8Volume[25];
		// socket address used to store client address
		struct sockaddr_in client_address;
		int client_address_len = 0;

		/*
		 * Wait for sark-110 connection
		 */
		printf("Waiting for sark-110\n");
		for (;;)
		{
			if (Sark_Connect() >= 0)
				break;
			sleep(5);	/* 5 seconds wait */
		}
		printf ("sark-110 connected\n");
		Sark_Version(&u16Ver, tu8FW);
		Sark_DiskVolume(tu8Volume);
		printf("Protocol Version: %04x, Firmware Version: %s, Disk Volume: %s\n", u16Ver, tu8FW, tu8Volume);

		// open a new socket to transmit data per connection
		int sock;
		if ((sock =
		         accept(listen_sock, (struct sockaddr *)&client_address,
		                &client_address_len)) < 0) {
			error("could not open a socket to accept data");
		}
		printf("client connected with ip address: %s\n",
		       inet_ntoa(client_address.sin_addr));
		/*
		 * Wait for commands
		 */
		printf("Waiting commands\n");
		clientlen = sizeof(clientaddr);
		for (;;)
		{
			uint8_t tx[SARKCMD_TX_SIZE];
			uint8_t rx[SARKCMD_RX_SIZE];
			int rc;

			/*
			* recvfrom: receive a  datagram from a client
			*/
			if ((msg_cnt-- & 1) != 0)
				printf("X\r");
			else
				printf("+\r");

			bzero(tx, SARKCMD_TX_SIZE);
			bzero(rx, SARKCMD_RX_SIZE);
			n = recv(sock, tx, SARKCMD_TX_SIZE, 0);
			if (n < 0)
			{
				perror("ERROR in recv");
				break;
			}
			int i;
			for (i = 0; i < SARKCMD_TX_SIZE; i++)
				if (tx[i] != 0xff)
					break;
			if (i >= SARKCMD_TX_SIZE)
			{
				printf ("Client sent disconnect request\n");
				send(sock, tx, SARKCMD_RX_SIZE, 0);	/* answers with same data */
				break;
			}
			rc = Sark_SndRcv(tx, rx);
			if (rc < 0)
			{
				printf("Error: sark-110 communication\n");
				break;
			}
			/*
			* sendto: send the response back to the client
			*/
			n = send(sock, rx, SARKCMD_RX_SIZE, 0);
			if (n < 0)
			{
				perror("ERROR in send");
				break;
			}
		}
		Sark_Close();
		close(sock);
	}
	close(listen_sock);
}

static void print_ifconfig (void)
{
    int fd;
    struct ifreq ifr;
	int i;

    char* iface[] = { "wlan0", "eth0" };

	for (i=0; i < sizeof(iface)/sizeof(char*); i++)
	{
		fd = socket(AF_INET, SOCK_DGRAM, 0);

		//Type of address to retrieve - IPv4 IP address
		ifr.ifr_addr.sa_family = AF_INET;

		//Copy the interface name in the ifreq structure
		strncpy(ifr.ifr_name , iface[i] , IFNAMSIZ-1);

		//get the ip address
		ioctl(fd, SIOCGIFADDR, &ifr);

		//display ip
		printf("IP address of %s - %s\n" , iface[i] , inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr) );

		//get the netmask ip
		ioctl(fd, SIOCGIFNETMASK, &ifr);

		//display netmask
		printf("Netmask of %s - %s\n" , iface[i] , inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr) );

		close(fd);
	}
 }

/**
  * @}
  */

/************* (C) COPYRIGHT 2011-2018 Melchor Varela - EA4FRB *****END OF FILE****/

