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

//variables
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
		DDRC|=(1<<PINC0);
		PORTC&=(0<<PINC0);
		//inicializarContadores();
		//while(chequeoFlag1()!=1){
			
		//}
		//cli();
		DDRC&=(0<<PINC0);
		PORTC|=(1<<PINC0);
		_delay_us(40);
		//result = LeerDHT(PINC0);
		//  if (result == DHTLIB_OK) {
			  SerialPort_Send_String(msg1);
			  SerialPort_Send_uint8_t(getHumedad());
			  SerialPort_Send_String(msg2);
			  SerialPort_Send_String(msg3);	
			  SerialPort_Send_uint8_t(getTemperatura());
			  SerialPort_Send_String(msg4);
			   RTC_GetDate(fecha);
			   SerialPort_Send_String(" FECHA: ");
			   SerialPort_Send_String(fecha);
			   RTC_GetTime(hora);
			   SerialPort_Send_String(" HORA: ");
			   SerialPort_Send_String(hora);
		//	  } else {
			  ("Error: ");
		 // }
		  sei();
		//  inicializarContadores();
		//  while(chequeoFlag2()!=1) {
			  
	//	    }
		  // Esperar 2 segundos antes de la siguiente lectura
	  }
	}
	return 0;
		}
