
#include "DHT11.h"
#include <stdint.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define TIMEOUT 1000 // Timeout para esperar el cambio de estado del pin

static uint8_t humedad;
static uint8_t temperatura;

int LeerDHT() {
	// Recepción de datos
	uint8_t data[5] = {0};
	uint16_t timeout = 0;

	// Iniciar comunicación con el sensor
	DDRC |= (1 << PINC0);
	PORTC |= (1 << PINC0);
	_delay_ms(1);
	PORTC &= ~(1 << PINC0);
	_delay_ms(20);
	PORTC |= (1 << PINC0);
	DDRC &= ~(1 << PINC0);

	// Esperar hasta que el pin se vuelva bajo
	timeout = 0;
	while((PINC & (1 << PINC0)) && timeout < TIMEOUT) {
		_delay_us(1);
		timeout++;
	}
	if (timeout >= TIMEOUT) return DHTLIB_ERROR_TIMEOUT;

	// Esperar hasta que el pin se vuelva alto
	timeout = 0;
	while(!(PINC & (1 << PINC0)) && timeout < TIMEOUT) {
		_delay_us(1);
		timeout++;
	}
	if (timeout >= TIMEOUT) return DHTLIB_ERROR_TIMEOUT;

	// Esperar hasta que el pin se vuelva bajo
	timeout = 0;
	while((PINC & (1 << PINC0)) && timeout < TIMEOUT) {
		_delay_us(1);
		timeout++;
	}
	if (timeout >= TIMEOUT) return DHTLIB_ERROR_TIMEOUT;

	// Leer los 5 bytes de datos
	for (int i = 0; i < 5; i++) {
		for (int j = 7; j >= 0; j--) {
			// Esperar hasta que el pin se vuelva alto
			while(!(PINC & (1 << PINC0))) {
				_delay_us(1);
			}

			_delay_us(30);
			if(PINC & (1 << PINC0)) {
				data[i] |= (1 << j); // Si la duración es mayor a 30us, el bit es un 1
			}

			// Esperar hasta que el pin se vuelva bajo
			timeout = 0;
			while(PINC & (1 << PINC0)) {
				_delay_us(1);
			}
		}
	}

	// Verificar checksum
	if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
		// Decodificar datos
		humedad = data[0];
		temperatura = data[2];
		return DHTLIB_OK; // Lectura exitosa
		} else {
		return DHTLIB_ERROR_CHECKSUM; // Error de checksum
	}
}

uint8_t getHumedad() {
	return humedad;
}

uint8_t getTemperatura() {
	return temperatura;
}