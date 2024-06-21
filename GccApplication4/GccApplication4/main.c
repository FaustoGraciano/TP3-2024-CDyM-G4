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
uint8_t chequeoFlag();

//variables
uint16_t count;
uint8_t Flag_tiempo;
char msg1[]=" Humedad: ";
char msg2[]="%  ";
char msg3[]=" Temperatura: ";
char msg4[]="C  ";
char fecha[];
char hora[];
char salto[]="\r\n";

volatile uint8_t suspendFlag = 0; // Bandera de suspensión
int main(void)
{ 
	
	RTC_Init();
	//RTC_SetDate();
	//RTC_SetTime();
	int result=0;
   configurarTimer1();
	SerialPort_Init(BR9600); 
	SerialPort_TX_Enable();		// Inicializo formato 8N1 y BAUDRATE = 9600bps
	SerialPort_RX_Enable();			// Activo el Receptor del Puerto Serie
	SerialPort_RX_Interrupt_Enable();
	sei();								// Activo la mascara global de interrupciones (Bit I del SREG en 1)
    while(1){ 
	if(!suspendFlag) {				
		result = LeerDHT();
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
			   SerialPort_Send_String(salto);
			  }
			  else {SerialPort_Send_String("Se requiere conectar un DHT11\r\n");}
		sei();
			inicializarContadores();
		  while(!chequeoFlag()) {
			  
		    }
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
}

// Función para inicializar los contadores
void inicializarContadores() {
	count = 0;
}

// Función para chequear la Flag2
uint8_t chequeoFlag() {
	if (Flag_tiempo == 1) {
		Flag_tiempo = 0;
		return 1;
		} else {
		return 0;
	}
}


// ISR del TIMER1
ISR(TIMER1_COMPA_vect) {
	count++;    // Incremento del contador para la Flag
	if (count == 2000) {
		Flag_tiempo = 1;
		count = 0;
	}
}