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
	uint8_t red; /**< red */
	uint8_t green; /**< green */
	uint8_t blue; /**< blue */
	uint8_t smoothness; /**< smoothness (time for fading from one color to the other */
};

/*!
 *******************************************************************************
 * struct for getting the current state of had
 *******************************************************************************/
struct _hadState
{
	struct _rgb rgbModuleValues[3]; /**< array holding current values of each light module */
	uint8_t relais_state; /**< state of the relais */
	uint8_t input_state; /**< state of the input port */
	uint16_t last_voltage[3]; /**< last voltage values of rf modules */
	uint8_t scrobbler_user_activated; /**< scrobbler activated? */
	uint8_t ledmatrix_user_activated; /**< ledmatrix activated? */
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
//	int16_t auto_temperature[4]; /**< the four different temperatures for automatic mode */
//	int8_t timer_mode;
};

/*!
 *******************************************************************************
 * init the network connection to had
 *
 * has to be called before any command can be executed
 *
 * \param *hostname currently this has to be an ip address!
 * \param *password password to connect to had
 *******************************************************************************/
extern int initLibHac(char *hostname, char *password);

/*!
 *******************************************************************************
 * terminate the network connection
 *******************************************************************************/
extern void closeLibHac(void);

/*!
 *******************************************************************************
 * set rgb values for one specific module
 *
 * \param modul can be 16..18
 * \param red red 0..255
 * \param green green 0..255
 * \param blue blue 0..255
 * \param smoothness smoothness 0..255
 *******************************************************************************/
extern int setRgbValueModul(int modul, int red, int green, int blue, int smoothness);

/*!
 *******************************************************************************
 * blink red 3 times
 *******************************************************************************/
extern int rgbBlink(int count, int color);

//extern int setRgbValues(int red, int green, int blue, int smoothness);

/*!
 *******************************************************************************
 * set relais port to parameter
 *******************************************************************************/
extern int setRelais(uint8_t relais);

/*!
 *******************************************************************************
 * get the state of the relais port
 *
 * \returns uint8_t containing the state
 *******************************************************************************/
extern uint8_t getRelaisState();

/*!
 *******************************************************************************
 * toggle the bits of the relais port
 *
 * internally gets the current state and does and xor with the argument
 *
 * \param relais byte to toggle
 *******************************************************************************/
extern int toggleRelais(uint8_t relais);

/*!
 *******************************************************************************
 * get the last transmitted temperature of a module
 *
 * \param modul can currently only be 3
 * \param sensor can currently only be 0 or 1
 * \param *temperature temperature gets stored here
 *******************************************************************************/
extern int getTemperature(uint8_t modul, uint8_t sensor, float *temperature);

/*!
 *******************************************************************************
 * get the last transmitted voltage of a module
 *
 * \param modul can currently only be 3
 * \param *voltageReturn voltage gets stored here
 *******************************************************************************/
extern int getVoltage(uint8_t modul, float *voltageReturn);

/*!
 *******************************************************************************
 * display a text on the led matrix display
 *
 * \param *string the string to be displayed
 * \param lifetime how often the text should be scrolled through
 *******************************************************************************/
extern void ledSendText(char *string, int color, int shift, uint16_t lifetime);


/*!
 *******************************************************************************
 * set the background light of the base station on
 *******************************************************************************/
extern void setBaseLcdOn();

/*!
 *******************************************************************************
 * set the background light of the base station off
 *******************************************************************************/
extern void setBaseLcdOff();

/*!
 *******************************************************************************
 * get the had state struct
 *
 * \param *hadState struct where the state will be stored
 *******************************************************************************/
extern void getHadState(struct _hadState *hadState);

/*!
 *******************************************************************************
 * set the had state
 *
 * \param hadState state to be set
 *******************************************************************************/
extern void setHadState(struct _hadState hadState);

/*!
 *******************************************************************************
 * deactivate scrobbler
 *******************************************************************************/
extern void setScrobblerOn(void);

/*!
 *******************************************************************************
 * activate scrobbler
 *******************************************************************************/
extern void setScrobblerOff(void);

/*!
 *******************************************************************************
 * activate ledmatrix
 *******************************************************************************/
extern void setLedmatrixOn(void);

/*!
 *******************************************************************************
 * deactivate ledmatrix
 *******************************************************************************/
extern void setLedmatrixOff(void);

/*!
 *******************************************************************************
 * get state of scrobbler (de-. activated?)
 *******************************************************************************/
extern int getScrobblerState();

/*!
 *******************************************************************************
 * get state of ledmatrix (de-. activated?)
 *******************************************************************************/
extern int getLedmatrixState();

/*!
 *******************************************************************************
 * get state of hr20 thermostat
 *
 * \param *hr20info struct where the data will be stored
 *******************************************************************************/
extern void hr20GetStatus(struct _hr20info *hr20info);

/*!
 *******************************************************************************
 * set wanted temperature for hr20
 *
 * \param temperature wanted temperature * 10 (only 0.5°C steps)
 *******************************************************************************/
extern void setHr20Temperature(int temperature);

/*!
 *******************************************************************************
 * set mode for hr20
 *
 * \param mode 1 for manual, 2 for automatic control
 *******************************************************************************/
extern void setHr20Mode(int8_t mode);

#endif

