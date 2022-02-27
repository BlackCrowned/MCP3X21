/*
MIT License

Copyright (c) 2018 Pavel Slama

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#if defined(ARDUINO)
#include <Wire.h>
#elif defined(IDF_VER)
#include <driver/i2c.h>
#else
#error Not Supported
#endif
#include "MCP3X21.h"

MCP3X21::MCP3X21(uint8_t slave_adr):
    _address(slave_adr) {
}

MCP3X21::~MCP3X21(void) {
    #ifdef ARDUINO
    _i2c->~TwoWire();
    #endif
}

#if defined(ARDUINO)
void MCP3X21::init(TwoWire * i2c_obj) {
    _i2c = i2c_obj;
}
#elif defined(IDF_VER)
void MCP3X21::init(i2c_port_t i2c_port) {
    _i2c_port = i2c_port;
}
#endif

void MCP3X21::init() {
    #ifdef ARDUINO
    _i2c = &Wire;
    #endif
}

uint16_t MCP3021::read() {
    #if defined(ARDUINO)
    _i2c->requestFrom(_address, 2U);

    if (_i2c->available() == 2) {
        return ((_i2c->read() << 6) | (_i2c->read() >> 2));
    }
    #elif defined(IDF_VER)
        uint8_t bytes[2];
        auto err = ESP_OK;
        auto cmd = i2c_cmd_link_create();
        err |= i2c_master_start(cmd);
        err |= i2c_master_write_byte(cmd, _address, true);
        err |= i2c_master_read_byte(cmd, bytes + 1, i2c_ack_type_t::I2C_MASTER_ACK);
        err |= i2c_master_read_byte(cmd, bytes + 0, i2c_ack_type_t::I2C_MASTER_NACK);
        err |= i2c_master_stop(cmd);
        err |= i2c_master_cmd_begin(_i2c_port, cmd, 0);
        i2c_cmd_link_delete(cmd);
        if (err == ESP_OK) {
            return bytes[1] << 6 | bytes[0] >> 2; 
        }
    #endif

    return 0xFFFF;
}

uint16_t MCP3221::read() {
    #if defined(ARDUINO)
    _i2c->requestFrom(_address, 2U);

    if (_i2c->available() == 2) {
        return ((_i2c->read() << 8) | _i2c->read());
    }
    #elif defined(IDF_VER)
        uint8_t bytes[2];
        auto err = ESP_OK;
        auto cmd = i2c_cmd_link_create();
        err |= i2c_master_start(cmd);
        err |= i2c_master_write_byte(cmd, _address, true);
        err |= i2c_master_read_byte(cmd, bytes + 1, i2c_ack_type_t::I2C_MASTER_ACK);
        err |= i2c_master_read_byte(cmd, bytes + 0, i2c_ack_type_t::I2C_MASTER_NACK);
        err |= i2c_master_stop(cmd);
        err |= i2c_master_cmd_begin(_i2c_port, cmd, 0);
        i2c_cmd_link_delete(cmd);
        if (err == ESP_OK) {
            return bytes[1] << 6 | bytes[0] >> 2;
        }
    #endif

    return 0xFFFF;
}

float MCP3021::readVoltage(float vref) {
    return toVoltage(read(), vref);
}

float MCP3221::readVoltage(float vref) {
    return toVoltage(read(), vref);
}

MCP3021::MCP3021(uint8_t slave_adr):
    MCP3X21(slave_adr) {
}

float MCP3021::toVoltage(uint16_t data, float vref) {
    return (vref * data / 1024);
}

MCP3221::MCP3221(uint8_t slave_adr):
    MCP3X21(slave_adr) {
}

float MCP3221::toVoltage(uint16_t data, float vref) {
    return (vref * data / 4096);
}
