/**********		Program Info		**********
bike.c
device: defined in Makefile
sysclock: defined in Makefile
watchdog: disabled completely


--pin assignments
	--PB0: FRONT_BUTTON
	--PB1: BACK_BUTTON
	--PB2: FRONT_LED
	--PB3: BACK_LED
	--PB4: SWITCH
	
--front notes
	--red: power (4.5v)
	--black: GND
	--yellow: FRONT_LED
	--blue: FRONT_BUTTON
		--FRONT_BUTTON is floating when button isn't pushed and pulled to 4V5 when
		button is pushed

--back notes
	--red: power (3.0v)
	--black: GND
	--yellow: BACK_LED
	--blue: BACK BUTTON
		--BACK_BUTTON is floating when button isn't pushed and pulled to GND
		when it is

--SWITCH is low when switch is turned on


********************/



/**********		directives		**********/

#define nop() asm("nop")

#include <avr/io.h>
#include <avr/interrupt.h>
#include <wait.h>
#include <fuses.h>
//#include <ee.h>
#include <avr/signature.h>
//#include <avr/eeprom.h>

#define FRONT_BUTTON_PIN 0
#define BACK_BUTTON_PIN 1
#define FRONT_LED_PIN 2
#define BACK_LED_PIN 3
#define SWITCH_PIN 4

#define FRONT 0
#define BACK 1

/**********		global declarations		**********/
	
uint8_t myGolbalVar = 0x02;


/**********		init function		**********/

void init(void) __attribute__ ((naked)) __attribute__ ((section(".init3")));
void init(void) {

	// disable WD completely
	MCUSR &= ~(1<<WDRF);
	WDTCR |= 1<<WDCE | 1<<WDE;
	WDTCR &= ~(1<<WDE);


	// initialize PortB
	// all inputs, with pullup applied on SWITCH_PIN only
	DDRB = 0<<SWITCH_PIN | 0<<BACK_LED_PIN | 0<<FRONT_LED_PIN | 0<<BACK_BUTTON_PIN | 0<<FRONT_BUTTON_PIN;
	PORTB = 1<<SWITCH_PIN | 0<<BACK_LED_PIN | 0<<FRONT_LED_PIN | 0<<BACK_BUTTON_PIN | 0<<FRONT_BUTTON_PIN;
	
	
	// prescale
	//CLKPR = 1<<CLKPCE;
	//CLKPR = 0<<CLKPS3 | 0<<CLKPS2 | 0<<CLKPS1 | 0<<CLKPS0;
	
	//enable pin change interrupt on PCINT4 only
	GIMSK = 1<<PCIE;
	PCMSK = 1<<PCINT4;

}

/**********		utility functions		**********/


void pushButton(uint8_t button, uint8_t time_ms) {
	//pushed either the front button (if button == 0) or the back button (else)
	
	if(!(button)) {
		//push front button for time_ms ms
		DDRB |= 1<<FRONT_BUTTON_PIN;
		PORTB |= 1<<FRONT_BUTTON_PIN;
		waitms(time_ms);
		PORTB &= ~(1<<FRONT_BUTTON_PIN);
		DDRB &= ~(1<<FRONT_BUTTON_PIN);
	}
	else {
		//push back button for time_ms ms
		DDRB |= 1<<BACK_BUTTON_PIN;
		PORTB |= 1<<BACK_BUTTON_PIN;
		waitms(time_ms);
		PORTB &= ~(1<<BACK_BUTTON_PIN);
		DDRB &= ~(1<<BACK_BUTTON_PIN);
	}
}

void turnOnFront(void) {

	//WIP
	
	pushButton(FRONT,200);
	waitms(250);
	pushButton(FRONT,200);

}



/**********		main function		**********/

void main(void) __attribute__ ((naked, noreturn));
void main(void) {

	//sei();
	
	while(1) {
		
		nop();
		
		pushButton(FRONT,200);
		waitms(250);
		waitms(250);
		waitms(250);
		waitms(250);
		pushButton(FRONT,200);
		waitms(100);
		pushButton(FRONT,200);
		waitms(100);
		
		//PORTB = 0x01;
		//waitms(250);
		//waitms(250);
		//waitms(250);
		//waitms(250);
		//PORTB = 0x02;
		//waitms(250);
		//waitms(250);
	}
}




/**********		pin change ISR		**********/

ISR(PCINT0_vect) {
	
	cli();	//disable interrupt (to make debounce work)
	
	if(PORTB & 1<<SWITCH_PIN) {
		//switch is off
		waitms(20);	//debounce
		
	}
	else {
		//switch is on
		
		
		
	}
	
	
	
	
	
}




// [end-of-file]
