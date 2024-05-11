/*
 * @file: tca9545a_i2c.c
 * @brief:
 * @author: shaxz
 * @created: 2024-04-24 14:44:54
 * Copyright 2024 , All Rights Reserved.
 */
/* includes ------------------------------------------------------------------*/
#include "tca9545a_i2c.h"

/* private marcos ------------------------------------------------------------*/
/* private types -------------------------------------------------------------*/
I2C_HandleTypeDef *I2c = NULL;

/* private variables ---------------------------------------------------------*/
/* private functions ---------------------------------------------------------*/
static inline void reset_(void);
static bool write_(unsigned char ucInput);
static unsigned char read_(void);

/* Function definitions */
bool tca9545a_init(I2C_HandleTypeDef *handle)
{
    I2c = handle;
    TCA9545A_Addr_A0_LOW;
    TCA9545A_Addr_A1_LOW;
    reset_();

    /* Try reading chip information */
    if (!tca9545a_current_ch())
    {
        return true;
    }
    return false;
}

bool tca9545a_select_ch(unsigned char ucCh)
{
    if (ucCh > 0x0F)
    {
#if DEBUG_ENABLE
        debug_print("Error: Invalid channel selection.\r\n");
#endif
        return false;
    }
    int iResult = write_(ucCh);
    if (!iResult)
    {
#if DEBUG_ENABLE
        debug_print("Error selecting channel.\r\n");
#endif
        return false;
    }
    /* Add appropriate delay */
    TCA9545A_DELAY_US(100);
    return iResult;
}

unsigned char tca9545a_current_ch(void)
{
    unsigned char ucValue = read_();
    if (ucValue == 0xFF)
    {
        return ucValue; // Return an invalid value to indicate error
    }
    return ucValue & 0x0F;
}

unsigned char tca9545a_current_it(void)
{
    unsigned char ucValue = read_();
    if (ucValue == 0xFF)
    {
        return ucValue; // Return an invalid value to indicate error
    }
    return (ucValue & 0xF0) >> 4;
}

unsigned char tca9545a_clear_it(unsigned char ucCh)
{
    // Read
    unsigned char ucValue = read_();
    // Mask
    unsigned char ucInterrupts = ucValue & 0xF0;
    ucInterrupts &= ~(ucCh << 4);
    ucValue = (ucValue & 0x0F) | ucInterrupts;
    // Write
    int iResult = write_(ucValue);
    /* Add appropriate delay */
    TCA9545A_DELAY_US(100);
    return iResult;
}

static inline void reset_(void)
{
    TCA9545A_RESET_LOW;
    TCA9545A_DELAY_MS(1);
    TCA9545A_RESET_HIGHT;
}

static bool write_(unsigned char ucInput)
{
    // Target address is shifted by 1
    unsigned short int usiAddressTarget = ADDR_ << 1;

    while (HAL_OK != HAL_I2C_Master_Transmit(I2c, usiAddressTarget, &ucInput, 1, TIMEOUT_))
    {
#if DEBUG_ENABLE
        debug_print("TCA9545 not found! Retrying...\r\n");
#endif
        if (0 == tca9545a_init(I2c))
        {
            continue;
        }
        return false;
    }
    return true;
}

static unsigned char read_(void)
{
    unsigned char ucRetry = 0;
    unsigned char ucBuf = 0;

    // Target address is shifted by 1
    unsigned short int usiAddressTarget = ADDR_ << 1;

    while (HAL_OK != HAL_I2C_Master_Receive(I2c, usiAddressTarget, &ucBuf, 1, TIMEOUT_))
    {
        ucRetry++;
        if (ucRetry > RETRY_)
        {
#if DEBUG_ENABLE
            debug_print("Failed to connect to TCA9545!\r\n");
#endif
            return 0xFF;
        }
#if DEBUG_ENABLE
        debug_print("TCA9545A not found! Retrying...\r\n");
#endif
        reset_();
    }
    return ucBuf;
}
