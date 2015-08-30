// Do not remove the include below
#include "NeoWirelessLib.h"


void clearAll()
{
	cmdPattern = 0;
	cmdRunway = 0;
	cmdBounce = 0;
	cmdMiddle = 0;
	cmdRainbowFade = 0;
	cmdRainbowFadeOne = 0;
	cmdRandomFlash = 0;
	cmdRandomFlashColor = 0;
	cmdSetAll = 0;

}


void dumpBuffer(uint8_t *buf, uint8_t len)
{
	printf("Command=");
	for(uint8_t i=0; i<len; i++)
	{
		printf("%.2X ", buf[i]);
	}
	printf("\n\r");
}

void dumpCommandBounce(uint8_t *buf)
{
	bounce_t *cmd;
	cmd = (bounce_t *)buf;

	printf("Command: %d\r\n", cmd->command);
	printf("Pattern: %d\r\n", cmd->pattern);
	printf("Direction: %d\r\n", cmd->direction);
	printf("Color1: 0x%08lx\r\n", cmd->onColor);
	printf("Color2: 0x%08lx\r\n", cmd->offColor);
	printf("onTime: 0x%08lx\r\n", cmd->onTime);
	printf("offTime: 0x%08lx\r\n", cmd->offTime);
	printf("bounceTime: 0x%08lx\r\n", cmd->bounceTime);
	printf("clearAfter: 0x%02x\r\n", cmd->clearAfter);
	printf("clearEnd: 0x%02x\r\n", cmd->clearEnd);

}
void dumpCommandRunway(uint8_t *buf)
{
	runway_t *cmd;
	cmd = (runway_t *)buf;

	printf("Command: %d\r\n", cmd->command);
	printf("Total: %d\r\n", cmd->pattern);
	printf("Direction: %d\r\n", cmd->direction);
	printf("Color1: 0x%08lx\r\n", cmd->onColor);
	printf("Color2: 0x%08lx\r\n", cmd->offColor);
	printf("onTime: 0x%08lx\r\n", cmd->onTime);
	printf("offTime: 0x%08lx\r\n", cmd->offTime);
	printf("clearAfter: 0x%02x\r\n", cmd->clearAfter);
	printf("clearEnd: 0x%02x\r\n", cmd->clearEnd);

}

void dumpCommandSetAll(uint8_t *buf)
{
	set_all_t *cmd;
	cmd = (set_all_t *)buf;

	printf("Command: %d\r\n", cmd->command);
	printf("Color: 0x%08lx\r\n", cmd->onColor);

}
