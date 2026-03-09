/**
 * display_update.h - Dynamic UI update based on detected sensors
 */
#ifndef DISPLAY_UPDATE_H
#define DISPLAY_UPDATE_H

void Display_Update(void);
void Display_UpdateIfChanged(void);   /* 갱신: 값 변경 시에만 (전략 C) */
void Display_NotifyScreenCleared(void);  /* 화면 클리어 후 호출 시 다음엔 강제 갱신 */

void ReDisplay_ch1(void);
void ReDisplay_ch2(void);
void ReDisplay_ch4(void);

#endif /* DISPLAY_UPDATE_H */
