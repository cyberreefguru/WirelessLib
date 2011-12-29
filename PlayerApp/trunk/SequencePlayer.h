/*
 * SequencePlayer.h
 *
 *  Created on: Dec 31, 2010
 *      Author: tom
 */

#ifndef SEQUENCEPLAYER_H_
#define SEQUENCEPLAYER_H_

#include "IoShield.h"

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


class SequencePlayer
{
public:
	SequencePlayer();
	void initializePlayer(uint16_t numberRows, uint16_t numberCols);
	void play(pattern *patterns, uint16_t len);
	void stop();
	void resume();
	void print();

private:
	pattern* pSequences;
	uint16_t numberRows;
	uint16_t numberCols;
	uint16_t numberSequences;
	uint16_t dataLen;

};


#else

typedef
	struct SequencePlayer
		SequencePlayer;


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
	extern SequencePlayer sequencePlayer;


#ifdef __cplusplus
} // end extern C
#endif



#endif /* SEQUENCEPLAYER_H_ */
