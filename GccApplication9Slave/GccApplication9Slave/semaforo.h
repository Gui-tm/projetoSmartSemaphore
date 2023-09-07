/*
 * semaforo.h
 *
 * Created: 15/07/2021 12:37:25
 *  Author: Guilherme
 */ 

#ifndef SEMAFORO_H_
#define SEMAFORO_H_
//////////////////////////////////////////////////////

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>


//////////////////////////////////////////////////////

void start(); 


//////////////////////////////////////////////////////



int clockSignal;



//////////////////////////////////////////////////////


#endif /* SEMAFORO_H_ */