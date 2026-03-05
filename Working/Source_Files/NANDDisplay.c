#include "NANDDisplay.h"


#define TITLE_Y_START	0
#define TITLE_Y_END		40

#define VIEW_Y_START	50
#define VIEW_Y_END		210

#define BOTTOM_Y_START	220
#define BOTTOM_Y_END	255


//uint8_t imageBuffer[43200] = { 0 , };
//uint8_t imageBuffer[48000] = {0,};
uint8_t imageBuffer[40960] = {0,};

uint16_t* GlobalMemory;


void DrawIcon(uint32_t icon, uint32_t draw) {
    //	const uint16_t axis[4][3] = {{1,1,0},{390,1,1600},{435,1,3200},{70,110,4800}};
    const uint16_t axis[6][3] = {
        {1, 4, 4680},		// CI
        {405, 4, 1600},		// BELL
        {440, 4, 2500},		// SDCARD
        {403, 55, 3400},	// CAUTION
        {1, 4, 4680},		// WORKING
        {185, 55, 3400}};	// CAUTION1

    if (icon > ICON_CAUTION1)
        return;

    if (draw == DRAW_IMAGE_ENABLE) {
        NAND_ReadICON();
        //		TFT_DrawImage(axis[icon][0], axis[icon][1], axis[icon][0]+40, axis[icon][1]+40, &GlobalMemory[axis[icon][2]], DRAW_REVERSE);
        switch (icon) {
            case ICON_CI:
                TFT_DrawImage(axis[icon][0], axis[icon][1], axis[icon][0] + 35, axis[icon][1] + 30, &GlobalMemory[axis[icon][2]], DRAW_NORMAL);
                break;
            case ICON_BELL:
                TFT_DrawImage(axis[icon][0], axis[icon][1], axis[icon][0] + 30, axis[icon][1] + 30, &GlobalMemory[axis[icon][2]], DRAW_NORMAL);
                break;
            case ICON_SDCARD:
                TFT_DrawImage(axis[icon][0], axis[icon][1], axis[icon][0] + 30, axis[icon][1] + 30, &GlobalMemory[axis[icon][2]], DRAW_NORMAL);
                break;
            case ICON_CAUTION:
                TFT_DrawImage(axis[icon][0], axis[icon][1], axis[icon][0] + 40, axis[icon][1] + 32, &GlobalMemory[axis[icon][2]], DRAW_NORMAL);
                break;
            case ICON_WORKING:
                TFT_DrawImage(axis[icon][0], axis[icon][1], axis[icon][0] + 35, axis[icon][1] + 30, &GlobalMemory[axis[icon][2]], DRAW_NORMAL);
                break;
            case ICON_CAUTION1:
                TFT_DrawImage(axis[icon][0], axis[icon][1], axis[icon][0] + 40, axis[icon][1] + 32, &GlobalMemory[axis[icon][2]], DRAW_NORMAL);
                break;
        };

    } else {
        //		TFT_Fill(axis[icon][0], axis[icon][1], axis[icon][0]+40, axis[icon][1]+32, WHITE);

        switch (icon) {
            case ICON_CI:
                TFT_Fill(axis[icon][0], axis[icon][1], axis[icon][0] + 35, axis[icon][1] + 30, WHITE);
                break;
            case ICON_BELL:
                TFT_Fill(axis[icon][0], axis[icon][1], axis[icon][0] + 30, axis[icon][1] + 30, WHITE);
                break;
            case ICON_SDCARD:
                TFT_Fill(axis[icon][0], axis[icon][1], axis[icon][0] + 30, axis[icon][1] + 30, WHITE);
                break;
            case ICON_CAUTION:
                TFT_Fill(axis[icon][0], axis[icon][1], axis[icon][0] + 40, axis[icon][1] + 32, WHITE);
                break;
            case ICON_WORKING:
                TFT_Fill(axis[icon][0], axis[icon][1], axis[icon][0] + 35, axis[icon][1] + 30, WHITE);
                break;
            case ICON_CAUTION1:
                TFT_Fill(axis[icon][0], axis[icon][1], axis[icon][0] + 40, axis[icon][1] + 32, WHITE);
                break;
        };
    }
}


//================================================


void DrawSmallNumber0(uint32_t x, uint32_t y, char* str, uint16_t color) {
    uint32_t i = 0;
    NAND_ReadSNumber();
    while (str[i] != 0) {
        if (str[i] > 0x2F && str[i] < 0x3A)
            TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[(str[i] - 0x30) * 15 * 17 * 2]), color);
        else if (str[i] == ' ')
            TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[13 * 15 * 17 * 2]), color);
        else if (str[i] == '-')
            TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[10 * 15 * 17 * 2]), color);
        else if (str[i] == ':')
            TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[11 * 15 * 17 * 2]), color);
        else if (str[i] == '.')
            TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[12 * 15 * 17 * 2]), color);
        else if (str[i] == 'C')
            TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[14 * 15 * 17 * 2]), color); // 온도 표시 추가하자.
        else
            return;
        x += 14; // 15
        ++i;
    }
}

void DrawMediumNumber0(uint32_t x, uint32_t y, char* str, uint16_t color) {
    uint32_t i = 0;
    uint16_t* buffer;

    if (color == YELLOW) {
        NAND_ReadMNumberY();
        buffer = (uint16_t*) YellowMinus;
    } else {
        NAND_ReadMNumberW();
        buffer = (uint16_t*) WhiteMinus;
    }
    while (str[i] != 0) {
        if (str[i] > 0x2F && str[i] < 0x3A)
            TFT_DrawImage(x, y, x + 22, y + 26, (uint16_t*)&(imageBuffer[(str[i] - 0x30) * 22 * 26 * 2]), DRAW_REVERSE);
        else if (str[i] == '.')
            TFT_DrawImage(x, y, x + 22, y + 26, (uint16_t*)&(imageBuffer[10 * 22 * 26 * 2]), DRAW_REVERSE);
        else if (str[i] == ' ')
            TFT_DrawImage(x, y, x + 22, y + 26, (uint16_t*)&(imageBuffer[11 * 22 * 26 * 2]), DRAW_REVERSE);
        else if (str[i] == '-')
            TFT_DrawImage(x, y, x + 22, y + 26, (uint16_t*) (buffer), DRAW_REVERSE);
        else
            return;
        x += 22;
        ++i;
    }
}

void DrawLargeNumber0(uint32_t x, uint32_t y, char* str) {
    uint32_t i = 0;
    NAND_ReadLNumber();
    while (str[i] != 0) {
        if (str[i] > 0x2F && str[i] < 0x3A)
            TFT_DrawImage(x, y, x + 36, y + 47, (uint16_t*)&(imageBuffer[(str[i] - 0x30) * 36 * 47 * 2]), DRAW_REVERSE);
        else if (str[i] == '.')
            TFT_DrawImage(x, y, x + 36, y + 47, (uint16_t*)&(imageBuffer[10 * 36 * 47 * 2]), DRAW_REVERSE);
        else if (str[i] == ' ')
            TFT_DrawImage(x, y, x + 36, y + 47, (uint16_t*)&(imageBuffer[11 * 36 * 47 * 2]), DRAW_REVERSE);
        else
            return;
        x += 36;
        ++i;
    }
}

//==========================================


void DrawLineRectangle(uint32_t x, uint32_t y, uint32_t xEnd, uint32_t yEnd, uint16_t color) {
    if (yEnd < 2 || xEnd < 2)
        return;

    TFT_Fill(x, y, xEnd, y + 1, color);
    TFT_Fill(x, y, x + 1, yEnd, color);
    TFT_Fill(xEnd - 1, y, xEnd, yEnd, color);
    TFT_Fill(x, yEnd - 1, xEnd, yEnd, color);
}

// 아래쪽 세로 작은 두줄 

void DrawBottomLine(void) {
    TFT_Fill(169, 235, 171, 272, BROWN);
    TFT_Fill(309, 235, 311, 272, BROWN);
}


//=====================================
#include "back_corner.h"
#include "back_corner2.h"


// TITLE 위 전체 

void ClearTitleArea(void) {
    TFT_Fill(0, 0, 480, TITLE_Y_END, WHITE);
}

// TITLE 중 가운데 

void ClearTitle(void) {
    //	TFT_Fill(110, 0, 290, TITLE_Y_END, WHITE);
    TFT_Fill(90, 0, 330, TITLE_Y_END, WHITE);
}


// VIEW  가운데


// 기본 모양
// 위부분에 흰색
// 아래 부분에 메뉴있음 
#ifdef CH2

void ClearViewArea(void) {
    TFT_Fill(0, VIEW_Y_START, 480, VIEW_Y_END, WHITE);

    TFT_Fill(0, TITLE_Y_END, 20, 272, BACK_COLOR);
    TFT_Fill(0, TITLE_Y_END, 480, 50, BACK_COLOR);
    TFT_Fill(460, TITLE_Y_END, 480, 272, BACK_COLOR);
    TFT_Fill(0, 208, 480, 272, BACK_COLOR);


    TFT_Fill(223+17, TITLE_Y_END, 253, 272, BACK_COLOR);

    TFT_DrawImage(19,  50,  19 + 17,  50 + 17, (uint16_t *) IMG_back_corner, DRAW_NORMAL);
    TFT_DrawImage(223, 50, 223 + 17,  50 + 17, (uint16_t *) IMG_back_corner, DRAW_RIGHT_TOP);
    TFT_DrawImage(19, 191,  19 + 17, 191 + 17, (uint16_t *) IMG_back_corner, DRAW_LEFT_BOTTOM);
    TFT_DrawImage(223, 191,223 + 17, 191 + 17, (uint16_t *) IMG_back_corner, DRAW_RIGHT_BOTTOM);

    TFT_DrawImage(253,  50, 253 + 17,  50 + 17, (uint16_t *) IMG_back_corner, DRAW_NORMAL);
    TFT_DrawImage(253, 191, 253 + 17, 191 + 17, (uint16_t *) IMG_back_corner, DRAW_LEFT_BOTTOM);

    TFT_DrawImage(443, 50,  443 + 17,  50 + 17, (uint16_t *) IMG_back_corner, DRAW_RIGHT_TOP);
    TFT_DrawImage(443, 191, 443 + 17, 191 + 17, (uint16_t *) IMG_back_corner, DRAW_RIGHT_BOTTOM);
}

#else

void ClearViewArea(void) {
    TFT_Fill(0, VIEW_Y_START, 480, VIEW_Y_END, WHITE);

    TFT_Fill(0, TITLE_Y_END, 20, 272, BACK_COLOR);
    TFT_Fill(0, TITLE_Y_END, 480, 50, BACK_COLOR);
    TFT_Fill(460, TITLE_Y_END, 480, 272, BACK_COLOR);
    TFT_Fill(0, 208, 480, 272, BACK_COLOR);

    TFT_DrawImage(19, 50, 19 + 17, 50 + 17, (uint16_t *) IMG_back_corner, DRAW_NORMAL);
    TFT_DrawImage(443, 50, 443 + 17, 50 + 17, (uint16_t *) IMG_back_corner, DRAW_RIGHT_TOP);
    TFT_DrawImage(19, 191, 19 + 17, 191 + 17, (uint16_t *) IMG_back_corner, DRAW_LEFT_BOTTOM);
    TFT_DrawImage(443, 191, 443 + 17, 191 + 17, (uint16_t *) IMG_back_corner, DRAW_RIGHT_BOTTOM);
}

#endif


// BOTTOM  아래쪽

void ClearBottomArea(void) {
    TFT_Fill(0, BOTTOM_Y_START, 480, 272, BACK_COLOR);
}

//=======================================

void DrawBack1(void) {
    TFT_Fill(0, 0, 480, 272, WHITE);

    ClearViewArea();
}

//=======================================

// 위부분에 배경있음 - 제목
// 아래 부분 전체 포함
void ClearViewArea2(void) {
    TFT_Fill(0, VIEW_Y_START, 480, 272, WHITE);
    TFT_Fill(0, VIEW_Y_START, 480, 117, BACK_COLOR2);

    TFT_Fill(0, TITLE_Y_END, 20, 272, BACK_COLOR);
    TFT_Fill(0, TITLE_Y_END, 480, 50, BACK_COLOR);
    TFT_Fill(460, TITLE_Y_END, 480, 272, BACK_COLOR);
    TFT_Fill(0, 257, 480, 272, BACK_COLOR);

    TFT_DrawImage(19, 50, 19 + 17, 50 + 17, (uint16_t *) IMG_back_corner2, DRAW_NORMAL);
    TFT_DrawImage(443, 50, 443 + 17, 50 + 17, (uint16_t *) IMG_back_corner2, DRAW_RIGHT_TOP);
    TFT_DrawImage(19, 240, 19 + 17, 240 + 17, (uint16_t *) IMG_back_corner, DRAW_LEFT_BOTTOM);
    TFT_DrawImage(443, 240, 443 + 17, 240 + 17, (uint16_t *) IMG_back_corner, DRAW_RIGHT_BOTTOM);
}


// 위부분에 배경있음 - 제목
// 아래부분에 메뉴있음
void ClearViewArea3(void) {
    TFT_Fill(0, VIEW_Y_START, 480, 272, WHITE);
    TFT_Fill(0, VIEW_Y_START, 480, 117, BACK_COLOR2);

    TFT_Fill(0, TITLE_Y_END, 20, 272, BACK_COLOR);
    TFT_Fill(0, TITLE_Y_END, 480, 50, BACK_COLOR);
    TFT_Fill(460, TITLE_Y_END, 480, 272, BACK_COLOR);
    TFT_Fill(0, 208, 480, 272, BACK_COLOR);

    TFT_DrawImage(19, 50, 19 + 17, 50 + 17, (uint16_t *) IMG_back_corner2, DRAW_NORMAL);
    TFT_DrawImage(443, 50, 443 + 17, 50 + 17, (uint16_t *) IMG_back_corner2, DRAW_RIGHT_TOP);
    TFT_DrawImage(19, 191, 19 + 17, 191 + 17, (uint16_t *) IMG_back_corner, DRAW_LEFT_BOTTOM);
    TFT_DrawImage(443, 191, 443 + 17, 191 + 17, (uint16_t *) IMG_back_corner, DRAW_RIGHT_BOTTOM);
}


// 교정이력에 사용함
// 위부분 배경없음
// 아래부분 전체 포함
void ClearViewArea4(void) {
    TFT_Fill(0, VIEW_Y_START, 480, 272, WHITE);
    TFT_Fill(0, VIEW_Y_START, 480, 117, WHITE);

    TFT_Fill(0, TITLE_Y_END, 20, 272, BACK_COLOR);
    TFT_Fill(0, TITLE_Y_END, 480, 50, BACK_COLOR);
    TFT_Fill(460, TITLE_Y_END, 480, 272, BACK_COLOR);
    TFT_Fill(0, 257, 480, 272, BACK_COLOR);

    TFT_DrawImage(19, 50, 19 + 17, 50 + 17, (uint16_t *) IMG_back_corner, DRAW_NORMAL);
    TFT_DrawImage(443, 50, 443 + 17, 50 + 17, (uint16_t *) IMG_back_corner, DRAW_RIGHT_TOP);
    TFT_DrawImage(19, 240, 19 + 17, 240 + 17, (uint16_t *) IMG_back_corner, DRAW_LEFT_BOTTOM);
    TFT_DrawImage(443, 240, 443 + 17, 240 + 17, (uint16_t *) IMG_back_corner, DRAW_RIGHT_BOTTOM);
}


// 진짜 중간부분 클리어...
void ClearViewArea5(void) {
    TFT_Fill(0, VIEW_Y_START, 480, VIEW_Y_END-2, WHITE);

    TFT_Fill(0, TITLE_Y_END, 20, 272, BACK_COLOR);
    TFT_Fill(0, TITLE_Y_END, 480, 50, BACK_COLOR);
    TFT_Fill(460, TITLE_Y_END, 480, 272, BACK_COLOR);

//    TFT_Fill(0, 208, 480, 272, BACK_COLOR);


    TFT_Fill(223+17, TITLE_Y_END, 253, VIEW_Y_END-2, BACK_COLOR);

    TFT_DrawImage(19,  50,  19 + 17,  50 + 17, (uint16_t *) IMG_back_corner, DRAW_NORMAL);
    TFT_DrawImage(223, 50, 223 + 17,  50 + 17, (uint16_t *) IMG_back_corner, DRAW_RIGHT_TOP);
    TFT_DrawImage(19, 191,  19 + 17, 191 + 17, (uint16_t *) IMG_back_corner, DRAW_LEFT_BOTTOM);
    TFT_DrawImage(223, 191,223 + 17, 191 + 17, (uint16_t *) IMG_back_corner, DRAW_RIGHT_BOTTOM);

    TFT_DrawImage(253,  50, 253 + 17,  50 + 17, (uint16_t *) IMG_back_corner, DRAW_NORMAL);
    TFT_DrawImage(253, 191, 253 + 17, 191 + 17, (uint16_t *) IMG_back_corner, DRAW_LEFT_BOTTOM);

    TFT_DrawImage(443, 50,  443 + 17,  50 + 17, (uint16_t *) IMG_back_corner, DRAW_RIGHT_TOP);
    TFT_DrawImage(443, 191, 443 + 17, 191 + 17, (uint16_t *) IMG_back_corner, DRAW_RIGHT_BOTTOM);
}


void DrawBack2(void) {
    TFT_Fill(0, 0, 480, 272, WHITE);

    ClearViewArea2();
}


