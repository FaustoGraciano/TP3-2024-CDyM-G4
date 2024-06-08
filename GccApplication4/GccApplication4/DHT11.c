#include "DHT11.h"
#include "Timer.h"
#include <stdint.h>
#define F_CPU 16000000UL
#include <util/delay.h>


static uint8_t humedad;
static uint8_t temperatura;

int LeerDHT(uint8_t pin) {
 // Recepción de datos
	uint8_t data[5] = {0};
	uint8_t timeout = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 7; j >= 0; j--) {
			while (pin == 0) {
				// Esperar hasta que el pin se vuelva alto
				if (++timeout > 60) {
					return DHTLIB_ERROR_TIMEOUT; // Error de timeout
				}

			}
			_delay_us(30);
			if (pin==1){
				data[i] |= (1 << j); // Si la duración es mayor a 40us, el bit es un 1
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