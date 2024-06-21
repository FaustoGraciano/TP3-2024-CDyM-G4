#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "DHT11.h"
#define F_CPU 16000000UL  
#include <util/delay.h>
#include "serialPort.h"
#include "RTC.h"
#define BR9600 0x67	// 0x67=103 configura BAUDRATE=9600@16MHz

// Declarar las funciones
void configurarTimer1();
void inicializarContador();
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
	RTC_Init();  //inicializamos el RTC
	//RTC_SetDate(); sirve por si se requiere setear una fecha (no pedido en el trabajo)
	//RTC_SetTime(); sirve por si se requiere setear una tiempo (no pedido en el trabajo)
	int result=0;
   configurarTimer1();
	SerialPort_Init(BR9600);   // Inicializo formato 8N1 y BAUDRATE = 9600bps
	SerialPort_TX_Enable();		// Activo el transmisor del Puerto Serie
	SerialPort_RX_Enable();			// Activo el Receptor del Puerto Serie
	SerialPort_RX_Interrupt_Enable(); // habilitacion de las interrupciones del receptor
	sei();								// habilitacion de interrupciones globales
    while(1){ 
	if(!suspendFlag) {	// si suspendFlag esta en 1, significa que el programa esta frenado.			
		result = LeerDHT();
		  if (result == DHTLIB_OK) {  //si no hubo errores en recibir los datos del DHT11
			  SerialPort_Send_String(msg1);          //"Humedad: "
			  SerialPort_Send_uint8_t(getHumedad()); //se envia el valor obtenido de la humedad del DHT11
			  SerialPort_Send_String(msg2); //"%  "
			  SerialPort_Send_String(msg3);	//" Temperatura: "
			  SerialPort_Send_uint8_t(getTemperatura());//se envia el valor obtenido de la temperatura del DHT11
			  SerialPort_Send_String(msg4); //"C  "
			   cli(); // se inhabilitan las interrupciones 
			   RTC_GetDate(fecha); //se obtiene la fecha del RTC
			   SerialPort_Send_String(" FECHA: ");
			   SerialPort_Send_String(fecha);
			   RTC_GetTime(hora); //se obtiene la hora del RTC
			   SerialPort_Send_String(" HORA: ");
			   SerialPort_Send_String(hora);
			   SerialPort_Send_String(salto); //salto de linea
			  }
			  else {SerialPort_Send_String("Ha ocurrido un error...\r\n");}
		      sei();  // habilitacion de interrupciones globales
			inicializarContador(); //count = 0
		  while(!chequeoFlag()) { 
			   // Esperar 2 segundos antes de la siguiente lectura
		    }
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
void inicializarContador() {
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