/*
 * SequencePlayer.cpp
 *
 *  Created on: Dec 31, 2010
 *      Author: tom
 */

#include "MatrixPlayer.h"


/** Define the static variable for SequencePlayer */
MatrixPlayer Player;


/**
 * Constructor
 */
MatrixPlayer::MatrixPlayer()
{
}

/**
 * Initialize routine
 */
void MatrixPlayer::initializePlayer(uint8_t rows, uint8_t cols, uint8_t *buffer1, uint8_t *buffer2)
{
	Matrix.initializeMatrix(rows, cols, buffer1, buffer2);
	Matrix.initializeTimer();

} // end initialize


void MatrixPlayer::play(pattern *patterns, uint16_t len)
{
	uint16_t i, j;

	Matrix.startTimer();

	for(i=0; i<len; i++)
	{
		Serial.println( i, HEX);


		for(j=0; j<patterns[i].repeat; j++)
		{
			if( patterns[i].holdBefore > 0 )
			{
				delay(patterns[i].holdBefore);
			}

			Matrix.writeFrame( patterns[i].frameData );

			if( patterns[i].holdAfter > 0 )
			{
				delay(patterns[i].holdAfter);
			}
		} // end for pattern.repeat
	} // end for pattern len

	Matrix.stopTimer();


}

void MatrixPlayer::stop()
{

}

void MatrixPlayer::resume()
{

}

void MatrixPlayer::print()
{

}

