/*
 * GccApplication7.c
 *
 * Created: 17/08/2021 10:39:57
 * Author : Guilherme
 */ 

#include "semaforo.h"


int main(void) {
	
	start();
	
	sei();
	
	while (1) {
		
		if(tempo_ms%500==0){//A cada 0.5s atualiza a string e verifica o valor lido
			if (luxRead>472){ //Caso em q Lux do ldr > 300
				OCR0A = 0 ;		//Desliga luz
			}
			else if (luxRead<473&& (PINB&(1<<PINB7)|| carFreq != 0)){ // caso lux do ldr <300 e pessoas ou carros
				OCR0A = 255 ;		//Acende luz 100%
			}
			else if (luxRead<473&& (!PINB&(1<<PINB7)|| carFreq == 0)){ // caso lux do ldr <300 sem pessoas ou carros
				OCR0A = 77 ;	//Acende luz 30%
			}
			reloadDisplay();
		}
		
		if(tempo_ms%985==0){//A cada 1s verifica quantos carros passaram //Por algum motivo pegar a "amostragem" de carros antes de 1segundo para prever o nmr de carros por minuto gera menos "variação"
			carFreq = carCount*60 ; //Multiplica por 60 a quantidade de carros/s para "prever" carros/minuto
			reloadDisplay();
			carCount = 0 ;
		}
		
	}
}

