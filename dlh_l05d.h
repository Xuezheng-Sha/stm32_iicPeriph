/*
 * @file: dlh_l05d.h
 * @brief:
 * @author: shaxz
 * @created: 2024-04-29 11:07:13
 * Copyright 2024 , All Rights Reserved.
 */
#ifndef __DLH_L05D_H__
#define __DLH_L05D_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* includes ------------------------------------------------------------------*/
#include "config.h"

#define DLH_L05D_OK 0
#define DLH_L05D_ERROR 1

#define DLH_L05D_ADRESS 0x29

#define DLH_L05D_SINGLE 0xAA
#define DLH_L050_AVG_2 0xAC
#define DLH_L050_AVG_4 0xAD
#define DLH_L050_AVG_8 0xAE
#define DLH_L050_AVG_16 0xAF

#define DLH_L05D_TIMEOUT (300)
#define DLH_AVERAGE_FILTER (0.005)        // for pressure average calculation
#define DLH_DICARD_TRIGGER_THREHOLD (306) //[ms] threshold for discard a wrong trigger. The sensor needs 5 + 1 ms. The timeout is 300ms
#define DLH_SENSOR_ZEROFLOW (500)
#define DLH_SENSOR_MAX_16B (32767)
#define DLH_SENSOR_DIG_OFFSET (32768)

#define DLH_SENSOR_MAX_24B (16777216)
#define DLH_SENSOR_TEMPER_MAX 125
#define DLH_SENSOR_TEMPER_MIN 40

#define DLH_SENSOR_FSS 10
#define DLH_SENSOR_OSdig 8388608 // Offset Output @ Zero Diff. Pressure (OSdig)  0.5 * 2^24

#define DLH_PEAK_THRESHOLD 50

    /*- export marcos ------------------------------------------------------------------------*/
    /*- export types -------------------------------------------------------------------------*/

    typedef enum
    {
        SINGLE = 0,
        AVG_2 = 1,
        AVG_4 = 2,
        AVG_8 = 3,
        AVG_16 = 4
    } dlhCmd_e;

    /*- Output Data Format */
    /*- S[7:0] | P[23:16] | P[15:8]  | P[7:0]   | T[23:16]    | T[15:8]     | T[7:0] ---------*/
    /*- Status | Pressure | Pressure | Pressure | Temperature | Temperature | Temperature ----*/
    /*- Byte   | Byte 3   | Byte 1   | Byte 0   | Byte 3      | Byte 1      | Byte 0 ---------*/

    /*  Status Byte Definition   -------------------------------------------------------------*/
    /*  Bit Description          -------------------------------------------------------------*/
    /*  Bit 7 [MSB] [Always = 0] -------------------------------------------------------------*/
    /*      6 Power : [1 = Power On] ---------------------------------------------------------*/
    /*      5 Busy: [ 1 = Processing Command, 0 = Ready] -------------------------------------*/
    /*      4:3 Mode: [00 = Normal Operation ]           -------------------------------------*/
    /*      2 Memory Error [ 1 = EEPROM Checksum Fail]   -------------------------------------*/
    /*      1 Sensor Configuration [ always = 0]         -------------------------------------*/
    /*  Bit 0 [LSB] ALU Error [1 = Error]                -------------------------------------*/

#pragma pack(1) // Use 1 byte alignment to ensure there are no extra padding bytes
    typedef struct
    {

        struct
        {
            unsigned int uiAluError : 1;
            unsigned int uiSensorConfig : 1;
            unsigned int uiMemoryError : 1;
            unsigned int uiMode : 2;
            unsigned int uiBusy : 1;
            unsigned int uiPower : 1;
            unsigned int uiReserved : 1;
        } status_s;

        unsigned char ucPressure[3];
        unsigned char ucTemper[3];
    } dlhData_s;
#pragma pack() // Restore default byte alignment

    /* export variables ----------------------------------------------------------*/
    /* export functions ----------------------------------------------------------*/
    unsigned char get_dlh_addr(void);
    unsigned char set_dlh_sensor_cmd(dlhCmd_e cmd);
    unsigned char calc_dlh_t(signed int siTemperOrg, float *fTemper);
    unsigned char calc_dlh_p(signed int siPressureOrg, float *fPressure);
    unsigned char parseStatusByte(unsigned char ucStatusByte, dlhData_s *data);

#ifdef __cplusplus
}
#endif

#endif
