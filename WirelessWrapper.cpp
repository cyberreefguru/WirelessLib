/*
 * WirelessWrapper.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: tsasala
 */

#include "WirelessWrapper.h"


/**
 * Constructor
 *
 */
WirelessWrapper::WirelessWrapper()
{
	mode = MODE_CLIENT;
	message_count = 0;
	message_good = 0;
	message_fail = 0;
	commandAvailable = false;

}

/**
 * Initializes the class
 *
 */
uint8_t WirelessWrapper::initialize(uint8_t m)
{
	uint8_t flag = false;

//    Serial.begin(115200);
//    printf_begin();

    // Set the mode
    mode = m;

    if( mode == MODE_CLIENT )
    {
		Serial.println(F("\n\n** Client ** \n\n"));
		Serial.println(F("Reading client configuration...."));
		if( !config.read() )
		{
			Serial.println(F("General error reading configuration; initializing...\n"));
			if( config.initialize( CONFIG_VERSION, CONFIG_NODE_ID, CONFIG_NODES ) )
			{
				Serial.println(F("Successfully Initialized.\n"));
				flag = true;
			}
		}
		else
		{
			Serial.println(F("Read Configuration; checking version...\n"));
			if( config.getVersion() != CONFIG_VERSION )
			{
				Serial.println(F("Configuration Version not matched; initializing...\n"));
				if( config.initialize( CONFIG_VERSION, CONFIG_NODE_ID, CONFIG_NODES ) )
				{
					Serial.println(F("Successfully Re-initialized.\n"));
					flag = true;
				}
				else
				{
					Serial.println(F("Error Re-initializing configuration.\n"));
				}
			}
			else
			{
				Serial.println(F("Version matches; configuring radio..."));
				flag = true;
			}
		}
		config.dump();

		if(flag)
		{
			// Setup and configure radio
			radio.begin();
			radio.enableAckPayload(); // enable payload ack
			radio.enableDynamicPayloads(); // Ack payloads are dynamic payloads
			initializeNode(); // TODO: see if we can move this after writeAckPayload
			radio.writeAckPayload(1, &message_count, sizeof(message_count));
			message_count++;
			Serial.println(F("Radio Details:"));
			radio.printDetails(); // Dump the configuration of the rf unit for debugging
			delay(50);

			// Attach interrupt handler to interrupt #0 (using pin 2) on BOTH the sender and receiver
			attachInterrupt(0, check_radio, LOW);
		}

    }
    else if( mode == MODE_SERVER )
    {
		Serial.println(F("\n\n** Server ** \n\n"));
		Serial.println(F("Reading configuration...."));

		if( !config.read() )
		{
			Serial.println(F("General error reading configuration; initializing...\n"));
			if( config.initialize( CONFIG_VERSION, CONFIG_NODE_ID, CONFIG_NODES ) )
			{
				Serial.println(F("Successfully Initialized.\n"));
				flag = true;
			}
		}
		else
		{
			Serial.println(F("Read Configuration; checking version...\n"));
			if( config.getVersion() != CONFIG_VERSION )
			{
				Serial.println(F("Configuration Version not matched; initializing...\n"));
				if( config.initialize( CONFIG_VERSION, CONFIG_NODE_ID, CONFIG_NODES ) )
				{
					Serial.println(F("Successfully Re-initialized.\n"));
					flag = true;
				}
				else
				{
					Serial.println(F("Error Re-initializing configuration.\n"));
				}
			}
			else
			{
				Serial.println(F("Version matches; configuring radio..."));
				flag = true;
			}
		}
		config.dump();

		if( flag )
		{
			// Setup and configure radio
			radio.begin();
			radio.enableAckPayload(); // enable payload ack
			radio.enableDynamicPayloads(); // Ack payloads are dynamic payloads
			Serial.println(F("\nRadio Details:"));
			radio.printDetails();
			delay(50);

			for (int i = 0; i < MAX_COMMAND_SIZE; i++)
			{
				commandBuffer[i] = 0xff;
			}

			radio.powerUp();                        //Power up the radio
		}

    }

	Serial.println(F("Initialization complete."));

    return flag;

} // end initialize

/**
 * Returns the node ID
 */
uint8_t WirelessWrapper::getNodeId()
{
	return config.getNodeId();
}

/**
 * Sets the Node ID
 *
 */
uint8_t WirelessWrapper::setNodeId(uint8_t id)
{
	uint8_t flag = false;

	config.setNodeId( id );
	flag = config.write();
	initializeNode();

	return flag;
}

void WirelessWrapper::initializeNode()
{
	// Set the node ID
	addresses[0][0] = config.getNodeId();
	// Stop listening
	radio.stopListening();
	// Open new pipe with new ID
	radio.openWritingPipe(addresses[0]);
	radio.openReadingPipe(1, addresses[0]);
	// Start listening for commands
	radio.startListening();
}

void WirelessWrapper::dump()
{
	config.dump();
	radio.printDetails();

}

/**
 * Send a command
 */
uint8_t WirelessWrapper::sendCommand()
{
    uint8_t flag = false;

    for(uint8_t i=0; i < config.getNumberNodes(); i++ )
    {
    	flag = sendCommand(i);
    }
//    flag = radio.writeFast(commandBuffer, MAX_COMMAND_SIZE);
//    if (!flag)
//    {
//        printf("Error sending Command: %d\n\r", commandBuffer[0]);
//        message_fail++;
//    }


    return flag;
}


uint8_t WirelessWrapper::sendCommand(uint8_t channel)
{
	uint8_t flag = false;

    radio.openWritingPipe(addresses[channel]);
    flag = radio.write( (const void *)commandBuffer, MAX_COMMAND_SIZE );
    if( flag == true )
    {
    	if( radio.available() )
    	{
            radio.read(&message_count, sizeof(message_count));
            printf("Ack Command 0x%02x from Node %d: %lu\n\r", commandBuffer[0], channel, message_count);
    	}
    	else
    	{
    		flag = false;
			printf("Error getting ACK Command 0x%02x from Node %d.\n\r", commandBuffer[0], channel);
    	}
    }
    else
    {
			printf("Error sending Command 0x%02x to Node %s.\n\r", commandBuffer[0], addresses[channel]);
    }
	return flag;
}





/**
 * Checks the radio for server traffic
 *
 */
void WirelessWrapper::checkRadio(void)
{

    bool tx, fail, rx;

    Serial.println(F("Checking radio..."));

    // What happened?
    radio.whatHappened(tx, fail, rx);

    if (tx)
    {
        // Have we successfully transmitted?
        Serial.println(F("Send OK"));
        message_good++;
    }

    if (fail)
    {
        // Have we failed to transmit?
        Serial.println(F("Send Failed"));
        message_fail++;
        //		printf("Send:Failed\n\r");
    }

    if (rx || radio.available())
    {
        Serial.println(F("Msg Received"));
    	if( mode == MODE_SERVER )
    	{
			// Did we receive a message?
			radio.read(&message_count, sizeof(message_count));
			printf("Ack:%lu\n\r", message_count);
    	}
    	else if( mode == MODE_CLIENT )
    	{
            Serial.println(F("Client Msg"));
    		// get payload and store in buffer; set command flag
    		while (radio.available())
    		{
    			radio.read((void *) commandBuffer, MAX_COMMAND_SIZE);
    		}
    		Serial.print(F("Message Count: "));
    		Serial.println( message_count );
    		Helper::dumpBuffer( (uint8_t *)commandBuffer, MAX_COMMAND_SIZE );
    		radio.writeAckPayload(1, &message_count, sizeof(message_count));
    		++message_count;
    		commandAvailable = true;
    	}
    	else
    	{
            Serial.println(F("Error - unknown mode!"));
    	}
    }
}




/**
 * Internal delay function that checks for an available command.
 * If a command is available, it will break out of the delay
 * routine and return true.  If delay times out, false is returned.
 *
 * @time the time to wait in ms
 * @return boolean, true=command available; false=no command available
 */
uint8_t WirelessWrapper::commandDelay(uint32_t time)
{
	boolean cmd = isCommandAvailable();
	if( !cmd )
	{
		for (uint32_t i = 0; i < time; i++)
		{
			delay(1);
			cmd = isCommandAvailable();
			if (cmd)
			{
				break;
			}
		}
	}
	return cmd;
}


/**
 * Checks if a command is available
 */
uint8_t WirelessWrapper::isCommandAvailable()
{
	return commandAvailable;
}

/**
 * Sets command available flage
 */
void WirelessWrapper::setCommandAvailable(uint8_t flag)
{
	commandAvailable = flag;
}

uint8_t* WirelessWrapper::getCommandBuffer()
{
	return (uint8_t *)&commandBuffer[0];
}

/**
 * Checks radio status
 *
 */
void check_radio()
{
	wireless.checkRadio();
}

void setCommandAvailable(uint8_t flag )
{
	wireless.setCommandAvailable(flag);
}

volatile uint8_t * getCommandBuffer()
{
	return wireless.getCommandBuffer();
}

/**
 * Returns flag if a new command is available
 *
 */
uint8_t isCommandAvailable()
{
	return wireless.isCommandAvailable();
}

/**
 * Delay function with command check
 */
uint8_t commandDelay(uint32_t time)
{
	return wireless.commandDelay(time);

}

