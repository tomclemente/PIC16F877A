#include <pic.h>

void initPIC();
void initADC();
void delay(int x);
interrupt isr();
static bit flag;


void initPIC()
{
	__CONFIG(0x3FF9);
	TRISB = 0x00;
	TRISC = 0x00;
	
}

void initADC()
{

	TRISA = 0X0FF; // set PORTA as inputs
	
	//ADCON0 = 0X41; // Fosc/8, ADON=1 and CHS2:CHS0=000(RA0/AN0)
	//ADCON1 = 0X80; // ADFM=1 (right justified)
	ADCON0 = 0x81;
	ADCON1 = 0x05;
	INTCON = 0XC0; // GIE=1 and PEIE=1
	PIR1 = 0;      // ADIF=0
	PIE1 = 0X40;   // ADIE=1

}

void delay (int x)
{
   int y, z;
   for(y=0; y<x; y++)
	for(z=1000; z>=0;z--)
      {}
}
interrupt isr()
{
	int result;
	if(ADIF){
		ADIF=0;
		flag = 1;

			result = ((ADRESH & 0x03) << 8);
			result = result + ADRESL;
			PORTC = (result>>2) & 0x0FF;
			PORTB = (result & 0x0FF);

		}

}


void main()
{

	
	initPIC();
	initADC();

	while(1){
		ADCON0 = 0x85; // Fosc/8, GO/DONE=1 and ADON=1 
		while(!flag){}
		};							
	
}
