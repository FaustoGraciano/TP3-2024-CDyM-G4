#ifndef RTC_H
#define RTC_H

#include <stdint.h>

// Funciones para inicializar el RTC y obtener la fecha y hora
void RTC_Init(void);
void RTC_GetDateTime(char* dateStr, char* timeStr);

#endif