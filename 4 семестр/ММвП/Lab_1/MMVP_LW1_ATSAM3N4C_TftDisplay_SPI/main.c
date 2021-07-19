#include <avr/sfr_defs.h>
#include <C:\WinAVR-20100110\avr\include\avr/iom128.h>
#include <avr/io.h>

#define CS0_SELECT() (PORTB &= ~_BV ( PB4 ), \
			PORTB |= (_BV ( PB0 ) | _BV ( PB5 )))
#define CS1_SELECT()	(PORTB |= _BV ( PB0 ), \
			PORTB &= ~(_BV ( PB4 ) | _BV ( PB5 )))
#define CS2_SELECT() (PORTB &= ~_BV ( PB5 ), \
			PORTB |= (_BV ( PB0 ) | _BV ( PB4 )))

#define CS3_SELECT() (PORTB &= ~(_BV ( PB0 ) | _BV ( PB4 ) | _BV ( PB5 )))
#define CS4_SELECT()	(PORTB |= _BV ( PB5 ), \
			 PORTB &= ~(_BV ( PB0 ) | _BV ( PB4 )))
#define CS5_SELECT()	(PORTB |= _BV ( PB4 ), \
			 PORTB &= ~(_BV ( PB0 ) | _BV ( PB5 )))
#define CS6_SELECT()	(PORTB |= (_BV ( PB0 ) | _BV ( PB4 ) | _BV ( PB5 )))
#define CS7_SELECT() (PORTB &= ~_BV ( PB0 ), \
			 PORTB |= (_BV ( PB4 ) | _BV ( PB5 )))

int main ( void ) {
	uint8_t unData;

	DDRB = _BV ( DDB0 ) | _BV ( DDB1 ) |
			 _BV ( DDB2 ) | _BV ( DDB4 ) |
			_BV ( DDB5 );
	SPCR = _BV ( SPR0 ) | _BV ( SPR1 ) |
			 _BV ( MSTR ) | _BV ( SPE );

	while (1) {
			CS3_SELECT();
			SPDR = 0;
		while ( (SPSR & _BV ( SPIF )) == 0 ) {
			}
			//unData = SPDR;
			CS2_SELECT();
			SPDR = unData;
			if(unData = 0x08)
			{SPDR = 0x01; }

			while ( (SPSR & _BV ( SPIF )) == 0 ) {
				}
				CS7_SELECT();
				CS2_SELECT();
			}
			return 0;
		 }
