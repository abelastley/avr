/**********		Program Info		**********
stopplight.c V0.8
device: Atmel ATtiny13A
clock: internal 128 kHz Osc; CKDIV8 fuse NOT programmed (F_CPU = 128 kHz)
brown-out detect: 1.8 V
watchdog: disabled completely

--pin connections for this platform (see schematic for more details):
	PB0 - REDLIGHT
	PB1 - YELLOWLIGHT
	PB2 - GREENLIGHT
	PB3 - MODEBUTTON
	
--This program will cycle through a number of modes (mode is kept track of by global variable g_mode):
--button push event
	--PCINT3 pin is configured as input w/ pullup, and external pushbutton switch is connected between this pin and GND
	--the external switch is a strange button - it stays closed when user presses and releases button, then stays
	open when user presses and releases button again.  User does not have to hold the button in order for switch
	to stay closed.
	--pin-change interrupt will fire wheneven user presses button (the state of the pin will change either from
	low to high or high to low every time button is pressed)
	--inside ISR:
		--debouncing takes place
		--mode is advanced by 1
		--sequence is reset to 0
	

********************/


/**********		version history		**********

v0.70
	--original

To Do:
	--check brownout threshold
	--configure PCMSK to only allow PCINT3 to work

v0.80
	--fix stack overflow issue by implementing FSM-style operation rather than having runMode() function which
	had been called from within the ISR
	--ISR is now simple (basically just increments g_mode)
	--main() is now complex checking for g_mode every 50 msec

********************/





/**********		directives		**********/

//DEBUG will enable a 4th output on PB4 which can be hooked to an LED for debug ability
#define DEBUG 1

//NUM_MODES should be equal to the number of modes.  Note: variable g_mode starts at 0 (not 1), so its max value
//is (NUM_MODES - 1)
#define NUM_MODES 8

//making TIME_CONSTANT larger will "slow down" each sequence; smaller TIME_CONSTANT will speed everything up
//45 is nominal
#define TIME_CONSTANT 45


#define nop() asm("nop")

//inputs
//#define MODEBUTTON_PINBIT PINB3
//#define MODEBUTTON_PINREG PINB

//#define modebutton() (MODEBUTTON_PINREG & 1<<MODEBUTTON_PINBIT)

//outputs
#define REDLIGHT_PORTBIT PORTB0
#define REDLIGHT_PORTREG PORTB
#define YELLOWLIGHT_PORTBIT PORTB1
#define YELLOWLIGHT_PORTREG PORTB
#define GREENLIGHT_PORTBIT PORTB2
#define GREENLIGHT_PORTREG PORTB

#define redon() REDLIGHT_PORTREG |= 1<<REDLIGHT_PORTBIT
#define yellowon() YELLOWLIGHT_PORTREG |= 1<<YELLOWLIGHT_PORTBIT
#define greenon() GREENLIGHT_PORTREG |= 1<<GREENLIGHT_PORTBIT

#define redoff() REDLIGHT_PORTREG &= ~(1<<REDLIGHT_PORTBIT)
#define yellowoff() YELLOWLIGHT_PORTREG &= ~(1<<YELLOWLIGHT_PORTBIT)
#define greenoff() GREENLIGHT_PORTREG &= ~(1<<GREENLIGHT_PORTBIT)



#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signature.h>
//#include <avr/eeprom.h>
#include <mystuff/wait.h>
#include <mystuff/fuses.h>
//#include <mystuff/ee.h>



/**********		global declarations		**********/
	
static volatile uint8_t g_mode = 0;	//keeps track of what mode we're in
static volatile uint16_t g_sequence = 0;	//keeps track of what sequence we're in (within each mode)
static volatile uint8_t g_buttonState;


/**********		init function		**********/

void init(void) __attribute__ ((naked)) __attribute__ ((section(".init3")));
void init(void) {

	//disable WD completely:
	MCUSR &= ~(1<<WDRF);
	WDTCR |= 1<<WDCE | 1<<WDE;
	WDTCR &= ~(1<<WDE);
	
	//initialize PORTB
	PORTB = 1<<PORTB3;	//enable pullup on PB3
	DDRB = 1<<DDB2 | 1<<DDB1 | 1<<DDB0;		//set PB2, 1, and 0 as outputs
	
	g_buttonState = PINB & 1<<PINB3;
	
	#ifdef DEBUG
	//also make PB4 an output
	DDRB |= 1<<DDB4;
	#endif
	
	//initialize PCINT
	GIMSK = 1<<PCIE;	//enable pin-change interrupt
	PCMSK = 1<<PCINT3;	//mask such that only PCINT3 is enabled
}

/**********		aux functions		**********/

void setState(uint8_t state) {
	//sets the state of the lights according to:
	
	// 	state	lights that are ON (others are off)
	//	--------------------------------
	//	0		none
	//	1		red only
	//	2		yellow only
	//	3		yellow and red
	//	4		green only
	//	5		green and red
	//	6		green and yellow
	//	7		green, yellow, and red
	
	switch(state) {
		case 0:
			greenoff(); yellowoff(); redoff();
			break;
		case 1:
			greenoff(); yellowoff(); redon();
			break;
		case 2:
			greenoff(); yellowon(); redoff();
			break;
		case 3:
			greenoff(); yellowon(); redon();
			break;
		case 4:
			greenon(); yellowoff(); redoff();
			break;
		case 5:
			greenon(); yellowoff(); redon();
			break;
		case 6:
			greenon(); yellowon(); redoff();
			break;
		case 7:
			greenon(); yellowon(); redon();
			break;
	}
}

void wait50msec() {
	//should wait approx 50 msec but is configurable with TIME_CONSTANT
	
	uint16_t counter;
	
	for(counter=0; counter<TIME_CONSTANT; counter+=1) {
		waitcycles(23);
	}
}



/**********		main function		**********/

void main(void) __attribute__ ((naked, noreturn));
void main(void) {

	#ifdef DEBUG
		//I believe this will give info about the reset reason
		uint8_t dummy;
		for(dummy=0; dummy<MCUSR; dummy+=1) {
			PORTB |= 1<<PORTB4;
			waitms(250);
			PORTB &= ~(1<<PORTB4);
			waitms(250);
		}
	#endif
	
	GIFR |= 1<<PCIF;	//clear PC interrupt flag
	sei();				//enable global interrupts
	
	while(1) {
		switch(g_mode) {
			case 0:
				//--mode 0 (regular traffic light) (default power-on mode)
				//red only for 23 sec
				//green only for 23 sec
				//yellow only for 2.15 sec
				//repeat
				if(g_sequence>=963) {g_sequence = 0;}
				if(g_sequence<460) {setState(1);}
				else if(g_sequence<920) {setState(4);}
				else if(g_sequence<963) {setState(2);}
				break;
			case 1:
				//--mode 1 (drag strip mode)
				//red only for 3 sec
				//yellow only for 1 sec
				//green only for 5 sec
				//all off for 2 sec
				//repeat
				if(g_sequence>=220) {g_sequence = 0;}
				if(g_sequence<60) {setState(1);}
				else if(g_sequence<80) {setState(2);}
				else if(g_sequence<180) {setState(4);}
				else if(g_sequence<220) {setState(0);}
				break;
			case 2:
				//--mode 2 (flashing red)
				//red only for 1 sec
				//all off for 1 sec
				//repeat
				if(g_sequence>=40) {g_sequence = 0;}
				if(g_sequence<20) {setState(1);}
				else if(g_sequence<40) {setState(0);}
				break;
			case 3:
				//--mode 3 (flashing yellow)
				//yellow only for 1 sec
				//all off for 1 sec
				//repeat
				if(g_sequence>=40) {g_sequence = 0;}
				if(g_sequence<20) {setState(2);}
				else if(g_sequence<40) {setState(0);}
				break;
			case 4:
				//--mode 4 (red on constant)
				//red only for ever
				setState(1);
				break;
			case 5:
				//--mode 5 (green on constant)
				//green only for ever
				setState(4);
				break;
			case 6:
				//--mode 6 (crazy)
				//red only for 1 sec
				//yellow only for 1 sec
				//green only for 1 sec
				//yellow only for 1 sec
				//red only for 1 sec
				//yellow only for 1 sec
				//green only for 1 sec
				//yellow only for 1 sec
				//red only for 0.5 sec
				//yellow only for 0.5 sec
				//green only for 0.5 sec
				//yellow only for 0.5 sec
				//red only for 0.5 sec
				//yellow only for 0.5 sec
				//green only for 0.5 sec
				//yellow only for 0.5 sec
				//all on for 0.5 sec
				//all off for 0.5 sec
				//all on for 0.5 sec
				//all off for 0.5 sec
				//all on for 0.5 sec
				//all off for 0.5 sec
				//??????????
				if(g_sequence>=360) {g_sequence = 0;}
				
				if(g_sequence<20) {setState(1);}
				else if(g_sequence<40) {setState(2);}
				else if(g_sequence<60) {setState(4);}
				else if(g_sequence<80) {setState(2);}
				else if(g_sequence<100) {setState(1);}
				else if(g_sequence<120) {setState(2);}
				else if(g_sequence<140) {setState(4);}
				else if(g_sequence<160) {setState(2);}
				
				else if(g_sequence<170) {setState(1);}
				else if(g_sequence<180) {setState(2);}
				else if(g_sequence<190) {setState(4);}
				else if(g_sequence<200) {setState(2);}
				else if(g_sequence<210) {setState(1);}
				else if(g_sequence<220) {setState(2);}
				else if(g_sequence<230) {setState(4);}
				else if(g_sequence<240) {setState(2);}
				
				else if(g_sequence<250) {setState(5);}
				else if(g_sequence<260) {setState(2);}
				else if(g_sequence<270) {setState(4);}
				else if(g_sequence<280) {setState(3);}
				else if(g_sequence<290) {setState(4);}
				else if(g_sequence<300) {setState(1);}
				else if(g_sequence<310) {setState(6);}
				else if(g_sequence<320) {setState(1);}
				else if(g_sequence<330) {setState(7);}
				else if(g_sequence<340) {setState(0);}
				else if(g_sequence<350) {setState(7);}
				else if(g_sequence<360) {setState(0);}
				break;
			case 7:
				//--mode 7 (game countdown mode)
				//green only for 40 sec
				//yellow only for 10 sec
				//blinking yellow off/on for 10 sec
				//red only for 5 sec
				//all off for 5 sec
				//repeat
				if(g_sequence>=1380) {g_sequence = 0;}
				if(g_sequence<800) {setState(4);}
				else if(g_sequence<1000) {setState(2);}
				else if(g_sequence<1020) {setState(0);}
				else if(g_sequence<1040) {setState(2);}
				else if(g_sequence<1060) {setState(0);}
				else if(g_sequence<1080) {setState(2);}
				else if(g_sequence<1100) {setState(0);}
				else if(g_sequence<1120) {setState(2);}
				else if(g_sequence<1140) {setState(0);}
				else if(g_sequence<1160) {setState(2);}
				else if(g_sequence<1180) {setState(0);}
				else if(g_sequence<1280) {setState(1);}
				else if(g_sequence<1380) {setState(0);}
				break;
			
		}
		g_sequence += 1;
		wait50msec();
	}
}




/**********		Pin change ISR		**********/

ISR(PCINT0_vect) {
	uint8_t l_buttonState;
	
	cli();
	
	//need to make sure buttonState has REALLY changed
	wait50msec();
	wait50msec();
	wait50msec();
	wait50msec();
	wait50msec();
	wait50msec();
	
	l_buttonState = PINB & 1<<PINB3;
	
	if(g_buttonState == l_buttonState) {
		//button state has not REALLY changed, it was just a transient power draw
		nop();
	}
	else {
		//button state has really changed
		
		//reset g_buttonState
		g_buttonState = l_buttonState;
		
		//turn off all lights
		redoff();
		yellowoff();
		greenoff();
		
		//cycle global mode var
		g_mode += 1;
		if(g_mode == NUM_MODES) {g_mode = 0;}
		else {;}
		
		//reset g_sequence to 0 so we always start at the begining of the sequence
		g_sequence = 0;
	}
	
	
	//prepare for next button push
	GIFR |= 1<<PCIF;	//clear PC interrupt flag
	sei();
	
	//run appropriate mode
	//!!! we cannot have while(1) inside an ISR - will lead to stack overflow
	//while(1) {runMode(g_mode);}
	
	
}



// [end-of-file]
