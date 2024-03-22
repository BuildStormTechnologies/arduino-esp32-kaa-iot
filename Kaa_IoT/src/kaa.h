 /**
 * \copyright Copyright (c) 2019-2024, Buildstorm Pvt Ltd
 *
 * \file Kaa.h
 * \brief Kaa library header file.
 *
 *
 *  This library handles the ThingsBoard platform's core features:
 *   - Telemetry updates
 *   - Metadata updates
 *   - Cmd handling
 *   - OTA
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

#ifndef __KAA_IOT_H__
#define __KAA_IOT_H__

 #include "KaaTypes.h"

class Kaa{

    private:

    public:
        bool isConnected();
        bool subscribe(char *pTopicStr, uint8_t qos_e);
        bool publish(mqttMsg_st *ps_msg);
        uint8_t available();
        bool read(mqttMsg_st *ps_msg);
        const char *getThingName();
	    bool metadataRegister(const char *pKey,valueType_et valuetype,metadataUpdateCallBack_t callbackHandler);
        bool metadataUpdate(const char *pKeyStr, void *pValue, valueType_et valuetype);
        bool telemetryUpdate(const char *pKeyStr, void *pValue,valueType_et valuetype);
        bool cmdRegister(const char *pCmdKey, commandCallBack_t callbackHandler);
        bool cmdUpdateStatus(const char *pCmdKey, const char *pCmdId,const char* pPayload,uint16_t statusCode,const char *phrase);
};

#endif //__KAA_IOT_H__
