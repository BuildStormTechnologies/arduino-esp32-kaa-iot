#include "app_config.h"
#include "LibSystem.h"
#include "LibUtils.h"
#include "Kaa.h"
#include "TFT_eSPI.h"
#include "icons.h"
#include "Free_Fonts.h"

LibSystem SYS;
Kaa KAA;
LibUtils UTILS;

/* Config -----------------------------------------------------------------*/
// #define TFT_WIDTH 320
// #define TFT_HEIGHT 170

#define TFT_WIDTH 470
#define TFT_HEIGHT 304

// #define TFT_WIDTH 320
// #define TFT_HEIGHT 240

#define font FF6

#define pos1 1.5
#define pos2 1.88

// #define pos1 1
// #define pos2 1.41

// #define pos1 1
// #define pos2 1

//progress bar variables
int progress = 0;
int x = 26;
int y = TFT_HEIGHT-160;
int w = TFT_WIDTH - 58; // Adjusting width to fit within display
int h = 18;
int blocks = 0;
/* Config -----------------------------------------------------------------*/
TFT_eSPI tft = TFT_eSPI();

#define gray 0x6B6D


typedef enum
{
  DISPLAY_STATUS_OFF,
  DISPLAY_STATUS_NO_CHANGE,
  DISPLAY_STATUS_TOGGLE,
  DISPLAY_STATUS_ON,
} displayStatusCodes_et;

static uint8_t eventWIFIState = DISPLAY_STATUS_OFF;
static uint8_t eventBLEState = DISPLAY_STATUS_OFF;
static uint8_t eventMQTTState = DISPLAY_STATUS_OFF;
bool printStatus = false;

uint32_t ledBlinkCount_u32 = 0;
uint32_t ledunit=0;
uint32_t nextBlinkTime_u32 = 0;

void metadata_updateCallBack(const char *pKeyStr, const void *pValue, valueType_et valuetype);
void cmd_callBackHander(const char *pCmd, const char* pCmdId, const char *pPayload);

void app_init();
void app_loop();

void setup() {
  Serial.begin(115200);
  app_init();
  disp_init();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  app_loop();
}


void app_eventsCallBackHandler(systemEvents_et event_e) {
  switch (event_e) {
    case EVENT_WIFI_CONNECTED:
      eventWIFIState = DISPLAY_STATUS_ON;
      printStatus = true;
      printf("\nEVENT_WIFI_CONNECTED");
      break;
    case EVENT_WIFI_DISCONNECTED:
      eventWIFIState = DISPLAY_STATUS_OFF;
      printStatus = true;
      printf("\nEVENT_WIFI_DISCONNECTED");
      break;

    case EVENT_MQTT_CONNECTED:
      eventMQTTState = DISPLAY_STATUS_ON;
      printStatus = true;
      printf("\nEVENT_MQTT_CONNECTED");
      break;
    case EVENT_MQTT_DISCONNECTED:
      eventMQTTState = DISPLAY_STATUS_OFF;
      printStatus = true;
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
    ledunit=ledBlinkCount_u32;
    if(UTILS.getNumU32(pPayload)==1)
    {
      ledunit=1;
      ledBlink(ledunit);
    }
    else if(UTILS.getNumU32(pPayload)==0)
    {
      ledunit=0;
      ledBlink(ledunit);
    }
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
            ledBlink(ledBlinkCount_u32/2);
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
  static uint8_t i=0;
  static uint8_t j=0;
  if (SYS.getOtaPercentage() == 100)
  {
    rebootingScreen();
    delay(10000);
  }

  if(nextPubTime == 0)
  {
       nextPubTime = millis();
       KAA.metadataUpdate("platform", (void *)"ESP32", VALUE_TYPE_STRING);
       KAA.metadataUpdate("bootTime", &nextPubTime, VALUE_TYPE_INT);
  }
  

  if (SYS.getMode() == SYSTEM_MODE_NORMAL) {
    if (KAA.isConnected()) {
      if ((millis() > nextPubTime)) 
      {
        printf("\n temp:%d humidity:%.2f",temp_u16, hum_f32);
        KAA.telemetryUpdate("temp", &temp_u16, VALUE_TYPE_INT);
        KAA.telemetryUpdate("hum", &hum_f32, VALUE_TYPE_FLOAT);
        attributesScreen(temp_u16, hum_f32);
        nextPubTime = millis() + 2000;
        temp_u16++;
        hum_f32 += 1.1;
        if(j==0)
        {
        ledBlink(0);
        j++;
        } 
      }  
    }

    app_handleLedCmd();
  }
  
  if (SYS.getMode() == SYSTEM_MODE_OTA) 
  {
    if (SYS.getOtaPercentage() == 0)
    {
      if (i == 0) 
      {
        OTARequestScreen();
        i++;
      }
    } 
    else 
    {
      progressbar(SYS.getOtaPercentage());
    }
    if (SYS.getOtaPercentage() == 100)
    {
      rebootingScreen();
      delay(10000);
    }
  }
  aicon_status();
  delay(200);
}

void disp_init()
{
  tft.init();
  tft.setRotation(1);

  tft.fillScreen(TFT_BLACK);

    tft.setSwapBytes(true);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(4);
 
  thingVersion();
}

void aicon_status()
{
  static bool blinkState = false;
  static int i=0;
  if (!eventWIFIState)
  {
    if (blinkState)
    {
        tft.pushImage(TFT_WIDTH-30, 2, 25, 25, wifiDiscon_icon);
    }
    else
    {
        tft.fillRect(TFT_WIDTH-30, 2, 25, 25, TFT_BLACK);
    }
    if(i==0)
    {
    conInScreen();
    i++;
    }
  }
  else
  {
      tft.pushImage(TFT_WIDTH-30, 2, 25, 25, wifi_icon);
  }

  if (eventWIFIState==DISPLAY_STATUS_ON && eventMQTTState==DISPLAY_STATUS_OFF)
  {
    if (blinkState)
    {
        tft.pushImage(TFT_WIDTH-60, 4, 25, 25, kaaDiscon_icon);
    }
    else
    {
        tft.fillRect(TFT_WIDTH-60, 4, 25, 25, TFT_BLACK);
    }
    if(i==1)
    {
      conTBScreen();
      i++;
    }
  }
  else if(eventMQTTState==DISPLAY_STATUS_OFF)
  {
      tft.pushImage(TFT_WIDTH-60, 4, 25, 25, kaaDiscon_icon);
  }
  if(eventMQTTState==DISPLAY_STATUS_ON)
  {
     tft.pushImage(TFT_WIDTH-60, 4, 25, 25, kaaCon_icon);
  }

  blinkState = !blinkState;
}



void thingVersion()
{
    tft.setTextColor(TFT_SKYBLUE);
    tft.setFreeFont(font);
    tft.setTextSize(1);
    tft.setCursor(10, 20);
    tft.print("cno2um4e0qcc73d6sung");
    int deviceNameLength = tft.textWidth("cno2um4e0qcc73d6sung");
    // tft.setCursor (deviceNameLength + 15, 15);
    // tft.setFreeFont(FF0);
    // tft.print (APP_VERSION);
    tft.setFreeFont(font);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("KAA IoT", TFT_WIDTH/2, TFT_HEIGHT-22);
    tft.drawRect(5, TFT_HEIGHT-33, TFT_WIDTH-10, 26, TFT_WHITE);
      tft.drawRect(0, 0, TFT_WIDTH, TFT_HEIGHT, TFT_WHITE);
}

void attributesScreen(uint16_t temp, float hum)
{
  static uint8_t i=0;
  if(i==0)
  {
    tft.fillRect(10,40, TFT_WIDTH-15, 225, TFT_BLACK);
    i++;
  }
  tft.drawRect(0, 0, TFT_WIDTH, TFT_HEIGHT, TFT_WHITE);
  tft.setFreeFont(FF6);
  // int color = stringToColor(txtColor);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("fw_ver :", 50 * pos1, 30 * pos2);   
  tft.drawString("LED    :", 50 * pos1, 55 * pos2);   
  tft.drawString("Temp   :", 50 * pos1, 80 * pos2);  
  tft.drawString("Hum    :", 50 * pos1, 105 * pos2);
  tft.drawString("MAC ID :", 50 * pos1, 130 * pos2);  
  static char prevTxtColor[20] = "";
  static int32_t prevTemp=-1;
  static float prevhum=1.0;

  tft.drawString(String(APP_VERSION), 140 * pos1, 30 * pos2); 
  // Compare the current values with the previous ones


  //   if (strcmp(txtColor, prevTxtColor) != 0) {
  //   // Update the display only if the value has changed
  //   tft.fillRect(130,110, 200, 40, TFT_BLACK);
  //   tft.drawString(txtColor, 140 * pos1, 70 * pos2);            
  //   strcpy(prevTxtColor, txtColor);
  // }

  if (temp != prevTemp)
  {
    tft.fillRect(130,130, 200, 40, TFT_BLACK);
    tft.drawString(String(temp), 140 * pos1, 80 * pos2);
    prevTemp=temp;
  }

  if (hum != prevhum)
  {
    tft.fillRect(130,170, 200, 40, TFT_BLACK);
    tft.drawFloat(hum, 2, 140 * pos1, 105 * pos2);
    prevhum=hum;
  }
  tft.drawString(String(SYS.getDeviceName()), 170 * pos1, 130 * pos2);
  tft.setFreeFont(font);
}

void ledBlink(uint32_t ledState)
{
    tft.setFreeFont(FF6);

    static int32_t prevLedState = -1;

    if (ledState != prevLedState) {
    // Update the display only if the value has changed
    // tft.fillRect(130, 70, 200, 40, TFT_BLACK);
    // tft.drawString(String(ledState), 140 * pos1, 45 * pos2);     
    tft.fillRect(130,90, 200, 40, TFT_BLACK);
    tft.drawString(String(ledState), 140 * pos1, 55 * pos2);  
    prevLedState = ledState;
  }
}

int32_t stringToColor(char *colorStr)
{
  int32_t tftColor = {0};

  if (strcmp(colorStr, "RED") == 0)
  {
    tftColor = TFT_RED;
  }
  else if (strcmp(colorStr, "GREEN") == 0)
  {
    tftColor = TFT_DARKGREEN;
  }
  else if (strcmp(colorStr, "BLUE") == 0)
  {
    tftColor = TFT_BLUE;
  }
  else
  {
    tftColor = TFT_WHITE;
  }

  return tftColor;
}

void OTARequestScreen()
{
  tft.drawRect  (  0, 0, TFT_WIDTH, TFT_HEIGHT, TFT_WHITE);
  tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(font);
  // tft.fillRect(TFT_WIDTH-35,TFT_HEIGHT/2 -8, 10, 20, TFT_BLACK);
  tft.fillRect(10,40, TFT_WIDTH-15, 225, TFT_BLACK);
  tft.drawString("OTA Requested", TFT_WIDTH / 2, TFT_HEIGHT / 2);
}

void progressbar(int progress)
{
    static int i=0;
    if(i==0)
    {
      tft.fillRect(60, 120, 340, 150, TFT_BLACK);
      tft.fillRect(10,40, TFT_WIDTH-15, 225, TFT_BLACK);
      i++;
    }
    tft.setFreeFont(&Orbitron_Light_24);
    tft.fillRect((TFT_WIDTH / 2) -40, 60, 80, 40, TFT_BLACK);
    tft.drawString(String(progress) + "%", TFT_WIDTH / 2, 80);


    int blockWidth = 4.9;
    int blocks = progress;
    tft.drawRoundRect(x, y, w, h, 3, TFT_WHITE);

    for (int i = 0; i < blocks; i++) {
        tft.fillRect(x + 6 + i * blockWidth, y + 4, blockWidth-1, 10, TFT_GREEN);
    }
    delay(200);
}

void rebootingScreen()
{
   tft.drawRect  (  0, 0, TFT_WIDTH, TFT_HEIGHT, TFT_WHITE);
   tft.setFreeFont(font);
  //  tft.fillRect(60, 50, 340, 150, TFT_BLACK);
   tft.fillRect(10,40, TFT_WIDTH-15, 225, TFT_BLACK);
   tft.drawString("Rebooting...", TFT_WIDTH/2, TFT_HEIGHT/2);
   aicon_status();
   delay(5000);
}


// void attributesScreen(uint32_t ledState, uint16_t temp, float hum)
// {
//   static uint8_t i=0;
//   if(i==0)
//   {
//     tft.fillRect(10,40, TFT_WIDTH-15, 225, TFT_BLACK);
//     i++;
//   }
//   tft.drawRect(0, 0, TFT_WIDTH, TFT_HEIGHT, TFT_WHITE);
//   tft.setFreeFont(FF6);
//   // int color = stringToColor(txtColor);
//   tft.setTextColor(TFT_WHITE);
//   tft.drawString("fw_ver :", 50 * pos1, 45 * pos2);   
//   tft.drawString("LED    :", 50 * pos1, 70 * pos2);   
//   tft.drawString("Temp   :", 50 * pos1, 95 * pos2);  
//   tft.drawString("Hum    :", 50 * pos1, 120 * pos2);  
//   static int32_t prevLedState = -1;
//   static char prevTxtColor[20] = "";
//   static int32_t prevTemp=-1;
//   static float prevhum=1.0;

//   tft.drawString(String(APP_VERSION), 140 * pos1, 45 * pos2); 
//   // Compare the current values with the previous ones
//   if (ledState != prevLedState) {
//     // Update the display only if the value has changed
//     // tft.fillRect(130, 70, 200, 40, TFT_BLACK);
//     // tft.drawString(String(ledState), 140 * pos1, 45 * pos2);     
//     tft.fillRect(130,110, 200, 40, TFT_BLACK);
//     tft.drawString(String(ledState), 140 * pos1, 70 * pos2);  
//     prevLedState = ledState;
//   }

//   //   if (strcmp(txtColor, prevTxtColor) != 0) {
//   //   // Update the display only if the value has changed
//   //   tft.fillRect(130,110, 200, 40, TFT_BLACK);
//   //   tft.drawString(txtColor, 140 * pos1, 70 * pos2);            
//   //   strcpy(prevTxtColor, txtColor);
//   // }

//   if (temp != prevTemp)
//   {
//     tft.fillRect(130,150, 200, 40, TFT_BLACK);
//     tft.drawString(String(temp), 140 * pos1, 95 * pos2);
//     prevTemp=temp;
//   }

//   if (hum != prevhum)
//   {
//     tft.fillRect(130,190, 200, 45, TFT_BLACK);
//     tft.drawFloat(hum, 2, 140 * pos1, 120 * pos2);
//     prevhum=hum;
//   }
//   thingVersion();
//   tft.setFreeFont(font);
// }

void conInScreen()
{
    tft.drawRect(0, 0, TFT_WIDTH, TFT_HEIGHT, TFT_WHITE);
    // tft.fillRect(60, 120, 340, 150, TFT_BLACK);
    tft.fillRect(10,40, TFT_WIDTH-15, 225, TFT_BLACK);
    tft.setFreeFont(FF6);
    tft.drawString("Connecting to WiFi..", TFT_WIDTH/2, TFT_HEIGHT/2); 
    delay(1000);
}

void conTBScreen()
{
    tft.drawRect(0, 0, TFT_WIDTH, TFT_HEIGHT, TFT_WHITE);
    // tft.fillRect(60, 120, 340, 150, TFT_BLACK);
    tft.fillRect(10,40, TFT_WIDTH-15, 225, TFT_BLACK);
    tft.setFreeFont(FF6);
    tft.drawString("Connecting to KAA IoT..", TFT_WIDTH/2, TFT_HEIGHT/2); 
    delay(1000);
}





