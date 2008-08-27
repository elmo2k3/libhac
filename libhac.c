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

int getRgbValues(int *red, int *green, int *blue, int *smoothness)
{
	int command;
	struct _rgbPacket rgbPacket;

	command = CMD_NETWORK_GET_RGB;

	send(client_sock, &command, 1, 0);
	recv(client_sock, &rgbPacket, sizeof(rgbPacket), 0);
	
	*red = (int)rgbPacket.red;
	*green = (int)rgbPacket.green;
	*blue = (int)rgbPacket.blue;
	*smoothness = (int)rgbPacket.smoothness;
	
	return 0;
}

int getRelaisState(uint8_t *relais)
{
	int relaisState;
	uint8_t command;
	command = CMD_NETWORK_GET_RELAIS;
	struct _relaisPacket relaisPacket;


	send(client_sock, &command, 1, 0);
	recv(client_sock, &relaisPacket, sizeof(relaisPacket), 0);

	relaisState = relaisPacket.port;
	*relais = relaisState;
	
	return 0;
}

int setRgbValues(int red, int green, int blue, int smoothness)
{
	int command;
	struct _rgbPacket rgbPacket;

	command = CMD_NETWORK_RGB;


	rgbPacket.red = (unsigned char)red;
	rgbPacket.green = (unsigned char)green;
	rgbPacket.blue = (unsigned char)blue;
	rgbPacket.smoothness = (unsigned char)smoothness;

	/* Modul 1 */
	rgbPacket.headP.address = 1;
	send(client_sock, &command, 1, 0);
	send(client_sock, &rgbPacket, sizeof(rgbPacket), 0);
	
	/* Modul 2 */
	rgbPacket.headP.address = 3;
	send(client_sock, &command, 1, 0);
	send(client_sock, &rgbPacket, sizeof(rgbPacket), 0);
	return 0;
}

int rgbBlink(int count, int color)
{
	int command;
	command = CMD_NETWORK_BLINK;
	send(client_sock, &command, 1, 0);
	return 0;
}

int setRelais(uint8_t relais)
{
	int command;
	command = CMD_NETWORK_RELAIS;
	send(client_sock, &command, 1, 0);
	send(client_sock, &relais, 1, 0);
	
	return 0;
}

int toggleRelais(uint8_t relais)
{
	int command;
	uint8_t relais_now;

	if(getRelaisState(&relais_now) < 0)
		return -1;

	if(setRelais(relais_now ^ relais) < 0)
		return -1;

	return 0;
}

void closeLibHac(void)
{
	int8_t command;
	command = CMD_NETWORK_QUIT;
	send(client_sock, &command, 1, 0);
	close(client_sock);
}

int getTemperature(uint8_t modul, uint8_t sensor, float *temperature)
{
	int command;
	int16_t celsius, decicelsius;

	command = CMD_NETWORK_GET_TEMPERATURE;
	send(client_sock, &command, 1, 0);

	send(client_sock, &modul, 1, 0);
	send(client_sock, &sensor, 1, 0);

	recv(client_sock, &celsius, sizeof(celsius), 0);
	recv(client_sock, &decicelsius, sizeof(decicelsius), 0);


	*temperature = (float)(celsius) + (float)(decicelsius)/10000;
	return 0;
}

int getVoltage(uint8_t modul, float *voltageReturn)
{
	int command;
	int16_t voltage;

	command = CMD_NETWORK_GET_VOLTAGE;
	send(client_sock, &command, 1, 0);

	send(client_sock, &modul, 1, 0);

	recv(client_sock, &voltage, sizeof(voltage), 0);

	switch(modul)
	{
		case 1: *voltageReturn = (float)ADC_MODUL_1/voltage;
			break;
		case 3: *voltageReturn = (float)ADC_MODUL_3/voltage;
			break;
		default: *voltageReturn = (float)ADC_MODUL_DEFAULT/voltage;
			break;
	}

	return 0;
}

int initLibHac(char *hostname)
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
//	inet_aton("127.0.0.1", &server.sin_addr);
	inet_aton(hostname, &server.sin_addr);
	

	if(connect(client_sock, (struct sockaddr*)&server, sizeof(server)) != 0)
	{
		printf("Konnte nicht verbinden\n");
		return -1;
	}	

	return 0;
}

