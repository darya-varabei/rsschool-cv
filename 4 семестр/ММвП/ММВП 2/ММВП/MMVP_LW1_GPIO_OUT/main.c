/*
 * main.c
 *
 *  Created on: 15.10.2014
 *      Author: Oleg
 */

#include <stddef.h>

#include <stdio.h>

#include "stm32f4xx_conf.h"

#define TIMEOUT_MAX 10000

#define MIN_X 110

#define MAX_X 4000

#define MIN_Y 150

#define MAX_Y 3850

#define RED_FILTER 0xF800

#define GREEN_FILTER 0x07E0
#define YELLOW_FILTER
0xFFE0

#define CYAN_FILTER
0x07FF

#define MAGENTA_FILTER
0xF81F

#define WHITE_FILTER
0xFFFF

#define FRAME_SIZE
16384

volatile int32_t ITM_RxBuffer;

__IO uint32_t TimingDelay;
uint8_t IOE_TS_Config ( void );

TS_STATE* IOE_TS_GetState ( void );

uint8_t Touch_I2C_SingleRandomWrite ( I2C_TypeDef*
I2Cx, uint8_t Device, uint16_t Addr, uint8_t Data );

uint8_t Touch_I2C_SingleRandomRead (
I2C_TypeDef* I2Cx, uint8_t Device, uint16_t Addr );

uint32_t IOE_TimeOut = TIMEOUT_MAX;
uint8_t DCMI_OV9655Config ( void );

void DCMI_Config ( void );

void LIS302DL_Reset ( void );

void I2C1_Config ( void );

void Delay ( uint32_t nTime );

extern const unsigned char gImage_BaseImage[];
uint16_t redButton_coor[] = {202,68,318,108};

uint16_t greenButton_coor[] = {202,118,318,158};

uint16_t blueButton_coor[] = {202,168,318,208};

uint16_t * currentButton;

uint32_t frame [ FRAME_SIZE ];

uint16_t frame_index = 0;

uint16_t data_0 = WHITE_FILTER;
uint16_t filtersArray[] = {

RED_FILTER,

GREEN_FILTER,

BLUE_FILTER,

WHITE_FILTER,

};

int main ( void ) {

NVIC_InitTypeDef NVIC_InitStructure;

TS_STATE *pstate = NULL;
uint8_t i;

for (i = 0; i < sizeof(filtersArray)/sizeof(uint16_t); i++) {

if (currentFilter == filtersArray[i]) {

break;

}

}

if (i == sizeof(filtersArray)/sizeof(uint16_t)) {

currentFilter = WHITE_FILTER;

currentButton = NULL;

}
/* SysTick end of count event each 1 ms */

RCC_GetClocksFreq ( &RCC_Clocks );

SysTick_Config ( RCC_Clocks.HCLK_Frequency / 1000 );

LIS302DL_Reset();

/* Initialize the LCD */

STM32f4_Discovery_LCD_Init();

LCD_Clear ( LCD_COLOR_BLACK );

LCD_SetBackColor ( LCD_COLOR_BLACK);
DCMI_Control_IO_Init();

LCD_DisplayStringLine ( LINE(0), (uint8_t *)" " );

LCD_DisplayStringLine ( LINE(1), (uint8_t *)" " );

LCD_DisplayStringLine ( LINE(2), (uint8_t *)" Идёт инициализация " );

LCD_DisplayStringLine ( LINE(3), (uint8_t *)" камеры... " );

LCD_DisplayStringLine ( LINE(4), (uint8_t *)" " );

LCD_DisplayStringLine ( LINE(5), (uint8_t *)" " );

LCD_DisplayStringLine ( LINE(6), (uint8_t *)" " );

LCD_DisplayStringLine ( LINE(7), (uint8_t *)" " );

LCD_DisplayStringLine ( LINE(8), (uint8_t *)" " );

LCD_DisplayStringLine ( LINE(9), (uint8_t *)" " );
/* OV9655 Camera Module configuration */

if (DCMI_OV9655Config() == 0x00) {

LCD_DisplayStringLine ( LINE(2), (uint8_t *)" " );

LCD_DisplayStringLine ( LINE(3), (uint8_t *)" " );

LCD_SetDisplayWindow ( 0, 0,
LCD_PIXEL_WIDTH, LCD_PIXEL_HEIGHT );

LCD_WriteRAM_Prepare();

/* Start Image capture and Display on the LCD ***/

/* Enable DMA transfer */
DMA_ITConfig ( DMA2_Stream1, DMA_IT_HT | DMA_IT_TC, ENABLE );

NVIC_PriorityGroupConfig ( NVIC_PriorityGroup_0 );

NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;

NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

NVIC_Init ( &NVIC_InitStructure );
/* Enable DCMI interface */

DCMI_Cmd ( ENABLE );

/* Start Image capture */

DCMI_CaptureCmd ( ENABLE );

} else {

LCD_SetTextColor ( LCD_COLOR_RED );

LCD_DisplayStringLine ( LINE(2), (uint8_t *)"Инициализация камеры" );

LCD_DisplayStringLine ( LINE(3), (uint8_t *)" не прошла!!! " );

LCD_DisplayStringLine ( LINE(5), (uint8_t *)"Проверьте соединение" );

LCD_DisplayStringLine ( LINE(6), (uint8_t *)" и повторите попытку" );
/* Go to infinite loop */

while (1);

}

/* TouchScreen interface initialization */

IOE_TS_Config();

/* Настройка сторожевого таймера */

IWDG_SetReload ( 0xFFF );

IWDG_ReloadCounter();
while (1) {

static uint16_t FailConnectionCounter = 0;

I2C1_Config();

Delay ( 100 );

IWDG_ReloadCounter();

pstate = IOE_TS_GetState();

if ( pstate->TouchDetected ) {

FailConnectionCounter = 0;
uint16_t coor_x =
(uint16_t)((float)LCD_PIXEL_WIDTH

* (MAX_X - pstate->X) / (MAX_X - MIN_X));

uint16_t coor_y =
(uint16_t)((float)LCD_PIXEL_HEIGHT

* (pstate->Y - MIN_Y) / (MAX_Y - MIN_Y));

if ( (coor_x > redButton_coor[0])

&& (coor_x < redButton_coor[2])

&& (coor_y > redButton_coor[1])

&& (coor_y < redButton_coor[3])) {
	currentButton = redButton_coor;

	} else if (

	(coor_x > greenButton_coor[0])

	&& (coor_x < greenButton_coor[2])

	&& (coor_y > greenButton_coor[1])

	&& (coor_y < greenButton_coor[3])) {

	currentFilter = GREEN_FILTER;

	currentButton = greenButton_coor;

	} else if (

	(coor_x > blueButton_coor[0])
	&& (coor_y > blueButton_coor[1])

	&& (coor_y < blueButton_coor[3])) {

	currentFilter = BLUE_FILTER;

	currentButton = blueButton_coor;

	} else if (

	(coor_x > 0) && (coor_x < LCD_PIXEL_WIDTH)

	&& (coor_y > 0) && (coor_y < 52)) {

	currentFilter = WHITE_FILTER;

	currentButton = NULL;

	}}
if ((pstate->CurState & 0x01) != 0x01) {

if (++FailConnectionCounter > 100) {

// Переход в бесконечный цикл и ожидание

// внутреннего сброса микроконтроллера

while (1);

}

}

}

}

return 0;
uint8_t DCMI_OV9655Config ( void ) {
	if ( DCMI_SingleRandomWrite (
	OV9655_DEVICE_WRITE_ADDRESS, 0x12, 0x80 ) ) {

	return (0xFF);

	}

	/* OV9655 Camera size setup */

	DCMI_OV9655_QVGASizeSetup();

	/* Set the RGB565 mode */

	DCMI_SingleRandomWrite (
			DCMI_SingleRandomWrite (
			OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM15, 0x10 );

			/* Invert the HRef signal*/

			DCMI_SingleRandomWrite (
			OV9655_DEVICE_WRITE_ADDRESS,
			OV9655_COM10, 0x08 );

			/* Configure the DCMI to interface with the OV9655 camera module */

			DCMI_Config();

			return (0x00);
			void I2C1_Config ( void ) {

			GPIO_InitTypeDef GPIO_InitStructure;

			I2C_InitTypeDef I2C_InitStruct;
			/* I2C1 clock enable */

			RCC_APB1PeriphClockCmd ( RCC_APB1Periph_I2C1, ENABLE );

			/* GPIOB clock enable */

			RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOB, ENABLE );

			/* Connect I2C1 pins to AF4 *****************/

			GPIO_PinAFConfig ( GPIOB, GPIO_PinSource9, GPIO_AF_I2C1 );

			GPIO_PinAFConfig ( GPIOB, GPIO_PinSource8, GPIO_AF_I2C1 );

			/* Configure I2C1 GPIOs ********************/

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;

			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;

			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

			GPIO_Init ( GPIOB, &GPIO_InitStructure );

			/* Configure I2C1 ***************/

			/* I2C DeInit */

			I2C_DeInit ( I2C1 );

			/* Enable the I2C peripheral */

			I2C_Cmd ( I2C1, ENABLE );
			I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;

			I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;

			I2C_InitStruct.I2C_OwnAddress1 = 0xFE;

			I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;

			I2C_InitStruct.I2C_AcknowledgedAddress =
			I2C_AcknowledgedAddress_7bit;

			I2C_InitStruct.I2C_ClockSpeed = I2C_SPEED;

			/* Initialize the I2C peripheral w/ selected parameters */

			I2C_Init ( I2C1, &I2C_InitStruct );
			void DCMI_Config ( void ) {

			DCMI_InitTypeDef DCMI_InitStructure;

			GPIO_InitTypeDef GPIO_InitStructure;

			DMA_InitTypeDef DMA_InitStructure;
			RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOC |
			RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOB |
			RCC_AHB1Periph_GPIOA, ENABLE );

			/* Enable DCMI clock */

			RCC_AHB2PeriphClockCmd (
			RCC_AHB2Periph_DCMI, ENABLE );
			GPIO_PinAFConfig ( GPIOA, GPIO_PinSource6, GPIO_AF_DCMI );

			/* D0-D7 */

			GPIO_PinAFConfig ( GPIOC, GPIO_PinSource6, GPIO_AF_DCMI );

			GPIO_PinAFConfig ( GPIOC, GPIO_PinSource7, GPIO_AF_DCMI );

			GPIO_PinAFConfig ( GPIOE, GPIO_PinSource0, GPIO_AF_DCMI );

			GPIO_PinAFConfig ( GPIOE, GPIO_PinSource1, GPIO_AF_DCMI );

			GPIO_PinAFConfig ( GPIOE, GPIO_PinSource4, GPIO_AF_DCMI );

			GPIO_PinAFConfig ( GPIOB, GPIO_PinSource6, GPIO_AF_DCMI );

			GPIO_PinAFConfig ( GPIOE, GPIO_PinSource5, GPIO_AF_DCMI );

			GPIO_PinAFConfig ( GPIOE, GPIO_PinSource6, GPIO_AF_DCMI );
			GPIO_PinAFConfig ( GPIOB, GPIO_PinSource7, GPIO_AF_DCMI );

			/* HSYNC */

			GPIO_PinAFConfig ( GPIOA, GPIO_PinSource4, GPIO_AF_DCMI );

			/* DCMI GPIO configuration *****************/

			/* D0 D1(PC6/7) */

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;

			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

			GPIO_Init ( GPIOC, &GPIO_InitStructure );
}
