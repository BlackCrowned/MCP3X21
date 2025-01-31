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

#ifndef MCP3X21_H
#define MCP3X21_H

#ifdef IDF_VER
#include <driver/i2c.h>
#endif

#define MCP3X21_DEFAULT_ADDRESS 0x4D

class MCP3X21 {
  public:
    explicit MCP3X21(uint8_t slave_adr);
    ~MCP3X21(void);
    #if defined(ARDUINO)
    void init(TwoWire * i2c_obj);
    #elif defined(IDF_VER)
    void init(i2c_port_t i2c_port);
    #endif
    void init();
    
  protected:
    uint16_t read();
    #if defined(ARDUINO)
    TwoWire * _i2c;
    #elif defined(IDF_VER)
    i2c_port_t _i2c_port = I2C_NUM_0;
    #endif
    const uint8_t _address;
    
};

class MCP3021 : public MCP3X21 {
  public:
    uint16_t read();
    float readVoltage(float vref);
    explicit MCP3021(uint8_t slave_adr = MCP3X21_DEFAULT_ADDRESS);
    float toVoltage(uint16_t data, float vref);
};

class MCP3221 : public MCP3X21 {
  public:
    uint16_t read();
    float readVoltage(float vref);
    explicit MCP3221(uint8_t slave_adr = MCP3X21_DEFAULT_ADDRESS);
    float toVoltage(uint16_t data, float vref);
};

#endif  // MCP3X21_H
