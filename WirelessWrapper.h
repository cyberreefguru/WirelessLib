/*
 * WirelessWrapper.h
 *
 *  Created on: Oct 20, 2015
 *      Author: tsasala
 */

#ifndef WIRELESSWRAPPER_H_
#define WIRELESSWRAPPER_H_

#include "Arduino.h"
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
//#include "printf.h"
#include "Configuration.h"
#include "Helper.h"

#define MODE_CLIENT 	0
#define MODE_SERVER		1
#define MAX_NUM_NODES	6

#define MAX_COMMAND_SIZE		27

#define CONFIG_VERSION 	CONFIG_V2
#define CONFIG_NODE_ID	'1'
#define CONFIG_NODES	2


// Hardware configuration
static RF24 radio(7, 8); // ce=7, cs=8

// Address of nodes
static uint8_t addresses[MAX_NUM_NODES][6] = { "1node", "2node", "3node", "4node", "5node", "6node" };

/**
 * Class definition
 */
class WirelessWrapper
{
public:
	WirelessWrapper();

	uint8_t initialize(uint8_t mode);

	void dump();
	void checkRadio(void);


	uint8_t getNodeId();
	uint8_t setNodeId(uint8_t id);

	uint8_t commandDelay(uint32_t time);
	uint8_t isCommandAvailable();
	void setCommandAvailable(uint8_t flag);
	uint8_t * getCommandBuffer();

	uint8_t sendCommand();
	uint8_t sendCommand(uint8_t channel);


private:

	void initializeNode();

	uint8_t mode;
	uint32_t message_count;
	uint32_t message_good;
	uint32_t message_fail;
	volatile uint8_t commandAvailable;
	volatile uint8_t commandBuffer[MAX_COMMAND_SIZE];

	Configuration config;

};

//end of add your includes here
#ifdef __cplusplus
extern "C"
{
#endif

static WirelessWrapper wireless;

void check_radio();
uint8_t isCommandAvailable();
uint8_t commandDelay(uint32_t time);
volatile uint8_t * getCommandBuffer();
void setCommandAvailable(uint8_t flag );



#ifdef __cplusplus
} // extern "C"
#endif

#endif /* WIRELESSWRAPPER_H_ */
