/*
* @file: dlh_l05d.c
* @brief: 
* @author: shaxz
* @created: 2024-04-29 17:40:58
* Copyright 2024 , All Rights Reserved. 
*/
/* includes ------------------------------------------------------------------*/
#include "dlh_l05d.h"

/* private marcos ------------------------------------------------------------*/
/* private types -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
/* private functions ---------------------------------------------------------*/

/***
 *
 * @brief get sensor address
 * @return sensor address
 */
unsigned char get_dlh_addr(void)
{
    return DLH_L05D_ADRESS;
}


unsigned char set_dlh_sensor_cmd(dlhCmd_e cmd)
{
    unsigned char ucRetCmd;

    switch(cmd){
        case SINGLE :
            ucRetCmd = DLH_L05D_SINGLE;
        break;
        case AVG_2 :
            ucRetCmd = DLH_L050_AVG_2;
        break;
        case AVG_4 :
            ucRetCmd = DLH_L050_AVG_4;
        break;
        case AVG_8 :
            ucRetCmd = DLH_L050_AVG_8;
        break;
        case AVG_16 :
            ucRetCmd = DLH_L050_AVG_16;
        break;
        default:
            ucRetCmd  = DLH_L05D_ERROR;
        break;
    }

    return ucRetCmd;
}


/***
 * @brief
 * Status Byte Definition ****************************
 *  Bit Description **********************************
 *  Bit 7 [MSB] [Always = 0] *************************
 *      6 Power : [1 = Power On]  ********************
 *      5 Busy: [ 1 = Processing Command, 0 = Ready] *
 *      4:3 Mode: [00 = Normal Operation ]  **********
 *      2 Memory Error [ 1 = EEPROM Checksum Fail] ***
 *      1 Sensor Configuration [ always = 0] *********
 *  Bit 0 [LSB] ALU Error [1 = Error]  ***************
 * @param sensor_data
 * @return DLH_L05D State
 */
unsigned  char parseStatusByte(unsigned char ucStatusByte, dlhData_s *data) {
    char *cStatusMsg = NULL;

 #if DEBUG_ENABLE
    char temp_str[50] ={0};
#endif

    data->status_s.uiPower = (ucStatusByte & 0x40) >> 6;
#if DEBUG_ENABLE
    if(!data->status_s.uiPower){
        sprintf(temp_str, "Power: %d\r\n", data->status_s.uiPower);
        debug_print(temp_str);
    }
#endif

    data->status_s.uiBusy = (ucStatusByte & 0x20) >> 5;
#if DEBUG_ENABLE
    if(!data->status_s.uiBusy){
        sprintf(temp_str, "Busy: %d\r\n", data->status_s.uiBusy);
        debug_print(temp_str);
    }
#endif

    data->status_s.uiMode = (ucStatusByte & 0x18) >> 3;
#if DEBUG_ENABLE
    if(data->status_s.uiMode != 0){
        sprintf(temp_str, "Mode: %d\r\n", data->status_s.uiMode);
        debug_print(temp_str);
    }
#endif

    data->status_s.uiMemoryError = (ucStatusByte & 0x04) >> 2;
#if DEBUG_ENABLE
    if(data->status_s.uiMemoryError != 0) {
        sprintf(temp_str, "Memory Error: %d\r\n", data->status_s.uiMemoryError);
        debug_print(temp_str);
    }
#endif

    data->status_s.uiSensorConfig = (ucStatusByte & 0x02) >> 1;
#if DEBUG_ENABLE
    if(data->status_s.uiSensorConfig != 0){
        sprintf(temp_str, "Sensor Configuration: %d\r\n", data->status_s.uiSensorConfig);
        debug_print(temp_str);
    }
#endif

    data->status_s.uiAluError = ucStatusByte & 0x01;
#if DEBUG_ENABLE
    if(data->status_s.uiAluError != 0) {
        sprintf(temp_str, "ALU Error: %d\r\n", data->status_s.uiAluError);
        debug_print(temp_str);
    }
#endif

    if (data->status_s.uiMode != 0) {
        switch (data->status_s.uiMode) {
            case 1:
                cStatusMsg = "device in command mode\r\n";
                break;
            case 2:
                cStatusMsg = "stale data\r\n";
                break;
            case 3:
                cStatusMsg = "diagnostic condition\r\n";
                break;
            default:
                break;
        }

        if (cStatusMsg != NULL) {
#if DEBUG_ENABLE > 0
            debug_print(cStatusMsg);
#endif
            return DLH_L05D_ERROR;
        }
    }

    return DLH_L05D_OK;
}

/***
 * @brief Calculate sensor pressure
 * 
 * @param sensor_data 
 *  @return ABPMANV State
*/
unsigned char calc_dlh_p(signed int siPressureOrg, float *fPressure)
{
    if (fPressure == NULL)
        return DLH_L05D_ERROR;

    *fPressure = 1.25 * ((float)(siPressureOrg - DLH_SENSOR_OSdig) / DLH_SENSOR_MAX_24B) * DLH_SENSOR_FSS;

    return DLH_L05D_OK;
}


/***
 * @brief Calculate sensor temperature
 * 
 * @param sensor_data 
 * @return DLH_L05D State
*/
unsigned char calc_dlh_t(signed int siTemperOrg, float *fTemper)
{
    if (fTemper == NULL)
        return DLH_L05D_ERROR;

    *fTemper = ((float)siTemperOrg * DLH_SENSOR_TEMPER_MAX / DLH_SENSOR_MAX_24B) - DLH_SENSOR_TEMPER_MIN;

    return DLH_L05D_OK;
}

