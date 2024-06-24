# TP3-2024-CDyM-G4 Graciano Zanetti
Proyecto de Sensor de Temperatura y Humedad con DHT11 y RTC

# Descripción
Este proyecto implementa un sistema de medición de temperatura y humedad utilizando el sensor DHT11 y un reloj en tiempo real (RTC) para registrar la fecha y la hora de las mediciones. La comunicación entre el microcontrolador y otros dispositivos se realiza a través de UART. El sistema está diseñado para funcionar con un microcontrolador ATmega328P y el entorno de desarrollo AVR.

# Componentes
-Microcontrolador: ATmega328P
-Sensor de temperatura y humedad: DHT11
-Reloj en tiempo real (RTC): DS3232
-Comunicación serial (UART): Para transmisión de datos a un PC

# Archivos del Proyecto
-main.c: Código principal del sistema.
-DHT11.c y DHT11.h: Implementación y definición de funciones para el sensor DHT11.
-serialPort.c y serialPort.h: Configuración e implementación de funciones para la comunicación UART.
-RTC.c y RTC.h: Implementación y definición de funciones para el módulo RTC.

# Funcionalidades

-Lectura de Datos del DHT11:
Inicialización del sensor.
Lectura de temperatura y humedad.
Manejo de errores (timeout y checksum).

-Comunicación UART:
Inicialización del puerto serie.
Transmisión y recepción de datos.
Envío de datos de temperatura y humedad a un PC.

-Manejo del RTC:
Inicialización del módulo RTC.
Lectura de fecha y hora actual.
Envío de fecha y hora a un PC.

-Temporización:
Configuración del Timer1 para generar interrupciones cada 1ms.
Uso de banderas para sincronización y temporización de eventos.

# Notas
-Asegúrese de que todos los componentes estén correctamente conectados y alimentados.
-Verifique que el baud rate del monitor serial coincida con el configurado en el código (9600 bps).
