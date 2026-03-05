#ifndef DISPLAY_H
#define DISPLAY_H
#include "Main.h"

#define GRAPH_Y_MAX						85
#define GRAPH_Y_ZERO					220 //173 //269

#define GRAPH_X_START					72
#define GRAPH_X_MAX						432

extern int32_t zerointVal;
extern int32_t offsetintVal;

void DrawGraphText(uint32_t cur);
void DrawGraph(uint32_t cur);
void ClearGraphArea(void);
void DrawCalibLog(uint32_t cur);
void DrawRelay1Wash(uint32_t cur, uint32_t color);
void DrawRelay2Alarm(uint32_t cur, uint32_t color);
void DrawRelay3Wash(uint32_t cur, uint32_t color);
void DrawRelayAlarm(uint32_t val);
void DrawAdjustFilter(uint32_t cur, uint32_t sen, uint32_t color);
void DrawAdjustOffset(uint32_t cur, uint32_t sen, uint32_t color);
void DrawAdjustGradient(uint32_t cur, uint32_t sen, uint32_t color);
//void DrawS2Cycle(void);
void DrawCalibAuto(uint32_t cur, uint32_t color);
void DrawCalibManual(uint32_t cur, uint32_t color);
void DrawCalibZero(uint32_t cur, uint32_t color);
void DrawOutputConfig(uint32_t cur, uint32_t color);
void DrawAlarmConfig(uint32_t cur, uint32_t color);
void DrawTimeConfig(uint32_t cur, uint32_t color);
void DrawCommConfig(uint32_t cur, uint32_t color);
void DrawBottomCurrent(uint32_t current);


//=========================================================



#endif


