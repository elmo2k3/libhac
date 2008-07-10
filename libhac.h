#ifndef __LIBHAC_H__
#define __LIBHAC_H__

#define HAD_PORT 4123
#define BUF_SIZE 1024
#define CMD_NETWORK_RGB 1
#define CMD_NETWORK_GET_RGB 2
#define CMD_NETWORK_BLINK 3

void getRgbValues(int *red, int *green, int *blue, int *smoothness);
void rgbBlink(int count, int color);
void setRgbValues(int red, int green, int blue, int smoothness);

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

#endif

