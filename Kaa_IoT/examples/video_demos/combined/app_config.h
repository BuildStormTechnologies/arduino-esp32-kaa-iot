/*******************************************************************************
 * Copyright (c) 2021-2024, Buildstorm Pvt Ltd
 *
 * Filename:     app_config.h
 *
 * Description:
 *               Contains macros for the application
 *******************************************************************************/

#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif


#define APP_VERSION "1.0.5"

#define TEST_WIFI_SSID "desktop"
#define TEST_WIFI_PASSWORD "12345678"

#define KAA_URI "mqtt://mqtt.cloud.kaaiot.com"
#define KAA_PORT 1883

#define KAA_APPLICATION_VERSION "your application version"
#define KAA_ENDPOINT_TOKEN "Endpoint Token"

#define LED_PIN 2

#ifdef __cplusplus
}
#endif

#endif
