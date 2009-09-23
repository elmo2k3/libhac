/*
 * Copyright (C) 2007-2009 Bjoern Biesenbach <bjoern@bjoern-b.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*!
 * \file	libhac.c
 * \brief	main file for libhac
 * \author	Bjoern Biesenbach <bjoern at bjoern-b dot de>
*/

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
#include <netdb.h>
#endif
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <errno.h>

#include "libhac.h"
#include "../version.h"

#ifdef _WIN32
static SOCKET client_sock;
#else
static int client_sock;
#endif

static int connected = 0;

char *libhacVersion(void)
{
	return LIBHAC_VERSION;
}

void hr20GetStatus(struct _hr20info *hr20info)
{
	int command;
	if(connected)
	{
		command = CMD_NETWORK_GET_HR20;
		send(client_sock, &command, 1, 0);
		recv(client_sock, hr20info, sizeof(struct _hr20info), 0);
	}
}

void setBaseLcdOn()
{
	int command;
	if(connected)
	{
		command = CMD_NETWORK_BASE_LCD_ON;
		send(client_sock, &command, 1, 0);
	}
}

void openDoor()
{
	int command;
	if(connected)
	{
		command = CMD_NETWORK_OPEN_DOOR;
		send(client_sock, &command, 1, 0);
	}
}

void setBaseLcdOff()
{
	int command;
	if(connected)
	{
		command = CMD_NETWORK_BASE_LCD_OFF;
		send(client_sock, &command, 1, 0);
	}
}

void getHadState(struct _hadState *hadState)
{
	int command = CMD_NETWORK_GET_HAD_STATE;
	if(connected)
	{
		send(client_sock, &command, 1, 0);
		recv(client_sock, hadState, sizeof(struct _hadState), 0);
	}
}

void setHr20Temperature(int temperature)
{
	int command = CMD_NETWORK_SET_HR20_TEMPERATURE;

	if(temperature % 5)
		return;
	if(temperature < 50 || temperature > 300)
		return;

	int16_t temp = (int16_t)temperature;
	if(connected)
	{
		send(client_sock, &command, 1, 0);
		send(client_sock, &temp, sizeof(temp), 0);
	}
}

void setHr20AutoTemperature(int slot, int temperature)
{
	int command = CMD_NETWORK_SET_HR20_AUTO_TEMPERATURE;

	if(temperature % 5)
		return;
	if(temperature < 50 || temperature > 300)
		return;

	int16_t temp = (int16_t)temperature;
	int8_t Slot = (int8_t)slot;
	if(connected)
	{
		send(client_sock, &command, 1, 0);
		send(client_sock, &Slot, sizeof(Slot), 0);
		send(client_sock, &temp, sizeof(temp), 0);
	}
}

void setHr20Mode(int8_t mode)
{
	int command = CMD_NETWORK_SET_HR20_MODE;

	if(mode != HR20_MODE_AUTO && mode != HR20_MODE_MANU)
		return;
	if(connected)
	{
		send(client_sock, &command, 1, 0);
		send(client_sock, &mode, sizeof(mode), 0);
	}
}

void setHadState(struct _hadState hadState)
{
	int command = CMD_NETWORK_SET_HAD_STATE;
	if(connected)
	{
		send(client_sock, &command, 1, 0);
		send(client_sock, &hadState, sizeof(hadState), 0);
	}
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

void setRgbValueModul(int modul, int red, int green, int blue, int smoothness)
{
	int command;
	struct _rgbPacket rgbPacket;

	command = CMD_NETWORK_RGB;

	rgbPacket.red = (unsigned char)red;
	rgbPacket.green = (unsigned char)green;
	rgbPacket.blue = (unsigned char)blue;
	rgbPacket.smoothness = (unsigned char)smoothness;

	rgbPacket.headP.address = modul;
	if(connected)
	{
		send(client_sock, &command, 1, 0);
		send(client_sock, &rgbPacket, sizeof(rgbPacket), 0);
	}
}

void ledSendText(char *string, int color, int shift, uint16_t lifetime)
{
	uint8_t command;
	uint16_t string_length;

	command = CMD_NETWORK_LED_DISPLAY_TEXT;
	string_length = strlen(string);
	if(connected)
	{
		send(client_sock, &command, 1, 0);
		send(client_sock, &string_length, 2, 0);
		send(client_sock, &lifetime, 2, 0);
		send(client_sock, string, string_length, 0);
	}
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

	if(connected)
	{
		/* Modul 1 */
		rgbPacket.headP.address = 16;
		send(client_sock, &command, 1, 0);
		send(client_sock, &rgbPacket, sizeof(rgbPacket), 0);
		
		/* Modul 2 */
		rgbPacket.headP.address = 17;
		send(client_sock, &command, 1, 0);
		send(client_sock, &rgbPacket, sizeof(rgbPacket), 0);
		
		/* Modul 3 */
		rgbPacket.headP.address = 18;
		send(client_sock, &command, 1, 0);
		send(client_sock, &rgbPacket, sizeof(rgbPacket), 0);
	}
}

void rgbBlink(int count, int color)
{
	int command;
	command = CMD_NETWORK_BLINK;
	if(connected)
		send(client_sock, &command, 1, 0);
}

void setRelais(uint8_t relais)
{
	struct _hadState hadState;

	getHadState(&hadState);
	hadState.relais_state = relais;
	setHadState(hadState);
}

void toggleRelais(uint8_t relais)
{
	int command;
	uint8_t relais_now;

	getRelaisState(&relais_now); 
	setRelais(relais_now ^ relais);
}

void closeLibHac(void)
{
	int8_t command;
	if(connected)
	{
		command = CMD_NETWORK_QUIT;
		send(client_sock, &command, 1, 0);
		close(client_sock);
		connected = 0;
	}
}

void getTemperature(uint8_t modul, uint8_t sensor, float *temperature)
{
	int command;
	int16_t celsius, decicelsius;

	command = CMD_NETWORK_GET_TEMPERATURE;

	if(connected)
	{
		send(client_sock, &command, 1, 0);
		send(client_sock, &modul, 1, 0);
		send(client_sock, &sensor, 1, 0);

		recv(client_sock, &celsius, sizeof(celsius), 0);
		recv(client_sock, &decicelsius, sizeof(decicelsius), 0);

		if(celsius < 0)
			*temperature = (float)(celsius) - (float)(decicelsius)/10000;
		else
			*temperature = (float)(celsius) + (float)(decicelsius)/10000;
	}
}

void getVoltage(uint8_t modul, float *voltageReturn)
{
	int command;
	int16_t voltage;

	command = CMD_NETWORK_GET_VOLTAGE;

	if(connected)
	{
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
	}
}


int initLibHac(char *hostname, char *password)
{
	struct sockaddr_in server,client;
	struct hostent *he;
	unsigned char buf[BUF_SIZE];
	int recv_size;
	int send_size;
	unsigned char command;
	struct timeval timeout;
	uint32_t rawtime;
	memset(&timeout, 0, sizeof(timeout));
#ifdef _WIN32
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,0), &wsa);
#endif

	struct _rgbPacket rgbPacket;
	client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(client_sock < 0)
		return errno;
	server.sin_family = AF_INET;
	server.sin_port = htons(HAD_PORT);
	if(!(he=gethostbyname(hostname)))
	{
		return errno;
	}
	if(he->h_addrtype == AF_INET)
	{
		memcpy((char*)&server.sin_addr.s_addr,(char*)he->h_addr,
			he->h_length);
	}
	else
	{
		return -1;
	}
	timeout.tv_sec = 2;
	setsockopt(client_sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(struct timeval));
#ifdef _WIN32
	if(connect(client_sock, (struct SOCKADDR *)&server, sizeof(server)) != 0)
#else
	if(connect(client_sock, (struct sockaddr*)&server, sizeof(server)) != 0)
#endif
	{
		return errno;
	}	
	else
	{
		char pass_salted[200];
		char buf[255];
		unsigned char digest[MD5_DIGEST_LENGTH];
		recv(client_sock, &rawtime, sizeof(rawtime), 0);
		sprintf(pass_salted,"%s%lld",password,(long long int)rawtime);
		MD5(pass_salted, strlen(pass_salted), digest);
		send(client_sock, digest, MD5_DIGEST_LENGTH, 0);
		recv(client_sock, buf, 1, 0);
		if(buf[0] == 0)
		{
			return LIBHAC_WRONG_PASSWORD;
		}
		connected = 1;
	}
	
	timeout.tv_sec = 0;
//	setsockopt(client_sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(struct timeval));

	return 0;
}

