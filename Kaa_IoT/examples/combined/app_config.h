/**
 * \copyright Copyright (c) 2019-2024, Buildstorm Pvt Ltd
 *
 * \file app_config.h
 * \brief app_config header file.
 *
 * The app_config Contains macros for the application.
 *
 */

#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif


#define APP_VERSION "1.0.1"

#define TEST_WIFI_SSID "desk"
#define TEST_WIFI_PASSWORD "12345678"

#define KAA_URI "mqtt://mqtt.cloud.kaaiot.com"
#define KAA_PORT 1883

#define KAA_APPLICATION_VERSION "Your Kaa device application version"
#define KAA_ENDPOINT_TOKEN "Endpoint Token"

#define LED_PIN 2

#ifdef __cplusplus
}
#endif

#endif //_APP_CONFIG_H_
