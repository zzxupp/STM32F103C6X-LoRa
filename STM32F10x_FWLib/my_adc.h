#ifndef __MY_ADC_H
#define __MY_ADC_H

#include "adc.h"
#include "bgr.h"

extern volatile u32t u32AdcCo;
extern volatile u32t u32AdcBAT;


void AdcPortInit(void);
void AdcInit(void);
void AdcSQRCfg(void);
void Adc_GetCOLibatResult(u16t *CO_DATA, ft *Bat);
void App_ExrefPortInit(void);

#endif
