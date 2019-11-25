#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

volatile char buffer[10];
volatile int pos_antiga = 0;
volatile int pos_livre = 0;
volatile int contagem = 0;
volatile int count = 1;
volatile char mensagem_enviada[50];

void adicionar_buffer(volatile char c)
{
    if(contagem < 10)
    {
        if(pos_livre == 9)
        {
            buffer[pos_livre] = c;
            pos_livre = 0;
        }
        else
        {
            buffer[pos_livre] = c;
            pos_livre ++;
        }
        contagem ++;
    }
}

void remover_buffer() {
    if(contagem > 0)
    {
        if(pos_antiga == 9)
        {
            pos_antiga = 0;
        }
        else
        {
            pos_antiga++;
        }
        contagem--;
    }
}

ISR(USART_RX_vect)
{
    adicionar_buffer(UDR0);
}

ISR(USART_TX_vect)
{
    if(mensagem_enviada[count] != '\0')
    {
        UDR0 = mensagem_enviada[count];
        count ++;
    }
    else
    {
        count = 1;
    }
}

int configuracao()
{
    //configurando todos os PORTB como saídas
    DDRB = 0xFF;
    //configurando baud rate para 9600
    UBRR0H = 0;
    UBRR0L = 103;
    //MODO DE TRANSMISSÃO NORMAL e modo multiprocessador desabilitado
    UCSR0A &= 0b11111100;
    //habilita interrupção do tipo transmissão completa e recepção completa
    UCSR0B |= 0b11000000;
    //habilita o tx e o rx
    UCSR0B |= 0b00011000;
    //usart no modo assíncrono
    UCSR0C &= 0b00111111;
    //sem bits de paridade
    UCSR0C &= 0b11001111;
    //2 bits de parada
    UCSR0C |= 0b00001000;
    //8 bits por frame
    UCSR0C |= 0b00000110;
    UCSR0B &= 0b11111011;
    
    
    //habilitando as interrupções globais
    sei();
    
}



//função criada para decodificar qual led deve ser aceso e qual mensagem deve ser enviada


int envia_dados()
{
    if(contagem != 0)
    {
        
        if(buffer[pos_antiga] == 'r')
        {
            PORTB = 1;
            remover_buffer();
            strcpy (mensagem_enviada, "Comando: Acender LED - cor vermelha\n");
            UDR0 = mensagem_enviada[0];
            _delay_ms(200);
        }
        
        else if(buffer[pos_antiga] == 'g')
        {
            PORTB = 2;
            remover_buffer();
            strcpy (mensagem_enviada, "Comando: Acender LED - cor verde\n");
            UDR0 = mensagem_enviada[0];
            _delay_ms(200);
        }
        
        else if(buffer[pos_antiga] == 'b')
        {
            PORTB = 4;
            remover_buffer();
            strcpy (mensagem_enviada, "Comando: Acender LED - cor azul\n");
            UDR0 = mensagem_enviada[0];
            _delay_ms(200);
        }
        
        else if(buffer[pos_antiga] == 'y')
        {
            PORTB = 3;
            remover_buffer();
            strcpy (mensagem_enviada, "Comando: Acender LED - cor amarela\n");
            UDR0 = mensagem_enviada[0];
            _delay_ms(200);
        }
        
        else if(buffer[pos_antiga] == 'c')
        {
            PORTB = 6;
            remover_buffer();
            strcpy (mensagem_enviada, "Comando: Acender LED - cor ciano\n");
            UDR0 = mensagem_enviada[0];
            _delay_ms(200);
        }
        
        else if(buffer[pos_antiga] == 'm')
        {
            PORTB = 5;
            remover_buffer();
            strcpy (mensagem_enviada, "Comando: Acender LED - cor magenta\n");
            UDR0 = mensagem_enviada[0];
            _delay_ms(200);
        }
        
        else if(buffer[pos_antiga] == 'w')
        {
            PORTB = 7 ;
            remover_buffer();
            strcpy (mensagem_enviada, "Comando: Acender LED - cor branca\n");
            UDR0 = mensagem_enviada[0];
            _delay_ms(200);
        }
        else
        {
            remover_buffer();
            strcpy(mensagem_enviada, "Comando incorreto\n");
            UDR0 = mensagem_enviada[0];    
        }
        PORTB = 0;
    }
    else
    {
        strcpy (mensagem_enviada, "Vazio!\n");
        UDR0 = mensagem_enviada[0];
        _delay_ms(10);
    }
    
}

int main(void)
{
    configuracao();
    while (1)
    {
        envia_dados();
    }
}




