#include <pic.h>


void interrupt INT_RTN();
static bit flag;
int value=0, counter=0, last=0;
void initPIC();


void initPIC() {
__CONFIG(0x3FF9);
OPTION = 0b00000100;
GIE = 1;
//T0IE = 1;
INTE = 1;
TRISA = 0X01;
TRISB = 0X01;



}

void interrupt INT_RTN() {
 
	if(INTF) {
		INTF=0;value = value << 1;
		value = value + RA0;
		counter+=1;
			if(counter==8){ 
			counter=0;
			value=0;
			}	
		
		
	
	}


}

void main() {
initPIC();

	while(1) {
	//	if(counter==8) {
			//PORTA = value << 1;
			//PORTB = value >> 5;
		//	counter=0; 
		//	value = 0;
			PORTB = value << 1;
			PORTA = value >> 7;
	//	}
	
	}

}



	


