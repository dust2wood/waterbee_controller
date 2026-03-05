#ifndef CurrentSorceTrans_H
#define CurrentSorceTrans_H

#include "Main.h"

//extern uint32_t finaladj_4mA;
//extern uint32_t finaladj_20mA;
extern uint16_t trans_anlog_pwmVaule;
extern uint16_t trans_temp_pwmVaule;
void TIM8_Chage_Duty_Channel(uint16_t channel , uint16_t Duty);
void Senosr1_Slop_Cal (uint16_t lowpwmvaule , uint16_t highpwmvaule);
void Rpm_AD_Cal (uint16_t RpmConstant);
void Update_CurrentTrans (void);
void Temperture_Slop_Cal (uint16_t lowpwmvaule , uint16_t highpwmvaule);
void current_conversion_slop_cal(uint16_t lowpwmvaule , uint16_t highpwmvaule);
#endif
