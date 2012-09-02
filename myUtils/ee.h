/*
ee.h v0.85
new in v0.85: tiny45 support - loosely verified
*/

#if defined (__AVR_ATtiny13A__)
	
void writeEE(unsigned char eeAddr, unsigned char eeData) {
	
	//if anything is currently writing to EE, wait for it to finish
	while(EECR & (1<<EEWE)) {
		nop();
	}
	
	EECR &= ~(1<<EEPM1 | 1<<EEPM0);	//erase and write in one operation
	EEARL = eeAddr;
	EEDR = eeData;			//(2l) write args to registers
	EECR |= 1<<EEMWE;
	EECR |= 1<<EEWE;		//(2l) write eeprom
}



unsigned char readEE(unsigned char eeAddr) {

	// Wait for completion of previous write
	while(EECR & (1<<EEWE)) {
		nop();
	}

	EEARL = eeAddr;		//write arg to register
	EECR |= 1<<EERE;	//read eeprom

	return EEDR;
}


#elif defined (__AVR_ATtiny45__)


void writeEE(unsigned char eeAddr, unsigned char eeData) {

	//if anything is currently writing to EE, wait for it to finish
	while(EECR & (1<<EEPE)) {
		nop();
	}
	
	EECR &= ~(1<<EEPM1 | 1<<EEPM0);	//erase and write in one operation
	EEAR = eeAddr;
	EEDR = eeData;			//(2l) write args to registers
	EECR |= 1<<EEMPE;
	EECR |= 1<<EEPE;		//(2l) write eeprom
}



unsigned char readEE(unsigned char eeAddr) {

	// Wait for completion of previous write
	while(EECR & (1<<EEPE)) {
		nop();
	}

	EEAR = eeAddr;		//write arg to register
	EECR |= 1<<EERE;	//read eeprom

	return EEDR;
}

#else
	#error "ee.h does not support the device"
#endif

// [end-of-file]