/*
wait.h v0.91
waitms(x) will wait approximately x ms
wait() does not account for the case where it is called while CLKPR is in the process of being changed
The symbol F_CPU should be defined in the makefile, for example:
	CFLAGS += -DF_CPU=9600000UL
v0.90 has been loosely verified on '13A and '45
*/

#ifndef nop()
	#define nop() asm("nop")
#endif

#define wait(x) waitms(x)	//for back compatibility


void waitms(unsigned char msWaitTime) {
	
	#if F_CPU == 16000000

		if(CLKPR < 5) {
		
			unsigned char i;
			unsigned char j;
			unsigned char k;
			unsigned char MiddleLoopCount = 16 >> CLKPR;
			
			for(i=0; i < msWaitTime; i++) {
				for(j=0; j < MiddleLoopCount; j++) {
					for(k=0; k < 250; k++) {
						nop();
					}
				}
			}
		}
		
		else {
		
			unsigned char i;
			unsigned char j;
			unsigned char InnerLoopCount = 125 >> (CLKPR - 5);
			
			for(i=0; i < msWaitTime; i++) {
				for(j=0; j < InnerLoopCount; j++) {
					nop();
				}
			}
		}
	
	#elif F_CPU == 9600000

		if(CLKPR < 6) {
		
			unsigned char i;
			unsigned char j;
			unsigned char k;
			unsigned char MiddleLoopCount = 32 >> CLKPR;
			
			for(k=0; k < msWaitTime; k++) {
				for(j=0; j < MiddleLoopCount; j++) {
					for(i=0; i < 75; i++) {
						nop();
					}
				}
			}
		}
		
		else {
		
			unsigned int i;
			unsigned int OnlyLoopCount = ((unsigned int)(msWaitTime) * 25) >> (CLKPR - 6);	//NTS: try making this a const
			
			for(i=0; i < OnlyLoopCount; i++) {
				nop();
			}
		}
	
	#elif F_CPU == 8000000

		if(CLKPR < 4) {
		
			unsigned char i;
			unsigned char j;
			unsigned char k;
			unsigned char MiddleLoopCount = 16 >> CLKPR;
			
			for(i=0; i < msWaitTime; i++) {
				for(j=0; j < MiddleLoopCount; j++) {
					for(k=0; k < 125; k++) {
						nop();
					}
				}
			}
		}
		
		else {
		
			unsigned char i;
			unsigned char j;
			unsigned char InnerLoopCount = 125 >> (CLKPR - 4);
			
			for(i=0; i < msWaitTime; i++) {
				for(j=0; j < InnerLoopCount; j++) {
					nop();
				}
			}
		}
	
	#elif F_CPU == 4800000
	
		if(CLKPR < 5) {
		
			unsigned char i;
			unsigned char j;
			unsigned char k;
			unsigned char MiddleLoopCount = 16 >> CLKPR;
			
			for(k=0; k < msWaitTime; k++) {
				for(j=0; j < MiddleLoopCount; j++) {
					for(i=0; i < 75; i++) {
						nop();
					}
				}
			}
		}
		
		else {
		
			unsigned int i;
			unsigned int OnlyLoopCount = ((unsigned int)(msWaitTime) * 25) >> (CLKPR - 5);	//NTS: try making this a const
			
			for(i=0; i < OnlyLoopCount; i++) {
				nop();
			}
		}
	
	#elif F_CPU == 1600000

		if(CLKPR < 4) {
		
			unsigned char i;
			unsigned char j;
			unsigned char k;
			unsigned char MiddleLoopCount = 16 >> CLKPR;
			
			for(i=0; i < msWaitTime; i++) {
				for(j=0; j < MiddleLoopCount; j++) {
					for(k=0; k < 25; k++) {
						nop();
					}
				}
			}
		}
		
		else {
		
			unsigned char i;
			unsigned char j;
			unsigned char InnerLoopCount = 25 >> (CLKPR - 4);
			
			for(i=0; i < msWaitTime; i++) {
				for(j=0; j < InnerLoopCount; j++) {
					nop();
				}
			}
		}
	
	#elif F_CPU == 128000
	
	//this 128kHz part might be broken on '13A but seems to work on '45
	
		if(CLKPR < 6) {
		
			unsigned char i;
			unsigned char j;
			unsigned char InnerLoopCount = 32 >> CLKPR;
			
			for(j=0; j < msWaitTime; j++) {
				for(i=0; i < InnerLoopCount; i++) {
					nop();
				}
			}
		}
		
		else {
		/*NOTE: this else{} should never be necessary since CLKPR should never be greater
		than 4 (prescaler = 16) when 128 KHz is used.  Having prescaler of 32 or more will make
		flash inaccessible to mkII (that's according to AVR studio).  If code space is at a premium,
		you can probably comment this out (along with the preceding if()) since, if your program wants
		to enter this else{} then you're probably screwed anyway.
		*/
			
			unsigned char i;
			unsigned char OnlyLoopCount = msWaitTime >> (CLKPR - 5);
			
			for(i=0; i < OnlyLoopCount; i++) {
				nop();
			}		
		}
	
	#else
		#error "wait.h needs F_CPU defined as: 16000000 | 9600000 | 8000000 | 4800000 | 1600000 | 128000"
	#endif
}

void waitcycles(unsigned char cycles) {

	unsigned char i;
	unsigned char j;
	unsigned char k;
	
	for(k = 0; k < 100; k++) {
		for(j = 0; j < 10; j++) {
			for(i = 0; i < cycles; i++) {
				nop();
			}
		}
	}
	
}

// [end-of-file]