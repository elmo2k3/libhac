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

#define ADC_RES 1024

/* Experimental discovered values */
#define ADC_MODUL_1 ADC_RES*1.22
#define ADC_MODUL_3 ADC_RES*1.3
#define ADC_MODUL_DEFAULT ADC_RES*1.25


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

int initLibHac(char *hostname);
void closeLibHac(void);

int setRgbValueModul(int modul, int red, int green, int blue, int smoothness);
int getRgbValues(int *red, int *green, int *blue, int *smoothness);
int rgbBlink(int count, int color);
int setRgbValues(int red, int green, int blue, int smoothness);
int setRelais(uint8_t relais);
int getRelaisState(uint8_t *relais);
int toggleRelais(uint8_t relais);
int getTemperature(uint8_t modul, uint8_t sensor, float *temperature);
int getVoltage(uint8_t modul, float *voltageReturn);
void ledSendText(char *string, int color, int shift, uint16_t lifetime);
void setBaseLcdOn();
void setBaseLcdOff();
void getHadState(struct _hadState *hadState);
#endif

