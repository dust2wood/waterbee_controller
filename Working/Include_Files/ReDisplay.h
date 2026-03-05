#ifndef PICTURE_H
#define PICTURE_H
#include "Main.h"


extern const char* comModeText[3];
extern const char* comDatabitText[2];
extern const char* comBaudrateText[4];
extern int32_t tempIntVal;
extern int32_t zerointVal;
extern int32_t offsetintVal;

void RedrawTitle(void);
void RedrawValue(void);
void RedrawViewArea(void);
void RedrawBottomArea(void);
void RedrawMainView(void);
void RedrawMainValue(void);
#endif










