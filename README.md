# Esp32 Kaa Iot Arduino Platform
The Buildstorm platform simplifies the integration of Kaa IoT core features onto ESP32 Hardware,  from basic IoT pub/sub functionalities to advanced features like commands and OTA updates.

The architecture is based on the core IDF APIs, complemented by a C++ wrapper tailored for application use, guaranteeing non-blocking operation of all APIs. Each user action, including HTTP requests, MQTT publishing, Shadow updates, and OTA, is logged as an event and seamlessly managed in the background. To maintain this seamless operation, the platform effectively runs a system task.

This establishes a robust foundation for your upcoming IoT project.

Supported Kaa IoT features:

1. MQTT pub/sub
2. Telemetry updates
3. Metadata updates
4. Handling commands
5. OTA firmware updates

## Solutions
1. [ESP32 Kaa IoT Platform](https://buildstorm.com/solutions/esp32-kaa-iot-platform/)

## Blogs
1. [Kaa OTA Updates](https://buildstorm.com/blog/kaa-iot-ota-updates/)
2. [API Reference Guide]


## Example Setup
1. Follow [this article](https://buildstorm.com/solutions/kaa-iot-device-setup/) to setup Kaa IoT and generate application version and endpoint token.
2. Same will be used for all examples
3. Update the following WiFi and Kaa IoT parameters in `app_config.h` of the example

```
#define TEST_WIFI_SSID "Wifi-Router"
#define TEST_WIFI_PASSWORD "Wifi Passwors"

#define KAA_URI "mqtt://mqtt.cloud.kaaiot.com"
#define KAA_PORT 1883

#define KAA_APPLICATION_VERSION "Your Kaa device application version"
#define KAA_ENDPOINT_TOKEN "Endpoint Token"
```

---
---
## SoC Compatibility

| Name            | BLE           | OTA           |
|-----------------|---------------|---------------|
| ESP32           | Not Supported | Supported     |
| ESP32 S3        | Supported     | Supported     |

While our Arduino IoT platform currently supports BLE on ESP32S3, these capabilities are accessible on our ESP-IDF platform for all ESP32 SoCs. These functionalities will soon be integrated into Arduino. Until then, you can explore the IDF version of our platform [ESP32-idf Kaa IoT Platform](https://github.com/BuildStormTechnologies/esp32-idf-kaa-iot).

---
## Arduino setting
1. ESP32 settings

    ![esp32 arduino settings](<images/esp32 arduino settings.png>)
<br />
<br />
<br />

2. ESP32S3 settings

   ![esp32s3 arduino settings](<images/esp32s3 arduino settings.png>)

## Device Menu
1. On serial terminal, press `m` to display device menu


