#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 16000000

volatile unsigned char *DDRB;
volatile unsigned char *PORTB;
volatile unsigned char *PINB;
volatile unsigned char *UCSR0A;
volatile unsigned char *UCSR0B;
volatile unsigned char *UCSR0C;
volatile unsigned char *UDR0;
volatile unsigned char *UBRR0H;
volatile unsigned char *UBRR0L;

void setup(){
    
    UCSR0A = (unsigned char *) 0xC0;
    UCSR0B = (unsigned char *) 0xC1;
    UCSR0C = (unsigned char *) 0xC2;
    UBRR0H = (unsigned char *) 0xC4;
    UBRR0L = (unsigned char *) 0xC5;
    UDR0 = (unsigned char *) 0xC6;
    
    /*
     * ESPECIFICAÇÕES PARA UART:
     * 
     * Velocidade de transmissão normal (i.e., modo double-speed desativado)
     * Modo de transmissão multi-processador desabilitado
     * Número de bits de dados por frame igual a 8: UCZ0n[2:0] => 011 (UCSR0B UCSR0C)
     * Modo assíncrono de funcionamento da USART: UCSR0C[7:6] => 00(UMSEL) e UCSR0C[0] => 0
     * Sem bits de paridade: UCSR0C[5:4] => 00
     * Uso de dois bits de parada
     * Baud rate igual a 9.600 bps: UBBRn => 103
     * 
     */
    
    *UCSR0A |= 0x00;
    *UCSR0B |= 0x08;
    *UCSR0C |= 0x06;

	//Setando o baundrate como 9600 bps, UBRRn[11:0] precisa ser setado como 103
	*UBRR0H = 0;
	*UBRR0L = 103; //0b01100111 = 103

	/*
	 * Para USART Control Status Registrador A (UCSR0A):
	 *  
	 * RXCO: esta flag é setada quando existem dados válidos que ainda não foram lidos no buffer de recepção.
	 * 
	 * TXCO: esta flag é setada quando todo o frame no buffer de transmissão foi deslocado (ou seja, o dado foi enviado) 
	 * e não há mais novos dados presentes no registrador UDR0.
	 * 
	 * UDRE0: esta flag indica se o buffer de transmissão (UDR0) está pronto para receber novos dados. 
	 * Se UDRE0 = 1, então o buffer está vazio e pode ser escrito.
	 * 
	 */

	*DDRB |= 0x07;
	*PORTB &= 0xF8; //Leds inicialmente desligados
}

/*
void le_strings(char vet[]){
	int i;

	for( i=0 ; vet[i]!='\0' ;){
		*p_udr0 = vet[i];
		
		if(*p_ucsra & 0b00100000){
			i++;
			_delay_ms(100);
		}
	}

}*/

int main (){

	setup();

	while(1){

	}
}
