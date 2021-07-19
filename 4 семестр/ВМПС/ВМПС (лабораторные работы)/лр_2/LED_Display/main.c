#include <C:\Users\dariavarabei\4\avr\include\avr\sfr_defs.h>
#include <C:\Users\dariavarabei\4\avr\include\avr\iom128.h>
#include <C:\Users\dariavarabei\4\avr\include\avr\io.h>

#define REG_STRING_ENABLE	0x02
#define REG_COLUMN_ENABLE	0x01

#define STR_QUANTITY	8
#define DIGIT_PERIOD	0x1000

static uint8_t aunFont[][STR_QUANTITY] = {
		{
				(uint8_t)~0x00,
				(uint8_t)~0x38,
				(uint8_t)~0x24,
				(uint8_t)~0x22,
				(uint8_t)~0x22,
				(uint8_t)~0x24,
				(uint8_t)~0x38,
				(uint8_t)~0x00,
		},
		{
						(uint8_t)~0x00,
						(uint8_t)~0x3C,
						(uint8_t)~0x66,
						(uint8_t)~0x66,
						(uint8_t)~0x7E,
						(uint8_t)~0x66,
						(uint8_t)~0x66,
						(uint8_t)~0x66,
				},
				{
								(uint8_t)~0x00,
								(uint8_t)~0x3C,
								(uint8_t)~0x20,
								(uint8_t)~0x3C,
								(uint8_t)~0x04,
								(uint8_t)~0x04,
								(uint8_t)~0x3C,
								(uint8_t)~0x00,
						},
		{
				(uint8_t)~0x00,
				(uint8_t)~0x24,
				(uint8_t)~0x24,
				(uint8_t)~0x3C,
				(uint8_t)~0x24,
				(uint8_t)~0x24,
				(uint8_t)~0x24,
				(uint8_t)~0x00,
		},
		{
				(uint8_t)~0x00,
				(uint8_t)~0x3C,
				(uint8_t)~0x66,
				(uint8_t)~0x66,
				(uint8_t)~0x7E,
				(uint8_t)~0x66,
				(uint8_t)~0x66,
				(uint8_t)~0x66,
		},
};
int main ( void ) {
	uint8_t unDispString = 1;
	uint8_t unFontIndex = 0;
	int32_t nDelayIn, nDelayOut;
	int32_t nDigit = 0;

	DDRA = 0xFF;
	DDRB = 0x03;
	PORTA = 0;
	PORTB = 0;
	while (1) {
		nDelayOut = DIGIT_PERIOD;
		while (--nDelayOut > 0) {
			PORTA = unDispString;
			PORTB |= REG_STRING_ENABLE;
			PORTB &= (~REG_STRING_ENABLE);
			unDispString <<= 1;
			if (!unDispString) {
				unDispString = 1;
			}
			PORTA = aunFont[nDigit][unFontIndex++];
			PORTB |= REG_COLUMN_ENABLE;
			PORTB &= (~REG_COLUMN_ENABLE);
			unFontIndex &= (STR_QUANTITY - 1);

			for (nDelayIn = 0; nDelayIn < 100; nDelayIn++) {
			}
		}
		nDigit++;
        nDigit %= (sizeof(aunFont) / STR_QUANTITY / sizeof(uint8_t));
	}
	return 0;
 }
