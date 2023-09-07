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
#include "nokia5110.h"


//////////////////////////////////////////////////////

void start(); 
void greenLight();
void yellowLight();
void redLight();
void callDelayGreen();
void callDelayYellow();
void callDelayRed();
void buttonGreen();
void buttonYellow();
void buttonRed();
void reloadDisplay();
void reloadTextsGreen();
void reloadTextsYellow();
void reloadTextsRed();
void selectConfig();
void configTimer();
void lightConfig();
void reloadTextsMode();
void reloadTextsCarFreq();
void buttonMode();
void checkMode();
void reloadTextsLux();
void printLine();
void USART_clock();

//////////////////////////////////////////////////////

int runTime ;
int tempo_ms ;
int countGreen ;
int countYellow  ;
int countRed  ;
int colorTime ;
int lightCounter ;
int greenTimer ;
int redTimer ;
int yellowTimer ;
char greenNumber; 
char yellowNumber;
char redNumber;
int carFreq,carFreqAux;
int carCount;
char freq[5] ;
int autoGreen, manualGreen ;
int autoRed, manualRed ;
int luxRead;
//char luxWrite[5];


typedef enum {GREEN,YELLOW,RED,MODE_SELECT} select;
select State,nextState;

typedef enum {MANUAL,AUTO} modeConfig;
modeConfig actualMode,nextMode;

typedef enum {GREEN_STATE,YELLOW_STATE,RED_STATE} lightStage;
lightStage lightState;

//////////////////////////////////////////////////////


#endif /* SEMAFORO_H_ */