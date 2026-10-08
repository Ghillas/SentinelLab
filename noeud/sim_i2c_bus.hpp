#pragma once
#include "ii2cbus.hpp"
#include <map>
#include <cstdint>
#include <cstddef>

class SimI2cBus : public II2cBus {
private:
    bool traceI2c_;
    std::map<uint8_t, std::map<uint8_t, uint8_t>> memoryMap_;

public:
    explicit SimI2cBus(bool traceI2c = false);
    
    void setRegister(uint8_t addr, uint8_t reg, uint8_t val);
    
    bool writeRead(uint8_t addr, const uint8_t* tx, size_t ntx,
                   uint8_t* rx, size_t nrx) override;
};
