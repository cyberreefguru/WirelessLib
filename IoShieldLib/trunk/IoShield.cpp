#include "IoShield.h"

/** Define the static variable for IoShield */
IoShield IoPort;

IoShield::IoShield() {
	ledPin = 13;
	pinMode(ledPin, OUTPUT);
	_portDirection = DIRECTION_INPUT;

}

void IoShield::setPin13(unsigned char value)
{
	digitalWrite(ledPin, value);
}

void IoShield::togglePin13()
{
	digitalWrite(ledPin, HIGH);
	asm volatile("nop\nnop\n");
	asm volatile("nop\nnop\n");
	asm volatile("nop\nnop\n");
	asm volatile("nop\nnop\n");
	digitalWrite(ledPin, LOW);
}

void IoShield::test_output_ports() {
	digitalWrite(ledPin, HIGH); // set the LED on
	write_value(OUTPUT_PORT_1, 0x00);
	delay(250);
	digitalWrite(ledPin, LOW); // set the LED off

	delay(250);

	digitalWrite(ledPin, HIGH); // set the LED on
	write_value(OUTPUT_PORT_1, 0xFF);
	delay(250);
	digitalWrite(ledPin, LOW); // set the LED off

	delay(250);

	digitalWrite(ledPin, HIGH); // set the LED on
	write_value(OUTPUT_PORT_1, 0x55);
	delay(250);
	digitalWrite(ledPin, LOW); // set the LED off

	delay(250);

	digitalWrite(ledPin, HIGH); // set the LED on
	write_value(OUTPUT_PORT_1, 0xAA);
	delay(250);
	digitalWrite(ledPin, LOW); // set the LED off

	delay(250);

	shift_output(OUTPUT_PORT_1, 0x01, 0x00, SHIFT_LEFT, 1, 9, 50);
	shift_output(OUTPUT_PORT_1, 0x80, 0x00, SHIFT_RIGHT, 1, 9, 50);

	delay(250);

	for (int i = 0; i < 4; i++) {
		write_value(OUTPUT_PORT_1, 0xff);
		delay(100);
		write_value(OUTPUT_PORT_1, 0x00);
		delay(100);
	} // end for

	delay(500);

}

/**
 * Shifts the specified value the specified number of times with the specified delay
 * between shifts in the specified direction.  Delay is specified in ms.
 * left = 0, right = 1
 *
 **/
void IoShield::shift_output(unsigned char port, unsigned char value,
		unsigned char endValue, unsigned char shiftDirection,
		unsigned char shiftValue, unsigned char totalShifts, int delayValue) {
	for (unsigned char i = 0; i < totalShifts; i++) {
		write_value(port, value);
		delay(delayValue);

		if (shiftDirection == SHIFT_LEFT) {
			value = value << shiftValue;
		} else if (shiftDirection == SHIFT_RIGHT) {
			value = value >> shiftValue;
		}

	} // end for totalShifts

	write_value(port, endValue);

} // end shift_output


/**
 * Shifts a single bit back and forth the specified number of times from the specified duration (to hold each bit).
 * If LEFT, then 0x01 is shifted left; if RIGHT, then 0x80 is shifted right.
 *
 **/
void IoShield::bounce_output(unsigned char port, unsigned char bounceDirection,
		unsigned char totalBounces, int delayValue) {

	for (int i = 0; i < totalBounces; i++) {
		if (bounceDirection == SHIFT_LEFT) {
			// Shift 0x01 left 8 times, 250ms delay
			shift_output(OUTPUT_PORT_1, 0x01, 0x00, SHIFT_LEFT, 1, 7,
					delayValue);

			// Shift 0x80 right 8 times, 250ms delay
			shift_output(OUTPUT_PORT_1, 0x80, 0x00, SHIFT_RIGHT, 1, 7,
					delayValue);
		} else if (bounceDirection == SHIFT_RIGHT) {
			// Shift 0x01 left 8 times, 250ms delay
			shift_output(OUTPUT_PORT_1, 0x80, 0x00, SHIFT_RIGHT, 1, 7,
					delayValue);

			// Shift 0x80 right 8 times, 250ms delay
			shift_output(OUTPUT_PORT_1, 0x01, 0x00, SHIFT_LEFT, 1, 7,
					delayValue);
		}

	} // end for bounces


	if (bounceDirection == SHIFT_LEFT) {
		write_value(port, 0x01);
		delay(delayValue);
	} else if (bounceDirection == SHIFT_RIGHT) {
		write_value(port, 0x80);
		delay(delayValue);
	}

	write_value(port, 0x00);

} // end bounce_output


/**
 * Initializes the ports for our hardware layout.  Port C is the control port.
 * Ports B and D are the data ports.  PB1..PB0 and PD7..PD2 are data lines.
 *
 * NOTE: This routine does not use the predefined libraries to control
 *       the direction of the port.
 **/
void IoShield::initialize_ports() {
	// set PC to output
	ADCSRA = 0x00;
	PORTC = 0x00;
	DDRC = 0x3F;

	// Set port to input
	set_direction_input();

} // end initialize_ports

/**
 * Writes the specified value to the specified port (e.g., address)
 *
 * NOTE: This routine does not use the predefined libraries to control
 *       the value of the port.
 **/
void IoShield::write_value(unsigned char port, unsigned char value) {

	// Set data port to output
	set_port_direction(DIRECTION_OUTPUT);

	// write address
	write_address(port);

	// write value to ports
	PORTD = (PORTD & ~MASK_PORTD) | ((value << 2) & MASK_PORTD);
	PORTB = (PORTB & ~MASK_PORTB) | ((value >> 6) & MASK_PORTB);

	// Latch value to output port
	toggle_address_enable(); // write value to port

} // end write_value

/**
 * Reads the value on the specified port (e.g., address)
 *
 * NOTE: This routine does not use the predefined libraries to control
 *       the value of the port.
 **/
unsigned char IoShield::read_value(unsigned char port) {
	unsigned char inputValue;

	// Set data port to input
	set_port_direction(DIRECTION_INPUT);

	// write address
	write_address(port);

	// Enable input port
	set_address_enable(HIGH);

	// read value from ports
	inputValue = 0;

	// Must have nop in place to ensure port is syncrhonized with pins
	asm volatile("nop\nnop\n");

	inputValue |= (PIND >> 2); // TODO: Might need to & 0x3F
	inputValue |= (PINB << 6); // TODO: Might need to & 0xC0

	// Disable input port
	set_address_enable(LOW);

	return inputValue;

} // end read_value

/**
 * Toggles the ADDRESS_EN output by bringing is high and then low.
 *
 * NOTE: This routine does not use the predefined libraries to control
 *       the value of the port.
 *
 * NOTE: Assumes AEN is low before calling.
 *
 **/
void IoShield::toggle_address_enable() {

	PORTC |= MASK_ADDRESS_ENABLE; // raise high
	asm volatile("nop\nnop\nnop\n");
	PORTC &= ~MASK_ADDRESS_ENABLE; // bring low

} // end toggle_address_enable

/**
 * Toggles the ADDRESS_EN output by bringing is high and then low.
 *
 * NOTE: This routine does not use the predefined libraries to control
 *       the value of the port.
 *
 * NOTE: Assumes AEN is low before calling.
 *
 **/
void IoShield::set_address_enable(unsigned char value) {
	if (value == HIGH) {
		PORTC |= MASK_ADDRESS_ENABLE; // raise high
	} else if (value == LOW) {
		PORTC &= ~MASK_ADDRESS_ENABLE; // bring low
	}

} // end set_address_enable


/**
 * Writes the address to the control port; does not affect any other bits on the port.
 *
 * NOTE: This routine does not use the predefined libraries to control
 *       the value of the port.
 **/
void IoShield::write_address(unsigned char address) {

	PORTC = (PORTC & ~MASK_ADDRESS) | (address & MASK_ADDRESS);

} // end write_address

/**
 * Sets the port direction.  Data port is defined by PD7..PD2, and PB1..PB0.
 *
 * NOTE: This routine does not use the predefined libraries to control
 *       the direction of the port.
 **/
void IoShield::set_port_direction(unsigned char direction) {
	// Optimize to save cycles
	if (_portDirection == direction) {
		return;
	}

	if (direction == DIRECTION_OUTPUT && _portDirection == DIRECTION_INPUT) {
		// Going from input to output
		// ({DDxn, PORTxn} = 0b00)
		PORTD &= ~MASK_PORTD;
		PORTB &= ~MASK_PORTB;

		set_direction_output();

	} else if (direction == DIRECTION_INPUT && _portDirection
			== DIRECTION_OUTPUT) {
		// Going from output to input

		// Set port value to low
		// TODO: Do we need this?
		PORTD &= ~MASK_PORTD;
		PORTB &= ~MASK_PORTB;

		set_direction_input();

	} // end if direction

} // end set_port_direction

/**
 * Sets the direction to output
 *
 */
void IoShield::set_direction_output() {
	// Change direction
	DDRD |= MASK_PORTD; // Set PD7..PD2 to outputs
	DDRB |= MASK_PORTB; // Set PB1..PB0 to outputs

	// Set port value to low
	PORTD &= ~MASK_PORTD;
	PORTB &= ~MASK_PORTB;
	_portDirection = DIRECTION_OUTPUT;

}

/**
 * Sets the direction to input.
 *
 */
void IoShield::set_direction_input() {
	// Set port to input (e.g., 0)
	DDRD &= ~MASK_PORTD; // Set PD7..PD2 to inputs
	DDRB &= ~MASK_PORTB; // Set PB1..PB0 to inputs

	// Enable pull ups (e.g., HIGH)
	PORTD |= MASK_PORTD;
	PORTB |= MASK_PORTB;
	_portDirection = DIRECTION_INPUT;

} // end setDirectionInput

