/*
 * MatrixPlayer.h
 *
 *  Created on: Dec 31, 2010
 *      Author: tom
 */

#ifndef MATRIXPLAYER_H_
#define MATRIXPLAYER_H_

#include "MatrixDriver.h"

// Define the frame structure
struct pattern {
	uint16_t repeat;
	uint16_t holdBefore;
	uint16_t holdAfter;
	uint8_t *frameData;
};

// Define the frame structure
struct sequence {
	uint16_t repeatValue;
	uint8_t *patterns;
};


#ifdef __cplusplus


class MatrixPlayer
{

public:
	MatrixPlayer();
	void initializePlayer(uint8_t rows, uint8_t cols, uint8_t *buffer1, uint8_t *buffer2);
	void play(pattern *patterns, uint16_t len);
	void stop();
	void resume();
	void print();

private:
	pattern* pSequences;
	uint16_t numberSequences;

};


#else

typedef
	struct MatrixPlayer
	MatrixPlayer;


#endif // end if c++


#ifdef __cplusplus
extern "C" {
#endif

	// Define class methods for C code to access
	void initializePlayer(uint16_t numberRows, uint16_t numberCols);
	void play(pattern *patterns, uint16_t len);
	void stop();
	void resume();
	void print();

	// Define static class for C code to access
	extern MatrixPlayer Player;


#ifdef __cplusplus
} // end extern C
#endif



#endif /* MATRIXPLAYER_H_ */
