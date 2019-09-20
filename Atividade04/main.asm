.org 0x0000
jmp setup
.org 0x0034

;Setup do programa
setup:
lds r16,0x24	;endereço DDRB
ori r16,0x20    ;0x20 = 0010 0000, pois PB5 = 1
sts 0x24,r16

lds r16,0x27	;endereço DDRC
andi r16,0xFE   ;0xFE = 1111 1110, pois DDRC[0] precisa ser 0
sts 0x27,r16

lds r16,0x28	;endereco PORTC
ori r16,0x01	;pois PORTC[0] precisa ser 1 
sts 0x28,r16

lds r16,0x55	;endereço MCURC
andi r16,0xEF	;0xEF = 1110 1111, pois MCURC[4] precisa ser 0
sts (0x55),r16	

lds r16,0x25	;endereço PORTB
ori r16,0x20	;PORTB[5] = 1 --> LED inicialmente ligado
sts 0x25,r16	;guarda valor de r16 em 0x25

ldi r19,0x00	;r19 registrador de comparação para outros registradores
				;sempre com valor 0

;Laço principal do programa
while:
lds r16,0x26	;endereço PINC
andi r16,0x01	;máscara para isolar PINC[0] = 1 (no bit menos significativo)
cp r19,r16		;compara se r19(0x00) é igual a r16(0x00 se botão apertado, 0x01 se botão não apertado)
breq apaga_led	;faz um branch para o rótulo apaga_led se os dois registradores forem iguais
brne apaga_led2	;faz um branch para o rótulo apaga_led2 se os dois registradores forem diferentes

;Quando botão está apertado, o código vem pra cá
;o LED apaga durante 100ms e acende durante 100ms, período de 200ms

delay1:
ldi r19,0x00
ldi r20,0x49	;Valor encontrado via ajuste fino
delay1_1:
ldi r21,0x49	;Valor encontrado via ajuste fino
delay1_2:
ldi r22,0x49	;Valor encotrado via ajuste fino
delay2:
	subi r22,1
	cp r19,r22
	brne delay2
	subi r21,1
	cp r19,r21
	brne delay1_2
	subi r20,1
	cp r19,r20
	brne delay1_1
	ret

apaga_led:
lds r16,0x25	;endereço PORTB
andi r16,0xDF	;PORTB[5] = 0
sts 0x25,r16
call delay1
rjmp acende_led

delay3:
ldi r19,0
ldi r20,0x49
delay3_1:
ldi r21,0x49
delay3_2:
ldi r22,0x49
delay4:
	subi r22,1
	cp r19,r22
	brne delay4
	subi r21,1
	cp r19,r21
	brne delay3_2
	subi r20,1
	cp r19,r20
	brne delay3_1
	ret

acende_led:
lds r16,0x25	;endereço PORTB
ori r16,0x20	;PORTB[5] = 1
sts 0x25,r16
call delay3
rjmp while

;Quando o botão não está apertado, o código vem pra cá
;Led apaga por 500ms e acende por 500ms, Período de 1000ms

delay5:
ldi r19,0x00
ldi r20,0x7F	;7F = 127
delay5_1:
ldi r21,0x7F	;7F = 127
delay5_2:
ldi r22,0x7F	;7F = 127
delay6:
	subi r22,1
	cp r19,r22
	brne delay6
	subi r21,1
	cp r19,r21
	brne delay5_2
	subi r20,1
	cp r19,r20
	brne delay5_1
	ret

apaga_led2:
lds r16,0x25	;endereço PORTB
andi r16,0xDF	;PORTB[5] = 0
sts 0x25,r16
call delay5
rjmp acende_led2

delay7:
ldi r19,0
ldi r20,0x7F	;7F = 127
delay7_1:
ldi r21,0x7F	;7F = 127
delay7_2:
ldi r22,0x7F	;7F = 127
delay8:
	subi r22,1
	cp r19,r22
	brne delay8
	subi r21,1
	cp r19,r21
	brne delay7_2
	subi r20,1
	cp r19,r20
	brne delay7_1
	ret

acende_led2:
lds r16,0x25	;endereço PORTB
ori r16,0x20	;PORTB[5] = 1
sts 0x25,r16
call delay7
rjmp while