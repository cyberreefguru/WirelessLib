/*
 * Configuration.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: tsasala
 */

#include "Configuration.h"

Configuration::Configuration()
{
	version = DEFAULT_VERSION;
	nodeId = DEFAULT_NODE_ID;
	numberNodes = DEFAULT_NUM_NODES;
	crc = 0;
}

uint8_t Configuration::initialize(uint8_t v, uint8_t n, uint8_t nn)
{
	version = v;
	nodeId = n;
	numberNodes = nn;
	crc = computeChecksum( (uint8_t *)&version, CRC_SIZE );

	return write();

}
void Configuration::initialize()
{
	crc = computeChecksum( (uint8_t *)&version, CRC_SIZE);
}

uint8_t Configuration::getNodeId()
{
	return nodeId;
}

void Configuration::setNodeId(uint8_t nodeId)
{
	this->nodeId = nodeId;
}

uint8_t Configuration::getVersion()
{
	return version;
}

void Configuration::setVersion(uint8_t version)
{
	this->version = version;
}

uint8_t Configuration::getNumberNodes()
{
	return numberNodes;
}

void Configuration::setNumberNodes(uint8_t numberNodes)
{
	this->numberNodes = numberNodes;
}

uint8_t Configuration::getCrc()
{
	return crc;
}

void Configuration::setCrc(uint8_t crc)
{
	this->crc = crc;
}


void Configuration::dump()
{
	printf("Client Configuration:\n");
	printf("Version     : 0x%02x\n", version);
	printf("Node ID     : 0x%02x\n", nodeId);
	printf("Number Nodes: 0x%02x\n", numberNodes);
	printf("CRC         : 0x%02x\n\n", crc);
}

/**
 * Reads the configuration
 *
 */
uint8_t Configuration::read()
{
	uint8_t flag = false;
	uint8_t checksum = 0;
	int address = CONFIG_START_ADDRESS;

	version = EEPROM.read(address++);
	nodeId = EEPROM.read(address++);;
	numberNodes = EEPROM.read(address++);
	crc = EEPROM.read(address++);;

	checksum = computeChecksum((uint8_t *)&version, CRC_SIZE);

	flag = (checksum == crc);

	return flag;

}

/**
 * Writes the configuration
 */
uint8_t Configuration::write()
{
	uint8_t flag = false;
	uint16_t address = CONFIG_START_ADDRESS;

	crc = computeChecksum((uint8_t *)&version, CRC_SIZE);
	EEPROM.write(address++, version);
	EEPROM.write(address++, nodeId);
	EEPROM.write(address++, numberNodes);
	EEPROM.write(address, crc);

	flag = ( EEPROM.read(address) == crc );

	return flag;

}



/**
 * Computes CRC8 using data provided.
 *
 * CRC-8 - based on the CRC8 formulas by Dallas/Maxim
 * code released under the terms of the GNU GPL 3.0 license
 */
uint8_t Configuration::computeChecksum(uint8_t *data, uint8_t len)
{
	uint8_t crc = 0x00;
	while (len--)
	{
		uint8_t extract = *data++;
		for (uint8_t tempI = 8; tempI; tempI--)
		{
			uint8_t sum = (crc ^ extract) & 0x01;
			crc >>= 1;
			if (sum)
			{
				crc ^= 0x8C;
			}
			extract >>= 1;
		}
	}
	return crc;
}
