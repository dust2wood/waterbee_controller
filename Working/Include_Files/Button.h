#ifndef BUTTON_H
#define BUTTON_H

#include "Main.h"

#define BUTTON_NONE							0x00000000
#define BUTTON_MENU							0x00000001
#define BUTTON_HOME							0x00000002
#define BUTTON_BACK							0x00000004
#define BUTTON_ENTER							0x00000008
#define BUTTON_RIGHT							0x00000010
#define BUTTON_LEFT								0x00000020
#define BUTTON_UP								0x00000040
#define BUTTON_DOWN							0x00000080

extern uint32_t prevButton;
extern uint32_t ButtonTimer;

uint32_t CheckButton(void);


#endif
