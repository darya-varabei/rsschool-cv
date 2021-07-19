/**
 * \file
 *
 * \brief Startup file for SAM3N.
 *
 * Copyright (c) 2011-2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include "exceptions.h"
#include "system_sam3n.h"

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

void Reset_Handler(void);

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main(void);
/** \endcond */

void IntDefaultHandler(void);

static void TimingDelay_Decrement ( void );

/* Exception Table */
__attribute__ ((section(".vectors")))
IntFunc exception_table[] = {

	/* Configure Initial Stack Pointer, using linker-generated symbols */
	(IntFunc) (&_estack),
	Reset_Handler,

	IntDefaultHandler,
	IntDefaultHandler,
	IntDefaultHandler,
	IntDefaultHandler,
	IntDefaultHandler,
	0, 0, 0, 0,         /* Reserved */
	IntDefaultHandler,
	IntDefaultHandler,
	0,                  /* Reserved  */
	IntDefaultHandler,
	TimingDelay_Decrement,

	/* Configurable interrupts  */
	IntDefaultHandler,    /* 0  Supply Controller */
	IntDefaultHandler,    /* 1  Reset Controller */
	IntDefaultHandler,     /* 2  Real Time Clock */
	IntDefaultHandler,     /* 3  Real Time Timer */
	IntDefaultHandler,     /* 4  Watchdog Timer */
	IntDefaultHandler,     /* 5  PMC */
	IntDefaultHandler,     /* 6  EEFC */
	IntDefaultHandler,   /* 7  Reserved */
	IntDefaultHandler,   /* 8  UART0 */
	IntDefaultHandler,   /* 9  UART1 */
	IntDefaultHandler,   /* 10 Reserved */
	IntDefaultHandler,    /* 11 Parallel IO Controller A */
	IntDefaultHandler,    /* 12 Parallel IO Controller B */
#ifdef ID_PIOC
	IntDefaultHandler,    /* 13 Parallel IO Controller C */
#else
	IntDefaultHandler,
#endif
	IntDefaultHandler,  /* 14 USART 0 */
#ifdef ID_USART1
	IntDefaultHandler,  /* 15 USART 1 */
#else
	IntDefaultHandler,
#endif
	IntDefaultHandler,   /* 16 Reserved */
	IntDefaultHandler,   /* 17 Reserved */
	IntDefaultHandler,   /* 18 Reserved */
	IntDefaultHandler,    /* 19 TWI 0 */
	IntDefaultHandler,    /* 20 TWI 1 */
	IntDefaultHandler,     /* 21 SPI */
	IntDefaultHandler,   /* 22 Reserved */
	IntDefaultHandler,     /* 23 Timer Counter 0 */
	IntDefaultHandler,     /* 24 Timer Counter 1 */
	IntDefaultHandler,     /* 25 Timer Counter 2 */
#ifdef ID_TC3
	IntDefaultHandler,     /* 26 Timer Counter 3 */
#else
	IntDefaultHandler,
#endif
#ifdef ID_TC4
	IntDefaultHandler,     /* 27 Timer Counter 4 */
#else
	IntDefaultHandler,
#endif
#ifdef ID_TC5
	IntDefaultHandler,     /* 28 Timer Counter 5 */
#else
	IntDefaultHandler,
#endif
	IntDefaultHandler,     /* 29 ADC controller */
#ifdef ID_DACC
	IntDefaultHandler,    /* 30 DAC controller */
#else
	IntDefaultHandler,
#endif
	IntDefaultHandler,	 /* 31 PWM */
	IntDefaultHandler    /* 32 not used */
};

/* TEMPORARY PATCH FOR SCB */
#define SCB_VTOR_TBLBASE_Pos               29                            /*!< SCB VTOR: TBLBASE Position */
#define SCB_VTOR_TBLBASE_Msk               (1UL << SCB_VTOR_TBLBASE_Pos) /*!< SCB VTOR: TBLBASE Mask */

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void Reset_Handler(void)
{
	register uint32_t *pSrc, *pDest;

	/* Initialize the relocate segment */
	pSrc = &_etext;
	pDest = &_srelocate;

	if (pSrc != pDest) {
		for (; pDest < &_erelocate;) {
			*pDest++ = *pSrc++;
		}
	}

	/* Clear the zero segment */
	for (pDest = &_szero; pDest < &_ezero;) {
		*pDest++ = 0;
	}

	/* Set the vector table base address */
	pSrc = (uint32_t *) & _sfixed;
	SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

	if (((uint32_t) pSrc >= IRAM_ADDR) && ((uint32_t) pSrc < IRAM_ADDR + IRAM_SIZE)) {
		SCB->VTOR |= 1 << SCB_VTOR_TBLBASE_Pos;
	}

	/* Branch to main function */
	main();

	/* Infinite loop */
	while (1);
}

void IntDefaultHandler(void) {}

volatile unsigned long TimingDelay;

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
static void TimingDelay_Decrement ( void ) {
	if (TimingDelay != 0x00) {
		TimingDelay--;
	}
}

/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in milliseconds
 * @retval None
 */
void Delay ( unsigned long nTime ) {
	TimingDelay = nTime;
	while (TimingDelay != 0);
}
