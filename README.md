# Arduino ESP32 KAA IoT
The Buildstorm platform simplifies the integration of IoT core features onto ESP32 Hardware, spanning from basic IoT functionalities to advanced features like OTA updates.

The architecture is based on the core IDF APIs, complemented by a C++ wrapper tailored for application use, guaranteeing non-blocking operation of all APIs. Each user action, including HTTP requests, MQTT publishing, Telemetry/Commands, and OTA, is logged as an event and seamlessly managed in the background. To maintain this seamless operation, the platform effectively runs a system task.

This establishes a robust foundation for your upcoming IoT project.

Supported features:

1. Telemetry update
2. Metadata
3. Commands
4. Web OTA
5. Local websever OTA
6. BLE Device Provisioning

## Solution

1. [KAA IoT Platform](https://buildstorm.com/solutions/kaa-iot-platform/)

## References

1. [KAA IoT Platform](https://buildstorm.com/solutions/kaa-iot-platform/)
2. [KAA IoT Commands](https://buildstorm.com/blog/kaa-iot-commands/)
3. [OTA Updates](https://buildstorm.com/blog/kaa-iot-ota-updates/)

---
## Example Setup
1. Update the following WiFi and KAA parameters in `app_config.h` of the example
2. This configuration will be same accross all examples

```
#define TEST_WIFI_SSID "YOUR WIFI SSID"
#define TEST_WIFI_PASSWORD "YOUR PASSWORD"

#define KAA_URI "mqtt://mqtt.cloud.kaaiot.com"
#define KAA_PORT 1883

#define KAA_APPLICATION_VERSION "your application version"
#define KAA_ENDPOINT_TOKEN "your endpoint token"
```

---
## SoC Compatibility

| Name            | BLE           | OTA           |
|-----------------|---------------|---------------|
| ESP32           | Not Supported | Supported     |
| ESP32 S3        | Supported     | Supported     |

While our Arduino IoT platform currently supports BLE solely on ESP32S3, these capabilities are accessible on our ESP-IDF platform for all ESP32 SoCs. These functionalities will soon be integrated into Arduino. Until then, you can explore the IDF version of our platform [KAA IoT Platform](https://github.com/BuildStormTechnologies/esp32-kaa-iot).

---
## Arduino setting
1. ESP32 settings

    ![esp32 arduino settings](<images/esp32 arduino settings.png>)
<br />
<br />
<br />

2. ESP32S3 settings

   ![esp32s3 arduino settings](<images/esp32s3 arduino settings.png>)


