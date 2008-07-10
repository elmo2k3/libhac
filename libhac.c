#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libhac.h"

static int client_sock;
static int initLibHac(void);
static void closeLibHac(void);

void getRgbValues(int *red, int *green, int *blue, int *smoothness)
{
	int command;
	struct _rgbPacket rgbPacket;

	initLibHac();

	command = CMD_NETWORK_GET_RGB;

	send(client_sock, &command, 1, 0);
	recv(client_sock, &rgbPacket, sizeof(rgbPacket), 0);
	
	*red = (int)rgbPacket.red;
	*green = (int)rgbPacket.green;
	*blue = (int)rgbPacket.blue;
	*smoothness = (int)rgbPacket.smoothness;
	
	closeLibHac();
}

void setRgbValues(int red, int green, int blue, int smoothness)
{
	int command;
	struct _rgbPacket rgbPacket;

	command = CMD_NETWORK_RGB;


	rgbPacket.red = (unsigned char)red;
	rgbPacket.green = (unsigned char)green;
	rgbPacket.blue = (unsigned char)blue;
	rgbPacket.smoothness = (unsigned char)smoothness;

	initLibHac();
	/* Modul 1 */
	rgbPacket.headP.address = 1;
	send(client_sock, &command, 1, 0);
	send(client_sock, &rgbPacket, sizeof(rgbPacket), 0);
	
	closeLibHac();
	initLibHac();

	/* Modul 2 */
	rgbPacket.headP.address = 3;
	send(client_sock, &command, 1, 0);
	send(client_sock, &rgbPacket, sizeof(rgbPacket), 0);
	closeLibHac();
}

void rgbBlink(int count, int color)
{
	int command;
	initLibHac();
	command = CMD_NETWORK_BLINK;
	send(client_sock, &command, 1, 0);
	closeLibHac();
}

static void closeLibHac(void)
{
	close(client_sock);
}

static int initLibHac(void)
{
	struct sockaddr_in server,client;
	unsigned char buf[BUF_SIZE];
	int recv_size;
	int send_size;
	unsigned char command;

	struct _rgbPacket rgbPacket;

	client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(client_sock < 0)
		printf("Client_sock konnte nicht erstellt werden\n");
	server.sin_family = AF_INET;
	server.sin_port = htons(HAD_PORT);
	inet_aton("192.168.0.2", &server.sin_addr);
	

	if(connect(client_sock, (struct sockaddr*)&server, sizeof(server)) != 0)
		printf("Konnte nicht verbinden\n");
	

	return 0;
}

