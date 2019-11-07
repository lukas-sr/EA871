#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#define F_CPU 16000000

volatile unsigned char *p_ddrb;
volatile unsigned char *p_portb;
volatile unsigned char *p_ucsra;
volatile unsigned char *p_ucsrb;
volatile unsigned char *p_ucsrc;
volatile unsigned char *p_ubrrh;
volatile unsigned char *p_ubrrl;
volatile unsigned char *p_udr0;
volatile unsigned char *p_tccr2a;
volatile unsigned char *p_tccr2b;
volatile unsigned char *p_timsk;
volatile unsigned char *p_tcnt2;
volatile unsigned char *p_ocr2a;
volatile unsigned char *p_ocr2b;
unsigned volatile int k = 0, n = 0;

ISR(TIMER2_COMPA_vect){
	k++;
	if(k == 2000)	//*p_portb |= 0b00110000;
		*p_portb = 0xFF;
	
	if (k == 4000){
	//	*p_portb &= 0b11001111;
		*p_portb &= 0;
		k=0;
	}
}


void setup(){
	p_udr0 = (unsigned char *) 0xC6;
	p_ddrb = (unsigned char *) 0x24;
	p_portb = (unsigned char *) 0x25;
	p_ucsra = (unsigned char *) 0xC0;
	p_ucsrb = (unsigned char *) 0xC1;
	p_ucsrc = (unsigned char *) 0xC2;
	p_ubrrh = (unsigned char *) 0xC4;
	p_ubrrh = (unsigned char *) 0xC5;
	p_timsk = (unsigned char *) 0x70;
	p_tcnt2 = (unsigned char *) 0xB2;
	p_ocr2a = (unsigned char *) 0xB3;
	p_ocr2b = (unsigned char *) 0xB4;
	p_tccr2a = (unsigned char *) 0xB0;
	p_tccr2b = (unsigned char *) 0xB1;

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

	//Modo de operação Timer sendo o CTC, precisa-se setar WGM2[2:0] = 010 p/ TCCR2A, e modo normal de opr.
	*p_tccr2a = 0x02;
	//*p_tccr2a = 0b10100010;

	/*
	 * Para modo de operação não-PWM, TCCR2B[7:6] setados em nível alto; TCCR2B[5:4] não configuráveis  
	 * TCCR2B[3]: WGM2[2] = 0 como descrito acima; TCCR2B[2:0] = CS22:20 -> Clock Select
	 * CSS22:20 = 010 p/ Prescaler = 8 (CLK_T2S/8)
	 * CLK = 16MHz/8 = 2MHz -> Faixa(máx. intervalo de tempo que o temporizador consegue medir)=128µs
	 */

	*p_tccr2b = 0xC2;
	
	//Mascara de interrupção do Timer em B e em A setadas [TIMSK2[2:1] = 1]

	//Isso significa que a cada vez que o processador fizer uma comparação em A e B, vai acionar uma interrup.
	
	*p_timsk = 0x06;

	*p_ocr2a = 255;

	//*p_ocr2b = 250;
	
	*p_ddrb |= 0xFF;
	*p_portb &= 0b11111111;

	sei(); 
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
