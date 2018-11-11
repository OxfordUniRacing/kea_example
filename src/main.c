#include <SKEAZ1284.h>

#include "can.h"

#define PTE7 7         /* Port PTE7, bit 7: output to blue LED */
#define PTE4 4         /* Port PTE4, bit 4: input from BTN0*/

static void spi_init(void) {

	SIM_SCGC |= SIM_SCGC_SPI1_MASK; // enable preph clock

	// master
	// CPOL=0, CPHA=1
	// msb first
	// auto ss output
	// bus clock div 32

	SPI1_BR = 4;
	SPI1_C2 = SPI_C2_MODFEN_MASK;
	SPI1_C1 = SPI_C1_SPE_MASK | SPI_C1_MSTR_MASK | SPI_C1_CPHA_MASK | SPI_C1_SSOE_MASK;
}

int main(void) {
	                          /* Configure port E4 as GPIO input (BTN 0 [SW2] on EVB) */
	GPIOB_PDDR &= ~(1<<PTE4); /* Port E4: Data Direction= input (default) */
	GPIOB_PIDR &= ~(1<<PTE4); /* Port E4: Input Disable= 0 (input enabled) */
	PORT_PUE0 |= 0<<PTE4;     /* Port E4: No internal pullup (default) */
	GPIOB_PSOR |= 1<<PTE7;    /* Set Output on port E7 (LED off) */
	
	                          /* Configure port E7 as GPIO output (LED on EVB) */
	GPIOB_PDDR |= 1<<PTE7;    /* Port E7: Data Direction= output */
	GPIOB_PIDR &= 1<<PTE7;    /* Port E7: Input Disable= 1 (default) */

	/*
	for (int i = 0; i < 10000; i++);

	spi_init();

	while (!(SPI1_S & SPI_S_SPTEF_MASK));
	SPI1_D = 0x5A;
	while (!(SPI1_S & SPI_S_SPTEF_MASK));
	SPI1_D = 0x00;

	for (int i = 0; i < 10000; i++);

	while (!(SPI1_S & SPI_S_SPTEF_MASK));
	SPI1_D = 0x60;
	while (!(SPI1_S & SPI_S_SPTEF_MASK));
	SPI1_D = 0xC0;

	for (int i = 0; i < 10000; i++);
	*/

	do_init();

#ifdef TX
	while (1) {
		int ret = do_tx();
		if (!ret) {
			GPIOB_PDOR ^= 1 << PTE7; // toogle led
		} else {
			asm("nop"); // put breakpoint here to see error
		}
		for (int i = 0; i < 1000000; i++); // delay for a while
	}
#else
	while (1) {
		int ret = do_rx();
		if (!ret) {
			GPIOB_PDOR ^= 1 << PTE7; // toogle led
		} else {
			asm("nop"); // put breakpoint here to see error
		}
	}
#endif

	//for(;;) {
	//	if (GPIOB_PDIR & (1<<PTE4))  { /* If Pad Data Input = 1 (BTN0 [SW2] not pushed) */
	//		GPIOB_PSOR |= 1<<PTE7;     /* Set Output on port E7 (LED off) */
	//	}
	//	else {                      /* If BTN0 was pushed */
	//		GPIOB_PCOR |= 1<<PTE7;  /* Clear Output on port E7 (LED on) */
	//	}
	//}
}
