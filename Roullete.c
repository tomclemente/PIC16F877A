#include <pic.h>

void convert(int num);
void initPIC();
void initADC();
void delay(int num);
void interrupt INT_RTN();
static bit adcflag,tflag;
int result;

void initPIC()
{
	__CONFIG(0x3FF9);
	TRISB = 0x00;
	TRISC = 0x00;
	TRISD = 0x00;
	PORTC = 0;
	PORTD = 0;
	GIE = 1;
	//T0IE = 1;
	//T0IF = 1;
	OPTION = 0x04;
}

void initADC()
{

	TRISA = 0X0FF; // set PORTA as inputs
	ADCON0 = 0X41; // Fosc/8, ADON=1 and CHS2:CHS0=000(RA0/AN0)
	ADCON1 = 0X05; // ADFM=1 (right justified)
	INTCON = 0XC0; // GIE=1 and PEIE=1
	PIR1 = 0;      // ADIF=0
	PIE1 = 0X40;   // ADIE=1

}

void delay(int num){
int x,y;
for(x=num;x>=0;x--){
for(y=1000;y>=0;y--){}
}
}
/*
void delay(int num) {
	while(num>0) {
		if(tflag==1) {
		num=num-1;
		tflag=0;
		}
	}
	GIE=1;
}	*/


void interrupt INT_RTN()
{	
	if(T0IF) {	
		T0IF=0;
		tflag=1;
	}	
	
	if(ADIF){
		ADIF=0;
		adcflag = 1;
		result = ADRESH;
		/*	result = ((ADRESH & 0x03) << 8);
			result = result + ADRESL;		
			PORTB = (result>>2) & 0xFF;
			PORTC = (result & 0x03); */
	}
}


void main()
{
	
	initPIC();
	initADC();
	PR2 = 0b11111001 ;
	T2CON = 0b00000101 ;
	CCP1CON = 0b00111100 ;
 	//=	PR2 = 0b01111100 ;
 	//  T2CON = 0b00000101 ;
	//CCP1CON = 0b00111100 ;	
	//CCPR1L=0;
	while(1){
		ADCON0 = 0x45; // Fosc/8, GO/DONE=1 and ADON=1 
	 	if(adcflag){		
				convert(result);
				delay(20);
				adcflag=0;
			}
		}					
	
}

void convert(int num) {

	if(num>=180) {
	RD0=1;
	PORTB=255;
		if(CCPR1H==200){
		CCPR1L = 200;
		}		
		else if(CCPR1H>150 && CCPR1H<200){
		CCPR1L+=2;
		}
		else if(CCPR1H<=150) {
		CCPR1L+=10;
		}

	}

	else if(num<180 && num>=130) {
	RD0=1;
	PORTB=8;
		if(CCPR1H==175) {
		CCPR1L = 175;
		}	
		else if(CCPR1H>190 && CCPR1H<=255) {
		CCPR1L-=3;
		}
		else if(CCPR1H>130 && CCPR1H<=190) {
		CCPR1L+=5;
		}
		else if(CCPR1H<=100) {
		CCPR1L+=10;

		}

	}

	else if(num<130 && num>=90){
	RD0=1;
	PORTB=4;
		if(CCPR1H==125) {
		CCPR1L = 125;
		}	
		else if(CCPR1H>150 && CCPR1H<=255) {
		CCPR1L-=3;
		}
		else if(CCPR1H>90 && CCPR1H<=150) {
		CCPR1L+=5;
		}
		else if(CCPR1H<=90) {
		CCPR1L+=10;
		}
	}

	else if(num<90 && num>10) {
	RD0=1;
	PORTB=2;
		if(CCPR1H==70) {
		CCPR1L = 70;
		}	
		else if(CCPR1H>150 && CCPR1H<=255) {
		CCPR1L-=10;
		}	
		else if(CCPR1H>100 && CCPR1H<=150) {
		CCPR1L-=5;
		}
		else if(CCPR1H<=100)  {
		CCPR1L+=3;
		}


	} 

	else if(num<=10) {

	PORTB=1;
			if(CCPR1H<=10){
			CCPR1L = 0;
			RD0=0;
			}
			else if(CCPR1H>150 && CCPR1H<=255) {
			CCPR1L-=2;
			}	
			else if(CCPR1H>100 && CCPR1H<=150) {
			CCPR1L-=3;
			}
			else if(CCPR1H<=100) {
			CCPR1L-=4;
	
			}
		
	
	}


}