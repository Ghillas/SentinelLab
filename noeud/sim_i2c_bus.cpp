#include "sim_i2c_bus.hpp"
#include <iostream>
#include <iomanip>

SimI2cBus::SimI2cBus(bool traceI2c) : traceI2c_(traceI2c) {
    // Initialisation du capteur BME280 simulé à l'adresse 0x76
    memoryMap_[0x76][0xD0] = 0x60; // Chip ID correct
    memoryMap_[0x76][0xFA] = 0x50; // Donnée brute température
    memoryMap_[0x76][0xFB] = 0x30;
    memoryMap_[0x76][0xFC] = 0x00;
}

void SimI2cBus::setRegister(uint8_t addr, uint8_t reg, uint8_t val) {
    memoryMap_[addr][reg] = val;
}

bool SimI2cBus::writeRead(uint8_t addr, const uint8_t* tx, size_t ntx,
                          uint8_t* rx, size_t nrx) {
    if (ntx == 0) return false;
    
    uint8_t reg = tx[0];

    if (traceI2c_) {
        std::cout << "I2C S 0x" << std::hex << (int)addr << std::dec << " W [";
        for (size_t i = 0; i < ntx; ++i) {
            std::cout << std::hex << (int)tx[i] << (i + 1 < ntx ? " " : "");
        }
        std::cout << "]";
        if (nrx > 0) {
            std::cout << " Sr 0x" << std::hex << (int)addr << std::dec << " R [";
            uint8_t val = memoryMap_[addr][reg];
            std::cout << std::hex << (int)val << std::dec << "]";
        }
        std::cout << " P\n";
    }

    if (nrx > 0) {
        for (size_t i = 0; i < nrx; ++i) {
            rx[i] = memoryMap_[addr][reg + (uint8_t)i];
        }
    }
    return true;
}
