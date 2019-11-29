/* Atividade 09 - EA871
 *
 * Lukas da Rosa
 * RA 183167
 */
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned int control = 0;

ISR(TIMER2_OVF_vect){
	
}
	
void config(void){
	//Modo FAST PWM: WGM22 = 0, WGM21 = 1, WGM20 = 1  ||  Limpar OC2A na comparação, setando 0C2A com início da contagem
	//TCCR2A = 0b10000011
	TCCR2A = 0x83;
	
	//Prescaler: 256 - CS22 = 1, CS21 = 1, CS20 = 0; T_PWM(1s) = 256*k(=1)*[(Prescaler/16MHz)*256] 
	//TCCR2B = 0b00--0110
	TCCR2B = 0x06;
	
	//Habilitar interrupção por Overflow: TOIE = 1
	TIMSK2 = 0x01;
	
	//Utilizado pino digital PWM 11 do Arduino como INPUT
	DDRB = 0x07;
	
	//LED inicialmente ligado
	PORTB = 0x07;
	
	sei();
}

int main(void){
	config();
	
    while (1){
		
    }
}

