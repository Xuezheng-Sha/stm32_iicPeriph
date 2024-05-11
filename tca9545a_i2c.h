/*
 * @file: tca9545a_i2c.h
 * @brief:
 * @author: shaxz
 * @created: 2024-04-24 14:45:03
 * Copyright 2024 , All Rights Reserved.
 */
#ifndef __TCA9545A_I2C_H__
#define __TCA9545A_I2C_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* includes ------------------------------------------------------------------*/
#include "config.h"

/* export marcos -------------------------------------------------------------*/
/* export types --------------------------------------------------------------*/
/* export variables ----------------------------------------------------------*/
/* export functions ----------------------------------------------------------*/
#define ADDR_ (0x70U)
#define RETRY_ (10)
#define TIMEOUT_ (500)

#define TCA9545A_DELAY_MS(ms) HAL_Delay(ms)
#define TCA9545A_DELAY_US(us) delay_us(us)

#define TCA9545A_RESET_PORT I2C_RESET_GPIO_Port
#define TCA9545A_RESET_PIN I2C_RESET_Pin
#define TCA9545A_RESET_HIGHT HAL_GPIO_WritePin(I2C_RESET_GPIO_Port, I2C_RESET_Pin, GPIO_PIN_SET)
#define TCA9545A_RESET_LOW HAL_GPIO_WritePin(I2C_RESET_GPIO_Port, I2C_RESET_Pin, GPIO_PIN_RESET)

#define TCA9545A_Addr_A0_PORT I2C_A0_GPIO_Port
#define TCA9545A_Addr_A0_PIN I2C_A0_Pin
#define TCA9545A_Addr_A0_HIGHT HAL_GPIO_WritePin(I2C_A0_GPIO_Port, I2C_A0_Pin, GPIO_PIN_SET)
#define TCA9545A_Addr_A0_LOW HAL_GPIO_WritePin(I2C_A0_GPIO_Port, I2C_A0_Pin, GPIO_PIN_RESET)

#define TCA9545A_Addr_A1_PORT I2C_A1_GPIO_Port
#define TCA9545A_Addr_A1_PIN I2C_A1_Pin
#define TCA9545A_Addr_A1_HIGHT HAL_GPIO_WritePin(I2C_A1_GPIO_Port, I2C_A1_Pin, GPIO_PIN_SET)
#define TCA9545A_Addr_A1_LOW HAL_GPIO_WritePin(I2C_A1_GPIO_Port, I2C_A1_Pin, GPIO_PIN_RESET)

#define TCA9545A_CH_0 0x01U
#define TCA9545A_CH_1 0x02U
#define TCA9545A_CH_2 0x04U
#define TCA9545A_CH_3 0x08U

    bool tca9545a_init(I2C_HandleTypeDef *handle);
    bool tca9545a_select_ch(unsigned char ucCh);
    unsigned char tca9545a_current_ch(void);
    unsigned char tca9545a_current_it(void);
    unsigned char tca9545a_clear_it(unsigned char ucCh);

#ifdef __cplusplus
}
#endif

#endif
