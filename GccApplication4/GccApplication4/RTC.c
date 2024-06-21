#include "RTC.h"
#include <avr/io.h>
#include <util/twi.h>
#include <stdio.h>

#define DS3232_ADDRESS 0x68 // Dirección I2C del DS3232
#define I2C_WRITE 0
#define I2C_READ 1


// Prototipos de funciones internas
static uint8_t bcdToDec(uint8_t bcd);
static void I2C_Init(void);
static void I2C_Start(void);
static void I2C_Stop(void);
static void I2C_Write(uint8_t data);
static uint8_t I2C_ReadAck(void);
static uint8_t I2C_ReadNack(void);

void RTC_Init(void) {
	I2C_Init();
}
void RTC_SetTime(){
	I2C_Start();
	I2C_Write((DS3232_ADDRESS << 1) | I2C_WRITE);
	I2C_Write(0x00); // Dirección de registro de segundos
	I2C_Write(0x14);
	I2C_Write(0x30);
	I2C_Write(0x15);   
	I2C_Stop();
	
}

void RTC_SetDate(){
	I2C_Start();
	I2C_Write((DS3232_ADDRESS << 1) | I2C_WRITE);
	I2C_Write(0x04); // Dirección de registro de fecha
	I2C_Write(0x10);
	I2C_Write(0x06);
	I2C_Write(0x24);
	I2C_Stop();
	
}

void RTC_GetTime(char* timeStr) {
	// Iniciar lectura desde la dirección 0x00 (segundos)
	I2C_Start();
	I2C_Write((DS3232_ADDRESS << 1) | I2C_WRITE);
	I2C_Write(0x00); // Dirección de registro de segundos
	I2C_Stop();

	// Leer tiempo (segundos, minutos, horas)
	I2C_Start();
	I2C_Write((DS3232_ADDRESS << 1) | I2C_READ);
	uint8_t seconds = bcdToDec(I2C_ReadAck());
	uint8_t minutes = bcdToDec(I2C_ReadAck());
	uint8_t hours = bcdToDec(I2C_ReadNack());
	I2C_Stop();

	// Formatear hora
	sprintf(timeStr, "%02d:%02d:%02d", hours, minutes, seconds);
}

void RTC_GetDate(char* dateStr) {
	// Iniciar lectura desde la dirección 0x04 (día del mes)
	I2C_Start();
	I2C_Write((DS3232_ADDRESS << 1) | I2C_WRITE);
	I2C_Write(0x04); // Dirección de registro de día del mes
	I2C_Stop();

	// Leer fecha (día, mes, año)
	I2C_Start();
	I2C_Write((DS3232_ADDRESS << 1) | I2C_READ);
	uint8_t day = bcdToDec(I2C_ReadAck()); 
	uint8_t month = bcdToDec(I2C_ReadAck());
	uint8_t year = bcdToDec(I2C_ReadNack());
	I2C_Stop();

	// Formatear fecha
	sprintf(dateStr, "%02d/%02d/%02d", day, month, year);
}

static uint8_t bcdToDec(uint8_t bcd) {
	return (bcd / 16 * 10) + (bcd % 16);
}

static void I2C_Init(void) {
	TWSR = 0x00; // Configurar prescaler a 1
	TWBR = 255; // Configurar SCL a kHz para F_CPU = 16MHz
	TWCR = 0x04; // Habilitar TWI
}

static void I2C_Start(void) {
	TWCR = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

static void I2C_Stop(void) {
	TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
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