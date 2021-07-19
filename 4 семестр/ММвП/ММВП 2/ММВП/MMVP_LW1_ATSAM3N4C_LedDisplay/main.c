#include "sam3n4c.h"
#include "pio.h"
#include "board.h"

#define MAINCK			(16000000)		// 16 MHz

#define D0    {PIO_PA0, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define D1    {PIO_PA1, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define D2    {PIO_PA2, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define D3    {PIO_PA3, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define D4    {PIO_PA4, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define D5    {PIO_PA5, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define D6    {PIO_PA6, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define D7    {PIO_PA7, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}

#define CS0   {PIO_PA8, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
#define CS1   {PIO_PA9, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}

volatile int32_t ITM_RxBuffer;                    /*!< external variable to receive characters                    */

/// Pins to configure for the application.
const Pin LedControlPins[] = {
		D0, D1, D2, D3,
		D4, D5, D6, D7,
		CS0, CS1,
};
#define CS0_IND		8
#define CS1_IND		9


#define STR_QUANTITY	8

#define DIGIT_PERIOD	0x100

#define column_0	0x01
#define column_1	0x02
#define column_2	0x04
#define column_3	0x08
#define column_4	0x10
#define column_5	0x20
#define column_6	0x40
#define column_7	0x80
#define columnsOFF	0x00

static uint8_t aunFont[][STR_QUANTITY] = {
		{
				// 76543210
				// __111___
				// _11_11__
				// _11_11__
				// _11_11__
				// _11_11__
				// _11_11__
				// _11_11__
				// __111___
				column_5 | column_4 | column_3,
				column_6 | column_5 | column_3 | column_2,
				column_6 | column_5 | column_3 | column_2,
				column_6 | column_5 | column_3 | column_2,
				column_6 | column_5 | column_3 | column_2,
				column_6 | column_5 | column_3 | column_2,
				column_6 | column_5 | column_3 | column_2,
				column_5 | column_4 | column_3,
		},
		{
				// 76543210
				// __111___
				// ___11___
				// ___11___
				// ___11___
				// ___11___
				// ___11___
				// ___11___
				// __1111__
				column_5 | column_4 | column_3,
				column_4 | column_3,
				column_4 | column_3,
				column_4 | column_3,
				column_4 | column_3,
				column_4 | column_3,
				column_4 | column_3,
				column_5 | column_4 | column_3 | column_2,
		},
		{
				// 76543210
				// __111___
				// _11111__
				// 1___11__
				// ____11__
				// ____1___
				// ___1____
				// __1111__
				// _11111__
				column_5 | column_4 | column_3,
				column_6 | column_5 | column_4 | column_3 | column_2,
				column_7 | column_3 | column_2,
				column_3 | column_2,
				column_3,
				column_4,
				column_5 | column_4 | column_3 | column_2,
				column_6 | column_5 | column_4 | column_3 | column_2,
		},
};

void Delay ( unsigned long nTime );

int main ( void ) {
	int			iIndex;
	uint8_t		unDispString = 0;
	uint8_t		unFontIndex = 0;
	uint8_t		unDigit = 0;
	int			nDelayIn, nDelayOut;

	SysTick_Config ( MAINCK / 10000 );

	for (iIndex = 0; iIndex < sizeof(LedControlPins) / sizeof(Pin); iIndex++) {
		PIO_Configure ( LedControlPins + iIndex, PIO_LISTSIZE ( LedControlPins[iIndex] ) );
	}

	for(;;) {
		nDelayOut = DIGIT_PERIOD;
		while (--nDelayOut > 0) {
			for (iIndex = 0; iIndex < STR_QUANTITY; iIndex++) {
				if (iIndex == unDispString) {
					PIO_Clear ( LedControlPins + iIndex );
				} else {
					PIO_Set ( LedControlPins + iIndex );
				}
			}
			PIO_Set ( LedControlPins + CS0_IND );
			PIO_Clear ( LedControlPins + CS0_IND );

			for (iIndex = 0; iIndex < STR_QUANTITY; iIndex++) {
				if (aunFont[unDigit][unFontIndex] & (1 << iIndex)) {
					PIO_Set ( LedControlPins + iIndex );
				} else {
					PIO_Clear ( LedControlPins + iIndex );
				}
			}
			PIO_Set ( LedControlPins + CS1_IND );
			PIO_Clear ( LedControlPins + CS1_IND );
			unFontIndex = (unFontIndex + 1) & (STR_QUANTITY - 1);

			// Delay
			for (nDelayIn = 0; nDelayIn < 100; nDelayIn++) {
			}

			for (iIndex = 0; iIndex < STR_QUANTITY; iIndex++) {
				PIO_Clear ( LedControlPins + iIndex );
			}
			PIO_Set ( LedControlPins + CS1_IND );
			PIO_Clear ( LedControlPins + CS1_IND );
			if (++unDispString >= STR_QUANTITY) {
				unDispString = 0;
			}
		}
		unDigit++;
		unDigit %= (sizeof(aunFont) / STR_QUANTITY / sizeof(uint8_t));
	}
	return 0;
 }
