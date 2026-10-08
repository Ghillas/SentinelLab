#pragma once
#include "ii2cbus.hpp"
#include <cstdint>

class Bme280 {
private:
    II2cBus& bus_;
    uint8_t addr_;

public:
    explicit Bme280(II2cBus& bus, uint8_t addr = 0x76);
    
    bool identifier();
    bool readTemperature(float& temp);
};
