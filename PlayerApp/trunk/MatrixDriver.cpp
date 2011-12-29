/*
 * MatrixDriver.cpp
 *
 *  Created on: Jan 2, 2011
 *      Author: tom
 */

#include "MatrixDriver.h"

/** Define static variable for matrix */
MatrixDriver Matrix;

/**
 * Constructor
 */
MatrixDriver::MatrixDriver()
{
}

/**
 * Initializes the matrix with the row/col count.
 *
 */
void MatrixDriver::initializeMatrix(uint8_t numRows, uint8_t numCols, uint8_t *buff1, uint8_t *buff2)
{
	rows = numRows;
	cols = numCols;
	buffer1 = buff1;
	buffer2 = buff2;
	currentBuffer = buff1;
	nextBuffer = buff2;
	timerValue = 0;

	// Matrix brightness is binary, so frame data length == row count
//	matrixSize = rows*cols;
	matrixSize = rows;

	newDataFlag = false;
	currentRow = 0;
	currentColumn = 0;
	ready = false;

	IoPort.initialize_ports();

	// Initial port values to turn all LEDs off
	IoPort.write_value(OUTPUT_PORT_1, 0x00);
	IoPort.write_value(OUTPUT_PORT_2, 0xff);


} // end initializeMatrix

/**
 * Initializes the timer
 *
 */
void MatrixDriver::initializeTimer()
{
	TCCR2A = 0x02; // set the timer to CTC mode; disable output compare pin
	TCCR2B = 0x06; // clk/256 = 16us tclk

	// Set initial compare value to 62, ~1ms
	OCR2A = 0x3e;

	// Set timer value
	TCNT2 = 0;
	timerValue = 0;

} // end initializeTimer

/**
 * Resets the timer to 0.  Does not start or stop the timer.
 *
 */
void MatrixDriver::resetTimer()
{
	TCNT2 = 0;
	timerValue=0;
}

/**
 * Starts the timer from zero.
 *
 * NOTE: We use Time compare interrupt instead of global
 * interrupt so Arduino can still use interrupts for other
 * purposes.
 */
void MatrixDriver::startTimer()
{
	// Set timer count
	TCNT2 = 0;
	timerValue = 0;

	//Timer2 Compare Match A Interrupt Enable
	TIMSK2 = 0x02;

} // end startTimer

/**
 * Starts the timer from the value it was last stopped.
 *
 * NOTE: We use Time compare interrupt instead of global
 * interrupt so Arduino can still use interrupts for other
 * purposes.
 */
void MatrixDriver::restartTimer()
{
	// Set timer count
	TCNT2 = timerValue;

	//Timer2 Compare Match A Interrupt Enable
	TIMSK2 = 0x02;

} // end restartTimer


/**
 * Stops the timer by disabling interrupts for the timer.
 * Saves the current count value should you want to restart
 * the timer where you left off.
 */
void MatrixDriver::stopTimer()
{
	// Capture current timer value
	timerValue = TCNT2;

	//Timer2 Compare Match A Interrupt Disable
	TIMSK2 = 0x00;

} // end stopTimer


/**
 * Writes frame data to the matrix buffer
 *
 */
void MatrixDriver::writeFrame(uint8_t* frame)
{
	uint16_t index;

//	Serial.println("Writing Frame Data");

	// Stop timer to ensure dataFlag is updated without being interrupted
	stopTimer();
	if(newDataFlag == true )
	{
		// Houston, we have a problem
		// New data has arrived before old data was consumed
		newDataFlag = false;
	}
	// Restart timer
	startTimer();


//	Serial.print("Setting Frame Data: ");
	// Copy frame data into currentBuffer
	for(index = 0; index<matrixSize; index++)
	{
		nextBuffer[index] = frame[index];
//		Serial.print(nextBuffer[index], HEX );
//		Serial.print(" ");
	}
//	Serial.println();

	// Set new data flag
	newDataFlag = true;

	// Set ready flag
	if( ready == false )
	{
		ready = true;
	}

} // end writeFrame



/**
 * The interrupt service routine for timer 2
 *
 * Called once per bit position.
 *
 */
ISR( TIMER2_COMPA_vect )
{
	// temp variable to hold buffer ptr while swapping
	uint8_t volatile *temp;

	if( Matrix.ready == true )
	{
		// Turn off matrix
		IoPort.write_value(OUTPUT_PORT_2, 0xff);

		// Test if we've reach max row count (e.g., time to start over)
		if( Matrix.currentRow == Matrix.rows )
		{
			// We've reach max row count, reset row count
			Matrix.currentRow = 0;

			// Rotate buffers if new data exists
			if(Matrix.newDataFlag == true )
			{
				temp = Matrix.currentBuffer;
				Matrix.currentBuffer = Matrix.nextBuffer;
				Matrix.nextBuffer = temp;
				Matrix.newDataFlag = false;
			}
		}

		// Write new value (e.g., next row)
		IoPort.write_value(OUTPUT_PORT_1, Matrix.currentBuffer[Matrix.currentRow] );

		// Pulse current row to show value
		IoPort.write_value(OUTPUT_PORT_2, ~(1<<Matrix.currentRow) );

		// Increment row for next round
		Matrix.currentRow++;

		TCNT2 = 0; // reset the timer

	} // end if ready

} // end ISR

