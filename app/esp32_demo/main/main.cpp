#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sl_lidar.h"
#include "sl_lidar_driver.h"

using namespace sl;

extern "C" void app_main() {
    printf("RPLIDAR SDK ESP32 Demo\n");

    // Initialize the LIDAR driver
    ILidarDriver * lidar = *createLidarDriver();
    if (!lidar) {
        printf("Failed to create LIDAR driver\n");
        return;
    }

    // Create a serial port channel
    // UART_NUM_1 is used as an example, user should adjust pins if necessary
    IChannel* channel = *createSerialPortChannel("1", 115200);

    if (SL_IS_OK(lidar->connect(channel))) {
        sl_lidar_response_device_info_t deviceInfo;
        if (SL_IS_OK(lidar->getDeviceInfo(deviceInfo))) {
            printf("Model: %d, Firmware Version: %d.%d, Hardware Version: %d\n",
                deviceInfo.model,
                deviceInfo.firmware_version >> 8, deviceInfo.firmware_version & 0xffu,
                deviceInfo.hardware_version);
        } else {
            printf("Failed to get device info\n");
        }

        lidar->disconnect();
    } else {
        printf("Failed to connect to LIDAR\n");
    }

    delete lidar;
    delete channel;

    printf("Demo finished\n");
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
