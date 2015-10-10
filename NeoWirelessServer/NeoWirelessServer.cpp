/**
 * Copyright (c) 2015 Thomas M. Sasala
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT license.
 *
 */

#include "NeoWirelessServer.h"

void check_radio(void);

// Hardware configuration
RF24 radio(7, 8); // ce=7, cs=8

// Set address for channels
byte address[][5] =
{ 0xCC, 0xCE, 0xCC, 0xCE, 0xCC, 0xCE, 0xCC, 0xCE, 0xCC, 0xCE };

static uint32_t message_count = 0;
static uint32_t message_good = 0;
static uint32_t message_fail = 0;
static uint8_t commandBuffer[MAX_COMMAND_SIZE];

uint8_t sendCommand();
void test();

/**
 *
 */
void setup()
{

    Serial.begin(115200);
    printf_begin();
    printf("\n\rNeoPixelWirelessServer\n\r");

    // Setup and configure radio
    radio.begin();
    radio.enableAckPayload(); // enable payload ack
    radio.enableDynamicPayloads(); // Ack payloads are dynamic payloads

    radio.openWritingPipe(address[0]);
    radio.openReadingPipe(1, address[1]);
    radio.printDetails();
    delay(50);
    attachInterrupt(0, check_radio, LOW); // Attach interrupt handler to interrupt #0 (using pin 2) on BOTH the sender and receiver

    randomSeed(analogRead(0));              //Seed for random number generation

    for (int i = 0; i < MAX_COMMAND_SIZE; i++)
    {
        commandBuffer[i] = 0xff;
    }

    radio.powerUp();                        //Power up the radio

}

/**
 *
 */
void loop()
{

    test();

}

/**
 * Tests all commands
 */void test()
{
     // Test all on
     printf("All\n\r");
     cmdSetAll = (set_all_t *)commandBuffer;
     cmdSetAll->command = CMD_SET_ALL;
     cmdSetAll->color = WHITE;
     sendCommand();
     delay(4000);
     cmdSetAll->color = RED;
     sendCommand();
     delay(4000);
     cmdSetAll->color = BLACK;
     sendCommand();
     delay(4000);


    // Test Pattern
    printf("Pattern\n\r");
    cmdPattern = (pattern_t *) commandBuffer;
    cmdPattern->command = CMD_PATTERN;
    cmdPattern->pattern = 0x33;
    cmdPattern->direction = LEFT;
    cmdPattern->onColor = RED;
    cmdPattern->offColor = WHITE;
    cmdPattern->onTime = 250;
    cmdPattern->offTime = 250;
    sendCommand();
    delay(4000);

    // Test Runway
    printf("Runway\n\r");
    cmdRunway = (runway_t *) commandBuffer;
    cmdRunway->command = CMD_RUNWAY;
    cmdRunway->pattern = 2;
    cmdRunway->direction = LEFT;
    cmdRunway->onColor = RED;
    cmdRunway->offColor = BLUE;
    cmdRunway->onTime = 100;
    cmdRunway->offTime = 0;
    cmdRunway->clearAfter = true;
    cmdRunway->clearEnd = true;
    sendCommand();
    delay(6000);

    cmdRunway->onTime = 50;
    sendCommand();
    delay(4000);

    cmdRunway->onTime = 25;
    sendCommand();
    delay(4000);

    // Test bounce
    printf("Bounce\n\r");
    cmdBounce = (bounce_t *) commandBuffer;
    cmdBounce->command = CMD_BOUNCE;
    cmdBounce->pattern = 0x01;
    cmdBounce->direction = LEFT;
    cmdBounce->onColor = RED;
    cmdBounce->offColor = BLUE;
    cmdBounce->onTime = 25;
    cmdBounce->offTime = 0;
    cmdBounce->bounceTime = 0;
    cmdBounce->clearAfter = true;
    cmdBounce->clearEnd = true;
    sendCommand();
    delay(6000);

    // Test Middle
    printf("Middle\n\r");
    cmdMiddle = (middle_t *) commandBuffer;
    cmdMiddle->command = CMD_MIDDLE;
    cmdMiddle->direction = LEFT;
    cmdMiddle->onColor = RED;
    cmdMiddle->offColor = BLUE;
    cmdMiddle->onTime = 100;
    cmdMiddle->offTime = 0;
    cmdMiddle->clearAfter = true;
    cmdMiddle->clearEnd = true;
    sendCommand();
    delay(2000);
    cmdMiddle->direction = RIGHT;
    sendCommand();
    delay(2000);

    // Test rainbow fade
    printf("Fade\n\r");
    cmdRainbowFade = (rainbow_fade_t *) commandBuffer;
    cmdRainbowFade->command = CMD_RAINBOW_FADE;
    cmdRainbowFade->fadeTime = 50;
    sendCommand();
    delay(4000);

    // Test rainbow fade one
    printf("Fade One\n\r");
    cmdRainbowFadeOne = (rainbow_fade_one_t *) commandBuffer;
    cmdRainbowFadeOne->command = CMD_RAINBOW_FADE_ONE;
    cmdRainbowFadeOne->fadeTime = 50;
    sendCommand();
    delay(4000);

    // Test random flash
    printf("Random Flash\n\r");
    cmdRandomFlash = (random_flash_t *) commandBuffer;
    cmdRandomFlash->command = CMD_RANDOM_FLASH;
    cmdRandomFlash->onTime = 25;
    cmdRandomFlash->offTime = 0;
    sendCommand();
    delay(5000);

    // Test random flash with color
    printf("Random Flash Color\n\r");
    cmdRandomFlashColor = (random_flash_color_t *) commandBuffer;
    cmdRandomFlashColor->command = CMD_RANDOM_FLASH_COLOR;
    cmdRandomFlashColor->onColor = RED;
    cmdRandomFlashColor->offColor = BLUE;
    cmdRandomFlashColor->onTime = 25;
    cmdRandomFlashColor->offTime = 0;
    cmdRandomFlashColor->maxOn = 1;
    sendCommand();
    delay(5000);

}

/**
 * Send a command
 */
uint8_t sendCommand()
{
    uint8_t flag = false;

    flag = radio.writeFast(commandBuffer, MAX_COMMAND_SIZE);
    if (!flag)
    {
        printf("Error sending Command: %d\n\r", commandBuffer[0]);
        message_fail++;
    }

    return flag;
}

/**
 *
 */
void check_radio(void)
{

    bool tx, fail, rx;

    // What happened?
    radio.whatHappened(tx, fail, rx);

    if (tx)
    {
        // Have we successfully transmitted?
        message_good++;
        //		printf("Send:OK\n\r");
    }

    if (fail)
    {
        // Have we failed to transmit?
        message_fail++;
        //		printf("Send:Failed\n\r");
    }

    if (rx || radio.available())
    {
        // Did we receive a message?
        radio.read(&message_count, sizeof(message_count));
        printf("Ack:%lu\n\r", message_count);
    }
}
