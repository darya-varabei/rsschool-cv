/*
 * main.c
 *
 *  Created on: 15.10.2014
 *      Author: Oleg
 */

#include "stm32f4xx_conf.h"

volatile int32_t ITM_RxBuffer;                    /*!< external variable to receive characters                    */

RCC_ClocksTypeDef RCC_Clocks;

typedef enum {
	RED = 0,
	GREEN,
	BLUE,
} LED_t;

extern void ResetTickCountRed();
extern void ResetTickCountGreen();
extern void ResetTickCountBlue();

extern int32_t GetTickCountRed();
extern int32_t GetTickCountGreen();
extern int32_t GetTickCountBlue();

// Массив начальных времён [мс] включений красного, зелёного и синего светодиодов
static int32_t arStartTime[] = { 700, 1400, 2100 };
// Массив счётчиков итераций управления состояниями
//  красного [0], зелёного [1] и синего [2] светодиодов
static uint32_t arPulse[] = { 0, 0, 0 };
// Массив состояний красного, зелёного и синего светодиодов
//  (DISABLE - выключен, ENABLE - включен)
static FunctionalState arState[] = { DISABLE, DISABLE, DISABLE };
// Массив состояний "направления включения" красного, зелёного и синего светодиодов
//  (DISABLE - выключение, ENABLE - включение)
static FunctionalState arLightState[] = { DISABLE, DISABLE, DISABLE };
// Массив функций сброса таймеров-счётчиков красного, зелёного и синего светодиодов
static void (*ResetTickCount[])(void) = {
		ResetTickCountRed,
		ResetTickCountGreen,
		ResetTickCountBlue,
};
// Массив функций получения текущих значений таймеров-счётчиков красного, зелёного и синего светодиодов
static int32_t (*GetTickCount[])(void) = {
		GetTickCountRed,
		GetTickCountGreen,
		GetTickCountBlue,
};
// Массив функций коррекции ширины импульсов для красного, зелёного и синего светодиодов
static void (*TIM_SetCompare[])(TIM_TypeDef* TIMx, uint32_t Compare) = {
		TIM_SetCompare1,
		TIM_SetCompare3,
		TIM_SetCompare4,
};

extern const unsigned char gImage_Info[];
unsigned short InfoImage_coor[] = {0, 0, 0, 0};

void LedStateProcess ( LED_t led, uint32_t PeriodPWM );
void ShowImage ( const unsigned char * pImage, unsigned short * pCoordinates );

int main (void) {
	GPIO_InitTypeDef		GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef 		TIM_OCInitStruct;

	InfoImage_coor[2] = ((unsigned short)gImage_Info[3] << 8) + gImage_Info[2];
	InfoImage_coor[3] = ((unsigned short)gImage_Info[5] << 8) + gImage_Info[4];

	/* SysTick end of count event each 1 ms */
	RCC_GetClocksFreq ( &RCC_Clocks );
	SysTick_Config ( RCC_Clocks.HCLK_Frequency / 1000 );

	STM32f4_Discovery_LCD_Init();
	ShowImage ( gImage_Info, InfoImage_coor );

	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOD, ENABLE );

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init ( GPIOD, &GPIO_InitStruct );

	GPIO_PinAFConfig ( GPIOD, GPIO_PinSource12, GPIO_AF_TIM4 );
	GPIO_PinAFConfig ( GPIOD, GPIO_PinSource14, GPIO_AF_TIM4 );
	GPIO_PinAFConfig ( GPIOD, GPIO_PinSource15, GPIO_AF_TIM4 );

	RCC_APB1PeriphClockCmd ( RCC_APB1Periph_TIM4, ENABLE );

	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 1024;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit ( TIM4, &TIM_TimeBaseInitStruct );

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;

	TIM_OC1Init ( TIM4, &TIM_OCInitStruct );
	TIM_OC3Init ( TIM4, &TIM_OCInitStruct );
	TIM_OC4Init ( TIM4, &TIM_OCInitStruct );

	TIM_CtrlPWMOutputs ( TIM4, ENABLE );

	TIM_CCxCmd ( TIM4, TIM_Channel_1, TIM_CCx_Enable );
	TIM_CCxCmd ( TIM4, TIM_Channel_3, TIM_CCx_Enable );
	TIM_CCxCmd ( TIM4, TIM_Channel_4, TIM_CCx_Enable );

	TIM_Cmd ( TIM4, ENABLE );

	ResetTickCountRed();
	ResetTickCountGreen();
	ResetTickCountBlue();

	while (1) {
		LedStateProcess(RED, TIM_TimeBaseInitStruct.TIM_Period);
		LedStateProcess(GREEN, TIM_TimeBaseInitStruct.TIM_Period);
		LedStateProcess(BLUE, TIM_TimeBaseInitStruct.TIM_Period);
	}
	return 1;
}

///\brief	Show small image
void ShowImage ( const unsigned char * pImage, unsigned short * pCoordinates ) {
	unsigned long ulIndex_i, ulIndex_j;

	unsigned long bmpAddress = (unsigned long) pImage + 8;
	for ( ulIndex_i = 0; ulIndex_i < pCoordinates[3]; ulIndex_i++ ) {
		LCD_WriteReg ( SSD2119_X_RAM_ADDR_REG, pCoordinates[0] );
		LCD_WriteReg ( SSD2119_Y_RAM_ADDR_REG, pCoordinates[1] + ulIndex_i );
		LCD_WriteReg ( SSD2119_RAM_DATA_REG, *(__IO uint16_t *) bmpAddress );
		for ( ulIndex_j = 0; ulIndex_j < pCoordinates[2]; ulIndex_j++ ) {
			LCD_WriteRAM ( *(__IO uint16_t *) bmpAddress );
			bmpAddress += 2;
		}
	}
}

// Обработка состояния светодиода
void LedStateProcess ( LED_t led, uint32_t PeriodPWM ) {
	if (arState[led] == DISABLE) {
		if (GetTickCount[led]() > arStartTime[led]) {
			arState[led] = ENABLE;
			arLightState[led] = ENABLE;
			arPulse[led] = 0;
			ResetTickCount[led]();
		}
	} else {
		if (GetTickCount[led]() >= (PeriodPWM - arPulse[led]) / 200) {
			ResetTickCount[led]();
			if (arLightState[led] == ENABLE) {
				if (++arPulse[led] == PeriodPWM) {
					arLightState[led] = DISABLE;
				}
			} else {
				if (--arPulse[led] == 0) {
					arLightState[led] = ENABLE;
				}
			}
		}
		TIM_SetCompare[led] ( TIM4, arPulse[led] );
	}
}
