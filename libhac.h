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
 * \file	libhac.h
 * \brief	libhac header file
 * \author	Bjoern Biesenbach <bjoern at bjoern-b dot de>
*/

#ifndef __LIBHAC_H__
#define __LIBHAC_H__

#include <stdint.h>

#define HAD_PORT 4123
#define BUF_SIZE 1024

#define CMD_NETWORK_QUIT 0
#define CMD_NETWORK_RGB 1
#define CMD_NETWORK_GET_RGB 2
#define CMD_NETWORK_BLINK 3
#define CMD_NETWORK_GET_TEMPERATURE 4
#define CMD_NETWORK_GET_VOLTAGE 5
#define CMD_NETWORK_RELAIS 6
#define CMD_NETWORK_GET_RELAIS 7
#define CMD_NETWORK_LED_DISPLAY_TEXT 8
#define CMD_NETWORK_BASE_LCD_ON 9
#define CMD_NETWORK_BASE_LCD_OFF 10
#define CMD_NETWORK_BASE_LCD_TEXT 11
#define CMD_NETWORK_GET_HAD_STATE 12
#define CMD_NETWORK_SET_HAD_STATE 13
#define CMD_NETWORK_GET_HR20 14
#define CMD_NETWORK_SET_HR20_TEMPERATURE 15
#define CMD_NETWORK_SET_HR20_MODE 16

#define HR20_MODE_MANU 1
#define HR20_MODE_AUTO 2

#define ADC_RES 1024

/* Experimental discovered values */
#define ADC_MODUL_1 ADC_RES*1.22
#define ADC_MODUL_3 ADC_RES*1.3
#define ADC_MODUL_DEFAULT ADC_RES*1.25

/*!
 *******************************************************************************
 * struct holding the params of one light module
 *******************************************************************************/
struct _rgb
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t smoothness;
};

struct _hadState
{
	struct _rgb rgbModuleValues[3];
	uint8_t relais_state;
	uint8_t input_state;
	uint16_t last_voltage[3];
	uint8_t scrobbler_user_activated;
	uint8_t ledmatrix_user_activated;
};

struct headPacket
{
	unsigned char address;
	unsigned char count;
	unsigned char command;
};

struct _rgbPacket
{
	struct headPacket headP;
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char smoothness;
};

struct _relaisPacket
{
	struct headPacket headP;
	unsigned char port;
};

struct _hr20info
{
	int16_t tempis;
	int16_t tempset;
	int8_t valve;
	int16_t voltage;
	int8_t mode;
};

extern int initLibHac(char *hostname);
extern void closeLibHac(void);

extern int setRgbValueModul(int modul, int red, int green, int blue, int smoothness);
extern int rgbBlink(int count, int color);
extern int setRgbValues(int red, int green, int blue, int smoothness);
extern int setRelais(uint8_t relais);
extern uint8_t getRelaisState();
extern int toggleRelais(uint8_t relais);
extern int getTemperature(uint8_t modul, uint8_t sensor, float *temperature);
extern int getVoltage(uint8_t modul, float *voltageReturn);
extern void ledSendText(char *string, int color, int shift, uint16_t lifetime);
extern void setBaseLcdOn();
extern void setBaseLcdOff();
extern void getHadState(struct _hadState *hadState);
extern void setHadState(struct _hadState hadState);
extern void setScrobblerOn(void);
extern void setScrobblerOff(void);
extern void setLedmatrixOn(void);
extern void setLedmatrixOff(void);
extern int getScrobblerState();
extern int getLedmatrixState();
extern void hr20GetStatus(int16_t *tempis, int16_t *tempset, int8_t *valve, int16_t *voltage, int8_t *mode);
extern void setHr20Temperature(int temperature);
extern void setHr20Mode(int8_t mode);

#endif

