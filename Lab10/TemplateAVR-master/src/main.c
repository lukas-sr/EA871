#define SAMPLES 200
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

//Matriz "waves" com as seguintes formas de onda:
//Onda AM
//Onda Senoidal
//Onda Triangular
//Onda Dente de Serra
//Vazio
unsigned int i = 0, j = 0, k = 1, RX = 0;
unsigned char symbol;
unsigned char msg[40], waves[5][SAMPLES] = {{128, 141, 155, 168, 181, 192, 203, 211, 218, 223, 226, 226, 223, 218, 211, 201, 189, 176, 161, 145, 128, 110, 93, 76, 60, 46, 34, 23, 16, 10, 8, 9, 13, 19, 29, 41, 55, 71, 89, 108, 127, 147, 166, 185, 202, 217, 230, 241, 249, 253, 255, 253, 249, 241, 230, 217, 202, 185, 166, 147, 128, 108, 89, 71, 55, 41, 29, 19, 13, 9, 8 ,10, 16, 23, 34, 46, 60, 76, 93, 110, 127, 145, 161, 176, 189, 201, 211, 218, 223, 226, 226, 223, 218, 211,203,192,181,168,155,141,128,114,102,91,81,72,65,60,57,55,56,58,62,67,73,81,90,99,108,118,127,137,145,153,160,166,171,175,177,178,178,177,174,171,166,161,155,148,142,135,128,121,114,108,102,97,93,89,87,86,85,86,87,89,93,97,102,108,114,121,127,135,142,148,155,161,166,171,174,177,178,178,177,175,171,166,160,153,145,137,128,118, 108, 99, 90, 81, 73, 67, 62, 58, 56, 55, 57, 60, 65, 72, 81, 91, 102, 114},
											{127, 131, 135, 139, 143, 147, 151, 155, 159, 163, 166, 170, 174, 178, 181, 185, 188, 192, 195, 199, 202, 205, 208, 211, 214, 217, 220, 223, 225, 228, 230, 232, 235, 237, 239, 241, 242, 244, 246, 247, 248, 249, 250, 251, 252, 253, 253, 254, 254, 254, 255, 254, 254, 254, 253, 253, 252, 251, 250, 249, 248, 247, 246, 244, 242, 241, 239, 237, 235, 232, 230, 228, 225, 223, 220, 217, 214, 211, 208, 205, 202, 199, 195, 192, 188, 185, 181, 178, 174, 170, 166, 163, 159, 155, 151, 147, 143, 139, 135, 131, 127, 123, 119, 115, 111, 107, 103, 99, 95, 91, 88, 84, 80, 76, 73, 69, 66, 62, 59, 55, 52, 49, 46, 43, 40, 37, 34, 31, 29, 26, 24, 22, 19, 17, 15, 13, 12, 10, 8, 7, 6, 5, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 13, 15, 17, 19, 22, 24, 26, 29, 31, 34, 37, 40, 43, 46, 49, 52, 55, 59, 62, 66, 69, 73, 76, 80, 84, 88, 91, 95, 99, 103, 107, 111, 115, 119, 123},
											{0, 2, 5, 7, 10, 12, 15, 17, 20, 22, 25, 28, 30, 33, 35, 38, 40, 43, 45, 48, 50, 53, 56, 58, 61, 63, 66, 68, 71, 73, 76, 79, 81, 84, 86, 89, 91, 94, 96, 99, 101, 104, 107, 109, 112, 114, 117, 119, 122, 124, 127, 130, 132, 135, 137, 140, 142, 145, 147, 150, 153, 155, 158, 160, 163, 165, 168, 170, 173, 175, 178, 181, 183, 186, 188, 191, 193, 196, 198, 201, 204, 206, 209, 211, 214, 216, 219, 221, 224, 226, 229, 232, 234, 237, 239, 242, 244, 247, 249, 252, 255, 252, 249, 247, 244, 242, 239, 237, 234, 232, 229, 226, 224, 221, 219, 216, 214, 211, 209, 206, 204, 201, 198, 196, 193, 191, 188, 186, 183, 181, 178, 175, 173, 170, 168, 165, 163, 160, 158, 155, 153, 150, 147, 145, 142, 140, 137, 135, 132, 130, 127, 124, 122, 119, 117, 114, 112, 109, 107, 104, 101, 99, 96, 94, 91, 89, 86, 84, 81, 79, 76, 73, 71, 68, 66, 63, 61, 58, 56, 53, 50, 48, 45, 43, 40, 38, 35, 33, 30, 28, 25, 22, 20, 17, 15, 12, 10, 7, 5, 2},
											{0, 1, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 15, 17, 18, 19, 20, 22, 23, 24, 26, 27, 28, 29, 31, 32, 33, 34, 36, 37, 38, 40, 41, 42, 43, 45, 46, 47, 48, 50, 51, 52, 54, 55, 56, 57, 59, 60, 61, 62, 64, 65, 66, 68, 69, 70, 71, 73, 74, 75, 77, 78, 79, 80, 82, 83, 84, 85, 87, 88, 89, 91, 92, 93, 94, 96, 97, 98, 99, 101, 102, 103, 105, 106, 107, 108, 110, 111, 112, 113, 115, 116, 117, 119, 120, 121, 122, 124, 125, 126, 128, 129, 130, 131, 133, 134, 135, 136, 138, 139, 140, 142, 143, 144, 145, 147, 148, 149, 150, 152, 153, 154, 156, 157, 158, 159, 161, 162, 163, 164, 166, 167, 168, 170, 171, 172, 173, 175, 176, 177, 179, 180, 181, 182, 184, 185, 186, 187, 189, 190, 191, 193, 194, 195, 196, 198, 199, 200, 201, 203, 204, 205, 207, 208, 209, 210, 212, 213, 214, 215, 217, 218, 219, 221, 222, 223, 224, 226, 227, 228, 230, 231, 232, 233, 235, 236, 237, 238, 240, 241, 242, 244, 245, 246, 247, 249, 250, 251, 252, 254 },
											{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }};


ISR(USART_RX_vect){
	symbol = UDR0;
	RX = 1;		//mensagem recebida
}

ISR(USART_TX_vect) 
{
	if (msg[k] != '\0'){
		UDR0=msg[k];
		k++;
	} 
	else k = 1;
}

ISR(TIMER2_OVF_vect){
	//Gerando as ondas na saida PWM OCR2A 
	OCR2A = waves[i][j];
	j++;
	
	if (j==200) j = 0; 
}
void setup(){
	cli();
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
	
    //Modo FAST PWM: WGM22 = 0, WGM21 = 1, WGM20 = 1  ||  Limpar OC2A na comparação, setando 0C2A com início da contagem
	//TCCR2A = 0b10000011
	TCCR2A = 0x83;
	
	//Prescaler: 1 - CS22 = 0, CS21 = 0, CS20 = 1; T_PWM(1s) = 256*k*[(Prescaler/16MHz)*256] 
	//TCCR2B = 0b00--0001
	TCCR2B = 0x01;
	
	//Habilitar interrupção por Overflow: TOIE = 1
	TIMSK2 = 0x01;
	
	//Utilizado pino digital PWM 11 do Arduino como INPUT
	DDRB = 0x08;
		
	//OCR2A inicialmente igual a zero 
	OCR2A = 0;
	
	sei();
}

int main(){
	setup();

	while(1){
	if (RX){
		if(symbol == 'a'){
			i = 0;	//linha correspondente ao AM da matriz "waves"
			strcpy (msg, "Onda AM\n");
			UDR0 = msg[0];
			
			tempo_led=7812;
			apaga_led=7812;
		}
	
		else if(symbol == 's'){
			i = 1;	//linha correspondente ao SENO da matriz "waves"
			strcpy (msg, "Onda senoidal\n");
			UDR0 = msg[0];
			
			tempo_led=62500;
			apaga_led=62500;
		}

		else if(letra == 't'){
			i = 2;	//linha correspondente a Onda Triangular da matriz "waves"
			strcpy (msg, "Onda Triangular\n");
			UDR0 = msg[0];
					
			tempo_led=31250;
			apaga_led=31250;
		}
		else if(letra == 'd'){
			i = 3;	//linha correspondente a Dente-de-serra
			strcpy (msg, "Onda Dente-de-Serra\n");
			UDR0 = msg[0];

			tempo_led=15625;
			apaga_led=15625;
		}
		else{
			i = 4;	//linha correspondente ao vazio 
			strcpy (msg, "Nenhuma forma de onda selecionada\n");
			UDR0 = msg[0];
			PORTB=0x00;		
		}
		enviada=0;
		} 
	}
}