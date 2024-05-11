/*
 * @file: abpmanv.h
 * @brief:
 * @author: shaxz
 * @created: 2024-04-29 14:23:33
 * Copyright 2024 , All Rights Reserved.
 */
#ifndef __ABPMANV_H__
#define __ABPMANV_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* includes ------------------------------------------------------------------*/
#include "config.h"

#define ABPMANV_OK 0
#define ABPMANV_ERROR 1

#define ABPMANV_ADRESS 0x28

    // ----------Pressure-------------------------
    // Outputmax = output at max. pressure [counts]
    // Outputmin = output at min. pressure [counts]
    // Pressuremax = max. value of pressure range [bar, psi, kPa, etc.]
    // Pressuremin = min. value of pressure range [bar, psi, kPa, etc.]
    // Pressure = pressure reading [bar, psi, kPa, etc.]
    // Output = digital pressure reading [counts]

    // Outputmax = 14745 counts (90% of 214 counts or 0x3999)
    // Outputmin = 1638 counts (10% of 214 counts or 0x0666)

    // Pressuremax = consult manual according to sensor model
    // Pressuremin = Consult manual according to sensor model
    // Pressure = pressure in psi
    // Output = 1657 counts

// 015PG 0 15 psi
#define PRESSURE_015PG_MAX 15
#define PRESSURE_015PG_MIN 0

// 150PG 0 150 psi
#define PRESSURE_150PG_MAX 150
#define PRESSURE_150PG_MIN 0

#define PRESSURE_OUTPUT_MAX 14745
#define PRESSURE_OUTPUT_MIN 1638

#define TEMPER_OUTPUT_MAX 2040
#define TEMPER_MAX 200
#define TEMPER_MIN 50

    /* export marcos -------------------------------------------------------------*/
    /* export types --------------------------------------------------------------*/
    typedef enum
    {
        NORMAL_OPERATION = 0,
        COMMAND_MODE = 1,
        STALE_DATA = 2,
        DIAGNOSTIC_CONDITION = 3
    } abpmanvStatus_e;

    typedef enum
    {
        PRESSURE_015PG = 0,
        PRESSURE_150PG = 1
    } abpmanvModel_e;

#pragma pack(1) // Use 1 byte alignment to ensure there are no extra padding bytes
    typedef struct
    {
        unsigned int uiStatus : 2;
        /*- Pressure -------------------------------*/
        unsigned int uiPressure : 14;
        /*- Temperature -----------------------------*/
        unsigned int uiTemper : 11;
    } abpmanvData_s;
#pragma pack() // Restore default byte alignment

    /* export variables ----------------------------------------------------------*/
    /* export functions ----------------------------------------------------------*/

    unsigned char get_abpmanv_addr(void);
    unsigned char pars_status(abpmanvData_s sensor_data);
    unsigned char calc_abpmanv_p(abpmanvData_s sensor_data, abpmanvModel_e model, float *fPressure);
    unsigned char calc_abpmanv_t(abpmanvData_s sensor_data, float *fTemper);

#ifdef __cplusplus
}
#endif

#endif
