#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#define F_CPU 16000000

volatile unsigned char *p_ucsra;
volatile unsigned char *p_ucsrb;
volatile unsigned char *p_ucsrc;
volatile unsigned char *p_ubrrh;
volatile unsigned char *p_ubrrl;

setup(){
	*p_ucsra = (*unsigned char) 0xC0;
	*p_ucsrb = (*unsigned char) 0xC1;
	*p_ucsrc = (*unsigned char) 0xC2;
	*p_ubrrh = (*unsigned char) 0xC4;
	*p_ubrrh = (*unsigned char) 0xC5;

	*p_ucsra &= 0xFC; //0xFC = 0b1111_1100
	*p_ucsrb &= 0x08;
	*p_ucsrc &= 0x36;
	*p_ubrrh &= 
}

int main (){
	setup();

	int i;
	char msg = " Out of the night that covers me,\n Black as the Pit from pole to pole,\n I thank
	whatever gods may be\n For my unconquerable soul.\n In the fell clutch of circumstance\n I have not
	winced nor cried aloud.\n Under the bludgeonings of chance\n My head is bloody, but unbowed.\n
	Beyond this place of wrath and tears\n Looms but the Horror of the shade,\n And yet the menace of
	the years\n Finds, and shall find, me unafraid.\n It matters not how strait the gate,\n How charged
	with punishments the scroll.\n I am the master of my fate:\n I am the captain of my soul.\n";

	while(1){
		for(i=0;i<strlen(msg);i++){
			printf("%s",msg[i]);
		}
	}
}
