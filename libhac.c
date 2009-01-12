/* Header files for windows */
#ifdef _WIN32
#warning Building win32 version
#include <winsock.h>
#include <io.h>
/* Header files for linux */
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libhac.h"

#ifdef _WIN32
static SOCKET client_sock;
#else
static int client_sock;
#endif

void hr20GetStatus(int16_t *tempis, int16_t *tempset, int8_t *valve, int16_t *voltage, int8_t *mode)
{
	int command;
	struct _hr20info hr20info;
	
	command = CMD_NETWORK_GET_HR20;
	
	send(client_sock, &command, 1, 0);
	
	recv(client_sock, &hr20info, sizeof(hr20info), 0);

	*tempis = hr20info.tempis;
	*tempset = hr20info.tempset;
	*valve = hr20info.valve;
	*voltage = hr20info.voltage;
	*mode = hr20info.mode;
}

void setBaseLcdOn()
{
	int command;

	command = CMD_NETWORK_BASE_LCD_ON;

	send(client_sock, &command, 1, 0);
}

void setBaseLcdOff()
{
	int command;

	command = CMD_NETWORK_BASE_LCD_OFF;

	send(client_sock, &command, 1, 0);
}

void getHadState(struct _hadState *hadState)
{
	int command = CMD_NETWORK_GET_HAD_STATE;

	send(client_sock, &command, 1, 0);
	recv(client_sock, hadState, sizeof(struct _hadState), 0);
}

void setHr20Temperature(int temperature)
{
	int command = CMD_NETWORK_SET_HR20_TEMPERATURE;

	if(temperature % 5)
		return;
	if(temperature < 50 || temperature > 300)
		return;

	int16_t temp = (int16_t)temperature;

	send(client_sock, &command, 1, 0);
	send(client_sock, &temp, sizeof(temp), 0);
}

void setHr20Mode(int8_t mode)
{
	int command = CMD_NETWORK_SET_HR20_MODE;

	if(mode != HR20_MODE_AUTO && mode != HR20_MODE_MANU)
		return;

	send(client_sock, &command, 1, 0);
	send(client_sock, &mode, sizeof(mode), 0);
}

void setHadState(struct _hadState hadState)
{
	int command = CMD_NETWORK_SET_HAD_STATE;

	send(client_sock, &command, 1, 0);
	send(client_sock, &hadState, sizeof(hadState), 0);
}

int getLedmatrixState()
{
	struct _hadState hadState;

	getHadState(&hadState);
	return hadState.ledmatrix_user_activated;
}

int getScrobblerState()
{
	struct _hadState hadState;

	getHadState(&hadState);
	return hadState.scrobbler_user_activated;
}


void setLedmatrixOff()
{
	struct _hadState hadState;

	getHadState(&hadState);
	hadState.ledmatrix_user_activated = 0;
	setHadState(hadState);
}

void setLedmatrixOn()
{
	struct _hadState hadState;

	getHadState(&hadState);
	hadState.ledmatrix_user_activated = 1;
	setHadState(hadState);
}

void setScrobblerOff()
{
	struct _hadState hadState;

	getHadState(&hadState);
	hadState.scrobbler_user_activated = 0;
	setHadState(hadState);
}

void setScrobblerOn()
{
	struct _hadState hadState;

	getHadState(&hadState);
	hadState.scrobbler_user_activated = 1;
	setHadState(hadState);
}

uint8_t getRelaisState()
{
	struct _hadState hadState;

	getHadState(&hadState);
	return hadState.relais_state;
}

int setRgbValueModul(int modul, int red, int green, int blue, int smoothness)
{
	int command;
	struct _rgbPacket rgbPacket;

	command = CMD_NETWORK_RGB;


	rgbPacket.red = (unsigned char)red;
	rgbPacket.green = (unsigned char)green;
	rgbPacket.blue = (unsigned char)blue;
	rgbPacket.smoothness = (unsigned char)smoothness;

	rgbPacket.headP.address = modul;
	send(client_sock, &command, 1, 0);
	send(client_sock, &rgbPacket, sizeof(rgbPacket), 0);
	return 0;
}

void ledSendText(char *string, int color, int shift, uint16_t lifetime)
{
	uint8_t command;
	uint16_t string_length;

	command = CMD_NETWORK_LED_DISPLAY_TEXT;
	string_length = strlen(string);

	send(client_sock, &command, 1, 0);
	send(client_sock, &string_length, 2, 0);
	send(client_sock, &lifetime, 2, 0);
	send(client_sock, string, string_length, 0);
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
	
	/* Modul 3 */
	rgbPacket.headP.address = 4;
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
	struct _hadState hadState;

	getHadState(&hadState);
	hadState.relais_state = relais;
	setHadState(hadState);

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

	if(celsius < 0)
		*temperature = (float)(celsius) - (float)(decicelsius)/10000;
	else
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
#ifdef _WIN32
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,0), &wsa);
#endif

	struct _rgbPacket rgbPacket;
	client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(client_sock < 0)
		printf("Client_sock konnte nicht erstellt werden\n");
	server.sin_family = AF_INET;
	server.sin_port = htons(HAD_PORT);
//	inet_aton("127.0.0.1", &server.sin_addr);
#ifdef _WIN32
	unsigned long addr;
	addr = inet_addr("192.168.0.2");
	memcpy( (char *)&server.sin_addr, &addr, sizeof(addr));
#else
	inet_aton(hostname, &server.sin_addr);
#endif
	
#ifdef _WIN32
	if(connect(client_sock, (struct SOCKADDR *)&server, sizeof(server)) != 0)
#else
	if(connect(client_sock, (struct sockaddr*)&server, sizeof(server)) != 0)
#endif
	{
		printf("Konnte nicht verbinden\n");
		return -1;
	}	

	return 0;
}

