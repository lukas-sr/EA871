/* Atividade 09 - EA871
 *
 * Lukas da Rosa
 * RA 183167
 */
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned int control = 0;		//flag = 0 para sentido de subida

ISR(TIMER2_OVF_vect){
	if (!control){
		OCR2A++;
		if (OCR2A == 255) control = 1;	 //flag = 1 para sentido de descida
	}
	
	else{
		OCR2A--;
		if (OCR2A == 0) control = 0;	//flag = 0 novamente para sentido de subida
	}
}
	
void config(void){
	//Modo FAST PWM: WGM22 = 0, WGM21 = 1, WGM20 = 1  ||  Limpar OC2A na comparação, setando 0C2A com início da contagem
	//TCCR2A = 0b10000011
	TCCR2A = 0x83;
	
	//Prescaler: 256 - CS22 = 1, CS21 = 1, CS20 = 0; T_PWM(1s) = 256*k(=1)*[(Prescaler/16MHz)*256] 
	//TCCR2B = 0b00--0101
	TCCR2B = 0x06;
	
	//Habilitar interrupção por Overflow: TOIE = 1
	TIMSK2 = 0x01;
	
	//Utilizado pino digital PWM 11 do Arduino como INPUT
	DDRB = 0x08;
	
	//LED HIGH
	PORTB = 0x08;
	
	//OCR2A inicialmente igual a zero
	OCR2A = 0;
	
	sei();
}

int main(void){
	config();
	
    while (1){
		
    }
}

