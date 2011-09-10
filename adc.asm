 list p=16F877A  
INCLUDE "p16f877a.inc"

;  Registers


 __CONFIG _CP_OFF & _WDT_OFF & _XT_OSC & _PWRTE_ON & _DEBUG_OFF & _LVP_OFF & _BODEN_OFF & _CPD_OFF


  PAGE
;  Mainline of ADC

 		org    	0x0000
		clrf	PORTB                 ;  Turn off all the LED's
  		clrf   	PORTA                  ;  Use PORTA as the Input

  		bsf    STATUS, RP0    ;  select bank0 to set Port Direction
  		clrf   TRISB   ;  Set all the PORTB bits to Output
 	 	movlw  B'10000000'	;  For 16F877a, Make 10 Bit ADC "right Justified"
  		movwf  ADCON1
 		bcf    STATUS, RP0            ;  Go back to bank 0

  		movlw  0x081	;  Setup ADCON0 for ADC Conversion
  		movwf  ADCON0	;   ADCS1:ADCS0 - 10 for /32 Clock
				;   Unimplemented - 0
				;   CHS1:CHS0 - 00 for RA0/AN0
				;   Go/_Done - 0
				;   ADIF - 0
				;   ADON - 1

Loop		movlw  3	  ;  Wait 12 usec for ADC to Charge
  		addlw  0x0FF	  ;  Take One Away to Setup the Charge
  		btfss  STATUS, Z
  		goto  $ - 2

  		bsf    ADCON0, GO		;  Turn on the ADC
  		btfsc  ADCON0, GO		;  Wait for it to Complete
  		goto  $ - 1

  		bsf    STATUS, RP0
 
  		movf   ADRESL, w   ;  Read Most Significant 8 Bits in PIC16F877a

 		bcf    STATUS, RP0
  		movwf  PORTB

  		goto   Loop            ;  Get another Time Sample


  end                           

