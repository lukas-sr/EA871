#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 16000000UL

unsigned volatile int k = 0, n = 0;

ISR(TIMER2_COMPA_vect){
	k = k+1;							//Contador para piscar LED externo a placa
	if (k == (245)) PORTB |= 0x10;		//Como o tempo em segundos está configurado = 0,00102: (0,5/0,00102)/2 ~= 245 ciclos
	if (k == (245*2)) PORTB &= 0xEF;	
	if (k >= (245*3)) k = 0;
	
	n = n+1;						//Contador para piscar o LED interno da placa
	if (n <= (383)) PORTB |= 0x20;		//Precisa-se de 0,78: (0,78/0,00102)/2 ~= 383 ciclos
	if (n >= (383*2)) PORTB &= 0xDF;
	if (n >= (383*3)) n=0;
}


void setup(){
	/*
	 * Modo de transmissão multi-processador desabilitado e velocidade de transmissão normal: UCSRA[1:0] setados como 0
	 * Todas as interrupções relacionadas ao UART desabilitadas: UCSR0B[7:5] setados como 0
	 * Somente o transmissor ativado: UCSR0B[3] setado como 1 e UCSR0B[4] setado como 0
	 * Número de bits de dados por frame igual a 8: UCZ0n[2:0] = (011) -> Valores setados em UCSR0B(bit 2) em UCSR0C(bits 1 e 2)
	 * Modo Assíncrono de funcionamento da USART: UCSR0C[7:6] setados como 00(UMSEL = 00) e UCSR0C[0] setado como 0
	 * Uso de um bit de paridade (ímpar): UCSR0C[5:4] setados como 11 (UPM = 11)
	 * Uso de um bit apenas de parada: UCSR0C[3] setado como 0 (USBS0 = 0)
	 */
	UCSR0A |= 0x00;
	UCSR0B |= 0x08;
	UCSR0C |= 0x36;
	
	//Setando o baundrate como 38400, UBRRn[11:0] precisa ser setado como 25
	UBRR0H = 0;
	UBRR0L = 25;
	//Modo de operação Timer sendo o CTC, precisa-se setar WGM2[2:0] = 010 p/ TCCR2A, e modo normal de opr.
	TCCR2A = 0x02;
	//TCCR2A = 0b10100010;

	/*
	 * TCCR2B[5:4] não configuráveis, TCCR2B[3]: WGM2[2] = 0 como descrito acima;  
	 * TCCR2B[2:0] = CS2[2:0] -> Clock Select -> CSS2[2:2] = 100 p/ Prescaler -> 4 (CLK_T2S/64)
	 * Usando OCR2A = 255, o tempo em segundos -> 0,00102 segundos
	 */
	TCCR2B = 0x04;
	OCR2A = 255;
	/*
	 *Mascara de interrupção do Timer em B e em A setadas [TIMSK2[2:1] = 1]
	 *Isso significa que a cada vez que o processador fizer uma comparação em A e B, vai acionar uma interrupção
	 */
	TIMSK2 = 0x02;
	//*p_ocr2b = 250;
	
	DDRB |= 0xFF;
	PORTB &= 0;

	sei(); 
}

void le_strings(char vet[]){
	int i;

	for( i=0 ; vet[i]!='\0' ;){
		UDR0 = vet[i];
		
		while(!(UCSR0A & 0b00100000));
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
