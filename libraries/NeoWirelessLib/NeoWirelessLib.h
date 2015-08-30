// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _NeoWirelessLib_H_
#define _NeoWirelessLib_H_
#include "Arduino.h"



#define CMD_ERROR				0x00
#define CMD_PATTERN 			0x01
#define CMD_RUNWAY				0x02
#define CMD_BOUNCE				0x03
#define CMD_MIDDLE				0x04
#define CMD_RAINBOW_FADE		0x05
#define CMD_RAINBOW_FADE_ONE	0x06
#define CMD_RANDOM_FLASH		0x07
#define CMD_RANDOM_FLASH_COLOR	0x08
#define CMD_SET_ALL				0x09

#define NUM_COMMANDS			9
static const uint8_t COMMANDS[NUM_COMMANDS] = {CMD_PATTERN, CMD_RUNWAY, CMD_BOUNCE, CMD_MIDDLE, CMD_RAINBOW_FADE, CMD_RAINBOW_FADE_ONE, CMD_RANDOM_FLASH, CMD_RANDOM_FLASH_COLOR, CMD_SET_ALL};
static const uint8_t COMMAND_LEN[NUM_COMMANDS] = {18, 20, 20, 19, 4, 5, 7, 9};
#define MAX_COMMAND_SIZE		25

#define DOWN 	0
#define UP		1
#define RIGHT	0
#define LEFT	1
#define IN		0
#define OUT		1


#define WHITE	0x00FFFFFF
#define BLACK	0x00000000
#define RED		0x00FF0000
#define BLUE	0x000000FF
#define GREEN	0x0000FF00
#define MAGENTA	RED | BLUE
#define CYAN	BLUE | GREEN
#define YELLOW	RED | GREEN
#define ORANGE	0xFF3300
#define PURPLE	0x990099

typedef struct CommandPattern
{
	uint8_t command;
	uint8_t pattern;
	uint8_t direction;
	uint32_t onColor;
	uint32_t offColor;
	uint32_t onTime;
	uint32_t offTime;
} pattern_t;

typedef struct CommandRunway
{
	uint8_t command;
	uint8_t pattern;
	uint8_t direction;
	uint32_t onColor;
	uint32_t offColor;
	uint32_t onTime;
	uint32_t offTime;
	uint8_t clearAfter;
	uint8_t clearEnd;
} runway_t;

typedef struct CommandBounce
{
	uint8_t command;
	uint8_t pattern;
	uint8_t direction;
	uint32_t onColor;
	uint32_t offColor;
	uint32_t onTime;
	uint32_t offTime;
	uint32_t bounceTime;
	uint8_t clearAfter;
	uint8_t clearEnd;
} bounce_t;

typedef struct CommandMiddle
{
	uint8_t command;
	uint8_t direction;
	uint32_t onColor;
	uint32_t offColor;
	uint32_t onTime;
	uint32_t offTime;
	uint8_t clearAfter;
	uint8_t clearEnd;
} middle_t;

typedef struct CommandRainbowFade
{
	uint8_t command;
	uint32_t fadeTime;
} rainbow_fade_t;

typedef struct CommandRainbowFadeOne
{
	uint8_t command;
	uint32_t fadeTime;
} rainbow_fade_one_t;

typedef struct CommandRandomFlash
{
	uint8_t command;
	uint32_t onTime;
	uint32_t offTime;
} random_flash_t;

typedef struct CommandRandomFlashColor
{
	uint8_t command;
	uint32_t onColor;
	uint32_t offColor;
	uint32_t onTime;
	uint32_t offTime;
	uint8_t maxOn;
} random_flash_color_t;


typedef struct CommandSetAll
{
	uint8_t command;
	uint32_t onColor;
} set_all_t;


static pattern_t *cmdPattern;
static runway_t *cmdRunway;
static bounce_t *cmdBounce;
static middle_t *cmdMiddle;
static rainbow_fade_t *cmdRainbowFade;
static rainbow_fade_one_t *cmdRainbowFadeOne;
static random_flash_t *cmdRandomFlash;
static random_flash_color_t *cmdRandomFlashColor;
static set_all_t *cmdSetAll;

void dumpBuffer(uint8_t *buf, uint8_t len);
void dumpCommandRunway(uint8_t *buf);
void dumpCommandBounce(uint8_t *buf);



//Do not add code below this line
#endif /* _NeoWirelessLib_H_ */
