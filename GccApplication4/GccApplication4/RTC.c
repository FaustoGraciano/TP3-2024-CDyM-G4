#include "RTC.h"
#include <avr/io.h>
#include <util/twi.h>
#include <stdio.h>

#define DS3231_ADDRESS 0x68 // Dirección I2C del DS3231

// Prototipos de funciones internas
static uint8_t bcdToDec(uint8_t bcd);
static uint8_t decToBcd(uint8_t dec);
static void I2C_Init(void);
static void I2C_Start(void);
static void I2C_Stop(void);
static void I2C_Write(uint8_t data);
static uint8_t I2C_ReadAck(void);
static uint8_t I2C_ReadNack(void);

void RTC_Init(void) {
	I2C_Init();
}

void RTC_GetDateTime(char* dateStr, char* timeStr) {
	I2C_Start();
	I2C_Write((DS3231_ADDRESS << 1) | TW_WRITE);
	I2C_Write(0x00); // Dirección de registro de segundos
	I2C_Stop();

	I2C_Start();
	I2C_Write((DS3231_ADDRESS << 1) | TW_READ);
	uint8_t seconds = bcdToDec(I2C_ReadAck());
	uint8_t minutes = bcdToDec(I2C_ReadAck());
	uint8_t hours = bcdToDec(I2C_ReadAck());
	I2C_ReadAck(); // Día de la semana (ignorado)
	uint8_t day = bcdToDec(I2C_ReadAck());
	uint8_t month = bcdToDec(I2C_ReadAck());
	uint8_t year = bcdToDec(I2C_ReadNack());
	I2C_Stop();

	sprintf(dateStr, "%02d/%02d/%02d", day, month, year);
	sprintf(timeStr, "%02d:%02d:%02d", hours, minutes, seconds);
}

static uint8_t bcdToDec(uint8_t bcd) {
	return (bcd / 16 * 10) + (bcd % 16);
}

static uint8_t decToBcd(uint8_t dec) {
	return (dec / 10 * 16) + (dec % 10);
}

static void I2C_Init(void) {
	TWSR = 0x00; // Configurar prescaler a 1
	TWBR = 0x48; // Configurar SCL a 100kHz para F_CPU = 16MHz
	TWCR = 0x04; // Habilitar TWI
}

static void I2C_Start(void) {
	TWCR = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

static void I2C_Stop(void) {
	TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
	while (TWCR & (1 << TWSTO)); //dudoso
}

static void I2C_Write(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

static uint8_t I2C_ReadAck(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

static uint8_t I2C_ReadNack(void) {
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}