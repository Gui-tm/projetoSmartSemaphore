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
	
	DDRD  = 0b11110011 ; //Definir D como saida
	PORTD = 0b00000000 ;
	
	EIMSK = 0b00000011;  //Utilizar INT0 para saber se qualquer botão foi pressionado
	EICRA = 0b00001011; // Sensível borda de subida
	
	
	TCCR1A = 0b00000000; //habilita modo CTC do TC1
	TCCR1B |= (1 << WGM12)|(1 << CS11); //liga TC0 com prescaler = 8
	TCNT1 = 0;
	
	OCR1A = 499 ;
	TIMSK1 = 0b00000010;
	
	ADMUX = 0b01000000; //Tensão interna de ref (1.1V), canal 0
	ADCSRA = 0b11101111; //habilita o AD, habilita interrupção, modo de conversão contínua, prescaler = 128
	ADCSRB = 0x00; //modo de conversão contínua
	DIDR0 = 0b00111110; //habilita pino PC0 como entrada do ADC0
	//PWM////////////////////////////////////////////////////////////////////////////////////
	TCCR0A = 0b10100011; //PWM não invertido nos pinos OC0A e OC0B
	TCCR0B = 0b00000101; //liga TC0, prescaler = 1024, fpwm = f0sc/(256*prescaler) = 16MHz/(256*1024) = 61Hz
	OCR0A = 0 ; //Start PWM 0C0A - duty = 0/256 = 0%
	/////////////////////////////////////////////////////////////////////////////////////////
	//Configuração da USART
	UBRR0H = (unsigned char)(MYUBRR>>8);	//Ajusta a taxa de transmissão, parte alta
	UBRR0L = (unsigned char)MYUBRR;		//Ajusta a taxa de transmissão, parte baixa
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0); //Habilita a interrup. do receptor, Habilita o transmissor e o receptor
	UCSR0C = (3<<UCSZ00); //Ajusta o formato do frame: 8 bits de dados e 1 de parada, paridade none
	/////////////////////////////////////////////////////////////////////////////////////////
	runTime = 0 ; 
	tempo_ms = 0 ;		//Conta quanto tempo em ms o programa está rodando
	countGreen = 1;				//Inicializar o display e função de timer com 1s
	countYellow = 1;
	countRed = 1;
	colorTime = 0;
	lightCounter = 0;
	greenTimer = 1000 ;
	redTimer = 1000 ;
	yellowTimer = 4000 ;
	State = GREEN ;
	lightState = GREEN_STATE ;
	greenNumber = '1' ;
	yellowNumber = '1' ;
	redNumber = '1' ;
	carCount = 0 ;
	carFreq = 0 ;
	carFreqAux = 1 ;
	actualMode = MANUAL ;
	nextMode = AUTO ;
	/////////////////////////////////////////////////////////////////////////////////////////
	nokia_lcd_init();
	reloadDisplay();
}


void buttonGreen(){
	if ( PINB&(1<<PINB2) &&  countGreen < 9 && actualMode==MANUAL) {
		countGreen = countGreen + 1 ;
	}										//Função para verificar quando algum botão é pressionado
	if ( PINB&(1<<PINB3) && countGreen > 1 && actualMode==MANUAL) {
		countGreen = countGreen - 1 ;
	}
}

void buttonYellow(){
	if ( PINB&(1<<PINB2) &&  countYellow < 9 && actualMode==MANUAL) {
		countYellow = countYellow + 1 ;
		yellowTimer = 4000*(countYellow) ;
	}										//Função para verificar quando algum botão é pressionado
	if ( PINB&(1<<PINB3) && countYellow > 1 && actualMode==MANUAL) {
		countYellow = countYellow - 1 ;
		yellowTimer = 4000*(countYellow) ;
	}
}

void buttonRed(){
	if ( PINB&(1<<PINB2) &&  countRed < 9 && actualMode==MANUAL) {
		countRed = countRed + 1 ;
	}										//Função para verificar quando algum botão é pressionado
	if ( PINB&(1<<PINB3) && countRed > 1 && actualMode==MANUAL) {
		countRed = countRed - 1 ;
	}
}

void buttonMode(){
	if ( PINB&(1<<PINB2) || PINB&(1<<PINB3)) {
		actualMode = nextMode ;
		reloadDisplay();
	}
}

void reloadDisplay(){
	nokia_lcd_clear();
	nokia_lcd_write_string("MODO:",1);
	nokia_lcd_set_cursor(0, 10);
	nokia_lcd_write_string("========", 1);
	nokia_lcd_set_cursor(0, 20);
	nokia_lcd_write_string("T.G",1);
	nokia_lcd_set_cursor(0, 30);
	nokia_lcd_write_string("T.Y", 1);
	nokia_lcd_set_cursor(0, 40);
	nokia_lcd_write_string("T.R",1);
	nokia_lcd_set_cursor(50, 0);
	nokia_lcd_write_string(" Luz",1);
	checkMode();
	reloadTextsGreen();
	reloadTextsRed();
	printLine();
	nokia_lcd_set_cursor(50, 30);
	nokia_lcd_write_string("C/Min",1);
	nokia_lcd_set_cursor(50, 40);
	nokia_lcd_write_string("=",1);
	reloadTextsMode();
	reloadTextsCarFreq();
	reloadTextsYellow();
	reloadTextsLux();
	selectConfig();
	nokia_lcd_render();	
}

void reloadTextsGreen(){//G:45-20 Y:50-30  R:35-40	
	nokia_lcd_set_cursor(20, 20);
	if(actualMode == MANUAL){
		sprintf(greenNumber,"%d",manualGreen);
	}
	if(actualMode == AUTO){
		sprintf(greenNumber,"%d",autoGreen);
	}
	nokia_lcd_write_string(greenNumber,1);
	nokia_lcd_set_cursor(25,20);
	nokia_lcd_write_string("s",1);		
}

void reloadTextsYellow(){	
	nokia_lcd_set_cursor(20,30);
	sprintf(yellowNumber,"%d",countYellow);
	nokia_lcd_write_string(yellowNumber,1);
	nokia_lcd_set_cursor(25,30);
	nokia_lcd_write_string("s",1);
}
	
void reloadTextsRed(){	
	nokia_lcd_set_cursor(20,40);
	if(actualMode == MANUAL)
		sprintf(redNumber,"%d",manualRed);
	if(actualMode == AUTO)
		sprintf(redNumber,"%d",autoRed);
	nokia_lcd_write_string(redNumber,1);
	nokia_lcd_set_cursor(25,40);
	nokia_lcd_write_string("s",1);
	
}

void reloadTextsMode(){ //Mostrar no display o modo de operação atual
	nokia_lcd_set_cursor(35,0);
	if(actualMode == AUTO){
		nokia_lcd_write_string("A",1);
	}
	if(actualMode == MANUAL){
		nokia_lcd_write_string("M",1);
	}
}

void reloadTextsLux(){ //Mostrar no display o valor de lux
	nokia_lcd_set_cursor(50, 10);
	//////////////////////////////////////////////////////////////
	//sprintf(luxWrite,"%f",(844264600 + (14.21979 - 844264600))/(1 + powf((luxRead/6060036),1.574674)));				// Ver o valor lido 
	// Por algum motivo dando erro; Semana de prova ta osso
	//nokia_lcd_write_string(luxWrite,1);			//
	//////////////////////////////////////////////////////////////
	if (luxRead>472){ //Caso em q Lux do ldr > 300
		//Desliga luz
		nokia_lcd_write_string("OFF",1);
	}
	else if (luxRead<473&& (PINB&(1<<PINB7)|| carFreq != 0)){ // caso lux do ldr <300 e pessoas ou carros
		//Acende luz 100%
		nokia_lcd_write_string("ON",1);
	}
	else if (luxRead<473&& (!PINB&(1<<PINB7)|| carFreq == 0)){ // caso lux do ldr <300 sem pessoas ou carros
		//Acende luz 30%
		nokia_lcd_write_string("ON",1);
	}
	//////////////////////////////////////////////////////////////
}

void reloadTextsCarFreq(){
	sprintf(freq,"%d",carFreq);
	nokia_lcd_set_cursor(56,40);
	nokia_lcd_write_string(freq,1);
}

void printLine(){
	nokia_lcd_set_cursor(45,0);
	nokia_lcd_write_string("|",1);
	nokia_lcd_set_cursor(45,10);
	nokia_lcd_write_string("|",1);
	nokia_lcd_set_cursor(45,20);
	nokia_lcd_write_string("|",1);
	nokia_lcd_set_cursor(45,30);
	nokia_lcd_write_string("|",1);
	nokia_lcd_set_cursor(45,40);
	nokia_lcd_write_string("|",1);
	nokia_lcd_set_cursor(45,50);
	nokia_lcd_write_string("|",1);
}

void selectConfig(){ //Função para setar o indicador de estado no display e indicar qual o próximo estado
	switch(State){
		case GREEN:
			nokia_lcd_set_cursor(40,20);
			nokia_lcd_write_string("<",1);
			nextState = YELLOW;
			break;
		
		case YELLOW:
			nokia_lcd_set_cursor(40,30);
			nokia_lcd_write_string("<",1);
			nextState = RED;
			break;
		
		case RED:
			nokia_lcd_set_cursor(40,40);
			nokia_lcd_write_string("<",1);
			nextState = MODE_SELECT;
			break;
		case MODE_SELECT:
			nokia_lcd_set_cursor(40,0);
			nokia_lcd_write_string("<",1);	
			nextState = GREEN;
			break;
	}
	switch(actualMode){
		case AUTO:
			nextMode = MANUAL;
			break;
						//Alterna o proximo estado do modo de operação dependedo do atual
		case MANUAL:
			nextMode = AUTO;
			break; 
	}
}

void checkMode(){
	if(actualMode == MANUAL){//Configura as variaveis para o modo manual
		manualGreen = countGreen ;
		manualRed = countRed ;
		greenTimer = 1000*manualGreen ;
		redTimer = 1000*manualRed ;
	}
	if(actualMode == AUTO){//Configura as variaveis para o modo automatico
		autoGreen = (carFreq/60)+1;
		if(autoGreen>9){
			autoGreen = 9;
		}
		autoRed = 10-autoGreen;
		if(autoRed<1){
			autoRed = 1 ;
		}
		greenTimer = 1000*autoGreen ;
		redTimer = 1000*autoRed ;
	}
}

void USART_clock(){
	PORTB |= 0b00000010 ;		//Subida Clock
	PORTB &= 0b11111101 ;		//Descida Clock
	while(!( UCSR0A & (1<<UDRE0)));//Espera a limpeza do registr. de transmissão
	UDR0 = 1; //Coloca o dado no registrador e o envia
	while(!( UCSR0A & (1<<UDRE0)));//Espera a limpeza do registr. de transmissão
	UDR0 = 0; //Coloca o dado no registrador e o envia
}

ISR(INT0_vect){
	//Por algum motivo as interrupções no INT0 causam um "ruído" na frequencia estimada de carros
	if(actualMode == MANUAL){//Caso o modo esteja no automativo desabilita a escolha de alterar valores
		if(PINB &(1<<PINB4)){
			State = nextState;
			reloadDisplay();
		}
		if(State==GREEN){
			buttonGreen();	
		}
		if(State == YELLOW){
			buttonYellow();
		}
		if(State == RED){
			buttonRed();
		}
	}
	if(State == MODE_SELECT){//Independente do modo poderá alternar entre automatico e manual
		buttonMode();
	}
	reloadDisplay();
}

ISR(ADC_vect){
	luxRead = ADC;
}

ISR(INT1_vect)
{
	if((PIND &(1<<PIND3)) == 0 ){//Para qualquer variação
		carCount = carCount + 1 ;//variar para 0 e contar = borda de descida
	}
}

ISR(TIMER1_COMPA_vect) {
	lightCounter += 1 ;
	runTime += 1 ;
	if(lightCounter >= greenTimer && lightState == GREEN_STATE){
		USART_clock();
		lightCounter = 0;
		colorTime += 1 ;
		if(colorTime == 4){
			lightState = YELLOW_STATE;
			colorTime = 0 ;
		} 	
	}
	if(lightCounter >= yellowTimer && lightState == YELLOW_STATE){
		USART_clock();
		lightCounter = 0;
		lightState = RED_STATE;
	}	
	if(lightCounter >= redTimer && lightState == RED_STATE){
		USART_clock();
		lightCounter = 0;
		colorTime += 1 ;
		if(colorTime == 4){
			lightState = GREEN_STATE;
			colorTime = 0 ;
		}
	}
	if(runTime == 4){
		tempo_ms += 1 ;//Contagem de tempo em ms
		runTime = 0 ;
	}
}