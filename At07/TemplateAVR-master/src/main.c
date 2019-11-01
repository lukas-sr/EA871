#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#define F_CPU 16000000
#define MAX 10

volatile unsigned char *_DDRB;
volatile unsigned char *_PORTB;
volatile unsigned char *_PINB;
volatile unsigned char *_MCUCR;
volatile unsigned char *_UCSR0A;
volatile unsigned char *_UCSR0B;
volatile unsigned char *_UCSR0C;
volatile unsigned char *_UDR0;
volatile unsigned char *_UBRR0H;
volatile unsigned char *_UBRR0L;
unsigned char buffer[MAX], letra;
unsigned int i = 0, k = 0;

/*
void adicionar_buffer(char c) {
 if (k<MAX){
    buffer[i] = c;
    i++;
  	k++;
  }
}
*/
ISR (USART_RX_vect){
	letra = *_UDR0;
//	adicionar_buffer(letra);
}

void setup(){

    /*
     * ESPECIFICAÇÕES PARA UART:
     * 
     * Velocidade de transmissão normal (i.e., modo double-speed desativado)
     * Modo de transmissão multi-processador desabilitado
     * Número de bits de dados por frame igual a 8: UCZ0n[2:0] => 011 (UCSR0B UCSR0C)
     * Modo assíncrono de funcionamento da USART: UCSR0C[7:6] => 00(UMSEL) e UCSR0C[0] => 0
     * Sem bits de paridade: UCSR0C[5:4] => 00
     * Uso de dois bits de parada
     * Interrupção do tipo recepção completa habilitada: UCSR0B[7] => 1
     */
	
    _PINB = (unsigned char *) 0x23;
	_DDRB = (unsigned char *) 0x24;
	_PORTB = (unsigned char *) 0x25;
	_MCUCR = (unsigned char *) 0x55;
    _UCSR0A = (unsigned char *) 0xC0;
    _UCSR0B = (unsigned char *) 0xC1;
    _UCSR0C = (unsigned char *) 0xC2;
    _UBRR0H = (unsigned char *) 0xC4;
    _UBRR0L = (unsigned char *) 0xC5;
    _UDR0 = (unsigned char *) 0xC6;

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
	 */
    
    *_UCSR0A |= 0x00;
    *_UCSR0B |= 0x88;
    *_UCSR0C |= 0x06;	

	*_UBRR0H = 0;
	*_UBRR0L = 103;

	/* 
	 * Configurando o baundrate como 9600 bps, UBRRn[11:0] precisa ser setado como 0b01100111 = 103
	 */
	
	*_DDRB |= 0b00000111;
	*_PORTB|= 0b00000111;
//	*_PINB |= 0b00000111;	
	
	/*
	 * LED RGB conectado na GPIO pinos 8(B0), 9(B1) e 10(B2).
	 * Pinos setados como saída de dados DDRB[2:0] => 1
	 * LED RGB inicialmente desligado PORTB[2:0] => 0
	 */

//	*_MCUCR &= 0b11101111;
	
	sei();

	/*
	 * sei() seta o flag I no SREG, habilitando interrupção 
	 */
}


int main (){
	
	setup();
	
	while(1){
		if(letra == 'r'){
			 *_PORTB = 0x01;
			 //definir mensagem
			 //enviar -> interrupção TX OU buffer vazio
			 //enviar para UR0 (TX)
			 //
		}
		else if
	}
}
