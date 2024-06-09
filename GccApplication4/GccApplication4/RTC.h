#ifndef RTC_H
#define RTC_H

#include <stdint.h>

void RTC_Init(void);
void RTC_GetTime(char* timeStr);
void RTC_GetDate(char* dateStr);
void RTC_GetDateTime(char* dateStr, char* timeStr);
#endif