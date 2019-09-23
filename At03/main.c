/*
 * Atividade_Display_7seg.c
 * Author : Lukas Rosa
 */

#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 1600000UL

volatile unsigned char *p_ddrC; 
volatile unsigned char *p_pinC;
volatile unsigned char *p_portC;
volatile unsigned char *p_ddrD;
volatile unsigned char *p_portD;
volatile unsigned char *p_mcu;
unsigned char *p_ucsr0b; /* cria ponteiro para o registrador UCSR0B */

void setup(void)
{
	p_ddrC = (unsigned char *) 0x27; //endereço de DDRC
	p_ddrD = (unsigned char *) 0x2A; //endereço de DDRD
	p_portD = (unsigned char *) 0x2B; //endereço de PORTD
	p_portC = (unsigned char *) 0x28; //endereço de PORTC
	p_pinC = (unsigned char *) 0x26; //endereço de PINC
	p_mcu = (unsigned char *) 0x55;
	p_ucsr0b = (unsigned char *) 0xC1;
	
	*p_mcu &= 0b11101111;
	*p_ddrD |= 0xFE; // equivale a *p_ddrD = *p_ddrD | 0xFE; 0xFE (1111 1110) --> para habilitar escrita de todos os pinos no circuito
	*p_ddrC &= 0x00; // equivale a *p_ddrC = *p_ddrC & 0x01; 0x01 --> para habilitar leitura do circuito
	*p_portD |= 0x7E; // Display de 7 segmentos inicialmente igual a 'zero'
	*p_portC |= 0x01;
	*p_ucsr0b = 0;
}

int main(void)
{
	setup();

	unsigned int botao = 0;

	while (1){
		if ((*p_pinC & 0x01) == 0x00){
			_delay_ms(75);
			if ((*p_pinC & 0x01) == 0x01) botao++;
		}

		if(botao == 0) *p_portD = (unsigned char) 0x7E; // Display = 0
		
		if(botao == 1) *p_portD = (unsigned char) 0x0C; // Display = 1
		
		if(botao == 2) *p_portD = (unsigned char) 0xB6; // Display = 2
		
		if(botao == 3) *p_portD = (unsigned char) 0x9E; // Display = 3
		
		if(botao == 4) *p_portD = (unsigned char) 0xCC; // Display = 4
		
		if(botao == 5) *p_portD = (unsigned char) 0xDA; // Display = 5
		
		if(botao == 6) *p_portD = (unsigned char) 0xFA; // Display = 6
		
		if(botao == 7) *p_portD = (unsigned char) 0x0E; // Display = 7
		
		if(botao == 8) *p_portD = (unsigned char) 0xFF; // Display = 8
		
		if(botao == 9) *p_portD = (unsigned char) 0xDF; // Display = 9
		
		if(botao == 10) botao = 0; // Volta a 0 		
}
}