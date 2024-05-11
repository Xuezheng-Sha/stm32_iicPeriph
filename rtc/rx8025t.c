/*
* @file: rx8025t.c
* @brief: 
* @author: shaxz
* @created: 2024-04-18 11:04:05
* Copyright 2024 , All Rights Reserved. 
*/
/* includes ------------------------------------------------------------------*/
#include "rx8025t.h"

/* private marcos ------------------------------------------------------------*/
/* private types -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
/* private functions ---------------------------------------------------------*/

I2C_HandleTypeDef *pI2c = NULL;

static unsigned char B2D(unsigned char ucBCD);
static unsigned char D2B(unsigned char ucDecimal);
static unsigned char GetDoW(unsigned char ucVal);
static unsigned char SetDoW(unsigned char ucVal);

bool RX8025T_Init(I2C_HandleTypeDef *handle)
{
    unsigned char ucRegVal = 0;

    pI2c = handle;
    if (!ReadRegister(REGADDR_FLAG, &ucRegVal))
    {
        return false;
    }
    if ((ucRegVal & FLAG_VLF) != 0)
    {
        return true;
    }
    WriteRegister(REGADDR_FLAG, 0x00);
    WriteRegister(REGADDR_CONTROL, 0x40);

    return true;
}

bool RX8025T_GetTime(_RTC_s *rtc_s)
{
    unsigned char ucStartAddr = REGADDR_FLAG;
    unsigned char ucBuffer[7] = {0};

    if (!ReadRegister(REGADDR_FLAG, ucBuffer))
    {
        return false;
    }

    if ((ucBuffer[0] & FLAG_UF) != 0)
    {
        ucStartAddr = REGADDR_SEC;
        if (HAL_I2C_Master_Transmit(pI2c, RX8025T_READ_ADDR, &ucStartAddr, 1, HAL_MAX_DELAY) != HAL_OK)
        {
            return false;
        }
        if (HAL_I2C_Master_Receive(pI2c, RX8025T_READ_ADDR, ucBuffer, sizeof(ucBuffer), HAL_MAX_DELAY) != HAL_OK)
        {
            return false;
        }
        rtc_s->ucSec = B2D(ucBuffer[0] & 0x7F);
        rtc_s->ucMin = B2D(ucBuffer[1] & 0x7F);
        rtc_s->ucHour = B2D(ucBuffer[2] & 0x3F);
        rtc_s->ucWeekDay = GetDoW(ucBuffer[3] & 0x7f);
        rtc_s->ucDate = B2D(ucBuffer[4] & 0x3F);
        rtc_s->ucMonth = B2D(ucBuffer[5] & 0x1F);
        rtc_s->ucYear = B2D(ucBuffer[6]);
    }
    return true;
}

bool RX8025T_SetTime(_RTC_s *rtc_s)
{
    unsigned char ucStartAddr = REGADDR_SEC;
    unsigned char ucBuffer[8] =
    {
        ucStartAddr, D2B(rtc_s->ucSec),
        D2B(rtc_s->ucMin), D2B(rtc_s->ucHour),
        SetDoW(rtc_s->ucWeekDay),
        D2B(rtc_s->ucDate),
        D2B(rtc_s->ucMonth),
        D2B(rtc_s->ucYear)
    };
    if (HAL_I2C_Master_Transmit(pI2c, RX8025T_WRITE_ADDR, ucBuffer, sizeof(ucBuffer), 100) != HAL_OK)
    {
        return false;
    }
    return true;
}

bool RX8025T_SetINTPerSec(void)
{
    // once per second
    unsigned char ucRegVal = 0;
    ReadRegister(REGADDR_CONTROL, &ucRegVal);
    // ucRegVal |= (CONTR_UIE);
    ucRegVal |= (CONTR_UIE) | (CONTR_TIE);
    WriteRegister(REGADDR_CONTROL, ucRegVal);

    ReadRegister(REGADDR_EXTEN, &ucRegVal);
    ucRegVal &= ~(EXTEN_USEL);
    ucRegVal |= (EXTEN_TSEL1);
    ucRegVal &= ~(EXTEN_TSEL0);
    WriteRegister(REGADDR_EXTEN, ucRegVal);

    return true;
}

bool RX8025T_SetINTPerMin(void)
{
    // once per min
    unsigned char ucRegVal = 0;
    ReadRegister(REGADDR_CONTROL, &ucRegVal);
    // ucRegVal |= (CONTR_UIE);
    ucRegVal |= (CONTR_UIE) | (CONTR_TIE);
    WriteRegister(REGADDR_CONTROL, ucRegVal);

    ReadRegister(REGADDR_EXTEN, &ucRegVal);
    ucRegVal |= (EXTEN_USEL);
    // ucRegVal &= ~(EXTEN_USEL);
    ucRegVal |= (EXTEN_TSEL1) | (EXTEN_TSEL0);
    WriteRegister(REGADDR_EXTEN, ucRegVal);

    return true;
}

bool RX8025T_SetINTDisable(void)
{
    unsigned char ucRegVal = 0;
    ReadRegister(REGADDR_CONTROL, &ucRegVal);
    // ucRegVal |= (CONTR_UIE);
    ucRegVal &= ~(CONTR_UIE);
    WriteRegister(REGADDR_CONTROL, ucRegVal);

    return true;
}

bool ReadRegister(unsigned char ucRegAddr, unsigned char *ucValue)
{
    if (HAL_I2C_Master_Transmit(pI2c, RX8025T_READ_ADDR, &ucRegAddr, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        return false;
    }
    if (HAL_I2C_Master_Receive(pI2c, RX8025T_READ_ADDR, ucValue, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        return false;
    }
    return true;
}

bool WriteRegister(unsigned char ucRegAddr, unsigned char ucValue)
{
    unsigned char ucBuffer[2] = {ucRegAddr, ucValue};
    if (HAL_I2C_Master_Transmit(pI2c, RX8025T_WRITE_ADDR, ucBuffer, sizeof(ucBuffer), HAL_MAX_DELAY) != HAL_OK)
    {
        return false;
    }
    return true;
}

static unsigned char B2D(unsigned char ucBCD)
{
    return (ucBCD >> 4) * 10 + (ucBCD & 0x0F);
}

static unsigned char D2B(unsigned char ucDecimal)
{
    return (((ucDecimal / 10) << 4) | (ucDecimal % 10));
}

static unsigned char GetDoW(unsigned char ucVal)
{
    unsigned char ucRet = 0;
    switch (ucVal)
    {
    case 0x01:
        ucRet = 0;
        break;
    case 0x02:
        ucRet = 1;
        break;
    case 0x04:
        ucRet = 2;
        break;
    case 0x08:
        ucRet = 3;
        break;
    case 0x10:
        ucRet = 4;
        break;
    case 0x20:
        ucRet = 5;
        break;
    case 0x40:
        ucRet = 6;
        break;
    default:
        ucRet = 0;
        break;
    }
    return ucRet;
}

static unsigned char SetDoW(unsigned char ucVal)
{
    return ((ucVal > 6) ? 0 : 0x01 << ucVal);
}

