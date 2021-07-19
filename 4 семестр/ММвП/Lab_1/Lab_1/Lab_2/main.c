#include "sam3n4c.h"
#include "pmc.h"
#include "pio.h"
#include "board.h"

#define MAINCK	(32000000)	// 32 MHz

#define N	128
// TFT resolution 240 * 320
#define ILI9341_TFTWIDTH  240
#define ILI9341_TFTHEIGHT 320
#define MIN_X	0
#define MIN_Y	0
#define MAX_X	(ILI9341_TFTWIDTH - 1)
#define MAX_Y	(ILI9341_TFTHEIGHT - 1)
/**
  * @brief Various internal ILI9341 registers name labels
  */
#define ILI9341_READ_ID1	0xDA
#define ILI9341_READ_ID2	0xDB
#define ILI9341_READ_ID3	0xDC
#define ILI9341_READ_ID4	0xD3
#define ILI9341_NOP			0x00
#define ILI9341_SWRESET		0x01
#define ILI9341_RDDID		0x04
#define ILI9341_RDDST		0x09
#define ILI9341_SLPIN		0x10
#define ILI9341_SLPOUT		0x11
#define ILI9341_PTLON		0x12
#define ILI9341_NORON		0x13
#define ILI9341_RDMODE		0x0A
#define ILI9341_RDMADCTL	0x0B
#define ILI9341_RDPIXFMT	0x0C
#define ILI9341_RDIMGFMT	0x0A
#define ILI9341_RDSELFDIAG	0x0F
#define ILI9341_INVOFF		0x20
#define ILI9341_INVON		0x21
#define ILI9341_GAMMASET	0x26
#define ILI9341_DISPOFF		0x28
#define ILI9341_DISPON		0x29
#define ILI9341_CASET		0x2A
#define ILI9341_RASET		0x2B
#define ILI9341_RAMWR		0x2C
#define ILI9341_RAMRD		0x2E
#define ILI9341_PTLAR		0x30
#define ILI9341_MADCTL		0x36
#define ILI9341_PIXFMT		0x3A
#define ILI9341_FRMCTR1		0xB1
#define ILI9341_FRMCTR2		0xB2
#define ILI9341_FRMCTR3		0xB3
#define ILI9341_INVCTR		0xB4
#define ILI9341_DFUNCTR		0xB6
#define ILI9341_PWCTR1		0xC0
#define ILI9341_PWCTR2		0xC1
#define ILI9341_PWCTR3		0xC2
#define ILI9341_PWCTR4		0xC3
#define ILI9341_PWCTR5		0xC4
#define ILI9341_VMCTR1		0xC5
#define ILI9341_VMCTR2		0xC7
#define ILI9341_RDID1		0xDA
#define ILI9341_RDID2		0xDB
#define ILI9341_RDID3		0xDC
#define ILI9341_RDID4		0xDD
#define ILI9341_GMCTRP1		0xE0
#define ILI9341_GMCTRN1		0xE1
/*
#define ILI9341_PWCTR6  0xFC
*/
// Color definitions
#define ILI9341_BLACK		0x0000	/*   0,   0,   0 */
#define ILI9341_NAVY		0x000F	/*   0,   0, 128 */
#define ILI9341_DARKGREEN	0x03E0	/*   0, 128,   0 */
#define ILI9341_DARKCYAN	0x03EF	/*   0, 128, 128 */
#define ILI9341_MAROON		0x7800	/* 128,   0,   0 */
#define ILI9341_PURPLE		0x780F	/* 128,   0, 128 */
#define ILI9341_OLIVE		0x7BE0	/* 128, 128,   0 */
#define ILI9341_LIGHTGREY	0xC618	/* 192, 192, 192 */
#define ILI9341_DARKGREY	0x7BEF	/* 128, 128, 128 */
#define ILI9341_BLUE		0x001F	/*   0,   0, 255 */
#define ILI9341_GREEN		0x07E0	/*   0, 255,   0 */
#define ILI9341_CYAN		0x07FF	/*   0, 255, 255 */
#define ILI9341_RED 		0xF800	/* 255,   0,   0 */
#define ILI9341_MAGENTA		0xF81F	/* 255,   0, 255 */
#define ILI9341_YELLOW		0xFFE0	/* 255, 255,   0 */
#define ILI9341_WHITE 		0xFFFF	/* 255, 255, 255 */
#define ILI9341_ORANGE		0xFD20	/* 255, 165,   0 */
#define ILI9341_GREENYELLOW	0xAFE5	/* 173, 255,  47 */
#define ILI9341_PINK		0xF81F
/* PORTA[12 - 14]:MISO, MOSI, SCK
 * PA9 --- /CS
 * PA10 --- D/C
 */
/**
  * @brief ILI9341 control pins definition
  */
/* ILI9341 CS control ( /CHIP SELECT )*/
#define PA10_CS_PIN	{PIO_PA10, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/* ILI9341 D/C control ( DATA/COMMAND )*/
#define PA11_DC_PIN	{PIO_PA11, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
/* SPI:PA12 --- MISO, PA13 --- MOSI, PA14 --- SCK
 * (p. 431 doc. "sam3n_series.pdf")
 */
#define SPI_MISO_PIN    {PIO_PA12A_MISO, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define SPI_MOSI_PIN    {PIO_PA13A_MOSI, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define SPI_SCK_PIN		{PIO_PA14A_SPCK, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// Pins to configure for TFT LCD control & data bus.
Pin TFTControlAndBusPins[] = {
	PA10_CS_PIN,
	PA11_DC_PIN,
	SPI_MISO_PIN,
	SPI_MOSI_PIN,
	SPI_SCK_PIN,
};

#define CS_INDEX	0
#define DC_INDEX	1
volatile int32_t ITM_RxBuffer;
extern const unsigned char gImage_image[];
unsigned short image_coor[] = {0, 0, 0, 0};

void Delay ( unsigned long nTime );
void SPI_Init();
void SPI_Transfer ( uint8_t uData );
void WRITE_DATA ( uint8_t data );
uint8_t Read_Register ( uint8_t Addr, uint8_t xParameter);
void TFT_SendCMD ( uint8_t index );
uint8_t readID ( void );
void TFT_Init ( void );
void TFT_SetCol ( uint16_t StartCol, uint16_t EndCol );
void TFT_SetPage ( uint16_t StartPage, uint16_t EndPage );
void TFT_SetXY ( uint16_t poX, uint16_t poY );
void TFT_SetPixel ( uint16_t poX, uint16_t poY,uint16_t color );
void TFT_DrawLine ( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void TFT_DrawBackgroundLine ( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, const unsigned char * image );
void TFT_DrawRectangle ( uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color );
void TFT_DrawCircle ( int poX, int poY, int r, uint16_t color );
void TFT_FillCircle (int poX, int poY, int r, uint16_t color );
void TFT_DrawTraingle ( int poX1, int poY1, int poX2, int poY2, int poX3, int poY3, uint16_t color );
void TFT_FillScreen ( uint16_t XL, uint16_t XR, uint16_t YU, uint16_t YD, uint16_t color );
void TFT_FillRectangle ( uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color );
void TFT_DrawHorizontalLine ( uint16_t poX, uint16_t poY, uint16_t length, uint16_t color);
void TFT_DrawVerticalLine ( uint16_t poX, uint16_t poY, uint16_t length, uint16_t color );
void _fillScreen();
/*
 * TFT_CS control block definitions
 */
/* /CS = 0*/
void TFT_CS_LOW() {
	PIO_Clear ( TFTControlAndBusPins + CS_INDEX );
}
/* /CS = 1*/
void TFT_CS_HIGH() {
	PIO_Set ( TFTControlAndBusPins + CS_INDEX );
}
/*
 * TFT_DC control block definitions
 */
/* D/C == 1 (DATA)*/
void TFT_DC_HIGH() {
	PIO_Set ( TFTControlAndBusPins + DC_INDEX );
}
/* D/C == 0 (COMMAND)*/
void TFT_DC_LOW() {
	PIO_Clear ( TFTControlAndBusPins + DC_INDEX );
}

uint16_t f[N];

int main ( void ) {
	image_coor[2]= ((unsigned short)gImage_image[2] << 8) + gImage_image[3];
	image_coor[3]= ((unsigned short)gImage_image[4] << 8) + gImage_image[5];

// Настройка системного таймера для управления задержками: 1 tick = 1 ms
	SysTick_Config ( MAINCK / 100000 );
// Инициализация SPI для управления TFT-дисплеем
	SPI_Init();
// Инициализация TFT-дисплея
	TFT_Init();
	while (1) {
		uint8_t q;
		uint16_t k;

		for (q = 1; q < 5; q++) {
			for (k = 0; k < N; k++) {
				f[k] = (2 * q * k) % 256;
			}
// Координаты области для отображения сигнала:
//  левый нижний угол (56, 132)
//  правый верхний угол (184, 68)
//  Высота области = 64
//  Ширина области = 128
			for (k = 0; k < (N-1); k++) {
				TFT_DrawLine ( 56 + k, 132 - f[k] / 4, 57 + k, 132 - f[k+1] / 4, ILI9341_RED);
			}

//			Delay(2000);

			for (k = 0; k < (N-1); k++) {
				TFT_DrawBackgroundLine ( 56 + k, 132 - f[k] / 4, 57 + k, 132 - f[k+1] / 4, gImage_image );
			}
		}
	}

	return 0;
}
// Инициализация SPI для управления TFT-дисплеем
void SPI_Init() {
	int iIndex;
	unsigned int configuration;

	for (iIndex = 0; iIndex < sizeof(TFTControlAndBusPins) / sizeof(Pin); iIndex++) {
		PIO_Configure ( TFTControlAndBusPins + iIndex, PIO_LISTSIZE ( TFTControlAndBusPins[iIndex] ) );
	}
/* Enable SPI peripheral clock
 *  p. 357 "PMC Peripheral Clock Enable Register"
 *p. 34 "Peripheral Identifiers" ---> ID_SPI == 21
 */
	PMC_EnablePeripheral ( ID_SPI );// PMC - Power Management Controller

	configuration = SPI_MR_MSTR;
// Configure the SPI in the desired mode
	SPI_Configure ( (AT91S_SPI *) SPI, ID_SPI, configuration );
	configuration = SPI_CSR_BITS_8_BIT | SPI_CSR_SCBR ( 8 );
	SPI_ConfigureNPCS ( (AT91S_SPI *) SPI, 0, configuration);
// Enable SPI
	SPI_Enable ( (AT91S_SPI *) SPI );
}

void SPI_Transfer ( uint8_t uData ) {
	SPI_Write ( (AT91S_SPI *) SPI, 0, uData );// NPCS0 = 0
}

void TFT_SendCMD ( uint8_t index ) {
    TFT_DC_LOW();
    TFT_CS_LOW();
    SPI_Transfer ( index );
    TFT_CS_HIGH();
}

void TFT_SendData ( uint16_t data ) {
	uint8_t data1 = data >> 8;
	uint8_t data2 = data & 0xff;
	TFT_DC_HIGH();
	TFT_CS_LOW();
    SPI_Transfer ( data1 );
    SPI_Transfer ( data2 );
    TFT_CS_HIGH();
}

void WRITE_DATA( uint8_t data ) {
	TFT_DC_HIGH();
	TFT_CS_LOW();
    SPI_Transfer ( data );
    TFT_CS_HIGH();
}

uint8_t Read_Register ( uint8_t Addr, uint8_t xParameter) {
	uint8_t data = 0;
	TFT_SendCMD ( 0xD9 ); /* ext command                */
	WRITE_DATA ( 0x10 + xParameter ); /* 0x11 is the first Parameter  */
	TFT_DC_LOW();
	TFT_CS_LOW();
	SPI_Transfer ( Addr );
	TFT_DC_HIGH();
	SPI_Transfer ( 0 );
	data = SPI_Read ( (AT91S_SPI *) SPI );
	TFT_CS_HIGH();
	return data;
}

uint8_t readID ( void ) {
	uint8_t i = 0;
	uint8_t data[3] ;
	uint8_t ID[3] = { 0x00, 0x93, 0x41 };

	for ( i = 0; i < 3; i++ ) {
		data[i] = Read_Register ( 0xD3, i + 1 );

		if ( data[i] != ID[i] ) {
			return 0;
		}
	}

	return 1;
}
// Инициализация TFT-дисплея
void TFT_Init ( void ) {
	uint8_t i = 0;

	TFT_CS_HIGH();
	TFT_DC_HIGH();

	for ( i = 0; i < 3; i++ ) {
		readID();
	}

	Delay ( 5 );
    TFT_SendCMD ( ILI9341_SWRESET );
    Delay ( 2 );
    TFT_SendCMD ( ILI9341_PWCTR1 );/* Power control */
    WRITE_DATA ( 0x1B ); /* VRH[5:0]                   */

    TFT_SendCMD ( ILI9341_PWCTR2 );/* Power control */
    WRITE_DATA ( 0x10 );    /* SAP[2:0];BT[3:0] */

    TFT_SendCMD ( ILI9341_VMCTR1 );/* VCM control*/
    WRITE_DATA ( 0x3F );
    WRITE_DATA ( 0x3C );
    TFT_SendCMD ( ILI9341_VMCTR2 );/* VCM control2*/
    WRITE_DATA ( 0XB7 );
    TFT_SendCMD ( ILI9341_MADCTL );/* Memory Access Control */
    WRITE_DATA ( 0x08 );
    TFT_SendCMD ( ILI9341_PIXFMT );
    WRITE_DATA ( 0x55 );
    TFT_SendCMD ( ILI9341_FRMCTR1 );
    WRITE_DATA ( 0x00 );
    WRITE_DATA ( 0x1B );
    TFT_SendCMD ( ILI9341_DFUNCTR );/* Display Function Control     */
    WRITE_DATA ( 0x0A );
    WRITE_DATA ( 0xA2 );
    TFT_SendCMD ( ILI9341_GAMMASET );/* Gamma curve selected */
    WRITE_DATA ( 0x01 );
    TFT_SendCMD ( ILI9341_GMCTRP1 ); /* Set Gamma */
    WRITE_DATA ( 0x0F );
    WRITE_DATA ( 0x2A );
    WRITE_DATA ( 0x28 );
    WRITE_DATA ( 0x08 );
    WRITE_DATA ( 0x0E );
    WRITE_DATA ( 0x08 );
    WRITE_DATA ( 0x54 );
    WRITE_DATA ( 0XA9 );
    WRITE_DATA ( 0x43 );
    WRITE_DATA ( 0x0A );
    WRITE_DATA ( 0x0F );
    WRITE_DATA ( 0x00 );
    WRITE_DATA ( 0x00 );
    WRITE_DATA ( 0x00 );
    WRITE_DATA ( 0x00 );
    TFT_SendCMD ( ILI9341_GMCTRN1 );/* Set Gamma */
    WRITE_DATA ( 0x00 );
    WRITE_DATA ( 0x15 );
    WRITE_DATA ( 0x17 );
    WRITE_DATA ( 0x07 );
    WRITE_DATA ( 0x11 );
    WRITE_DATA ( 0x06 );
    WRITE_DATA ( 0x2B );
    WRITE_DATA ( 0x56 );
    WRITE_DATA ( 0x3C );
    WRITE_DATA ( 0x05 );
    WRITE_DATA ( 0x10 );
    WRITE_DATA ( 0x0F );
    WRITE_DATA ( 0x3F );
    WRITE_DATA ( 0x3F );
    WRITE_DATA ( 0x0F );
    TFT_SendCMD ( ILI9341_SLPOUT );/* Exit Sleep */
    Delay ( 1 );
    TFT_SendCMD ( ILI9341_DISPON );/* Display on */
    _fillScreen();
}

void TFT_SetCol ( uint16_t StartCol, uint16_t EndCol ) {
    /* Column Command address       */
	TFT_SendCMD(ILI9341_CASET );
	TFT_SendData ( StartCol );
	TFT_SendData ( EndCol );
}

void TFT_SetPage ( uint16_t StartPage, uint16_t EndPage ) {
    /* Column Command address       */
   TFT_SendCMD ( ILI9341_RASET );
   TFT_SendData ( StartPage );
   TFT_SendData ( EndPage );
}

int16_t constrain ( int16_t iCoor, int16_t iCoorL, int16_t iCoorR ) {
	if (iCoor >= iCoorR) {
		return (iCoorR - 1);
	}
	else if (iCoor < iCoorL) {
		return iCoorL;
	}
	else {
		return iCoor;
	}
}

void TFT_FillScreen(uint16_t XL, uint16_t XR, uint16_t YU, uint16_t YD,uint16_t color) {
	unsigned long  XY = 0;
	unsigned long i = 0;

	if ( XL > XR ) {
		XL = XL ^ XR;
		XR = XL ^ XR;
		XL = XL ^ XR;
	}

	if ( YU > YD ) {
		YU = YU ^ YD;
		YD = YU ^ YD;
		YU = YU ^ YD;
	}

	XL = constrain ( XL, MIN_X, MAX_X );
	XR = constrain ( XR, MIN_X, MAX_X );
	YU = constrain ( YU, MIN_Y, MAX_Y );
	YD = constrain ( YD, MIN_Y, MAX_Y );
	XY = (XR - XL + 1);
	XY = XY * (YD - YU + 1);

	TFT_SetCol ( XL, XR );
	TFT_SetPage ( YU, YD );
/* start to write to display ram */
	TFT_SendCMD ( ILI9341_RAMWR );

	TFT_DC_HIGH();
	TFT_CS_LOW();
	uint8_t Hcolor = color >> 8;
	uint8_t Lcolor = color & 0xff;

	for ( i = 0; i < XY; i++ ) {
		SPI_Transfer ( Hcolor );
		SPI_Transfer ( Lcolor );
	}

	TFT_CS_HIGH();
}

void _fillScreen ( void ) {
	uint32_t i;
	uint32_t imax = 2 * image_coor[2] * image_coor[3];
	TFT_SetCol ( 0, MAX_X );
	TFT_SetPage ( 0, MAX_Y );
	TFT_SendCMD ( ILI9341_RAMWR );/* start to write to 							display ram */
	TFT_DC_HIGH();
	TFT_CS_LOW();

	for ( i = 0; i < imax; i++) {
		SPI_Transfer ( gImage_image[i + 8] );
    }

    TFT_CS_HIGH();
}

void TFT_SetXY ( uint16_t poX, uint16_t poY ) {
	TFT_SetCol ( poX, poX );
	TFT_SetPage ( poY, poY );
	TFT_SendCMD ( ILI9341_RAMWR );
}

void TFT_SetPixel ( uint16_t poX, uint16_t poY, uint16_t color ) {
	TFT_SetXY ( poX, poY );
	TFT_SendData ( color );
}

void TFT_FillRectangle ( uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color ) {
	TFT_FillScreen ( poX, poX+length, poY, poY+width, color );
}

void TFT_DrawHorizontalLine ( uint16_t poX, uint16_t poY, uint16_t length, uint16_t color)  {
	uint16_t i;
	TFT_SetCol ( poX, poX + length );
	TFT_SetPage ( poY, poY );
	TFT_SendCMD ( ILI9341_RAMWR );

	for ( i = 0; i < length; i++ ) {
		TFT_SendData ( color );
	}
}

void TFT_DrawVerticalLine ( uint16_t poX, uint16_t 	poY, uint16_t length, uint16_t color ){
	uint16_t i;
	TFT_SetCol ( poX, poX );
	TFT_SetPage ( poY, poY + length );
	TFT_SendCMD ( ILI9341_RAMWR );

	for ( i = 0; i < length; i++ ) {
		TFT_SendData ( color );
	}
}

void TFT_DrawLine ( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	int x = x1 - x0;
	int y = y1 - y0;
	int dx = x < 0 ? -x : x, sx = x0 < x1 ? 1 : -1;
	int dy = y < 0 ? y : -y, sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;/* error value e_xy             */

	for (;;) {/* loop                         */
		TFT_SetPixel ( x0, y0, color );
		e2 = 2 * err;

		if (e2 >= dy) {/* e_xy+e_x > 0                 */
			if (x0 == x1) {
				break;
			}

			err += dy;
			x0 += sx;
		}

		if (e2 <= dx) {/* e_xy+e_y < 0                 */
			if (y0 == y1) {
				break;
			}

			err += dx;
			y0 += sy;
		}
	}
}

void TFT_DrawBackgroundLine ( uint16_t x0, 	uint16_t y0, uint16_t x1, uint16_t y1, const 	unsigned char * image ) {
	int iPixelIndex;
	unsigned short color;
	unsigned short width = ((unsigned short)image[2] << 8) + image[3];
	int x = x1 - x0;
	int y = y1 - y0;
	int dx = x < 0 ? -x : x, sx = x0 < x1 ? 1 : -1;
	int dy = y < 0 ? y : -y, sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;/* error value e_xy             */

	for (;;) {/* loop                         */
		iPixelIndex = 2 * (y0 * width + x0) + 8;
		color = ((unsigned short)image[iPixelIndex] << 8) + image[iPixelIndex + 1];
		TFT_SetPixel ( x0, y0, color );
		e2 = 2 * err;

		if (e2 >= dy) {/* e_xy+e_x > 0                 */
			if (x0 == x1) {
				break;
			}

			err += dy;
			x0 += sx;
		}

		if (e2 <= dx) {/* e_xy+e_y < 0 */
			if (y0 == y1) {
				break;
			}

			err += dx;
			y0 += sy;
		}
	}
}

void TFT_DrawRectangle ( uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t 	color ) {
	TFT_DrawHorizontalLine(poX, poY, length, color);
	TFT_DrawHorizontalLine ( poX, poY + width, 	length, color );
	TFT_DrawVerticalLine ( poX, poY, width, color );
	TFT_DrawVerticalLine ( poX + length, poY, width, 	color );
}

void TFT_DrawCircle ( int poX, int poY, int r, uint16_t color ) {
	int x = -r, y = 0, err = 2 - 2 * r, e2;

	do {
		TFT_SetPixel ( poX - x, poY + y, color );
		TFT_SetPixel ( poX + x, poY + y, color );
		TFT_SetPixel ( poX + x, poY - y, color );
		TFT_SetPixel ( poX - x, poY - y, color );
		e2 = err;

		if (e2 <= y) {
			err += ++y * 2 + 1;

			if (-x == y && e2 <= x) {
				e2 = 0;
			}
		}

		if (e2 > x) {
			err += ++x * 2 + 1;
		}
	}
	while (x <= 0);
}

void TFT_FillCircle(int poX, int poY, int r, uint16_t color ){
	int x = -r, y = 0, err = 2 - 2 * r, e2;

	do {
		TFT_DrawVerticalLine(poX - x, poY - y, 2 * y, color);
		TFT_DrawVerticalLine(poX + x, poY - y, 2* y, color);
		e2 = err;

		if (e2 <= y) {
			err += ++y * 2 + 1;

			if (-x == y && e2 <= x) {
				e2 = 0;
			}
		}

		if (e2 > x) {
			err += ++x * 2 + 1;
		}
	} while (x <= 0);
}

void TFT_DrawTraingle ( int poX1, int poY1,	int poX2, int poY2, int poX3, int poY3,	uint16_t color ) {
	TFT_DrawLine ( poX1, poY1, poX2, poY2, color );
	TFT_DrawLine ( poX1, poY1, poX3, poY3, color );
	TFT_DrawLine ( poX2, poY2, poX3, poY3, color );
}
