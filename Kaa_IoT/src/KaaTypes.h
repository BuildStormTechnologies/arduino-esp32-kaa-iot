 /**
 * \copyright Copyright (c) 2019-2024, Buildstorm Pvt Ltd
 *
 * \file KaaTypes.h
 * \brief KaaTypes library header file.
 *
 *
 * This file defines the structure and enums used by the ThingsBoard library.
 *
 * The libraries have been tested on the ESP32 modules.
 * Buildstorm explicitly denies responsibility for any hardware failures
 * arising from the use of these libraries, whether directly or indirectly.

 *
 * EULA LICENSE:
 * This library is licensed under end user license EULA agreement.
 * The EULA is available at https://buildstorm.com/eula/
 * For any support contact us at hello@buildstorm.com
 *
 */

#ifndef __KAA_TYPES_H__
#define __KAA_TYPES_H__

#include "LibConfig.h"
#include "LibMsgTypes.h"

#define LENGTH_METADATA_KEY_VALUE 20
#define LENGTH_TELEMETRY_KEY_VALUE 20

typedef struct
{
    char *pUriStr;                /*!< MQTT broker URI */
    char *pHostNameStr;           /*!< MQTT IoT Endpoint */
    uint16_t port_u16;            /*!< MQTT IoT port number */
    char *pCertStr;               /*!< Certificate string */
    char *pUserNameStr;           /*!< MQTT username */
    char *pPasswordStr;           /*!< MQTT password */
    char *pAccessTokenStr;        /*!< MQTT password */
    char *pClientIdStr;           /*!< MQTT password */
    const char *pThingNameStr;    /*!< MQTT IoT thing name */
    char *pKaaApplicationVersionStr; /*!< Application Token */
} mqttClientConfig_st;

/**
 * @struct value_st
 * kaa value type.
 */
typedef struct
{
    int32_t val_i32;                        /*!< Value of type int32_t */
    float val_f32;                          /*!< Value of type float */
    char str[LENGTH_METADATA_KEY_VALUE]; /*!< Value of type string */
} value_st;

/**
 * @enum valueType_et
 * An enum that represents all possible value types.
 */
typedef enum
{
    VALUE_TYPE_INT,    /*!< value type as integer */
    VALUE_TYPE_FLOAT,  /*!< value type as float */
    VALUE_TYPE_STRING, /*!< value type as string */
    VALUE_TYPE_MAX,    /*!< Total number of value types */
}valueType_et;


/**
 * @brief Metadata update callback function type. The application should
 * define the callback function and intialize it in the @ref attribute_st
 * configuration.
 */

typedef void (*metadataUpdateCallBack_t)(const char *pKeyStr, const void *pValue, valueType_et valuetype);
typedef void (*commandCallBack_t)(const char *pCmd, const char* pCmdId, const char *pPayload);


#endif //__THINGSBOARD_TYPES_H__