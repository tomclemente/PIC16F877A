#include <pic.h>	
	
#define SPI_CLOCK RC3			//  Define the Max6675 pins
#define SPI_CS RC5				
#define SPI_DATA RC4
#define SPI_SS RA5

void convert2(int num);
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
	T0IE=1;
	T0IF=0;

	GIE = 1;
	OPTION = 0x04;

	//ADCON0=0b00000000; 			
 	//ADCON1=0b00000111;			//A/D conversion off
	//TRISC|=0b00010000;			//SDI is automatically controlled by the SPI module,in any case i set it as an input (TRISC_4 set).
	//TRISC&=0b11011001;			//SDO must have TRISC_5 cleared (output)
	//TRISC&=0b11110001; 			//SCK in master mode must have TRISC_3 cleared (output)

	TRISC=0b00010000;			//CCP1=Output(TRISC<2>), SD0=output(TRISC<5>), SCK=output(TRISC<3>), SDI=input(TRISC<4>)
	TRISA=0b11011111; 			//SS must have TRISA_5 cleared (output)

	TRISD=0x7f;
	SSPCON=0b00110010;			//WCOL=0,SSPOV=0,enable SSP,idle state=1 (CKP=1),master mode, clock=FOSC/64. [CKP=1,CKE=0==>MODE(1,1)]
	SSPSTAT=0b10000000; 		//SMP=0, data transmitted on rising edge of SCK (CKE=0)         
              
	SPI_SS=1;
	SPI_SS=0;
	TRISB=0x00;
	PORTD = 0;
}

void initADC()	{	
	//ADCON0 = 0X41; // Fosc/8, ADON=1 and CHS2:CHS0=000(RA0/AN0)
	ADCON0 = 0b01001001;
	//ADCON1 = 0X0F; // ADFM=1 (right justified) ;
	ADCON1 = 0b00001101;
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


void interrupt INT_RTN()	{	
	int dummy;

	if(ADIF){
		ADIF=0;
		adcflag = 1;
		result = ADRESH;	
		
	
		SSPBUF = ADRESH;
		while(!BF);
		dummy = SSPBUF;
	}
}


void main()	{
	
	initPIC();
	initADC();
	PR2 = 0b11111001 ;
	T2CON = 0b00000101 ;
	CCP1CON = 0b00001100 ;
	PORTD = 0;
 	while(1){
	//	ADCON0 = 0x45; // Fosc/8, GO/DONE=1 and ADON=1 
	//	ADCON =  0b01000101
		ADCON0 = 0b01001101;
	 	if(adcflag){		
			convert(result);
			delay(15);
			adcflag=0;
			}
		}					
	
}
void convert2(int num) {
	RD0=1;
	if(num>70) {

	CCPR1L+=1;
	}


	else if(num<=70) {
	CCPR1L-=1;

	}

}



void convert(int num) {
	if(num>=180) {
	CCP1CON = 0b00001100 ;
	RD0=1;
//	SSPCON=0b00110010;	
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
	CCP1CON = 0b00001100 ;
	RD0=1;
	PORTB=8;
//	SSPCON=0b00110010;
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
	CCP1CON = 0b00001100 ;
	RD0=1;
	PORTB=4;
//	SSPCON=0b00110010;
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
	CCP1CON = 0b00001100 ;
	RD0=1;
//	SSPCON=0b00110010;
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

//	SSPCON=0b00010010;

			if(CCPR1H<=5){
			CCP1CON = 0b00000000 ;	
			CCPR1L = 0;
			RD0=0;
			}
			else if(CCPR1H>150 && CCPR1H<=255) {
			CCP1CON = 0b00001100 ;
			CCPR1L-=1;
			RD0=1;
			}	
			else if(CCPR1H>100 && CCPR1H<=150) {
		 	CCP1CON = 0b00001100 ;
			CCPR1L-=2;
			RD0=1;
			}
			else if(CCPR1H<=100) {
	 		CCP1CON = 0b00001100 ;
			CCPR1L-=4;
			RD0=1;
	
			}
		
	
	}


} 