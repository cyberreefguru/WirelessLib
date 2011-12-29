/*
 * main.c
 *
 *  Created on: Feb 7, 2010
 *      Author: tmsasala
 */
#include "PlayerApp.h"


uint8_t frameData1[8]  = {0x00,0x00,0x00,0x00};
uint8_t frameData2[8]  = {0x00,0xff,0x00,0xff};
uint8_t frameData3[8]  = {0xff,0xff,0xff,0xff};
uint8_t frameData4[8]  = {0xff,0x00,0xff,0x00};
uint8_t frameData9[8]  = {0x00,0x00,0x00,0xff};
uint8_t frameData10[8] = {0x00,0x00,0xff,0x00};
uint8_t frameData11[8] = {0x00,0xff,0x00,0x00};
uint8_t frameData12[8] = {0xff,0x00,0x00,0x00};

// {repeat, holdBefore, holdAfter, *frameData }
pattern pattern1 = {1, 0, 250, frameData1};
pattern pattern2 = {1, 0, 250, frameData2};
pattern pattern3 = {1, 0, 250, frameData3};
pattern pattern4 = {1, 0, 250, frameData4};
pattern pattern5 = {1, 0, 1000, frameData1};

pattern hb1 = {1, 0, 200, frameData3};
pattern hb2 = {1, 0, 125, frameData1};
pattern hb3 = {1, 0, 200, frameData3};
pattern hb4 = {1, 0, 1250, frameData1};

pattern heartBeat[4] = { hb1, hb2, hb3, hb4 };

uint8_t chase1[8]  = {0xff,0x00,0x00,0x00};
uint8_t chase2[8]  = {0x00,0xff,0x00,0x00};
uint8_t chase3[8]  = {0x00,0x00,0xff,0x00};
uint8_t chase4[8]  = {0xff,0x00,0x00,0xff};
uint8_t chase5[8]  = {0x00,0xff,0x00,0x00};
uint8_t chase6[8]  = {0x00,0x00,0xff,0x00};

pattern c1 = {1, 0, 200, chase1};
pattern c2 = {1, 0, 200, chase2};
pattern c3 = {1, 0, 200, chase3};
pattern c4 = {1, 0, 200, chase4};
pattern c5 = {1, 0, 200, chase5};
pattern c6 = {1, 0, 200, chase6};

pattern chase_1[2] = {c1, c2};
pattern chase_2[4] = {c3, c4, c5, c6};


void test_output();
void shift_matrix_down(uint8_t *buffer);
void shift_matrix_left(uint8_t *buffer);

int main(void)
{
	uint8_t buffer[4];
	uint8_t buffer1[4];
	uint8_t buffer2[4];
	memset(buffer, 0x00, 4);
	memset(buffer1, 0x00, 4);
	memset(buffer2, 0x00, 4);

	init();

	// Initialize serial port for debugging
	Serial.begin(57600);

	Player.initializePlayer(4, 3, buffer1, buffer2 );

//	// Initialize matrix
//	Matrix.initializeMatrix(4, 3, buffer1, buffer2);
//	Matrix.initializeTimer();
//	// Display sample patterns
//	test_output();
//	// Start timer
//	Matrix.enableTimer();

	Player.play(chase_1, 2);

	while(1)
	{
		Player.play(chase_2, 3);
	}


//	while(1)
//	{
//		shift_matrix_left(buffer);
//		shift_matrix_down(buffer);
//	}



	// Just in case
	Serial.println("Complete.");
	IoPort.write_value(OUTPUT_PORT_1, 0x00);
	IoPort.write_value(OUTPUT_PORT_2, 0xff);
	while(1);


}

void test_output()
{

	// Turn on all LEDs
	IoPort.write_value(OUTPUT_PORT_1, 0xff);
	IoPort.write_value(OUTPUT_PORT_2, 0x00);
	delay(500);

	// Turn off all LEDs
	IoPort.write_value(OUTPUT_PORT_1, 0x00);
	delay(500);

	// Turn on columns
	IoPort.write_value(OUTPUT_PORT_1, 0x04);
	delay(500);
	IoPort.write_value(OUTPUT_PORT_1, 0x02);
	delay(500);
	IoPort.write_value(OUTPUT_PORT_1, 0x01);
	delay(500);
	IoPort.write_value(OUTPUT_PORT_1, 0x00);
	delay(500);

}

void shift_matrix_left(uint8_t *buffer)
{
	uint8_t value;

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<3; j++)
		{
			value = 0x4>>j;
			buffer[i] = value;
			Matrix.writeFrame( buffer );
			buffer[i] = 0x00;
			delay(100);
		}
	}

}

void shift_matrix_down(uint8_t *buffer)
{
	uint8_t value;

	for(int i=0; i<3; i++)
	{
		value = 1<<i;
		for(int j=0; j<4; j++)
		{
			buffer[j] = value;
			Matrix.writeFrame( buffer );
			buffer[j] = 0x00;
			delay(100);
		}
	}

}


extern "C" void __cxa_pure_virtual()
{
  cli();
  for (;;);
}


