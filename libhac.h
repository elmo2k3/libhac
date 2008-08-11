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

int initLibHac(char *hostname);
void closeLibHac(void);

int getRgbValues(int *red, int *green, int *blue, int *smoothness);
int rgbBlink(int count, int color);
int setRgbValues(int red, int green, int blue, int smoothness);
int setRelais(uint8_t relais);
int getRelaisState(uint8_t *relais);
int toggleRelais(uint8_t relais);
int getTemperature(uint8_t modul, uint8_t sensor, float *temperature);

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
#endif

