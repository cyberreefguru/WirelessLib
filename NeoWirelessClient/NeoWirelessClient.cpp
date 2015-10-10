/**
 * Copyright (c) 2015 Thomas M. Sasala
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT license.
 *
 */
#include "NeoWirelessClient.h"

void check_radio(void);
//void setWipeColor(uint32_t newColor, uint16_t index, uint32_t onTime, uint32_t offTime, uint8_t clearAfter);
//void writeColor(uint32_t color, uint8_t show);
//void colorWipe(uint8_t pattern, uint8_t direction, uint32_t color1, uint32_t color2, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd);
//void pattern(uint8_t pattern, uint8_t direction, uint32_t onColor, uint32_t offColor, uint32_t onTime, uint32_t offTime);
//void colorBounce(uint8_t pattern, uint8_t direction, uint32_t color1, uint32_t color2, uint32_t onTime, uint32_t offTime, uint32_t bounceTime, uint8_t clearAfter, uint8_t clearEnd);
//void middle(uint8_t direction, uint32_t color1, uint32_t color2, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd);
//void randomFlashColor(uint32_t onTime, uint32_t offTime, uint32_t onColor, uint32_t offColor);
//void randomFlash(uint32_t onTime, uint32_t offTime);
//void rainbowFadeOne(uint32_t fadeTime);
//void rainbowFadeStrip(uint32_t fadeTime);
void parseCommand();


//uint8_t commandDelay(uint32_t time);

//uint32_t Wheel(byte WheelPos);

NeoPixelWrapper controller = NeoPixelWrapper();
volatile uint32_t endTime;
//void commandDelay(uint32_t time);

boolean isCommandAvailable();


// Hardware configuration
RF24 radio(7, 8); // ce=7, cs=8
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIN);

// Set address for channels
byte address[][5] =
		{ 0xCC, 0xCE, 0xCC, 0xCE, 0xCC, 0xCE, 0xCC, 0xCE, 0xCC, 0xCE };

static uint32_t message_count = 0;
static uint32_t message_good = 0;
static uint32_t message_fail = 0;
static volatile uint8_t commandAvailable = false;
static volatile uint8_t commandBuffer[MAX_COMMAND_SIZE];

/**
 *
 */
void setup() {

	Serial.begin(115200);
	printf_begin();
	printf("\n\rNeoPixelWirelessClient\n\r");

	// Setup and configure radio
	radio.begin();
	radio.enableAckPayload(); // enable payload ack
	radio.enableDynamicPayloads(); // Ack payloads are dynamic payloads

	radio.openWritingPipe(address[1]);
	radio.openReadingPipe(1, address[0]);
	radio.startListening(); // we're the client, so start listening
	radio.writeAckPayload(1, &message_count, sizeof(message_count));
	++message_count;
	radio.printDetails(); // Dump the configuration of the rf unit for debugging
	delay(50);
	attachInterrupt(0, check_radio, LOW); // Attach interrupt handler to interrupt #0 (using pin 2) on BOTH the sender and receiver

	if( controller.initialize(50, 2) == false )
	{

	}
	else
	{
		controller.writeColor(CRGB::Black, true);
	}

}

/**
 *
 */
void loop() {

	// Wait for command
	if (commandAvailable)
	{
		parseCommand();

	} // end commandAvailable

} // end loop

/**
 * Handles interrupts from the radio
 *
 *
 */
void check_radio(void) {

	bool tx, fail, rx;

	// What happened?
	radio.whatHappened(tx, fail, rx);

	if (tx) {
		// Have we successfully transmitted?
		message_good++;
//		printf("Ack Payload:Sent\n\r");
	}

	if (fail) {
		// Have we failed to transmit?
		message_fail++;
//		printf("Ack Payload:Failed\n\r");
	}

	// Did we receive a message?
	if (rx || radio.available()) {

		// get payload and store in buffer; set command flag
	     while(radio.available()){
	      radio.read((void *)commandBuffer,MAX_COMMAND_SIZE);
	     }
	     radio.writeAckPayload(1, &message_count, sizeof(message_count));
	     ++message_count;
	     commandAvailable = true;
	}
}

void parseCommand()
{
	commandAvailable = false;
	printf("COMMAND RECEIVED: 0x%02x - ", commandBuffer[0]);
	switch (commandBuffer[0]) {
	case CMD_PATTERN:
		printf("PATTERN\n\r");
		cmdPattern = (pattern_t *)commandBuffer;
		controller.pattern(cmdPattern->pattern, cmdPattern->direction, cmdPattern->onColor, cmdPattern->offColor, cmdPattern->onTime, 0);
		break;
	case CMD_RUNWAY:
		printf("RUNWAY\n\r");
		dumpCommandRunway((uint8_t *)commandBuffer);
		cmdRunway = (runway_t *)commandBuffer;
		controller.colorWipe(cmdRunway->pattern, cmdRunway->direction, cmdRunway->onColor, cmdRunway->offColor, cmdRunway->onTime, 0, cmdRunway->clearAfter, cmdRunway->clearEnd);
		break;
	case CMD_BOUNCE:
		printf("BOUNCE\n\r");
		cmdBounce = (bounce_t *)commandBuffer;
		dumpCommandBounce((uint8_t *)commandBuffer);
		controller.colorBounce(cmdBounce->pattern, cmdBounce->direction, cmdBounce->onColor, cmdBounce->offColor, cmdBounce->onTime, 0, 0, cmdBounce->clearAfter, cmdBounce->clearEnd);
		break;
	case CMD_MIDDLE:
		printf("MIDDLE\n\r");
		cmdMiddle = (middle_t *)commandBuffer;
		controller.middle(cmdMiddle->direction, cmdMiddle->onColor, cmdMiddle->offColor, cmdMiddle->onTime, cmdMiddle->offTime, cmdMiddle->clearAfter, cmdMiddle->clearEnd);
		break;
	case CMD_RAINBOW_FADE:
		printf("RAINBOW_FADE\n\r");
		cmdRainbowFade = (rainbow_fade_t *)commandBuffer;
		controller.rainbow( 0 );
		break;
	case CMD_RAINBOW_FADE_ONE:
		printf("CMD_RAINBOW_FADE_ONE\n\r");
		cmdRainbowFadeOne = (rainbow_fade_one_t *)commandBuffer;
		controller.fadeRainbow();
		break;
	case CMD_RANDOM_FLASH:
		printf("RANDOM_FLASH\n\r");
		cmdRandomFlash = (random_flash_t *)commandBuffer;
		controller.randomFlash( cmdRandomFlash->onTime, cmdRandomFlash->offTime);
		break;
	case CMD_RANDOM_FLASH_COLOR:
		printf("RANDOM_FLASH_COLOR\n\r");
		cmdRandomFlashColor = (random_flash_color_t *)commandBuffer;
		controller.randomFlashColor( cmdRandomFlashColor->onTime, cmdRandomFlashColor->offTime, cmdRandomFlashColor->onColor, cmdRandomFlashColor->offColor );
		break;
	case CMD_SET_ALL:
		printf("SET_ALL\n\r");
		cmdSetAll = (set_all_t *)commandBuffer;
		controller.writeColor( cmdSetAll->color, true );
		break;
	case CMD_ERROR:
	default:
		printf("ERROR\n\r");
		break;
	}

}
//
///**
// * Sets strip to specified pattern
// */
//void setPattern(uint8_t pattern, uint32_t onColor, uint32_t offColor)
//{
//	uint16_t i, j;
//
//	j = 0;
//
//	for(i=0; i<pixels.numPixels(); i++)
//	{
//		if( (pattern>>j) & 0x01 )
//		{
//			pixels.setPixelColor(i, onColor);
//		}
//		else
//		{
//			pixels.setPixelColor(i, offColor);
//		}
//		j = j+1;
//		if( j == 8 )
//		{
//			j = 0;
//		}
//	}
//	pixels.show();
//
//}
//
///**
// * Rotates a pattern across the stripe; onTime determines pause between rotation
// */
//void pattern(uint8_t pattern, uint8_t direction, uint32_t onColor, uint32_t offColor, uint32_t onTime, uint32_t offTime)
//{
//	uint16_t i;
//
//	i = 0;
//	while(commandAvailable == false )
//	{
//		setPattern(pattern, onColor, offColor);
//		if( commandDelay(onTime) ) break;
//		if( direction == LEFT )
//		{
//			if( pattern & 0x80)
//			{
//				i = 0x01;
//			}
//			else
//			{
//				i = 0x00;
//			}
//			pattern = pattern<<1;
//			pattern = pattern | i;
//		}
//		else if (direction == RIGHT )
//		{
//			if( pattern & 0x01)
//			{
//				i = 0x80;
//			}
//			else
//			{
//				i = 0x00;
//			}
//			pattern = pattern>>1;
//			pattern = pattern | i;
//		}
//	}
//	printf("pattern: end");
//}
//
///**
// * Cycles all colors through the entire strip.
// *
// *
// */
//void rainbowFadeStrip(uint32_t fadeTime)
//{
//	uint16_t i, j;
//
//	while( commandAvailable == false )
//	{
//		for(j=0; j<256; j++)
//		{
//			for(i=0; i<pixels.numPixels(); i++)
//			{
//				pixels.setPixelColor(i, Wheel((j) & 255));
//				if( commandAvailable ) return;
//			}
//			pixels.show();
//			if( commandDelay(fadeTime) ) return;
//
//		} // end for
//	}
//}
//
///**
// * Cycles a rainbow through the length of a strip
// *
// */
//void rainbowFadeOne(uint32_t fadeTime)
//{
//	uint16_t i, j;
//	while( commandAvailable == false )
//	{
//		for(j=0; j<(256*5); j++) { // 5 cycles of all colors on wheel
//			for(i=0; i< pixels.numPixels(); i++) {
//				pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
//				if( commandAvailable ) return;
//			}
//			pixels.show();
//			if( commandDelay(fadeTime) ) return;
//		}
//	}
//}
//
///**
// * Flashes random LED with random color
// */
//void randomFlash(uint32_t onTime, uint32_t offTime)
//{
//	while(commandAvailable == false)
//	{
//		randomFlashColor(onTime, offTime, Wheel( random(0xff) ), 0x0);
//		if(commandAvailable) return;
//	}
//
//} // randomFlash
//
//
///**
// * Flashes random LED with specified color
// */
//void randomFlashColor(uint32_t onTime, uint32_t offTime, uint32_t onColor, uint32_t offColor)
//{
//	uint8_t i;
//
//	writeColor( offColor, true);
//
//	while(commandAvailable == false)
//	{
//		i = random(pixels.numPixels());
//		pixels.setPixelColor(i, onColor);
//		pixels.show();
//		if( commandDelay(onTime) ) return;
//		pixels.setPixelColor(i, offColor);
//		if( commandDelay(offTime) ) return;
//	}
//
//	writeColor( offColor, true);
//
//} // randomFlash
//
//
///**
// * Starts in the middle and works out; or starts in the end and works in
// */
//void middle(uint8_t direction, uint32_t color1, uint32_t color2, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd)
//{
//	uint8_t numPixels = pixels.numPixels();
//	uint8_t halfNumPixels = numPixels/2;
//
//	writeColor(color2, true);
//	if(direction == IN)
//	{
//		for(uint8_t i=0; i<halfNumPixels; i++)
//		{
//			pixels.setPixelColor(i, color1);
//			pixels.setPixelColor((numPixels-1)-i, color1);
//			pixels.show();
//			if( commandDelay(onTime) ) return;
//
//			if( clearAfter == true )
//			{
//				pixels.setPixelColor(i, color2);
//				pixels.setPixelColor((numPixels-1)-i, color2);
//				pixels.show();
//				if( commandDelay(offTime) ) return;
//			}
//		}
//	}
//	else if( direction == OUT )
//	{
//		for(uint8_t i=0; i<halfNumPixels; i++)
//		{
//			pixels.setPixelColor(halfNumPixels-i, color1);
//			pixels.setPixelColor(halfNumPixels+i, color1);
//			pixels.show();
//			if( commandDelay(onTime) ) return;
//
//			if( clearAfter == true )
//			{
//				pixels.setPixelColor(halfNumPixels-i, color2);
//				pixels.setPixelColor(halfNumPixels+i, color2);
//				pixels.show();
//				if( commandDelay(offTime) ) return;
//			}
//		}
//	}
//}
//
//
//
///**
// *
// */
//void colorBounce(uint8_t pattern, uint8_t direction, uint32_t color1, uint32_t color2, uint32_t onTime, uint32_t offTime, uint32_t bounceTime, uint8_t clearAfter, uint8_t clearEnd)
//{
//	printf("colorBounce: pattern=0x%02x, direction=%d, colorOn=0x%08lx, colorOff=0x%08lx, onTime=0x%08lx, offTime=0x%08lx, bounceTime=0x%08lx, clearAfter=%d, clearEnd=%d\n",
//			             pattern, direction, color1, color2, onTime, offTime, bounceTime, clearAfter, clearEnd);
//
//	if( direction == LEFT )
//	{
//		colorWipe(pattern, LEFT, color1, color2, onTime, offTime, clearAfter, clearEnd);
//		if(commandAvailable) return;
//		if( commandDelay(bounceTime) ) return;
//		colorWipe(pattern, RIGHT, color1, color2, onTime, offTime, clearAfter, clearEnd);
//		if(commandAvailable) return;
//		if( commandDelay(bounceTime) ) return;
//	}
//	else if (direction == RIGHT )
//	{
//		colorWipe(pattern, RIGHT, color1, color2, onTime, offTime, clearAfter, clearEnd);
//		if(commandAvailable) return;
//		if( commandDelay(bounceTime) ) return;
//		colorWipe(pattern, LEFT, color1, color2, onTime, offTime, clearAfter, clearEnd);
//		if(commandAvailable) return;
//		if( commandDelay(bounceTime) ) return;
//	}
//}
//
//
//
///**
// * Turns on LEDs one at time in sequence.  LEFT = 0->n; RIGHT = n -> 0
// *
// * @total - number of LEDs to wipe
// * @direction - left (up) or right (down)
// * @color - color to fill LEDs with
// * @waitTime - time to keep LED on
// * @clearAfter - turn LED off after waiting
// * @clearBetween - clear string in between repeats
// */
//void colorWipe(uint8_t pattern, uint8_t direction, uint32_t color1, uint32_t color2, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd)
//{
//
//	printf("colorWipe: %d, %d\n\r", clearAfter, clearEnd);
//	writeColor(color2, true);
//	if( direction == LEFT)
//	{
//		for(uint8_t i=0; i<pixels.numPixels(); i++)
//		{
//			setWipeColor(color1, i, onTime, offTime, clearAfter);
//			if(commandAvailable) return;
//		}
//	}
//	else if (direction == RIGHT )
//	{
//		for(uint8_t i=pixels.numPixels(); i>0; i--)
//		{
//			setWipeColor(color1, i, onTime, offTime, clearAfter);
//			if(commandAvailable) return;
//		}
//	}
//
//	if( clearEnd )
//	{
//		writeColor(color2, true);
//	}
//}
//
///**
// * Sets the color to wipe string with
// */
//void setWipeColor(uint32_t newColor, uint16_t index, uint32_t onTime, uint32_t offTime, uint8_t clearAfter)
//{
//	uint32_t curColor;
//
//	curColor = pixels.getPixelColor(index);
//	pixels.setPixelColor(index, newColor);
//	pixels.show();
//	commandDelay(onTime);
//	if(clearAfter == true)
//	{
//		pixels.setPixelColor(index,curColor);
//		pixels.show();
//	}
//	commandDelay(offTime);
//
//}
//
///**
// * sets a color to write to all pixels
// *
// * @color - color to set
// * @show - if true, sets color immediately
// */
//void writeColor(uint32_t color, uint8_t show)
//{
//	for(uint8_t i=0; i<NUM_PIXELS; i++)
//	{
//		pixels.setPixelColor(i, color);
//	}
//	if(show)
//	{
//		pixels.show();
//	}
//}
//
//
///**
// * Input a value 0-255 and get a RGB color back
// */
//uint32_t Wheel(byte WheelPos)
//{
//	if(WheelPos < 85) {
//		return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//	} else if(WheelPos < 170) {
//		WheelPos -= 85;
//		return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
//	} else {
//		WheelPos -= 170;
//		return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
//	}
//}

//uint8_t commandDelay(uint32_t time)
//{
//	uint8_t cmd = false;
//	for(uint32_t i = 0; i<time; i++)
//	{
//		delay(1);
//		if( commandAvailable )
//		{
//			cmd = true;
//			break;
//		}
//	}
//	return cmd;
//}

/**
 * Returns flag if a new command is available
 *
 */
boolean isCommandAvailable()
{
	return commandAvailable;
}

