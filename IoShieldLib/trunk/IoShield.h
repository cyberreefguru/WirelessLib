#ifndef IoShield_h
#define IoShield_h

#include <WProgram.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//
// Hardware agnostic definitions
//

// Defines flag indicating the direction of the port
#define DIRECTION_OUTPUT   0x01
#define DIRECTION_INPUT    0x02

// Defines direction to shift or roll
#define SHIFT_LEFT 0
#define SHIFT_RIGHT 1

//
// Hardware specific definitions
//

// Defines the address of the output ports
#define OUTPUT_PORT_1  0x00
#define OUTPUT_PORT_2  0x01

// Defines the address of the input ports

// Defines the masks for the data bus
#define MASK_ADDRESS_ENABLE  0x08
#define MASK_ADDRESS         0x07
#define MASK_PORTD           0xFC
#define MASK_PORTB           0x03

#ifdef __cplusplus

class IoShield
{

public:
	IoShield();
	void initialize_ports();
	unsigned char read_value(unsigned char port);
	void write_value(unsigned char port, unsigned char value);

	void test_output_ports();
	void shift_output(unsigned char port, unsigned char value,
					unsigned char endValue, unsigned char shiftDirection,
					unsigned char shiftValue, unsigned char totalShifts,
					int delayValue);
	void bounce_output(unsigned char port, unsigned char bounceDirection,
			unsigned char totalBounces, int delayValue);

	void toggle_address_enable();
	void set_address_enable(unsigned char value);
	void write_address(unsigned char address);
	void set_port_direction(unsigned char direction);
	void set_direction_input(void);
	void set_direction_output(void);
	void setPin13(unsigned char value);
	void togglePin13();



private:
	unsigned char ledPin;
	unsigned char _portDirection;

};

#else

typedef
	struct IoShield
		IoShield;

#endif // end if c++

#ifdef __cplusplus
extern "C" {
#endif

	void initialize_ports();
	unsigned char read_value(unsigned char port);
	void write_value(unsigned char port, unsigned char value);

	void test_output_ports();
	void shift_output(unsigned char port, unsigned char value,
					unsigned char endValue, unsigned char shiftDirection,
					unsigned char shiftValue, unsigned char totalShifts,
					int delayValue);
	void bounce_output(unsigned char port, unsigned char bounceDirection,
			unsigned char totalBounces, int delayValue);

	void toggle_address_enable();
	void set_address_enable(unsigned char value);
	void write_address(unsigned char address);
	void set_port_direction(unsigned char direction);
	void set_direction_input(void);
	void set_direction_output(void);

	extern IoShield IoPort;


#ifdef __cplusplus
} // end extern C
#endif


#endif // end ifndef IoShield_h

