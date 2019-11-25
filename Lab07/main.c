#include <stdio.h>

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
     * ESPECIFICAÇÕES DA USART:
     * 
     * Velocidade de transmissão normal (i.e., modo double-speed desativado)
     * Modo de transmissão multi-processador desabilitado
     * Número de bits de dados por frame igual a 8
     * Modo assíncrono de funcionamento da USART
     * Sem bits de paridade
     * Uso de dois bits de parada
     * Baud rate igual a 9.600 bps
     * 
     */
    
    UCSR0A = 0x00; //0b00000000
    UCSR0B = 0x08;
    UCSR0C = 0;
    
}

int main(){
    
    return 0;
}
