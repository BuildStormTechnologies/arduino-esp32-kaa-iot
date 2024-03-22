#include "app_config.h"
#include "LibSystem.h"
#include "LibUtils.h"
#include "Kaa.h"

LibSystem SYS;
Kaa KAA;
LibUtils UTILS;

uint32_t ledBlinkCount_u32 = 0;
uint32_t nextBlinkTime_u32 = 0;

void metadata_updateCallBack(const char *pKeyStr, const void *pValue, valueType_et valuetype);
void cmd_callBackHander(const char *pCmd, const char* pCmdId, const char *pPayload);

void app_init();
void app_loop();

void setup() {
  Serial.begin(115200);
  app_init();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  app_loop();
}


void app_eventsCallBackHandler(systemEvents_et event_e) {
  switch (event_e) {
    case EVENT_WIFI_CONNECTED:
      printf("\nEVENT_WIFI_CONNECTED");
      break;
    case EVENT_WIFI_DISCONNECTED:
      printf("\nEVENT_WIFI_DISCONNECTED");
      break;

    case EVENT_MQTT_CONNECTED:
      printf("\nEVENT_MQTT_CONNECTED");
      break;
    case EVENT_MQTT_DISCONNECTED:
      printf("\nEVENT_MQTT_DISCONNECTED");
      break;

    default:
      break;
  }
}


void cmd_callBackHander(const char *pCmd, const char *pCmdId, const char *pPayload)
{
  printf("\nid:%s [%s]:%s", pCmdId, pCmd, pPayload);
  if (strcmp(pCmd, "LED") == 0)
  {
    ledBlinkCount_u32 = UTILS.getNumU32(pPayload);
    if ((ledBlinkCount_u32 == 0) || (ledBlinkCount_u32 == 1))
    {
      digitalWrite(LED_PIN, ledBlinkCount_u32); // 0-LED OFF,  1-> LED ON
      ledBlinkCount_u32 = 0;
    }
    else // N- Blink led N-times
    {
      ledBlinkCount_u32 = ledBlinkCount_u32 * 2; // double it for ON->OFF cycle
    }
  }
  KAA.cmdUpdateStatus(pCmd, pCmdId,pPayload, 200, "Ok");
}

void app_handleLedCmd()
{
    if (ledBlinkCount_u32)
    {
        if (millis() > nextBlinkTime_u32)
        {
            ledBlinkCount_u32--;
            nextBlinkTime_u32 = millis() + 500;
            digitalWrite(LED_PIN, ledBlinkCount_u32 & 0x01);
        }
    }
}

void metadata_updateCallBack(const char *pKeyStr, const void *pValue, valueType_et valuetype)
{
    switch (valuetype)
    {
    case VALUE_TYPE_INT:
        printf("\n[%s]: %d", pKeyStr, *(int *)pValue);
        break;

    case VALUE_TYPE_FLOAT:
        printf("\n[%s]: %.2f", pKeyStr, *(float *)pValue);
        break;

    case VALUE_TYPE_STRING:
        printf("\n[%s]: %s", pKeyStr, pValue);
        break;

    default:
        break;
    }
}



/**
 * @brief    entry point of the project
 * @param    None
 * @return   None
 */
void app_init() {

  systemInitConfig_st sysConfig = { 0 };

  sysConfig.systemEventCallbackHandler = app_eventsCallBackHandler;
  sysConfig.pAppVersionStr = (char *)APP_VERSION;

  /* Telemetery, MetaData and Commands config */
  sysConfig.maxMetaElements_u8 = 3;
  sysConfig.maxTelemetryElements_u8 = 2;
  sysConfig.maxCommands_u8 = 2;

  /* Wifi config*/
  sysConfig.pWifiSsidStr = (char *)TEST_WIFI_SSID;
  sysConfig.pWifiPwdStr = (char *)TEST_WIFI_PASSWORD;

  /* Things bOards configuration */
  sysConfig.s_mqttClientConfig.pUriStr = (char *)KAA_URI;
  sysConfig.s_mqttClientConfig.port_u16 = KAA_PORT;
  sysConfig.s_mqttClientConfig.pKaaApplicationVersionStr = (char *)KAA_APPLICATION_VERSION;
  sysConfig.s_mqttClientConfig.pAccessTokenStr = (char *)KAA_ENDPOINT_TOKEN;

  if (SYS.init(&sysConfig)) {
    SYS.start();

        KAA.metadataRegister("testMeta", VALUE_TYPE_STRING, metadata_updateCallBack);
        KAA.cmdRegister("LED", cmd_callBackHander);
  } else {
    while (1) {
      printf("\n System Init failed, verify the init config ....");
      delay(5000);
    }
  }
}

void app_loop() {

  static uint32_t nextPubTime = 0;
  static float hum_f32 = 10.6;
  static uint16_t temp_u16 = 20;

  if(nextPubTime == 0)
  {
       nextPubTime = millis();
       KAA.metadataUpdate("platform", (void *)"ESP32", VALUE_TYPE_STRING);
       KAA.metadataUpdate("bootTime", &nextPubTime, VALUE_TYPE_INT);
  }
  

  if (SYS.getMode() == SYSTEM_MODE_NORMAL) {
    if (KAA.isConnected()) {
      if ((millis() > nextPubTime)) {

        KAA.telemetryUpdate("temp", &temp_u16, VALUE_TYPE_INT);
        KAA.telemetryUpdate("hum", &hum_f32, VALUE_TYPE_FLOAT);
        printf("\n temp:%d humidity:%.2f",temp_u16, hum_f32);

        temp_u16++;
        hum_f32 += 1.1;
        nextPubTime = millis() + 1000;
      }

      app_handleLedCmd();
    }
  }

  delay(100);
}
