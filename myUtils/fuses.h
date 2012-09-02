/*
fuses.h version 0.92
fuses.h defines all the fuse bits and tells the compiler to include information in the *.elf file
that corresponds to these bits.  I don't think it uses up any program or data memory.

new in version 0.92:
	--finish implementing support for 'tiny45
	--general clean-up

When programming the chip, you still have to tell the programmer to program the fuse bits.

fuses.h makes use of the following symbols:
	CLOCKDIVIDE8 (verified on '13a and '45)
	STARTUPDELAY (seems to work on '13a and 45)
	F_CPU (verified on '13a and 45)
	BROWNOUT (verified on '13a and '45)
	
which are defined by the makefile
*/


#if defined (__AVR_ATtiny13A__)
	
	/******** Low Fuse Byte for ATtiny 13A ********

	bit		name		configurer	default (0 = programmed)
	----	------		----------	------------------------
	bit7	SPIEN		hardcode	0	//enable SPI programming
	bit6	EESAVE		hardcode	1	//Don't keep EEprom contents during chip erase
	bit5	WDTON		hardcode	1	//Watch dog timer not always on
	bit4	CKDIV8		makefile	0	//on reset load CLKPR[3:0] with 0b0011
	bit3	SUT1		makefile	1
	bit2	SUT0		makefile	0	//2l startup time (from pwr-dwn and pwr-save) = 14CK + 64 ms
	bit1	CKSEL1		makefile	1
	bit0	CKSEL0		makefile	0	//2l clock source = 9.6MHz

	*/
	
	#define LFUSE7 (unsigned char)0
	#define LFUSE6 (unsigned char)1
	#define LFUSE5 (unsigned char)1

	#ifndef CLOCKDIVIDE8
		#warning "CLOCKDIVIDE8 isnt defined so using default value of 1"
		#define LFUSE4 (unsigned char)0
	#elif CLOCKDIVIDE8 == 1
		#define LFUSE4 (unsigned char)0
	#elif CLOCKDIVIDE8 == 0
		#define LFUSE4 (unsigned char)1
	#else
		#error "fuses.h doesnt understand the value that CLOCKDIVIDE8 is defined as"
	#endif
	
	#ifndef STARTUPDELAY
		#warning "STARTUPDELAY isnt defined so using default value of 64"
		#define LFUSE3 (unsigned char)1
		#define LFUSE2 (unsigned char)0
	#elif STARTUPDELAY == 0
		#define LFUSE3 (unsigned char)0
		#define LFUSE2 (unsigned char)0
	#elif STARTUPDELAY == 4
		#define LFUSE3 (unsigned char)0
		#define LFUSE2 (unsigned char)1
	#elif STARTUPDELAY == 64
		#define LFUSE3 (unsigned char)1
		#define LFUSE2 (unsigned char)0
	#else
		#error "fuses.h doesnt understand the value that STARTUPDELAY is defined as"
	#endif

	#ifndef F_CPU
		#warning "F_CPU isnt defined so using default value of 9600000"
		#define LFUSE1 (unsigned char)1
		#define LFUSE0 (unsigned char)0
	#elif F_CPU == 9600000
		#define LFUSE1 (unsigned char)1
		#define LFUSE0 (unsigned char)0
	#elif F_CPU == 4800000
		#define LFUSE1 (unsigned char)0
		#define LFUSE0 (unsigned char)1
	#elif F_CPU == 128000
		#define LFUSE1 (unsigned char)1
		#define LFUSE0 (unsigned char)1
	#else
		#error "fuses.h doesnt understand the value that F_CPU is defined as"
	#endif
	
	/******** High Fuse Byte for ATtiny 13A ********

	bit		name		configurer	default (0 = programmed)
	----	------		----------	------------------------
	bit7	reserved	hardcode	1
	bit6	reserved	hardcode	1
	bit5	reserved	hardcode	1
	bit4	SELFPRGEN	hardcode	1	//don't enable SPM instruction
	bit3	DWEN		hardcode	1	//don't enable debugWire
	bit2	BODLEVEL1	makefile	1
	bit1	BODLEVEL0	makefile	0	//2l BOD threshold = 1.8 V
	bit0	RSTDISBL	hardcode	1	//don't disable external reset (you'll loose the chip if you program this)
	
	*/

	#define HFUSE7 (unsigned char)1
	#define HFUSE6 (unsigned char)1
	#define HFUSE5 (unsigned char)1
	#define HFUSE4 (unsigned char)1
	#define HFUSE3 (unsigned char)1

	#ifndef BROWNOUT
		#warning "BROWNOUT isnt defined so using default value of 1.8 V"
		#define HFUSE2 (unsigned char)1
		#define HFUSE1 (unsigned char)0
	#elif BROWNOUT == 0
		#define HFUSE2 (unsigned char)1
		#define HFUSE1 (unsigned char)1
	#elif BROWNOUT == 18
		#define HFUSE2 (unsigned char)1
		#define HFUSE1 (unsigned char)0
	#elif BROWNOUT == 27
		#define HFUSE2 (unsigned char)0
		#define HFUSE1 (unsigned char)1
	#elif BROWNOUT == 43
		#define HFUSE2 (unsigned char)0
		#define HFUSE1 (unsigned char)0
	#else
		#error "fuses.h doesnt understand the value that BROWNOUT is defined as"
	#endif
		
	#define HFUSE0 (unsigned char)1
	
	FUSES = 
	{
		.low = (LFUSE7<<7 | LFUSE6<<6 | LFUSE5<<5 | LFUSE4<<4 | LFUSE3<<3 | LFUSE2<<2 | LFUSE1<<1 | LFUSE0<<0),
		.high = (HFUSE7<<7 | HFUSE6<<6 | HFUSE5<<5 | HFUSE4<<4 | HFUSE3<<3 | HFUSE2<<2 | HFUSE1<<1 | HFUSE0<<0),
	};



#elif defined (__AVR_ATtiny45__)

	/******** Low Fuse Byte for ATtiny 45 ********

	bit		name		configurer	default (0 = programmed)
	----	------		----------	------------------------
	bit7	CKDIV8		makefile	0	//on reset load CLKPR[3:0] with 0b0011
	bit6	CKOUT		hardcode	1
	bit5	SUT1		makefile	1	
	bit4	SUT0		makefile	0	//2l startup time (from pwr-dwn and pwr-save) = 14CK + 64 ms
	bit3	CKSEL3		makefile	0
	bit2	CKSEL2		makefile	0	
	bit1	CKSEL1		makefile	1
	bit0	CKSEL0		makefile	0	//4l clock source = 8.0 MHz internal RC

	*/

	#ifndef CLOCKDIVIDE8
		#warning "CLOCKDIVIDE8 isnt defined so using default value of 1"
		#define LFUSE7 (unsigned char)0
	#elif CLOCKDIVIDE8 == 1
		#define LFUSE7 (unsigned char)0
	#elif CLOCKDIVIDE8 == 0
		#define LFUSE7 (unsigned char)1
	#else
		#error "fuses.h doesnt understand the value that CLOCKDIVIDE8 is defined as"
	#endif

	
	#define LFUSE6 (unsigned char)1
	

	//TODO: delete these lines once STARTUPDELAY is implemented
	//#define LFUSE5 (unsigned char)1
	//#define LFUSE4 (unsigned char)0

	
	
	#ifndef F_CPU	//TODO: make this and other defaults independent statements: ifndef... def... endif
		#warning "F_CPU isnt defined so using default value of 8000000"
		#define LFUSE3 (unsigned char)0
		#define LFUSE2 (unsigned char)0
		#define LFUSE1 (unsigned char)1
		#define LFUSE0 (unsigned char)0
		#ifndef STARTUPDELAY
			#warning "STARTUPDELAY isnt defined so using default value of 64"
			#define LFUSE5 (unsigned char)1
			#define LFUSE4 (unsigned char)0
		#elif STARTUPDELAY == 0
			#define LFUSE5 (unsigned char)0
			#define LFUSE4 (unsigned char)0
		#elif STARTUPDELAY == 4
			#define LFUSE5 (unsigned char)0
			#define LFUSE4 (unsigned char)1
		#elif STARTUPDELAY == 64
			#define LFUSE5 (unsigned char)1
			#define LFUSE4 (unsigned char)0
		#else
			#error "fuses.h doesnt understand the value that STARTUPDELAY is defined as"
		#endif	/*ifndef STARTUPDELAY*/

	#elif F_CPU == 16000000
		#define LFUSE3 (unsigned char)0
		#define LFUSE2 (unsigned char)0
		#define LFUSE1 (unsigned char)0
		#define LFUSE0 (unsigned char)1
		#ifndef STARTUPDELAY
			#warning "STARTUPDELAY isnt defined so using default value of 69"
			#define LFUSE5 (unsigned char)1
			#define LFUSE4 (unsigned char)1
		#elif STARTUPDELAY == 8
			#define LFUSE5 (unsigned char)0
			#define LFUSE4 (unsigned char)0
		#elif STARTUPDELAY == 9
			#define LFUSE5 (unsigned char)0
			#define LFUSE4 (unsigned char)1
		#elif STARTUPDELAY == 68
			#define LFUSE5 (unsigned char)1
			#define LFUSE4 (unsigned char)0
		#elif STARTUPDELAY == 69
			#define LFUSE5 (unsigned char)1
			#define LFUSE4 (unsigned char)1
		#else
			#error "fuses.h doesnt understand the value that STARTUPDELAY is defined as"
		#endif	/*ifndef STARTUPDELAY*/
		
	#elif F_CPU == 8000000
		#define LFUSE3 (unsigned char)0
		#define LFUSE2 (unsigned char)0
		#define LFUSE1 (unsigned char)1
		#define LFUSE0 (unsigned char)0
		#ifndef STARTUPDELAY
			#warning "STARTUPDELAY isnt defined so using default value of 64"
			#define LFUSE5 (unsigned char)1
			#define LFUSE4 (unsigned char)0
		#elif STARTUPDELAY == 0
			#define LFUSE5 (unsigned char)0
			#define LFUSE4 (unsigned char)0
		#elif STARTUPDELAY == 4
			#define LFUSE5 (unsigned char)0
			#define LFUSE4 (unsigned char)1
		#elif STARTUPDELAY == 64
			#define LFUSE5 (unsigned char)1
			#define LFUSE4 (unsigned char)0
		#else
			#error "fuses.h doesnt understand the value that STARTUPDELAY is defined as"
		#endif	/*ifndef STARTUPDELAY*/
		
	#elif F_CPU == 1600000
		#define LFUSE3 (unsigned char)0
		#define LFUSE2 (unsigned char)0
		#define LFUSE1 (unsigned char)1
		#define LFUSE0 (unsigned char)1
		#ifndef STARTUPDELAY
			#warning "STARTUPDELAY isnt defined so using default value of 64"
			#define LFUSE5 (unsigned char)0
			#define LFUSE4 (unsigned char)0
		#elif STARTUPDELAY == 0
			#define LFUSE5 (unsigned char)1
			#define LFUSE4 (unsigned char)1
		#elif STARTUPDELAY == 4
			#define LFUSE5 (unsigned char)1
			#define LFUSE4 (unsigned char)0
		#elif STARTUPDELAY == 64
			#define LFUSE5 (unsigned char)0
			#define LFUSE4 (unsigned char)0
		#else
			#error "fuses.h doesnt understand the value that STARTUPDELAY is defined as"
		#endif	/*ifndef STARTUPDELAY*/

	#elif F_CPU == 128000
		#define LFUSE3 (unsigned char)0
		#define LFUSE2 (unsigned char)1
		#define LFUSE1 (unsigned char)0
		#define LFUSE0 (unsigned char)0
		#ifndef STARTUPDELAY
			#warning "STARTUPDELAY isnt defined so using default value of 64"
			#define LFUSE5 (unsigned char)1
			#define LFUSE4 (unsigned char)0
		#elif STARTUPDELAY == 0
			#define LFUSE5 (unsigned char)0
			#define LFUSE4 (unsigned char)0
		#elif STARTUPDELAY == 4
			#define LFUSE5 (unsigned char)0
			#define LFUSE4 (unsigned char)1
		#elif STARTUPDELAY == 64
			#define LFUSE5 (unsigned char)1
			#define LFUSE4 (unsigned char)0
		#else
			#error "fuses.h doesnt understand the value that STARTUPDELAY is defined as"
		#endif	/*ifndef STARTUPDELAY*/
		
	#else
		#error "fuses.h doesnt understand the value that F_CPU is defined as"
	#endif	/*ifndef F_CPU*/

	/******** High Fuse Byte for ATtiny 45 ********

	bit		name		configurer	default (0 = programmed)
	----	------		----------	------------------------
	bit7	RSTDISBL	hardcode	1	//don't disable external reset (you'll loose the chip if you program this)
	bit6	DWEN		hardcode	1	//don't enable debugWire
	bit5	SPIEN		hardcode	0	//enable SPI programming
	bit4	WDTON		hardcode	1	//Watch dog timer not always on
	bit3	EESAVE		hardcode	1	//Don't keep EEprom contents during chip erase
	bit2	BODLEVEL2	makefile	1	
	bit1	BODLEVEL1	makefile	1
	bit0	BODLEVEL0	makefile	0	//3l brownout level = 1.8V

	*/
	
	#define HFUSE7 (unsigned char)1
	#define HFUSE6 (unsigned char)1
	#define HFUSE5 (unsigned char)0
	#define HFUSE4 (unsigned char)1
	#define HFUSE3 (unsigned char)1
	
	#ifndef BROWNOUT
		#warning "BROWNOUT isnt defined so using default value of 1.8 V"
		#define HFUSE2 (unsigned char)1
		#define HFUSE1 (unsigned char)1
		#define HFUSE0 (unsigned char)0
	#elif BROWNOUT == 0
		#define HFUSE2 (unsigned char)1
		#define HFUSE1 (unsigned char)1
		#define HFUSE0 (unsigned char)1
	#elif BROWNOUT == 18
		#define HFUSE2 (unsigned char)1
		#define HFUSE1 (unsigned char)1
		#define HFUSE0 (unsigned char)0
	#elif BROWNOUT == 27
		#define HFUSE2 (unsigned char)1
		#define HFUSE1 (unsigned char)0
		#define HFUSE0 (unsigned char)1
	#elif BROWNOUT == 43
		#define HFUSE2 (unsigned char)1
		#define HFUSE1 (unsigned char)0
		#define HFUSE0 (unsigned char)0
	#else
		#error "fuses.h doesnt understand the value that BROWNOUT is defined as"
	#endif


	/******** Extended Fuse Byte for ATtiny 45 ********

	bit		name		configurer	default (0 = programmed)
	----	------		----------	------------------------
	bit7	reserved	hardcode	1
	bit6	reserved	hardcode	1
	bit5	reserved	hardcode	1
	bit4	reserved	hardcode	1
	bit3	reserved	hardcode	1
	bit2	reserved	hardcode	1
	bit1	reserved	hardcode	1
	bit0	SELFPRGEN	hardcode	1	//don't enable SPM instruction

	*/

	#define EFUSE7 (unsigned char)1
	#define EFUSE6 (unsigned char)1
	#define EFUSE5 (unsigned char)1
	#define EFUSE4 (unsigned char)1
	#define EFUSE3 (unsigned char)1
	#define EFUSE2 (unsigned char)1
	#define EFUSE1 (unsigned char)1
	#define EFUSE0 (unsigned char)1

	FUSES = 
	{
		.low = (LFUSE7<<7 | LFUSE6<<6 | LFUSE5<<5 | LFUSE4<<4 | LFUSE3<<3 | LFUSE2<<2 | LFUSE1<<1 | LFUSE0<<0),
		.high = (HFUSE7<<7 | HFUSE6<<6 | HFUSE5<<5 | HFUSE4<<4 | HFUSE3<<3 | HFUSE2<<2 | HFUSE1<<1 | HFUSE0<<0),
		.extended = (EFUSE7<<7 | EFUSE6<<6 | EFUSE5<<5 | EFUSE4<<4 | EFUSE3<<3 | EFUSE2<<2 | EFUSE1<<1 | EFUSE0<<0),
	};
	

#else
	#error "fuses.h does not support the device"
#endif


// [end-of-file]