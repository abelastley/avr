/**********		Program Info		**********
template_avr.c
device: defined in Makefile
sysclock: defined in Makefile
watchdog: disabled completely


********************/



/**********		directives		**********/

#define nop() asm("nop")

#include <avr/io.h>
#include <wait.h>
#include <fuses.h>
#include <ee.h>
#include <avr/signature.h>
#include <avr/eeprom.h>


/**********		global declarations		**********/
	
uint8_t myGolbalVar = 0x02;
uint8_t EEMEM myEepromVar = 0x04;


/**********		init function		**********/

void init(void) __attribute__ ((naked)) __attribute__ ((section(".init3")));
void init(void) {

	// disable WD completely
	MCUSR &= ~(1<<WDRF);
	WDTCR |= 1<<WDCE | 1<<WDE;
	WDTCR &= ~(1<<WDE);


	// initialize PortB
	DDRB = 1<<DDB4 | 1<<DDB3 | 1<<DDB2 | 1<<DDB1 | 1<<DDB0;
	PORTB = 0<<PORTB4 | 0<<PORTB3 | 0<<PORTB2 | 0<<PORTB1 | 0<<PORTB0;
	
	// prescale
	//CLKPR = 1<<CLKPCE;
	//CLKPR = 0<<CLKPS3 | 0<<CLKPS2 | 0<<CLKPS1 | 0<<CLKPS0;

}



/**********		main function		**********/

void main(void) __attribute__ ((naked, noreturn));
void main(void) {

	writeEE(&myEepromVar, 0x08);
	while(1) {
		
		PORTB = 0x01;
		waitms(250);
		PORTB = myGolbalVar;
		waitms(250);
		PORTB = 0x04;
		waitms(250);
		//PORTB = readEE(&myEepromVar);
		PORTB = 0x08;
		waitms(250);
		PORTB = 0x10;
		waitms(250);
	}
}


// [end-of-file]
