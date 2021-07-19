#include "sam3n4c.h"
#include "pmc.h"
#include "pio.h"
#include "board.h"

#define BIT(x,b)		((x&b)==b)

#define MAINCK	(32000000)		// 32 MHz

#define WIDTH		320
#define HEIGHT		240
#define	DELAY	0

/**
  * @brief Various internal ILI9341 registers name labels
  */
#define	ILI9341_READ_ID1		0xDA
#define	ILI9341_READ_ID2		0xDB
#define	ILI9341_READ_ID3		0xDC
#define	ILI9341_READ_ID4		0xD3
#define	ILI9341_TFTWIDTH 	240
#define	ILI9341_TFTHEIGHT	320

#define	ILI9341_NOP		0x00
#define	ILI9341_SWRESET	0x01
#define	ILI9341_RDDID		0x04
#define	ILI9341_RDDST		0x09

#define	ILI9341_SLPIN		0x10
#define	ILI9341_SLPOUT		0x11
#define	ILI9341_PTLON		0x12
#define	ILI9341_NORON		0x13
#define	ILI9341_RDMODE		0x0A
#define	ILI9341_RDMADCTL		0x0B
#define	ILI9341_RDPIXFMT		0x0C
#define	ILI9341_RDIMGFMT		0x0A
#define	ILI9341_RDSELFDIAG	0x0F

#define	ILI9341_INVOFF		0x20
#define	ILI9341_INVON			0x21
#define	ILI9341_GAMMASET		0x26
#define	ILI9341_DISPOFF		0x28
#define	ILI9341_DISPON		0x29
#define	ILI9341_CASET	0x2A
#define	ILI9341_RASET	0x2B
#define	ILI9341_RAMWR	0x2C
#define	ILI9341_RAMRD	0x2E

#define	ILI9341_PTLAR	0x30
#define	ILI9341_MADCTL	0x36
#define	ILI9341_PIXFMT	0x3A

#define	ILI9341_FRMCTR1	0xB1
#define	ILI9341_FRMCTR2	0xB2
#define	ILI9341_FRMCTR3	0xB3
#define	ILI9341_INVCTR	0xB4
#define	ILI9341_DFUNCTR	0xB6
#define	ILI9341_CASET	0x2A
#define	ILI9341_RASET	0x2B
#define	ILI9341_RAMWR	0x2C
#define	ILI9341_RAMRD	0x2E

#define	ILI9341_PTLAR	0x30
#define	ILI9341_MADCTL	0x36
#define	ILI9341_PIXFMT	0x3A

#define	ILI9341_FRMCTR1	0xB1
#define	ILI9341_FRMCTR2	0xB2
#define	ILI9341_FRMCTR3	0xB3
#define	ILI9341_INVCTR	0xB4
#define	ILI9341_DFUNCTR	0xB6
#define	ILI9341_PWCTR1	0xC0
#define	ILI9341_PWCTR2	0xC1
#define	ILI9341_PWCTR3	0xC2
#define	ILI9341_PWCTR4	0xC3
#define	ILI9341_PWCTR5	0xC4
#define	ILI9341_VMCTR1	0xC5
#define	ILI9341_VMCTR2	0xC7

#define	ILI9341_RDID1		0xDA
#define	ILI9341_RDID2		0xDB
#define	ILI9341_RDID3		0xDC
#define	ILI9341_RDID4		0xDD
#define ILI9341_GMCTRP1	0xE0
#define ILI9341_GMCTRN1	0xE1

// Color definitions
#define ILI9341_BLACK		0x0000	/*   0,   0,   0 */
#define ILI9341_NAVY		0x000F	/*   0,   0, 128 */
#define ILI9341_DARKGREEN	0x03E0	/*   0, 128,   0 */
#define ILI9341_DARKCYAN	0x03EF	/*   0, 128, 128 */
#define ILI9341_MAROON		0x7800	/* 128,   0,   0 */
#define ILI9341_PURPLE		0x780F	/* 128,   0, 128 */
#define ILI9341_OLIVE		0x7BE0 	/* 128, 128,   0 */
#define ILI9341_LIGHTGREY	0xC618 	/* 192, 192, 192 */
#define ILI9341_DARKGREY	0x7BEF	/* 128, 128, 128 */
#define ILI9341_BLUE		0x001F	/*   0,   0, 255 */
#define	ILI9341_GREEN		0x07E0 /*   0, 255,   0 */
#define	ILI9341_CYAN		0x07FF /*   0, 255, 255 */
#define	ILI9341_RED 		0xF800 /* 255,   0,   0 */
#define	ILI9341_MAGENTA		0xF81F /* 255,   0, 255 */
#define	ILI9341_YELLOW		0xFFE0 /* 255, 255,   0 */
#define	ILI9341_WHITE		0xFFFF /* 255, 255, 255 */
#define	ILI9341_ORANGE		0xFD20 /* 255, 165,   0 */
#define	ILI9341_GREENYELLOW	0xAFE5 /* 173, 255,  47 */
#define	ILI9341_PINK		0xF81F

volatile int32_t ITM_RxBuffer;
extern const unsigned char gImage_image[];
unsigned short image_coor[] = {0, 0, 0, 0};

/* PORTA[12..0]:PA[7..0] --- D[7..0]
 * PA8 --- /RES
 * PA9 --- /CS
 * PA10 --- D/C
 * PA11 --- /WR
 * PA12 --- /RD
 */

/**
  * @brief ILI9341 control pins definition
  */
/* ILI9341 RES control ( /RESET )*/
#define PA8_RES_PIN {PIO_PA8, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
/* ILI9341 CS control ( /CHIP SELECT )*/
#define PA9_CS_PIN {PIO_PA9, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/* ILI9341 D/C control ( DATA/COMMAND )*/
#define PA10_DC_PIN {PIO_PA10, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
/* ILI9341 /WR control ( /WRITE )*/
#define PA11_WR_PIN {PIO_PA11, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/* ILI9341 /RD control ( /READ )*/
#define PA12_RD_PIN {PIO_PA12, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/* LED as an indicator of the display process end*/
#define PA13_LED_PIN {PIO_PA13, (AT91S_PIO *) PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}

Pin TFTBusPins[] = {
{
PIO_PA0,
(AT91S_PIO *) PIOA,
AT91C_ID_PIOA,
PIO_OUTPUT_0,
PIO_OPENDRAIN
},
{PIO_PA1,
(AT91S_PIO *) PIOA,
AT91C_ID_PIOA,
PIO_OUTPUT_0,
PIO_OPENDRAIN},
{PIO_PA2,
(AT91S_PIO *) PIOA,
AT91C_ID_PIOA,
PIO_OUTPUT_0,
PIO_OPENDRAIN},
{PIO_PA3,
(AT91S_PIO *) PIOA,
AT91C_ID_PIOA,
PIO_OUTPUT_0,
PIO_OPENDRAIN},
{PIO_PA4,
(AT91S_PIO *) PIOA,
AT91C_ID_PIOA,
PIO_OUTPUT_0,
PIO_OPENDRAIN},
{PIO_PA5,
(AT91S_PIO *) PIOA,
AT91C_ID_PIOA,
PIO_OUTPUT_0,
PIO_OPENDRAIN},
{PIO_PA6,
(AT91S_PIO *) PIOA,
AT91C_ID_PIOA,
PIO_OUTPUT_0,
PIO_OPENDRAIN},
{PIO_PA7,
(AT91S_PIO *) PIOA,
AT91C_ID_PIOA,
PIO_OUTPUT_0,
PIO_OPENDRAIN},	};

#define	RES_INDEX	0
#define	CS_INDEX		1
#define	DC_INDEX		2
#define	WR_INDEX	3
#define	RD_INDEX		4
#define	LED_INDEX	5

/// Pins to configure for TFT LCD control.
const Pin TFTBusCtrlPins[] = {
PA8_RES_PIN,
PA9_CS_PIN,
PA10_DC_PIN,
PA11_WR_PIN,
PA12_RD_PIN,
PA13_LED_PIN,	};
uint32_t	LCD_Data;
uint32_t	uiData;

void Delay ( unsigned long nTime );

void ILI9341_DataBusOut() {
unsigned int uiIndex;
for (uiIndex = 0; uiIndex < sizeof(TFTBusPins) / sizeof(Pin); uiIndex++) {
TFTBusPins[uiIndex].type = PIO_OUTPUT_0;
PIO_Configure ( TFTBusPins + uiIndex, PIO_LISTSIZE ( TFTBusPins[uiIndex] ) );
}
}

void ILI9341_DataBusIn() {
unsigned int uiIndex;
for (uiIndex = 0; uiIndex < sizeof(TFTBusPins) / sizeof(Pin); uiIndex++) {
TFTBusPins[uiIndex].type = PIO_INPUT;
PIO_Configure ( TFTBusPins + uiIndex, PIO_LISTSIZE ( TFTBusPins[uiIndex] ) );
}
}

void ILI9341_CtrlBusOut() {
unsigned int uiIndex;
for (uiIndex = 0; uiIndex < sizeof(TFTBusCtrlPins) / sizeof(Pin); uiIndex++) {
PIO_Configure ( TFTBusCtrlPins + uiIndex, PIO_LISTSIZE ( TFTBusCtrlPins[uiIndex] ) );
}
}

void ILI9341_SetDataBus ( uint8_t data ) {
TFTBusPins[0].pio->PIO_SODR = data & 0xFF;
TFTBusPins[0].pio->PIO_CODR = ~data & 0xFF;
}

uint8_t ILI9341_GetDataBus() {
uint8_t data;
data = TFTBusPins[0].pio->PIO_PDSR;
return data;
}

/* /CS == 0*/
void ILI9341_SELECT() {
PIO_Clear ( TFTBusCtrlPins + CS_INDEX );
}
/* /RES == 1*/
void ILI9341_RESET() {
PIO_Set ( TFTBusCtrlPins + RES_INDEX );
}
/* /WR == 1*/
void ILI9341_WR_HIGH() {
PIO_Set ( TFTBusCtrlPins + WR_INDEX );
}
/* /WR == 0*/
void ILI9341_WR_LOW() {
PIO_Clear ( TFTBusCtrlPins + WR_INDEX );
}
/* /RD == 1*/
void ILI9341_RD_HIGH() {
PIO_Set ( TFTBusCtrlPins + RD_INDEX );
}
/* /RD == 0*/
void ILI9341_RD_LOW() {
PIO_Clear ( TFTBusCtrlPins + RD_INDEX );
}
/* D/C == 1 (DATA)*/
void ILI9341_DATA() {
PIO_Set ( TFTBusCtrlPins + DC_INDEX );
}

/* D/C == 0 (COMMAND)*/
void ILI9341_COMMAND() {
PIO_Clear ( TFTBusCtrlPins + DC_INDEX );
}
/**
  * @brief  Writes to the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @param  LCD_RegValue: value to write to the selected register.
  * @retval None
  */
void LCD_WriteCommand ( uint8_t LCD_Reg ) {
/* Write 8-bit Index, then Write Reg */
ILI9341_COMMAND();
ILI9341_SetDataBus ( LCD_Reg );
Delay ( DELAY );
ILI9341_WR_LOW();
Delay ( DELAY );
ILI9341_WR_HIGH();	}
/**
  * @brief  Writes to the next LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @param  LCD_RegValue: value to write to the selected register.
  * @retval None
  */
void LCD_WriteData ( uint8_t LCD_RegValue ) {
/* Write 8-bit Reg */
ILI9341_DATA();
ILI9341_SetDataBus ( LCD_RegValue );
Delay ( DELAY );
ILI9341_WR_LOW();
Delay ( DELAY );
ILI9341_WR_HIGH();	}
/**
  * @brief  Reads the selected LCD Register.
  * @param  LCD_Reg: address of the selected register.
  * @retval LCD Register Value.
  */
uint8_t LCD_ReadReg ( uint8_t LCD_Reg ) {
uint8_t LCD_Data;

/* Write 16-bit Index (then Read Reg) */
ILI9341_COMMAND();
ILI9341_SetDataBus ( LCD_Reg & 0xFF );
ILI9341_WR_LOW();
ILI9341_WR_HIGH();
/* Read 32-bit Reg (4 by 8-bits Reg) */
ILI9341_DATA();
ILI9341_DataBusIn();
ILI9341_RD_LOW();
LCD_Data = ILI9341_GetDataBus();
ILI9341_RD_HIGH();
ILI9341_DataBusOut();

return ( LCD_Data );
}
/**
  * @brief  Reads the next LCD Register.
  * @retval LCD Register Value.
  */
uint8_t LCD_ReadNextReg ( void ) {
uint8_t LCD_Data;
/* Read 32-bit Reg (4 by 8-bits Reg) */
ILI9341_DATA();
ILI9341_DataBusIn();
ILI9341_RD_LOW();
LCD_Data = ILI9341_GetDataBus();
ILI9341_RD_HIGH();
ILI9341_DataBusOut();
return ( LCD_Data );	}
/**
  * @brief  Reads the selected LCD Register.
  * @param  LCD_Reg: address of the selected register.
  * @retval LCD Register Value.
  */
uint32_t LCD_Read4Reg ( uint8_t LCD_Reg ) {
uint32_t LCD_Data;

/* Write 16-bit Index (then Read Reg) */
ILI9341_COMMAND();
ILI9341_DataBusOut();
ILI9341_SetDataBus ( LCD_Reg & 0xFF );
ILI9341_WR_LOW();
ILI9341_WR_HIGH();
/* Read 32-bit Reg (4 by 8-bits Reg) */
ILI9341_DATA();
ILI9341_DataBusIn();
ILI9341_RD_LOW();
ILI9341_SetDataBus ( 0xFF );
LCD_Data = ILI9341_GetDataBus();
ILI9341_RD_HIGH();

ILI9341_RD_LOW();
ILI9341_SetDataBus ( 0xFF );
LCD_Data = (LCD_Data << 8) | ILI9341_GetDataBus();
ILI9341_RD_HIGH();
ILI9341_RD_LOW();
ILI9341_SetDataBus ( 0xFF );
LCD_Data = (LCD_Data << 8) | ILI9341_GetDataBus();
ILI9341_RD_HIGH();

ILI9341_RD_LOW();
ILI9341_SetDataBus ( 0xFF );
LCD_Data = (LCD_Data << 8) | ILI9341_GetDataBus();
ILI9341_RD_HIGH();
ILI9341_DataBusOut();
return ( LCD_Data );
}
void setAddrWindow ( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 ) {
LCD_WriteCommand ( ILI9341_CASET ); // Column addr set
LCD_WriteData ( x0 >> 8 );
LCD_WriteData ( x0 & 0xFF ); // XSTART
LCD_WriteData ( x1 >> 8 );
LCD_WriteData ( x1 & 0xFF ); // XEND
LCD_WriteCommand ( ILI9341_RASET ); // Row addr set
LCD_WriteData ( y0 >> 8 );
LCD_WriteData ( y0 ); // YSTART
LCD_WriteData ( y1 >> 8 );
LCD_WriteData ( y1 ); // YEND
LCD_WriteCommand ( ILI9341_RAMWR ); // write to RAM
}
int main ( void ) {
SysTick_Config ( MAINCK / 100000 );
// PMC - Power Management Controller
PMC_EnablePeripheral ( ID_PIOA );

ILI9341_DataBusOut();
ILI9341_CtrlBusOut();

image_coor[2] = ((unsigned short)gImage_image[2] << 8) + gImage_image[3];
image_coor[3] = ((unsigned short)gImage_image[4] << 8) + gImage_image[5];

ILI9341_SELECT();
ILI9341_RESET();
ILI9341_WR_HIGH();
ILI9341_RD_HIGH();
/* Read ID */
uiData = LCD_Read4Reg ( ILI9341_READ_ID4 );
// Power control:VRH[5:0]
LCD_WriteCommand ( ILI9341_PWCTR1 );
LCD_WriteData ( 0x23 );
// Power control:SAP[2:0];BT[3:0]
LCD_WriteCommand ( ILI9341_PWCTR2 );
LCD_WriteData ( 0x10 );
// VCM control
LCD_WriteCommand ( ILI9341_VMCTR1 );
LCD_WriteData ( 0x3E );
LCD_WriteData ( 0x28 );
// VCM control2
LCD_WriteCommand ( ILI9341_VMCTR2 );
LCD_WriteData ( 0x86 );
// Memory Access Control
LCD_WriteCommand ( ILI9341_MADCTL );
LCD_WriteData ( 0x48 );
LCD_WriteCommand ( ILI9341_PIXFMT );
LCD_WriteData ( 0x55 );

LCD_WriteCommand ( ILI9341_FRMCTR1 );
LCD_WriteData ( 0x00 );
LCD_WriteData ( 0x18 );
// Display Function Control
LCD_WriteCommand ( ILI9341_DFUNCTR );
LCD_WriteData ( 0x08 );
LCD_WriteData ( 0x82 );
LCD_WriteData ( 0x27 );
//Gamma curve selected
LCD_WriteCommand ( ILI9341_GAMMASET );
LCD_WriteData ( 0x01 );
//Set Gamma
LCD_WriteCommand ( ILI9341_GMCTRP1 );
LCD_WriteData ( 0x0F );
LCD_WriteData ( 0x31 );
LCD_WriteData ( 0x2B );
LCD_WriteData ( 0x0C );
LCD_WriteData ( 0x0E );
LCD_WriteData ( 0x08 );
LCD_WriteData ( 0x4E );
LCD_WriteData ( 0xF1 );
LCD_WriteData ( 0x37 );
LCD_WriteData ( 0x07 );
LCD_WriteData ( 0x10 );
LCD_WriteData ( 0x03 );
LCD_WriteData ( 0x0E );
LCD_WriteData ( 0x09 );
LCD_WriteData ( 0x00 );
//Set Gamma
LCD_WriteCommand ( ILI9341_GMCTRN1 );
LCD_WriteData ( 0x00 );
LCD_WriteData ( 0x0E );
LCD_WriteData ( 0x14 );
LCD_WriteData ( 0x03 );
LCD_WriteData ( 0x11 );
LCD_WriteData ( 0x07 );
LCD_WriteData ( 0x31 );
LCD_WriteData ( 0xC1 );
LCD_WriteData ( 0x48 );
LCD_WriteData ( 0x08 );
LCD_WriteData ( 0x0F );
LCD_WriteData ( 0x0C );
LCD_WriteData ( 0x31 );
LCD_WriteData ( 0x36 );
LCD_WriteData ( 0x0F );
//Exit Sleep
LCD_WriteCommand ( ILI9341_SLPOUT );
//Display on
LCD_WriteCommand ( ILI9341_DISPON );
setAddrWindow(0, 0, image_coor[3] - 1, image_coor[2] - 1);
int i, iMax = 2 * image_coor[2] * image_coor[3];
for ( i = 0; i < iMax; i++ ) {
LCD_WriteData ( gImage_image [ 8 + i ] );
}
/* Display process finished */
PIO_Set ( TFTBusCtrlPins + LED_INDEX );

for (;;) {
}
return 0;
}
