#ifndef INTERRUPT_H
#define INTERRUPT_H
#include "Main.h"

#define FLAG10MS_BUTTON				  0x00000010
#define FLAG10MS_RX1						0x00000100
#define FLAG10MS_RX2						0x00000040
#define FLAG10MS_RX3						0x00000080
#define FLAG10MS_RS485					0x00000100

extern uint32_t flag10ms;
extern uint16_t rx1Buffer[50];
extern uint16_t rx2Buffer[256];
extern uint16_t rx3Buffer[256];
extern uint16_t tx1Buffer[15];
extern uint16_t tx2Buffer[256];
extern uint16_t tx3Buffer[256];
extern uint32_t tx1Size, tx1Count, rx1Size;
extern uint32_t tx2Size, tx2Count, rx2Size;
extern uint32_t tx3Size, tx3Count, rx3Size;
extern uint32_t com485State;


#endif

