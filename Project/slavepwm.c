#include <pic.h>
#include <string.h>

#define SPI_CLOCK RC3			//  Define the Max6675 pins
#define SPI_CS RC5				
#define SPI_DATA RC4
#define SPI_SS RA5

void convert(int num);
void delay();
void initPIC(void){
	__CONFIG(0x3FF9);
	ADCON0=0b00000000; 			
 	ADCON1=0b00000111;			//A/D conversion off
	TRISC|=0b00010000;			//SDI is automatically controlled by the SPI module,in any case i set it as an input (TRISC_4 set).
	TRISC&=0b11011111;			//SDO must have TRISC_5 cleared (output)
	TRISC&=0b11111111; 		
	TRISA&=0b11011111; 			//SS must have TRISA_5 cleared (output)
	TRISD=0x00;
	SSPCON=0b00110101;			//WCOL=0,SSPOV=0,enable SSP,idle state=1 (CKP=1),slave mode
	SSPSTAT=0b00000000; 		//SMP=0, data transmitted on rising edge of SCK (CKE=0)                       
	TRISB=0x00;

}

void main()
{
int store;
initPIC();
	PR2 = 0b11111001 ;
	T2CON = 0b00000101 ;
	CCP1CON = 0b00111100 ;
	while(1)
	{

			store = SSPBUF;
			PORTD = store;
			convert(store);

		
		while(!BF);
		
		SSPBUF = 255;
		
			
	}

}

void delay()
{
for(int x=0;x<10000;x++){};
}




void convert(int num) {


	if(num>=180) {
	RD0=1;
	PORTB=255;
		if(CCPR1H==200){
		CCPR1L=200;
		}		
		else if(CCPR1H>150 && CCPR1H<200){
		CCPR1L+=2;
		}
		else if(CCPR1H<=150) {
		CCPR1L+=5;
		}

	}

	else if(num<180 && num>=90){
	RD0=1;
	PORTB=4;
		if(CCPR1H==150) {
		CCPR1L=150;
		}	
		else if(CCPR1H>150 && CCPR1H<=255) {
		CCPR1L-=2;
		}	
		else if(CCPR1H>100 && CCPR1H<=150) {
		CCPR1L+=3;
		}
		else if(CCPR1H<=100) {//if(CCPR1L<100) {
		CCPR1L+=4;
		}
	}

	else if(num<90 && num>0) {
	RD0=1;
	PORTB=2;
		if(CCPR1H==100) {
		CCPR1L = 100;
		}
		else if(CCPR1H>150 && CCPR1H<=255) {
		CCPR1L-=7;
		}	
		else if(CCPR1H>100 && CCPR1H<=150) {
		CCPR1L-=5;
		}
		else if(CCPR1H<=100)  {//if(CCPR1L<100) {
		CCPR1L+=3;
		}
	} 

	else if(num<=10) {

	PORTB=1;
			if(CCPR1H==0){
			CCPR1L = 0;
			RD0=0;
			}
			else if(CCPR1H>150 && CCPR1H<=255) {
			CCPR1L-=1;
			}	
			else if(CCPR1H>100 && CCPR1H<=150) {
			CCPR1L-=2;
			}
			else if(CCPR1H<=100) {//if(CCPR1L<100) {
			CCPR1L-=4;
			}
		
	
	}

}