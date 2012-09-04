/**********		Program Info		**********
bike.c
device: defined in Makefile
sysclock: defined in Makefile
watchdog: disabled completely


--pin assignments
	--PB0: FRONT_BUTTON
	--PB1: BACK_BUTTON
	--PB2: SWITCH
	--PB3: BACK_LED
	--PB4: FRONT_LED
	
--front notes
	--red: power (4.5v)
	--black: GND
	--green: FRONT_LED
	--white: FRONT_BUTTON
		--FRONT_BUTTON is floating when button isn't pushed and pulled to 4V5 when
		button is pushed

--back notes
	--red: power (3.0v)
	--black: GND
	--green: BACK_LED
	--white: BACK BUTTON
		--BACK_BUTTON is floating when button isn't pushed and pulled to GND
		when it is

--SWITCH is low when switch is turned on


********************/



/**********		directives		**********/

#define nop() asm("nop")
#define sleep() asm("sleep")

#include <avr/io.h>
#include <avr/interrupt.h>
#include <wait.h>
#include <fuses.h>
//#include <ee.h>
#include <avr/signature.h>
//#include <avr/eeprom.h>

#define FRONT_BUTTON_PIN 0
#define BACK_BUTTON_PIN 1
#define SWITCH_PIN 2
#define BACK_LED_PIN 3
#define FRONT_LED_PIN 4

#define FRONT 0
#define BACK 1
#define FRONTANDBACK 2

#define BUTTONPRESS_DURATION 200



/**********		init function		**********/

void init(void) __attribute__ ((naked)) __attribute__ ((section(".init3")));
void init(void) {

	// disable WD completely
	MCUSR &= ~(1<<WDRF);
	WDTCR |= 1<<WDCE | 1<<WDE;
	//WDTCR &= ~(1<<WDE);
	WDTCR = 0x00;


	// initialize PortB
	// all inputs, with pullup applied on SWITCH_PIN only
	DDRB = 0<<SWITCH_PIN | 0<<BACK_LED_PIN | 0<<FRONT_LED_PIN | 0<<BACK_BUTTON_PIN | 0<<FRONT_BUTTON_PIN;
	PORTB = 1<<SWITCH_PIN | 0<<BACK_LED_PIN | 0<<FRONT_LED_PIN | 0<<BACK_BUTTON_PIN | 0<<FRONT_BUTTON_PIN;
	
	
}

/**********		utility functions		**********/


void pushButton(uint8_t button, uint8_t time_ms) {
	//pushes the front button (if button == 0), back button (if button == 1),
	//or both buttons (if button == 2)
	
	if(button == 0) {
		//push front button for time_ms ms
		DDRB |= 1<<FRONT_BUTTON_PIN;
		PORTB |= 1<<FRONT_BUTTON_PIN;
		waitms(time_ms);
		PORTB &= ~(1<<FRONT_BUTTON_PIN);
		DDRB &= ~(1<<FRONT_BUTTON_PIN);
	}
	else if(button == 1) {
		//push back button for time_ms ms
		PORTB &= ~(1<<BACK_BUTTON_PIN);
		DDRB |= 1<<BACK_BUTTON_PIN;
		waitms(time_ms);
		DDRB &= ~(1<<BACK_BUTTON_PIN);
	}
	else if(button == 2) {
		//push both buttons for time_ms ms
		DDRB |= 1<<FRONT_BUTTON_PIN;
		PORTB |= 1<<FRONT_BUTTON_PIN;
		PORTB &= ~(1<<BACK_BUTTON_PIN);
		DDRB |= 1<<BACK_BUTTON_PIN;
		waitms(time_ms);
		PORTB &= ~(1<<FRONT_BUTTON_PIN);
		DDRB &= ~(1<<FRONT_BUTTON_PIN);
		DDRB &= ~(1<<BACK_BUTTON_PIN);
	}
	else {;}
	
	//need some delay because, if there's another call to this function 
	//immediately following this one, the next button push won't register
	//unless we have this dealy
	waitms(50);
}

void turnOnLights(void) {

	//currently I blindly assume that the button press works...
	//future enhancement would be to check LED's to make sure
	//they're on and blinking
	
	pushButton(FRONTANDBACK,BUTTONPRESS_DURATION);
	waitms(100);
	pushButton(FRONTANDBACK,BUTTONPRESS_DURATION);

}

void enterSleepMode(void) {
	
	//disable BOD (note: the nominclature of these bits is incosistent between
	//the ATtiny13A datasheet and the iotn13a.h file that comes with WinAVR-20100110.
	//In the datasheet, bits BPDS and BPDSE are referred to as BODS and BODSE respectivelly)
	BODCR = 1<<BPDS | 1<<BPDSE;
	BODCR = 1<<BPDS | 0<<BPDSE;
	
	//disable ADC
	ADCSRA &= ~(1<<ADEN);
	
	//disable analog comparator
	ACSR &= ~(1<<ACIE);
	ACSR |= 1<<ACD;
	
	//disable clocks to timer 0 and ADC
	PRR = 1<<PRTIM0 | 1<<PRADC;
	
	// disable WD completely (again)
	MCUSR &= ~(1<<WDRF);
	WDTCR |= 1<<WDCE | 1<<WDE;
	WDTCR = 0x00;
	
	
	//enable sleep and set sleep mode to "power-down"
	MCUCR |= 1<<SE | 1<<SM1;
	MCUCR &= ~(1<<SM0);
	
	sleep();
}

/**********		main function		**********/

void main(void) __attribute__ ((naked, noreturn));
void main(void) {

	
	//assume switch is off at bootup (see flowchart)
	
	//enable global interrupts
	sei();
	
	//enable pin change interrupt on SWITCH_PIN, FRONT_LED_PIN, and BACK_LED_PIN
	PCMSK = 1<<SWITCH_PIN | 1<<FRONT_LED_PIN | 1<<BACK_LED_PIN;
	
	//clear PC interrupt flag in case it got randomly set during bootup
	GIFR |= 1<<PCIF;
	
	//enable PC interrupt
	GIMSK = 1<<PCIE;
	
	while(1) {
		
		//everything happens in PC interrupt routine
		enterSleepMode();
	}
}




/**********		pin change ISR		**********/

ISR(PCINT0_vect) {
	
	//disable PC interrupt while in this routine
	GIMSK &= ~(1<<PCIE);
	
	waitms(5);	//debounce
	
	
	if(PINB & 1<<SWITCH_PIN) {		//switch is off
		
		if(PINB & 1<<FRONT_LED_PIN) {
			//front LED is on so turn it off
			pushButton(FRONT,BUTTONPRESS_DURATION);
		}
		if(PINB & 1<<BACK_LED_PIN) {
			//back LED is on so turn it off
			pushButton(BACK,BUTTONPRESS_DURATION);
		}
		
		//because switch is off, we want to enable PC interrupt on the LED pins
		//(as well as SWITCH_PIN) so that if the LED's change state we can wake
		//up and push button again to turn them off
		PCMSK = 1<<SWITCH_PIN | 1<<FRONT_LED_PIN | 1<<BACK_LED_PIN;
		
	}
	
	else {		//switch is on
		
		turnOnLights();
		
		//because switch is on and the LED's are now blinking (hopefully!), configure
		//PC interrupt for switch only (disable interrupt for LED pins)
		PCMSK = 1<<SWITCH_PIN;
	}
	
	
	
	//clear flag and enable interrupt before returning
	GIFR |= 1<<PCIF;
	GIMSK |= 1<<PCIE;
	
	return;
}




// [end-of-file]
