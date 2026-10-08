#pragma once
#include <cstdint>
#include <cstddef>

class II2cBus {
public:
    virtual ~II2cBus() = default;
    virtual bool writeRead(uint8_t addr, const uint8_t* tx, size_t ntx, uint8_t* rx, size_t nrx) = 0;
};
