/*
 * CFile1.c
 *
 * Created: 15/07/2021 12:38:59
 *  Author: Guilherme
 */ 

#include "semaforo.h"


void start(){
	///////////////////////////////////////////////////////////////////////////////////////////
	DDRB  = 0b00000010 ; //Definir qual pino usar no caso B1
	PORTB = 0b00000000 ;//Inicializar o  B=0
	
	DDRC  = 0b11111110 ; //Definir qual pino usar no caso C1(clockSignal),C0(reset)
	PORTC = 0b00000000 ;//Inicializar o  C = 0
	
	/////////////////////////////////////////////////////////////////////////////////////////
	//Configuração da USART
	UBRR0H = (unsigned char)(MYUBRR>>8);	//Ajusta a taxa de transmissão, parte alta
	UBRR0L = (unsigned char)MYUBRR;		//Ajusta a taxa de transmissão, parte baixa
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0); //Habilita a interrup. do receptor, Habilita o transmissor e o receptor
	UCSR0C = (3<<UCSZ00); //Ajusta o formato do frame: 8 bits de dados e 1 de parada, paridade none
	/////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////
	
}

ISR(USART_RX_vect){
	clockSignal = UDR0;
	if(clockSignal==1){ //Comando para dar um pulso de clock
		PORTB |= 0b00000010 ;		//Subida Clock
		PORTB &= 0b11111101 ;		//Descida Clock
	}
}