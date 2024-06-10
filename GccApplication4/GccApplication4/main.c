#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "DHT11.h"
#include "Timer.h"
#define F_CPU 16000000UL
#include <util/delay.h>
#include "serialPort.h"
#include "RTC.h"
#define BR9600 0x67	// 0x67=103 configura BAUDRATE=9600@16MHz

// Declarar las funciones
void configurarTimer1();
void inicializarContadores();
uint8_t chequeoFlag2();
uint8_t chequeoFlag1();

//variables
uint8_t Flag_MEF;
uint8_t countMEF;
uint8_t count2;
uint8_t Flag_tiempo;
char msg1[]=" Humedad: ";
char msg2[]="%  ";
char msg3[]=" Temperatura: ";
char msg4[]="ÕC  ";
char fecha[];
char hora[];
volatile uint8_t suspendFlag = 0; // Bandera de suspensión
int main(void)
{ 
	
	RTC_Init();
	int result=0;
    configurarTimer1();
	SerialPort_Init(BR9600); 
	SerialPort_TX_Enable();		// Inicializo formato 8N1 y BAUDRATE = 9600bps
	SerialPort_RX_Enable();			// Activo el Receptor del Puerto Serie
	SerialPort_RX_Interrupt_Enable();
	sei();								// Activo la mascara global de interrupciones (Bit I del SREG en 1)
    while(1){ 
	if(!suspendFlag) {		
		result = LeerDHT(PINC0);
		  if (result == DHTLIB_OK) {
			  SerialPort_Send_String(msg1);
			  SerialPort_Send_uint8_t(getHumedad());
			  SerialPort_Send_String(msg2);
			  SerialPort_Send_String(msg3);	
			  SerialPort_Send_uint8_t(getTemperatura());
			  SerialPort_Send_String(msg4);
			  cli();
			   RTC_GetDate(fecha);
			   SerialPort_Send_String(" FECHA: ");
			   SerialPort_Send_String(fecha);
			   RTC_GetTime(hora);
			   SerialPort_Send_String(" HORA: ");
			   SerialPort_Send_String(hora);
			  }
		sei();
		//  inicializarContadores();
		  //while(chequeoFlag2()) {
			  _delay_ms(2000);
		    //}
		  // Esperar 2 segundos antes de la siguiente lectura
	  }
	}
	return 0;
		}


// Función para configurar el Timer1
void configurarTimer1() {
	TCCR1A = 0;
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10); // Modo CTC, prescaler de 64
	OCR1A = 249; // Configurar OCR1A para que el comparador coincida cada 1 milisegundo
	TIMSK1 |= (1 << OCIE1A); // Habilitar interrupción de comparador A
	sei(); // Habilitar interrupciones globales
}

// Función para inicializar los contadores
void inicializarContadores() {
	count2 = 0;
	countMEF = 0;
}

// Función para chequear la Flag2
uint8_t chequeoFlag2() {
	if (Flag_tiempo == 1) {
		Flag_tiempo = 0;
		return 1;
		} else {
		return 0;
	}
}

// Función para chequear la Flag1
uint8_t chequeoFlag1() {
	if (Flag_MEF == 1) {
		Flag_MEF = 0;
		return 1;
		} else {
		return 0;
	}
}

// ISR del TIMER1
ISR(TIMER1_COMPA_vect) {
	countMEF++;    // Incremento del contador para la Flag
	count2++;
	if (countMEF == 20) {   // Cada 10 interrupciones se activa el FLAG de chequeo de MEF
		Flag_MEF = 1;
		countMEF = 0;
	}
	if (count2 == 200) {
		Flag_tiempo = 1;
		count2 = 0;
	}
}