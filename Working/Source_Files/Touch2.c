/*
 * Project           : Ground Impedance Tester
 * Author            :  JMC
 * Version          : V1.0
 * Date               : 170425
 */
#include "Main.h"

void CheckTouchInput(void) {
    if (!(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))) {
        touchVal = read_once();
        touchInput = CheckTouchArea(touchVal);
    } else {
        touchInput = BUTTON_NULL;
        if (prevTouchInput == BUTTON_NULL)
            touchKey = 0;
        else {
            touchKey = prevTouchInput;
        }
    }
    prevTouchInput = touchInput;
}

uint16_t CheckTouchArea(TouchPoint touchVal) {
    uint16_t ret = 0;

    switch (state) {
        case STATE_TITLE:
            ///////////////////////////////////////////////////////////////////check Touch Axis
            if (touchVal.Y > 3450 && touchVal.Y < 3750) {
                if (touchVal.X > 2800 && touchVal.X < 3600)
                    buttonState = 1;
                else if (touchVal.X > 1650 && touchVal.X < 2450)
                    buttonState = 2;
                else if (touchVal.X > 400 && touchVal.X < 1200)
                    buttonState = 4;
                else
                    buttonState = 0;
            } else
                buttonState = 0;
            /////////////////////////////////////////////////////////////////Button Re Draw			
            if (prevButtonState != buttonState)//Redraw ButtonImage
            {
                if ((prevButtonState ^ buttonState)&1) {
                    if (buttonState & 1)//button down
                    {
                        NANDButtonDisplay(BUTTON_RESISTOR_DOWN);
                        ret = BUTTON_RESISTOR_DOWN;
                    } else//button up
                    {
                        NANDButtonDisplay(BUTTON_RESISTOR_UP);
                        ret = BUTTON_RESISTOR_UP;
                    }
                }
                if ((prevButtonState ^ buttonState)&2) {
                    if (buttonState & 2)//button down
                    {
                        NANDButtonDisplay(BUTTON_IMPEDANCE_DOWN);
                        ret = BUTTON_IMPEDANCE_DOWN;
                    } else//button up
                    {
                        NANDButtonDisplay(BUTTON_IMPEDANCE_UP);
                        ret = BUTTON_IMPEDANCE_UP;
                    }
                }
                if ((prevButtonState ^ buttonState)&4) {
                    if (buttonState & 4)//button down
                    {
                        NANDButtonDisplay(BUTTON_CONFIG_DOWN);
                        ret = BUTTON_CONFIG_DOWN;
                    } else//button up
                    {
                        NANDButtonDisplay(BUTTON_CONFIG_UP);
                        ret = BUTTON_CONFIG_UP;
                    }
                }
            }
            break;

        case STATE_RESISTOR:
            ////////////////////////////////////////////////////////////////////check Touch Axis
            if (touchVal.Y > 3450 && touchVal.Y < 3750) {
                if (touchVal.X > 3150 && touchVal.X < 3950)
                    buttonState = 1;
                else if (touchVal.X > 2150 && touchVal.X < 2950)
                    buttonState = 2;
                else if (touchVal.X > 1150 && touchVal.X < 1950)
                    buttonState = 4;
                else if (touchVal.X > 150 && touchVal.X < 950)
                    buttonState = 8;
                else
                    buttonState = 0;
            } else
                buttonState = 0;
            ///////////////////////////////////////////////////////////////////////Button Re Draw			
            if (prevButtonState != buttonState)//Redraw ButtonImage
            {
                if ((prevButtonState ^ buttonState)&1) {
                    if (buttonState & 1)//button down
                    {
                        NANDButtonDisplay(BUTTON_FREQPLUS_DOWN);
                        ret = BUTTON_FREQPLUS_DOWN;
                    } else//button up
                    {
                        NANDButtonDisplay(BUTTON_FREQPLUS_UP);
                        ret = BUTTON_FREQPLUS_UP;
                    }
                }
                if ((prevButtonState ^ buttonState)&2) {
                    if (buttonState & 2)//button down
                    {
                        NANDButtonDisplay(BUTTON_FREQMINUS_DOWN);
                        ret = BUTTON_FREQMINUS_DOWN;
                    } else//button up
                    {
                        NANDButtonDisplay(BUTTON_FREQMINUS_UP);
                        ret = BUTTON_FREQMINUS_UP;
                    }
                }
                if ((prevButtonState ^ buttonState)&4) {
                    if (measureOn) {
                        if (buttonState & 4)//button down
                        {
                            NANDButtonDisplay(BUTTON_MEASURESTOP_DOWN);
                            ret = BUTTON_MEASURESTOP_DOWN;
                        } else//button up
                        {
                            NANDButtonDisplay(BUTTON_MEASURESTOP_UP);
                            ret = BUTTON_MEASURESTOP_UP;
                        }
                    } else {
                        if (buttonState & 4)//button down
                        {
                            NANDButtonDisplay(BUTTON_MEASURE_DOWN);
                            ret = BUTTON_MEASURE_DOWN;
                        } else//button up
                        {
                            NANDButtonDisplay(BUTTON_MEASURE_UP);
                            ret = BUTTON_MEASURE_UP;
                        }
                    }
                }
                if ((prevButtonState ^ buttonState)&8) {
                    if (buttonState & 8)//button down
                    {
                        NANDButtonDisplay(BUTTON_TITLE_DOWN);
                        ret = BUTTON_TITLE_DOWN;
                    } else//button up
                    {
                        NANDButtonDisplay(BUTTON_TITLE_UP);
                        ret = BUTTON_TITLE_UP;
                    }
                }
            }

            if (prevButtonState != buttonState) {
            }
            break;

        case STATE_IMPEDANCE:

            if (prevButtonState != buttonState) {
            }
            break;

        case STATE_CONFIG:

            if (prevButtonState != buttonState) {
            }
            break;
    }

    prevButtonState = buttonState;
    return ret;
}

uint32_t ReadTouchScreen(TouchPoint* touchVal) {
    if (!(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))) {
        *touchVal = read_once();
        return 1;
    } else
        return 0;
}

void TouchCalibration(void) {
    //Flash Check & TFT Position ^ Touch Position Sync
}

void TouchDebuggingFunction(void) {
    if (!(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))) {
        TouchPoint a = read_once();
        sprintf((char*) tmp, "%d", GPIO_ReadInputData(GPIOB)&1);
        TFT_NewString(50, 10, tmp, WHITE, BLACK);

        sprintf((char*) tmp, "%04d", a.X);
        TFT_NewString(50, 50, tmp, WHITE, BLACK);

        sprintf((char*) tmp, "%04d", a.Y);
        TFT_NewString(50, 100, tmp, WHITE, BLACK);
    }
}

void start_spi(void) {
    TCS_SET(0);
}

uint16_t SPI_Flash_ReadByte(uint8_t a) {
    return (SPI_Flash_SendByte(a));
}

uint16_t SPI_Flash_SendByte(uint8_t byte) {
    /* Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    //NotSelect_Flash();  while(1);
    /* Send byte through the SPI2 peripheral */
    SPI_I2S_SendData(SPI2, byte);

    /* Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI2);
}

TouchPoint read_once(void) {
    uint16_t a, b;
    TouchPoint Point = {0, 0};

    if (GPIO_ReadInputData(GPIOB)&1) //Touch Interrupt Pin	
        return Point;


    start_spi();
    SPI_Flash_SendByte(CMD_RDY);
    delay_us(10);
    a = SPI_Flash_ReadByte(0);
    a = a << 8;
    a |= SPI_Flash_ReadByte(0);
    a >>= 3;
    Point.Y = a;
    SPI_Flash_SendByte(CMD_RDX);
    delay_us(10);
    b = SPI_Flash_ReadByte(0);
    b = b << 8;
    b |= SPI_Flash_ReadByte(0);
    b >>= 3;
    Point.X = b;
    TCS_SET(1);
    /*
    if(Point.X>100&&Point.Y>100&&Point.X<4090&&Point.Y<4090)
    return Point;
    else 
    {
    Point.X = 0;
    Point.Y = 0;*/
    return Point;
    //	}
}

void delay_us(uint32_t nCount) {
    uint32_t i, j;

    for (i = 0; i < nCount; i++)
        for (j = 0; j < 5; j++);
}








