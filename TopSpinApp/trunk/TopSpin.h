/*
 * TopSpin.h
 *
 *  Created on: Dec 26, 2011
 *      Author: tom
 */

#ifndef TOPSPIN_H_
#define TOPSPIN_H_

#include <WProgram.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <FlexiTimer2.h>


#define LED_PIN		13

#define MAIN_ARM	0
#define CHAIR		1

#define BASE_INPUT_PIN	11
#define SIDE_INPUT_PIN	12

#define BASE_STATUS_PIN	6
#define SIDE_STATUS_PIN	7

#define FORWARD 	0
#define BACKWARD 	1

// Right Motor -> Y1 = Blue
// Right Motor -> Y2 = Red/Orange

uint8_t motor[2][3] = { {2, 3, 9}, {4, 5, 10} }; // Input 1, Input 2, Enable




#endif /* TOPSPIN_H_ */
