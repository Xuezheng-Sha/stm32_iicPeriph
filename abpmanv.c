/*
 * @file: abpmanv.c
 * @brief:
 * @author: shaxz
 * @created: 2024-04-29 17:41:05
 * Copyright 2024 , All Rights Reserved.
 */
/* includes ------------------------------------------------------------------*/
#include "abpmanv.h"

/* private marcos ------------------------------------------------------------*/
/* private types -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
/* private functions ---------------------------------------------------------*/

/***
 *
 * @brief get sensor address
 * @return sensor address
 */
unsigned char get_abpmanv_addr(void)
{
    return ABPMANV_ADRESS;
}

/***
 * @brief
 * S1    S0 ************************************************
 * 0     0  normal operation, valid data *******************
 * 0     1  device in command mode* ************************
 * 1     0  stale data: data that has already been fetched *
 *          since the last measurement cycle, or data ******
 *          fetched before the first measurement has *******
 *          been completed *********************************
 * 1     1 diagnostic condition ****************************
 * @param sensor_data
 * @return ABPMANV State
 */
unsigned char pars_status(abpmanvData_s sensor_data)
{
    char *cStatusMsg = NULL;

    switch (sensor_data.uiStatus)
    {
    case NORMAL_OPERATION:
        break;
    case COMMAND_MODE:
        cStatusMsg = "device in command mode* \r\n";
        break;
    case STALE_DATA:
        cStatusMsg = "stale data \r\n";
        break;
    case DIAGNOSTIC_CONDITION:
        cStatusMsg = "diagnostic condition \r\n";
        break;
    }

    if (cStatusMsg != NULL)
    {
#if DEBUG_ENABLE > 0
        debug_print(cStatusMsg);
#endif
        return ABPMANV_ERROR;
    }

    return ABPMANV_OK;
}

/***
 * @brief Calculate sensor pressure
 *
 * @param sensor_data
 *  @return ABPMANV State
 */
unsigned char calc_abpmanv_p(abpmanvData_s sensor_data, abpmanvModel_e model, float *fPressure)
{
    if (fPressure == NULL)
        return ABPMANV_ERROR;

    switch (model)
    {
    case PRESSURE_015PG:
        *fPressure = (sensor_data.uiPressure - PRESSURE_OUTPUT_MIN) * (PRESSURE_015PG_MAX - (-PRESSURE_015PG_MIN));
        *fPressure = *fPressure / (PRESSURE_OUTPUT_MAX - PRESSURE_OUTPUT_MIN) + (-PRESSURE_015PG_MIN);
        break;
    case PRESSURE_150PG:
        *fPressure = (sensor_data.uiPressure - PRESSURE_OUTPUT_MIN) * (PRESSURE_150PG_MAX - (-PRESSURE_150PG_MIN));
        *fPressure = *fPressure / (PRESSURE_OUTPUT_MAX - PRESSURE_OUTPUT_MIN) + (-PRESSURE_150PG_MIN);
        break;
    default:
        return ABPMANV_ERROR;
        break;
    }

    return ABPMANV_OK;
}

/***
 * @brief Calculate sensor temperature
 *
 * @param sensor_data
 *  @return ABPMANV State
 */
unsigned char calc_abpmanv_t(abpmanvData_s sensor_data, float *fTemper)
{
    if (fTemper == NULL)
        return ABPMANV_ERROR;

    *fTemper = ((float)sensor_data.uiTemper / TEMPER_OUTPUT_MAX * TEMPER_MAX) - TEMPER_MIN;

    return ABPMANV_OK;
}
