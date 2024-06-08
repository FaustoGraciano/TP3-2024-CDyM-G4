	//FILE TIMER.C
	#include <stdint.h>
	#include <avr/interrupt.h>
	#include "Timer.h"

	static uint8_t Flag_MEF;
	static uint8_t countMEF;
	static uint8_t count2;
	static uint8_t Flag_tiempo;


	void configurarTimer1()
	{	TCCR1A = 0;
		TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10); // Modo CTC, prescaler de 64
		OCR1A = 249; // Configurar OCR1A para que el comparador coincida cada 1 milisegundo
		TIMSK1 |= (1 << OCIE1A); // Habilitar interrupción de comparador A
		sei(); // Habilitar interrupciónes globales
	}

	void inicializarContadores(){
		count2=0;
		countMEF=0;
	}

	uint8_t chequeoFlag2(){
		if (Flag_tiempo == 1){
			Flag_tiempo=0;
			return 1;
		}
		else return 0;
	
	}


	uint8_t chequeoFlag1(){
		if (Flag_MEF == 1){
			Flag_MEF=0;
			return 1;
		}
		else return 0;
	
	}

	ISR(TIMER1_COMPA_vect) {
		countMEF++;    //Incremento del contador para la Flag
		count2++;
		if(countMEF == 20){		//cada 10 interrupciones se activa el FLAG de chequeo de MEF
			Flag_MEF = 1;
			countMEF = 0;
		}
		if(count2==200){
			Flag_tiempo=1;
			count2=0;
		}
	}

