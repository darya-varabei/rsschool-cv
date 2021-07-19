#include <stddef.h>
#include "stm32f4xx_conf.h"

#define TIMEOUT_MAX		10000
#define	MIN_X	110
#define	MAX_X	4000
#define	MIN_Y	150
#define	MAX_Y	3850

#define FRAME_SIZE (LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT)
#define CCMDATA_SIZE	(CCMDATARAM_END - CCMDATARAM_BASE - 1)

#define DMA_BUFFER_SIZE	(LCD_PIXEL_WIDTH * 2)

#define MAX_THRESHOLD	31

volatile int32_t ITM_RxBuffer;

__IO uint32_t TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

uint8_t IOE_TS_Config ( void );
TS_STATE* IOE_TS_GetState ( void );

uint8_t Touch_I2C_SingleRandomWrite ( I2C_TypeDef* I2Cx, uint8_t Device, uint16_t Addr, uint8_t Data );
uint8_t Touch_I2C_SingleRandomRead ( I2C_TypeDef* I2Cx, uint8_t Device, uint16_t Addr );

uint32_t IOE_TimeOut = TIMEOUT_MAX;

TS_STATE TS_State;


uint8_t DCMI_OV9655Config ( void );
void DCMI_Config ( void );
void LIS302DL_Reset ( void );
void I2C1_Config ( void );

void Delay ( uint32_t nTime );

void FrameToLCD ( void );

extern const unsigned char gImage_BaseImage[];

uint16_t buttonUp_coor[] = { 272, 74, 320, 122 };
uint16_t buttonDown_coor[] = { 272, 131, 320, 179 };

uint8_t currentThreshold;
uint32_t frame [ DMA_BUFFER_SIZE ];
uint16_t frame_index = 0;

uint16_t * lcdFrame_1 = (uint16_t *)CCMDATARAM_BASE; // 65534 bytes ~= 64KB
uint16_t lcdFrame_2 [ FRAME_SIZE - CCMDATA_SIZE / 2 ]; // 88066 bytes
uint16_t data_0;

int main ( void )
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TS_STATE *pstate = NULL;
	data_0 = *((const uint16_t *)gImage_BaseImage);

	currentThreshold = (currentThreshold > MAX_THRESHOLD) ? 0 : currentThreshold;
	/* SysTick end of count event each 1 ms */
	RCC_GetClocksFreq ( &RCC_Clocks );
    SysTick_Config ( RCC_Clocks.HCLK_Frequency / 1000 );

	LIS302DL_Reset();

	/* Initialize the LCD */
	STM32f4_Discovery_LCD_Init();
	LCD_Clear ( LCD_COLOR_BLACK );
	LCD_SetBackColor ( LCD_COLOR_BLACK);
	LCD_SetTextColor ( LCD_COLOR_MAGENTA );

	DCMI_Control_IO_Init();

	LCD_DisplayStringLine ( LINE(0), (uint8_t *)"                    " );
	LCD_DisplayStringLine ( LINE(1), (uint8_t *)"                    " );
	LCD_DisplayStringLine ( LINE(2), (uint8_t *)" Идёт инициализация " );
	LCD_DisplayStringLine ( LINE(3), (uint8_t *)"      камеры...     " );
	LCD_DisplayStringLine ( LINE(4), (uint8_t *)"                    " );
	LCD_DisplayStringLine ( LINE(5), (uint8_t *)"                    " );
	LCD_DisplayStringLine ( LINE(6), (uint8_t *)"                    " );
	LCD_DisplayStringLine ( LINE(7), (uint8_t *)"                    " );
	LCD_DisplayStringLine ( LINE(8), (uint8_t *)"                    " );
	LCD_DisplayStringLine ( LINE(9), (uint8_t *)"                    " );

	/* OV9655 Camera Module configuration */
	if (DCMI_OV9655Config() == 0x00)
	{
		LCD_DisplayStringLine ( LINE(2), (uint8_t *)"                    " );
		LCD_DisplayStringLine ( LINE(3), (uint8_t *)"                    " );
		LCD_SetDisplayWindow ( 0, 0, LCD_PIXEL_WIDTH, LCD_PIXEL_HEIGHT );
		LCD_WriteRAM_Prepare();
		/* Start Image capture and Display on the LCD ***/
		/* Enable DMA transfer */
		DMA_Cmd ( DMA2_Stream1, ENABLE );
		// Для кадра в ОЗУ
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
	}
	else
	{
		LCD_SetTextColor ( LCD_COLOR_RED );
		LCD_DisplayStringLine ( LINE(2), (uint8_t *)"Инициализация камеры" );
		LCD_DisplayStringLine ( LINE(3), (uint8_t *)"     не прошла!!!   " );
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
	IWDG_Enable();
	while (1)
	{
		static uint16_t FailConnectionCounter = 0;

		I2C1_Config();

		Delay ( 50 );
		IWDG_ReloadCounter();
		pstate = IOE_TS_GetState();
		if ( pstate->TouchDetected )
		{
			FailConnectionCounter = 0;
			if ( (pstate->X > MIN_X) && (pstate->Y > MIN_Y) )
			{
				uint16_t coor_x = (uint16_t)((float)LCD_PIXEL_WIDTH * (MAX_X - pstate->X) / (MAX_X - MIN_X));
				uint16_t coor_y = (uint16_t)((float)LCD_PIXEL_HEIGHT * (pstate->Y - MIN_Y) / (MAX_Y - MIN_Y));
				if ( (coor_x > buttonUp_coor[0]) && (coor_x < buttonUp_coor[2]) && (coor_y > buttonUp_coor[1]) && (coor_y < buttonUp_coor[3]))
				{
					if (++currentThreshold > MAX_THRESHOLD)
					{
						currentThreshold = MAX_THRESHOLD;
					}
				}
				else if ( (coor_x > buttonDown_coor[0]) && (coor_x < buttonDown_coor[2]) &&
						(coor_y > buttonDown_coor[1]) &&
						(coor_y < buttonDown_coor[3]))
				{
					if (currentThreshold > 0)
					{
						currentThreshold--;
					}
				}
			}
		}
		else
		{
			if ((pstate->CurState & 0x01) != 0x01)
			{
				if (++FailConnectionCounter > 100)
				{
					// Переход в бесконечный цикл и ожидание
					// внутреннего сброса микроконтроллера
						while (1);
				}
			}
		}
		FrameToLCD();
	}
	return 0;
}

/**
  * @brief  Configures all needed resources (I2C, DCMI and DMA) to interface with
  *         the OV9655 camera module
  * @param  None
  * @retval 0x00 Camera module configured correctly
  *         0xFF Camera module configuration failed
  */
uint8_t DCMI_OV9655Config ( void )
{
/* I2C1 will be used for OV9655 camera configuration */
	I2C1_Config();
	/* Reset and check the presence of the OV9655 camera module */
	if ( DCMI_SingleRandomWrite ( OV9655_DEVICE_WRITE_ADDRESS, 0x12, 0x80 ) )
	{
		return (0xFF);
	}
	/* OV9655 Camera size setup */
	DCMI_OV9655_QVGASizeSetup();

	/* Set the RGB565 mode */
	DCMI_SingleRandomWrite ( OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, 0x63 );
	DCMI_SingleRandomWrite ( OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM15, 0x10 );
	/* Invert the HRef signal*/
	DCMI_SingleRandomWrite ( OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM10, 0x08 );
/* Configure the DCMI to interface with the OV9655 camera module */
	DCMI_Config();

	return (0x00);
}

/**
  * @brief  Configures the I2C1 used for OV9655 camera module configuration.
  * @param  None
  * @retval None
  */
void I2C1_Config ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStruct;
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
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );

	/* Configure I2C1 ***************/
	/* I2C DeInit */
	I2C_DeInit ( I2C1 );

	/* Enable the I2C peripheral */
	I2C_Cmd ( I2C1, ENABLE );
	/* Set the I2C structure parameters */
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0xFE;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed = I2C_SPEED;
   /* Initialize the I2C peripheral w/ selected parameters */
	I2C_Init ( I2C1, &I2C_InitStruct );
}

/**
  * @brief  Configures the DCMI to interface with the OV9655 camera module.
  * @param  None
  * @retval None
  */
void DCMI_Config ( void )
{
	DCMI_InitTypeDef DCMI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	/* Enable DCMI GPIOs clocks */
    RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA, ENABLE );

	/* Enable DCMI clock */
	RCC_AHB2PeriphClockCmd ( RCC_AHB2Periph_DCMI, ENABLE );
	/* Connect DCMI pins to AF13 ***************/
	/* PCLK */
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
	/* VSYNC */
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
	/* D2..D4(PE0/1/4) D6/D7(PE5/6) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init ( GPIOE, &GPIO_InitStructure );
	/* D5(PB6), VSYNC(PB7) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );
	/* PCLK(PA6) HSYNC(PA4)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );
	/* DCMI configuration **********************/
	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
	DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Falling;
	DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;
	DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_High;
	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
	DCMI_Init ( &DCMI_InitStructure );
  /* Configures the DMA2 to transfer Data from DCMI to the LCD ****/
	/* Enable DMA2 clock */
	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_DMA2, ENABLE );
	/* DMA2 Stream1 Configuration */
	DMA_DeInit ( DMA2_Stream1 );
	DMA_InitStructure.DMA_Channel = DMA_Channel_1;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DCMI->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)frame;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = DMA_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void LIS302DL_Reset ( void )
{
	uint8_t ctrl = 0;

	LIS302DL_InitTypeDef LIS302DL_InitStruct;
	LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptStruct;
	/* Set configuration of LIS302DL*/
	LIS302DL_InitStruct.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
	LIS302DL_InitStruct.Output_DataRate = LIS302DL_DATARATE_100;
	LIS302DL_InitStruct.Axes_Enable = LIS302DL_X_ENABLE | LIS302DL_Y_ENABLE | LIS302DL_Z_ENABLE;
	LIS302DL_InitStruct.Full_Scale = LIS302DL_FULLSCALE_2_3;
	LIS302DL_InitStruct.Self_Test = LIS302DL_SELFTEST_NORMAL;
	LIS302DL_Init ( &LIS302DL_InitStruct );
    /* Set configuration of Internal High Pass Filter of LIS302DL*/
	LIS302DL_InterruptStruct.Latch_Request = LIS302DL_INTERRUPTREQUEST_LATCHED;
	LIS302DL_InterruptStruct.SingleClick_Axes = LIS302DL_CLICKINTERRUPT_Z_ENABLE;
	LIS302DL_InterruptStruct.DoubleClick_Axes = LIS302DL_DOUBLECLICKINTERRUPT_Z_ENABLE;
	LIS302DL_InterruptConfig ( &LIS302DL_InterruptStruct );
	/* Required delay for the MEMS Accelerometre: Turn-on time = 3/Output data Rate
	 = 3/100 = 30ms */
	Delay(30);
	/* Configure Click Window */
	ctrl = 0xC0;
	LIS302DL_Write ( &ctrl, LIS302DL_CLICK_CTRL_REG3_ADDR, 1 );
}

#define SCREEN_WH	(LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT);

void DMA2_Stream1_Handler ( void )
{
	static uint32_t lcdFrameIndex = 0;
	static uint32_t baseImageIndex = 0;
	unsigned long i;
	uint16_t * pCameraPixel, * pBaseImagePixel;
	uint16_t cameraPixel_0, cameraPixel_1;
	uint32_t red, green, blue, lum;
	uint16_t baseImagePixel_0, baseImagePixel_1;
	uint16_t resultPixel_0, resultPixel_1;
    DMA_ClearFlag ( DMA2_Stream1, DMA_FLAG_HTIF1 | DMA_FLAG_TCIF1 );
	for (i = 0; i < (DMA_BUFFER_SIZE / 2); i++)
	{
		pCameraPixel = (uint16_t *)(frame + frame_index++);
		frame_index %= DMA_BUFFER_SIZE;

		pBaseImagePixel = (uint16_t *) gImage_BaseImage + baseImageIndex;
		baseImageIndex = ( baseImageIndex + 2) % SCREEN_WH;

		cameraPixel_0 = *pCameraPixel;
		cameraPixel_1 = pCameraPixel[1];

		red = (cameraPixel_0 & LCD_COLOR_RED) >> 11;
		green = (cameraPixel_0 & LCD_COLOR_GREEN) >> 6;
		blue = cameraPixel_0 & LCD_COLOR_BLUE;
		lum = (77 * red + 151 * green + 28 * blue) >> 8;
		if (lum < currentThreshold)
		{
			lum = 0;
		}
		cameraPixel_0 = (lum << 11) | (lum << 6) | lum;

		red = (cameraPixel_1 & LCD_COLOR_RED) >> 11;
		green = (cameraPixel_1 & LCD_COLOR_GREEN) >> 6;
		blue = cameraPixel_1 & LCD_COLOR_BLUE;
		lum = (77 * red + 151 * green + 28 * blue) >> 8;
		if (lum < currentThreshold)
		{
			lum = 0;
		}
		cameraPixel_1 = (lum << 11) | (lum << 6) | lum;

		baseImagePixel_0 = *pBaseImagePixel;
		baseImagePixel_1 = pBaseImagePixel[1];

		resultPixel_0 = (baseImagePixel_0 == data_0) ?
		cameraPixel_0 : baseImagePixel_0;
		resultPixel_1 = (baseImagePixel_1 == data_0) ?
		cameraPixel_1 : baseImagePixel_1;

		baseImagePixel_0 = *pBaseImagePixel;
		baseImagePixel_1 = pBaseImagePixel[1];

		if (lcdFrameIndex <= CCMDATA_SIZE / 2)
		{
			lcdFrame_1 [ lcdFrameIndex ] = resultPixel_0;
		}
		else
		{
			lcdFrame_2 [ lcdFrameIndex - CCMDATA_SIZE / 2 - 1 ] = resultPixel_0;
		}
		lcdFrameIndex=(lcdFrameIndex+1)%FRAME_SIZE;
		if (lcdFrameIndex <= CCMDATA_SIZE / 2)
		{
			lcdFrame_1 [ lcdFrameIndex ] = resultPixel_1;
		}
		else
		{
			lcdFrame_2 [ lcdFrameIndex - CCMDATA_SIZE / 2 - 1 ] = resultPixel_1;
		}
		lcdFrameIndex=(lcdFrameIndex+1)%FRAME_SIZE;
	}
}

void FrameToLCD ( void )
{
	int i;
	LCD_WriteReg(SSD2119_X_RAM_ADDR_REG, 0x00);
	LCD_WriteReg(SSD2119_Y_RAM_ADDR_REG, 0x00);
	LCD_WriteReg(SSD2119_RAM_DATA_REG, lcdFrame_1[0]);
	for (i = 1; i <= CCMDATA_SIZE / 2; i++)
	{
		LCD_WriteRAM ( lcdFrame_1 [ i ] );
	}
	for (i=0; i< FRAME_SIZE - CCMDATA_SIZE / 2; i++)
	{
			LCD_WriteRAM ( lcdFrame_2 [ i ] );

	}
}

/**
  * @brief  Configures the touch Screen Controller (Single point detection)
  * @param  None
  * @retval IOE_OK if all initializations are OK. Other value if error.
  */
uint8_t IOE_TS_Config ( void )
{
	uint8_t tmp = 0;
	/* Enable TSC Fct: already done in IOE_Config */
	tmp = Touch_I2C_SingleRandomRead ( IOE_I2C, IOE_1_ADDR, IOE_REG_SYS_CTRL2 );
	tmp &= ~(uint32_t) ( IOE_TS_FCT | IOE_ADC_FCT );
	Touch_I2C_SingleRandomWrite ( IOE_I2C, IOE_1_ADDR, IOE_REG_SYS_CTRL2, tmp );
	/* Enable the TSC global interrupts */
	tmp = Touch_I2C_SingleRandomRead ( IOE_I2C, IOE_1_ADDR, IOE_REG_INT_EN );
	tmp |= (uint32_t) ( IOE_GIT_TOUCH | IOE_GIT_FTH );
	Touch_I2C_SingleRandomWrite ( IOE_I2C, IOE_1_ADDR, IOE_REG_INT_EN, tmp );

	/* Select Sample Time, bit number and ADC Reference */
	Touch_I2C_SingleRandomWrite ( IOE_I2C, IOE_1_ADDR, IOE_REG_ADC_CTRL1, 0x49 );

	/* Wait for ~2 ms */
	Delay(2);
	/* Select the ADC clock speed: 3.25 MHz */
	Touch_I2C_SingleRandomWrite ( IOE_I2C, IOE_1_ADDR, IOE_REG_ADC_CTRL2, 0x01 );

	/* Select TSC pins in non default mode */
	tmp = Touch_I2C_SingleRandomRead ( IOE_I2C, IOE_1_ADDR, IOE_REG_GPIO_AF );
	tmp &= ~(uint8_t) TOUCH_IO_ALL;
	Touch_I2C_SingleRandomWrite ( IOE_I2C, IOE_1_ADDR, IOE_REG_GPIO_AF, tmp );
	/* Select 2 nF filter capacitor */
	Touch_I2C_SingleRandomWrite ( IOE_I2C, IOE_1_ADDR, IOE_REG_TSC_CFG, 0x9A );
	/* Select single point reading  */
	Touch_I2C_SingleRandomWrite ( IOE_I2C, IOE_1_ADDR, IOE_REG_FIFO_TH, 0x01 );

	/* Write 0x01 to clear the FIFO memory content. */
	Touch_I2C_SingleRandomWrite ( IOE_I2C, IOE_1_ADDR, IOE_REG_FIFO_STA, 0x01 );

	/* Write 0x00 to put the FIFO back into operation mode  */
	Touch_I2C_SingleRandomWrite ( IOE_I2C, IOE_1_ADDR, IOE_REG_FIFO_STA, 0x00 );
	/* set the data format for Z value: 7 fractional part and 1 whole part */
	Touch_I2C_SingleRandomWrite ( IOE_I2C, IOE_1_ADDR, IOE_REG_TSC_FRACT_XYZ, 0x01 );

	/* set the driving capability of the device for TSC pins: 50mA */
	Touch_I2C_SingleRandomWrite ( IOE_I2C, IOE_1_ADDR, IOE_REG_TSC_I_DRIVE, 0x01 );

	/* Use no tracking index, touchscreen controller operation mode (XYZ) and enable the TSC */
	Touch_I2C_SingleRandomWrite ( IOE_I2C, IOE_1_ADDR, IOE_REG_TSC_CTRL, 0x01 );
	/*  Clear all the status pending bits */
	Touch_I2C_SingleRandomWrite ( IOE_I2C, IOE_1_ADDR, IOE_REG_INT_STA, 0xFF );

	/* Initialize the TS structure to their default values */
	TS_State.TouchDetected = TS_State.X = TS_State.Y = TS_State.Z = 0;

	/* All configuration done */
	return IOE_OK;
}

/**
  * @brief  Returns Status and positions of the Touch screen.
  * @param  None
  * @retval Pointer to TS_STATE structure holding Touch Screen information.
  */
TS_STATE* IOE_TS_GetState ( void )
{
	uint16_t xDiff, yDiff, x, y;
	static uint16_t _x = 0, _y = 0;
	/* Check if the Touch detect event happened */
	TS_State.CurState = Touch_I2C_SingleRandomRead ( IOE_I2C, IOE_1_ADDR, IOE_REG_TSC_CTRL );
	TS_State.TouchDetected = TS_State.CurState & 0x80;
	if ( TS_State.TouchDetected )
	{
		x = ( Touch_I2C_SingleRandomRead ( IOE_I2C, IOE_1_ADDR, IOE_REG_TSC_DATA_X ) << 8 ) | Touch_I2C_SingleRandomRead ( IOE_I2C, IOE_1_ADDR, IOE_REG_TSC_DATA_X + 1 );
		y = ( Touch_I2C_SingleRandomRead ( IOE_I2C, IOE_1_ADDR, IOE_REG_TSC_DATA_Y ) << 8 ) | Touch_I2C_SingleRandomRead ( IOE_I2C, IOE_1_ADDR, IOE_REG_TSC_DATA_Y + 1 );
		xDiff = x > _x ? (x - _x) : (_x - x);
		yDiff = y > _y ? (y - _y) : (_y - y);
		if (xDiff + yDiff > 5)
		{
			_x = x;
			_y = y;
		}
	}
	/* Update the X position */
	TS_State.X = _x;
	/* Update the Y position */
	TS_State.Y = _y;
	/* Update the Z position index */
	TS_State.Z = Touch_I2C_SingleRandomRead ( IOE_I2C, IOE_1_ADDR, IOE_REG_TSC_DATA_Z );
	/* Clear the interrupt pending bit and enable the FIFO again */
	Touch_I2C_SingleRandomWrite ( IOE_I2C, IOE_1_ADDR, IOE_REG_FIFO_STA, 0x01 );
	Touch_I2C_SingleRandomWrite ( IOE_I2C, IOE_1_ADDR, IOE_REG_FIFO_STA, 0x00 );
	/* Return pointer to the updated structure */
	return &TS_State;
}

/**
  * @brief  Writes a byte at a specific TS register
  * @param  Device: device address
  * @param  Addr: register address
  * @param  Data: data to be written to the specific register
  * @retval 0x00 if write operation is OK
  *         0xFF if timeout condition occured (device not connected or bus error).
  */
uint8_t Touch_I2C_SingleRandomWrite ( I2C_TypeDef* I2Cx, uint8_t Device, uint16_t Addr, uint8_t Data )
{
	uint32_t timeout = TIMEOUT_MAX;
	/* Generate the Start Condition */
	I2C_GenerateSTART ( I2Cx, ENABLE );
	/* Test on I2Cx EV5 and clear it */
	timeout = TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2Cx, I2C_EVENT_MASTER_MODE_SELECT ))
	{
		/* If the timeout delay is exceeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}
	/* Send TS selected device slave Address for write */
	I2C_Send7bitAddress ( I2Cx, Device, I2C_Direction_Transmitter );
	/* Test on I2Cx EV6 and clear it */
	timeout = TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING )) {
		/* If the timeout delay is exceeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}
	/* Send I2Cx location address LSB */
	I2C_SendData ( I2Cx, (uint8_t) (Addr) );
	/* Test on I2Cx EV8 and clear it */
	timeout = TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED )) {
		/* If the timeout delay is exceeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}
	/* Send Data */
	I2C_SendData ( I2Cx, Data );
	/* Test on I2Cx EV8 and clear it */
	timeout = TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED )) {
		/* If the timeout delay is exceeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;	}
	/* Send I2Cx STOP Condition */
	I2C_GenerateSTOP ( I2Cx, ENABLE );
	/* If operation is OK, return 0 */
	return 0;
}

/**
  * @brief  Reads a byte from a specific TS register
  * @param  Device: device address
  * @param  Addr: register address
  * @retval data read from the specific register or 0xFF if timeout condition
  *         occured
  */
uint8_t Touch_I2C_SingleRandomRead ( I2C_TypeDef* I2Cx, uint8_t Device, uint16_t Addr )
{
	uint32_t timeout = TIMEOUT_MAX;
	uint8_t Data = 0;
	/* Generate the Start Condition */
	I2C_GenerateSTART ( I2Cx, ENABLE );
	/* Test on I2Cx EV5 and clear it */
	timeout = TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2Cx, I2C_EVENT_MASTER_MODE_SELECT )) {
		/* If the timeout delay is exceeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}
	/* Send TS selected device slave Address for write */
	I2C_Send7bitAddress ( I2Cx, Device, I2C_Direction_Transmitter );
	/* Test on I2Cx EV6 and clear it */
	timeout = TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ))
	{
		/* If the timeout delay is exceeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}
	/* Send I2Cx location address LSB */
	I2C_SendData ( I2Cx, (uint8_t) (Addr) );
	/* Test on I2Cx EV8 and clear it */
	timeout = TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ))
	{
		/* If the timeout delay is exceeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}
	/* Clear AF flag if arised */
	I2Cx->SR1 |= (uint16_t) 0x0400;
	/* Generate the Stop Condition */
	I2C_GenerateSTOP ( I2Cx, ENABLE );
	/* Generate the Start Condition */
	I2C_GenerateSTART ( I2Cx, ENABLE );
	/* Test on I2Cx EV6 and clear it */
	timeout = TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2Cx, I2C_EVENT_MASTER_MODE_SELECT ))
	{
		/* If the timeout delay is exceeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}
	/* Send TS selected device slave Address for write */
	I2C_Send7bitAddress ( I2Cx, Device, I2C_Direction_Receiver );
	/* Test on I2Cx EV6 and clear it */
	timeout = TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ))
	{
		/* If the timeout delay is exceeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}
	/* Prepare an NACK for the next data received */
	I2C_AcknowledgeConfig ( I2Cx, DISABLE );
	/* Test on I2Cx EV7 and clear it */
	timeout = TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED ))
	{
	/* If the timeout delay is exceeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}
	/* Prepare Stop after receiving data */
	I2C_GenerateSTOP ( I2Cx, ENABLE );
	/* Receive the Data */
	Data = I2C_ReceiveData ( I2Cx );
	/* return the read data */
	return Data;
}









































