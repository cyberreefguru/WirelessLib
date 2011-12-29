/*
 * SequencePlayer.cpp
 *
 *  Created on: Dec 31, 2010
 *      Author: tom
 */

#include "SequencePlayer.h"


/** Define the static variable for SequencePlayer */
SequencePlayer sequencePlayer;


/**
 * Constructor
 */
SequencePlayer::SequencePlayer()
{
}

/**
 * Initialize routine
 */
void SequencePlayer::initializePlayer(uint16_t rows, uint16_t cols)
{
	// Save pointer to project data
	numberRows = rows;
	numberCols = cols;


} // end initialize


void SequencePlayer::play(pattern *patterns, uint16_t len)
{
	uint16_t i, j, irow, icol;
	uint8_t outValue;

	Serial.println("Playing Sequence...");

	for(i=0; i<len; i++)
	{
		Serial.print("Sequence: ");
		Serial.println( i, HEX);

		outValue = 0;
		for(j=0; j<8; j++)
		{
			if( patterns[i].frameData[j] > 0 )
			{
				outValue |= (1<<j);
			}
		}

		Serial.print("Out Value: ");
		Serial.println( outValue, HEX);

		for(j=0; j<patterns[i].repeat; j++)
		{
			if( patterns[i].holdBefore > 0 )
			{
				Serial.print("Hold Before: ");
				Serial.println( patterns[i].holdBefore, HEX);
				delay(patterns[i].holdBefore);
			}
			else
			{
				Serial.println( "No Hold Before" );
			}
			IoPort.write_value(OUTPUT_PORT_1, outValue);
			if( patterns[i].holdAfter > 0 )
			{
				Serial.print("Hold After: ");
				Serial.println( patterns[i].holdBefore, HEX);
				delay(patterns[i].holdAfter);
			}
			else
			{
				Serial.println( "No Hold After" );
			}

		}
	}

}

void SequencePlayer::stop()
{

}

void SequencePlayer::resume()
{

}

void SequencePlayer::print()
{

}

