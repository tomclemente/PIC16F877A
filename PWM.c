#include<pic.h>

void DCmotor(int speed);
void initPIC();
unsigned char button;

void DCmotor(int speed){
	int temp = speed;
	if(temp==1){
/*
 * PWM registers configuration
 * Fosc = 4000000 Hz
 * Fpwm = 300.48 Hz (Requested : 300 Hz)
 * Duty Cycle = 50 %
 * Resolution is 10 bits
 * Prescaler is 16
 */
PR2 = 0b11001111 ;
T2CON = 0b00000111 ;
CCPR1L = 0b01100111 ;
CCP1CON = 0b00111100 ;
	  // Example:
      //PR2 = 0xF9;					    // LIMIT FOR TIMER2 BEFORE IT RESETS
       
	  //T2CON = 0x05;					// Timer 2 on, Prescaler: 1:4
	  /*TMR2ON = 1;						// Timer 2 on
		T2CKPS1 = 0;					// Timer2 Clock Prescale Select bits: 1:4
		T2CKPS0 = 1;*/
		
	  //CCP1CON = 0x3C;					// Set Two LSbs of the PWM Duty Cycle, PWM Mode
	  /*CCPxX = 1;						// Bit 1 of PWM Duty Cycle
		CCPxY = 1;						// Bit 0 of PWM Duty Cycle
	    CCPxM3 = 1;						// Mode Select Bits: PWM Mode
		CCPxM2 = 1;*/
	
	  //CCPR1L = 0x31;					// The 8 MSbs of the 10 Bit Resolution
	}
	else if(temp==2){	
/*
 * PWM registers configuration
 * Fosc = 4000000 Hz
 * Fpwm = 702.25 Hz (Requested : 700 Hz)
 * Duty Cycle = 70 %
 * Resolution is 10 bits
 * Prescaler is 16
 */
PR2 = 0b01011000 ;
T2CON = 0b00000111 ;
CCPR1L = 0b00111110 ;
CCP1CON = 0b00011100 ;
	}
	else if(temp==3){
/*
 * PWM registers configuration
 * Fosc = 4000000 Hz
 * Fpwm = 1000.00Hz (Requested : 1000 Hz)
 * Duty Cycle = 50 %
 * Resolution is 10 bits
 * Prescaler is 4
 */
PR2 = 0b11111001;
T2CON = 0b00000101;
CCPR1L = 0b11111100;
CCP1CON = 0b00111100;
	}
	else{
/*
 * PWM registers configuration
 * Fosc = 4000000 Hz
 * Fpwm = 905.80 Hz (Requested : 900 Hz)
 * Duty Cycle = 0 %
 * Resolution is 10 bits
 * Prescaler is 16
 */

	CCPR1L = 0b00000000;
	CCPR1H = 0b00000000;
	CCP1CON = 0b00000000;
	}

	
}

void initPIC(){

		__CONFIG(0x3FF9);				// Code Protection Off, WDT Disabled, XT Oscillator
        TRISC = 0x00;                   // set PORTC as output
        PORTC = 0;                      // clear PORTC
		TRISD = 0xFF;
		PORTD = 0;						// Clear PORTB
}

void main(){

    
		initPIC();

		while(1){
			button = PORTD;
			if(PORTD==0x20){
			
		DCmotor(1);
			
			}
			else if(PORTD==0x40){
				DCmotor(2);
			}
			else if(PORTD==0x80){
				DCmotor(3);
			}
			else{
				DCmotor(0);
			}
       }
}
