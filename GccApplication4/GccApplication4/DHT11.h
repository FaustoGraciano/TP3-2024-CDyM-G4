#define DHTLIB_OK 1
#define DHTLIB_ERROR_CHECKSUM -1
#define DHTLIB_ERROR_TIMEOUT -2
#include <stdint.h>

int LeerDHT();
	
uint8_t getHumedad();

uint8_t getTemperatura();

