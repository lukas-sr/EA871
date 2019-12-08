#define F_CPU 16000000UL
#define MAX 10
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

volatile int i = 0, k = 1;	//contadores
volatile int symbols = 0, old = 0, new = 0, flag=0;	//controle de posição e de simbolos no buffer
volatile int colors[7] = {0x01, 0x02, 0x04, 0x03, 0x06, 0x05, 0x07};
volatile char buffer[MAX];
char msg[30];

void add_buffer(volatile char c) {
	if (symbols <= MAX-1){
		if (new >= MAX-1){
			buffer[new] = c;
			new = 0;
		}
		
		else{
			buffer[new] = c;
			new++;
		}
		symbols++;
	}
}

void remove_buffer() {
	if (symbols > 0){
		if (old >= MAX-1){
			old = 0;
		}
		else old++;

		symbols--;
	}
}

ISR(USART_RX_vect){
	add_buffer(UDR0);
}

ISR(USART_TX_vect){
	if(msg[k] != '\0'){
		UDR0 = msg[k];
		k++;
	}
	else k=1;
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
	 * 
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

	UCSR0A &= 0b11111100;
	UCSR0B |= 0b11011000;
	UCSR0B &= 0b11111011;
	UCSR0C |= 0b00001110;
    UCSR0C &= 0b00001111;

	// Configurando o baundrate como 9600 bps, UBRRn[11:0] precisa ser setado como 0b01100111 = 103
	
	UBRR0H = 0;
	UBRR0L = 103;
	
	/*
	 * LED RGB conectado na GPIO pinos 8(B0), 9(B1) e 10(B2).
	 * Pinos setados como saída de dados DDRB[2:0] => 1
	 * LED RGB inicialmente desligado PORTB[2:0] => 0
	 */
	
	DDRB |= 0b00000111;
	PORTB |= 0b00000000;
	
	// sei() seta o flag I no SREG, habilitando interrupção 
	sei();
}

int main (){

	setup();

	while(1){
    if(symbols != 0)
    {
        switch (buffer[old])
		{
			case 'r':
				PORTB = colors[0];
				remove_buffer();
				strcpy (msg, "Comando: Acender LED - cor Vermelha\n");
				UDR0 = msg[0];
				flag=1;
				_delay_ms(200);		
				break;
			
			case 'g':
				PORTB = colors[1];
				remove_buffer();
				strcpy (msg, "Comando: Acender LED - cor Verde\n");
				UDR0 = msg[0];
				flag=1;
				_delay_ms(200);		
				break;

			case 'b':
				PORTB = colors[2];
				remove_buffer();
				strcpy (msg, "Comando: Acender LED - cor Azul\n");
				UDR0 = msg[0];
				flag=1;
				_delay_ms(200);		
				break;

			case 'y':
				PORTB = colors[3];
				remove_buffer();
				strcpy (msg, "Comando: Acender LED - cor Amarela\n");
				UDR0 = msg[0];
				flag=1;
				_delay_ms(200);		
				break;

			case 'c':
				PORTB = colors[4];
				remove_buffer();
				strcpy (msg, "Comando: Acender LED - cor Ciano\n");
				UDR0 = msg[0];
				flag=1;
				_delay_ms(200);		
				break;

			case 'm':
				PORTB = colors[5];
				remove_buffer();
				strcpy (msg, "Comando: Acender LED - cor Magenta\n");
				UDR0 = msg[0];
				flag=1;
				_delay_ms(200);		
				break;

			case 'w':
				PORTB = colors[6];
				remove_buffer();
				strcpy (msg, "Comando: Acender LED - cor Branca\n");
				UDR0 = msg[0];
				flag=1;
				_delay_ms(200);		
				break;

			default:
				remove_buffer();
			    PORTB = 0;

				if (!flag){
					strcpy(msg, "Comando incorreto\n");
					UDR0 = msg[0];
					_delay_ms(200);
				}
				flag=0;
				break;
			}


    }
    else
    {	
        strcpy (msg, "Vazio!\n");
        UDR0 = msg[0];
        _delay_ms(50);
    }   
	}
}