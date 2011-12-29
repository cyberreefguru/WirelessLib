/*
 * MatrixDriver.h
 *
 *  Created on: Jan 2, 2011
 *      Author: tom
 */

#ifndef MATRIXDRIVER_H_
#define MATRIXDRIVER_H_

#include <WProgram.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "IoShield.h"

#ifdef __cplusplus

extern "C" void TIMER2_COMPA_vect(void) __attribute__ ((signal));

class MatrixDriver
{

public:
	MatrixDriver();
	void initializeMatrix(uint8_t numberRows, uint8_t numberCols, uint8_t *buffer1, uint8_t *buffer2);
	void writeFrame(uint8_t* frame);
	void initializeTimer();
	void resetTimer();
	void startTimer();
	void stopTimer();
	void restartTimer();

	friend void TIMER2_COMPA_vect();

private:
	uint8_t rows;
	uint8_t cols;
	uint16_t matrixSize;
	uint8_t *buffer1;
	uint8_t *buffer2;
	uint8_t timerValue;

	uint8_t volatile *currentBuffer;
	uint8_t volatile *nextBuffer;
	uint8_t volatile newDataFlag;
	uint8_t volatile currentRow;
	uint8_t volatile currentColumn;
	uint8_t volatile ready;


};

#else

typedef
	struct MatrixDriver
		MatrixDriver;


#endif // end if c++


#ifdef __cplusplus
extern "C" {
#endif

	// Define class methods for C code to access
	void initializeMatrix(uint8_t numberRows, uint8_t numberCols, uint8_t *buffer1, uint8_t *buffer2);
	void writeFrame(unsigned char* frame);
	void initializeTimer();
	void resetTimer();
	void startTimer();
	void stopTimer();
	void restartTimer();

	// Define static class for C code to access
	extern MatrixDriver Matrix;


#ifdef __cplusplus
} // end extern C
#endif



#endif /* MATRIXDRIVER_H_ */
