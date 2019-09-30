#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 16000000

/* Variáveis:
 *
 * DDRB, DDRC e DDRD para configurarmos direção dos dados (data direction register)
 * LedRGB usado no DDRB, usando pinos 8,9 e 10 (PB0,PB1 e PB2) setado como saída/escrita
 * Primeiro botao no DDRC no pino 0 (A0) setado como entrada/leitura, então seu valor igual a 0, sentido da transição
 * Segundo botão no DDRD nos pino 2 (PD2) setado como entrada/leitura, então seu valor igual a 0, controle de velocidade 
 */

unsigned char *p_ddrB;
unsigned char *p_ddrC;
unsigned char *p_ddrD;

/* DDRB como saída/escrita, precisa-se do registrador de endereços de entrada da porta B (PINB)
 * DDRC como entrada/leitura do botão, precisa-se usar o registrador de dados da porta C (PORTC), alem do pullup nesse mesmo pino, então (PINC)=1 
 * DDRD como entrada/leitura do botão 2, precisa-se usar o registrador de dados da porta D (PORTD), alem do pullup nesse mesmo pino, então (PIND)=1
 */

unsigned char *p_portB;
unsigned char *p_pinB;
unsigned char *p_portC;
unsigned char *p_pinC;
unsigned char *p_portD;
unsigned char *p_pinD;

/* MCURC como setup do pull-up, precisa-se fazer MCURC[4] = 0 para habilitar o pull-up 
 * EICRA e EIMSK, registradores para configurar as interrupções nos pinos INT0
 * PCICR para habilitar PCINT1
 * PCICR1 para o botão que controla o sentido da transição em A0
 */

unsigned char *p_mcucr;
unsigned char *p_eicra;
unsigned char *p_eimsk;
unsigned char *p_pcicr;
unsigned char *p_pcmsk;
unsigned char *p_pcint;

unsigned int i = 0, cont_botao_A = 0, cont_botao_B = 0;
unsigned int vetor[8] = {0x00, 0x01, 0x02, 0x04, 0x03, 0x06, 0x05, 0x07};

ISR(INT0_vect){
	cont_botao_A++;
	if (cont_botao_A == 4) cont_botao_A = 0;
}

ISR(PCINT1_vect){
	cont_botao_B++;
	if (cont_botao_B == 4) cont_botao_B = 0;
}
void setup(void)
{
	p_ddrB = (unsigned char *) 0x24;
	p_ddrC = (unsigned char *) 0x27;
	p_ddrD = (unsigned char *) 0x2A;
	p_portB = (unsigned char *) 0x25;
	p_portC = (unsigned char *) 0x28;
	p_portD = (unsigned char *) 0x2B;
	p_pinB = (unsigned char *) 0x23;
	p_pinC = (unsigned char *) 0x26;
	p_pinD = (unsigned char *) 0x29;
	p_mcucr = (unsigned char *) 0x55;
	p_eicra = (unsigned char *) 0x69;
	p_eimsk = (unsigned char *) 0x3D;
	p_pcicr = (unsigned char *) 0x68;
	p_pcmsk = (unsigned char *) 0x6C;

	*p_ddrB |= 0b00000111;
	*p_ddrC &= 0b11111110;
	*p_ddrD &= 0b11111011;
	
	*p_pinB |= 0b00000111;					/* Pull-up nos pinos de saída p/ LEDs */
	*p_portB &= 0b11111000;					/* Inicialmente, LEDs apagados =(000) */
	*p_portC &= 0b00000001;					/* Para pull-up, PORTC[0] setado em 1 */
	*p_portD &= 0b00000100;					/* Para pull-up, PORTD[2] setado em 1 */

	*p_eicra |= 0b00000011;					/* INT0 configurado para ser ativado pela borda de subida no pino */
	*p_eimsk |= 0b00000001;					/* habilita INT0 (se fosse INT1, seria 0b00000010) */
	*p_mcucr &= 0b11101111;					/* habilita pullup nos pinos que forem setados posteriormente */
	*p_pcmsk |= 0b00000001;					/* habilita o disparo de interrupções a cada mudança de nível lógico no PCINT1 */
	*p_pcicr |= 0b00000010;					/* habilita as interrupções PCINT1 */

	sei(); 									/* seta o flag I no SREG, habilitando as interrupções */
}

void _sentido(void){
	//Sentido de rotação dos LEDs
	if (cont_botao_B == 0){
		i++;
		if(*p_portB == 0x07) i=0;
	}

	else if(cont_botao_B == 1){
		i++;
		if(*p_portB == 0x07) i=0;
	}

	else if(cont_botao_B == 2){
		i--;
		if (*p_portB == 0x00) i=7;
	}

	else if(cont_botao_B == 3){
		i++;
		if(*p_portB == 0x07) i=0;			
	}
}

void _tempo(void){
	int t;
	if (cont_botao_A == 1){
		for(t = 0; t<20 ; t++){
			_delay_ms(25);
		}
	}
	else if(cont_botao_A == 2){
		for(t = 0; t<10 ; t++){
			_delay_ms(25);
		}
	}
	else if(cont_botao_A == 3){
		for(t = 0; t<5 ; t++){
			_delay_ms(25);
		}
	}
	else{
		_delay_ms(1000);
	}
}
int main(void)
{
	setup();
	
	while (1){
		*p_portB = vetor[i];
		_tempo();
		_sentido();
	}
}
