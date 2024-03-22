# arduino-esp32-iot-platforms-Thing-pub-sub

## Solutions
1. [ESP32 Kaa IoT Platform](https://buildstorm.com/solutions/esp32-kaa-iot-platform/)

## Blogs
1. [Kaa OTA Updates](https://buildstorm.com/blog/kaa-ota-updates/)
2. [API Reference Guide]

## Requirements

1. [Download](https://www.arduino.cc/en/software) and install the Arduino IDE for windows, or you may follow the official instructions [here](https://docs.arduino.cc/software/ide-v2/tutorials/getting-started/ide-v2-downloading-and-installing/) for other OS.

---

## Example Setup
1. Follow [this article](https://buildstorm.com/blog/kaa-iot-device-provisioning-setup/) to setup Kaa IoT and generate Provision Device key and Provision Device Secret.
2. Same will be used for all examples
3. Copy the generated keys to `app_config.h` of the example
4. Update the following WiFi and Kaa IoT parameters in `app_config.h` of the example

```
#define TEST_WIFI_SSID "desk"
#define TEST_WIFI_PASSWORD "12345678"

#define KAA_URI "mqtt://mqtt.cloud.kaaiot.com"
#define KAA_PORT 1883

#define KAA_APPLICATION_VERSION "Your Kaa device application version"
#define KAA_ENDPOINT_TOKEN "Endpoint Token"
```

---

## Building the project

1. Open the `Arduino IDE`
2. Navigate to `File` and Select `Open` or press `Ctrl+O` on keyboard
3. Copy all the needed parameters to `app_config.h`
4. Click on `Verify` to compile the sketch
5. Click on `Upload` to upload the sketch to device


---

## Testing
1. On first boot, device will register itself and reboot
2. Open the [Kaa IoT console](https://kaa.cloud/home), and in the left menu, choose Entities, devices to open the attributes.
3. Following blog shows subscribe/publish functionality [Kaa IoT Device Provisioning Setup](https://buildstorm.com/blog/kaa-iot-device-provisioning-setup/)
4. Testing OTA
   1. It requires files to be uploaded and firmware to assigned to the device . Please follow the link [Kaa OTA Updates](https://buildstorm.com/blog/kaa-ota-updates/)

---

## Device Menu
1. On serial terminal, press `m` to display device menu


