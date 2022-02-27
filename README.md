# Arduino/ESP-IDF library for MCP3021 & MCP3221 I2C ADC
[![build](https://github.com/pilotak/MCP3X21/workflows/build/badge.svg)](https://github.com/pilotak/MCP3X21/actions) 
[![Framework Badge Arduino](https://img.shields.io/badge/framework-arduino-00979C.svg)](https://arduino.cc)

## Example
```cpp
#include <Wire.h>
#include "MCP3X21.h"

const uint8_t address = 0x4D;
const uint16_t ref_voltage = 3300;  // in mV

MCP3021 mcp3021(address);

void setup() {
    Serial.begin(115200);

#if defined(ESP8266)
    Wire.begin(SDA, SCL);
    mcp3021.init(&Wire);
#else
    mcp3021.init();
#endif
}

void loop() {
    uint16_t result = mcp3021.read();

    Serial.print(F("ADC: "));
    Serial.print(result);
    Serial.print(F(", mV: "));
    Serial.println(mcp3021.toVoltage(result, ref_voltage));

    delay(1000);
}
```

## Example setup for ESP-IDF
```cpp
#include <driver/i2c.h>

void setup() {
    i2c_config_t config = {};
    config = i2c_config_t{
        .mode = i2c_mode_t::I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {.clk_speed = 100000},
        .clk_flags = 0
    };
    i2c_param_config(I2C_NUM_0, &config);
    i2c_driver_install(I2C_NUM_0, i2c_mode_t::I2C_MODE_MASTER, 0, 0, ESP_INTR_FLAG_LEVEL3);
}
```