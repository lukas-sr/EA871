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
volatile unsigned char *p_udr0;
volatile unsigned char *p_tccr2a;
volatile unsigned char *p_tccr2b;
volatile unsigned char *p_timsk;

void setup(){
	p_ucsra = (unsigned char *) 0xC0;
	p_ucsrb = (unsigned char *) 0xC1;
	p_ucsrc = (unsigned char *) 0xC2;
	p_ubrrh = (unsigned char *) 0xC4;
	p_ubrrh = (unsigned char *) 0xC5;
	p_udr0 = (unsigned char *) 0xC6;

	/*
	 * Modo de transmissão multi-processador desabilitado e velocidade de transmissão normal: UCSRA[1:0] setados como 0
	 * Todas as interrupções relacionadas ao UART desabilitadas: UCSR0B[7:5] setados como 0
	 * Somente o transmissor ativado: UCSR0B[3] setado como 1 e UCSR0B[4] setado como 0
	 * Número de bits de dados por frame igual a 8: UCZ0n[2:0] = (011) -> Valores setados em UCSR0B(bit 2) em UCSR0C(bits 1 e 2)
	 * Modo Assíncrono de funcionamento da USART: UCSR0C[7:6] setados como 00(UMSEL = 00) e UCSR0C[0] setado como 0
	 * Uso de um bit de paridade (ímpar): UCSR0C[5:4] setados como 11 (UPM = 11)
	 * Uso de um bit apenas de parada: UCSR0C[3] setado como 0 (USBS0 = 0)
	 */

	*p_ucsra |= 0x00;
	*p_ucsrb |= 0x08;
	*p_ucsrc |= 0x36;
	
	//Setando o baundrate como 38400, UBRRn[11:0] precisa ser setado como 25
	*p_ubrrh = 0;
	*p_ubrrl = 25; 
}

void le_strings(char vet[]){
	int i;

	for( i=0 ; vet[i]!='\0' ;){
		*p_udr0 = vet[i];
		
		while(!(*p_ucsra & 0b00100000));
		i++;
		_delay_ms(100);

	}
}

int main (){
	setup();

	char msg[] = "Atividade 8 - Interrupcoes temporizadas tratam concorrencia entre taregas!\n";
	char end[] = "Mensagem transmitida com sucesso!";

	while(1){	
		le_strings(msg);
		le_strings(end);
		_delay_ms(5000);
	}
}
