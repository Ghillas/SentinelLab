#include "bme280.hpp"

Bme280::Bme280(II2cBus& bus, uint8_t addr) : bus_(bus), addr_(addr) {}

bool Bme280::identifier() {
    const uint8_t reg = 0xD0;
    uint8_t id = 0;
    return bus_.writeRead(addr_, &reg, 1, &id, 1) && id == 0x60;
}

bool Bme280::readTemperature(float& temp) {
    uint8_t reg = 0xFA;
    uint8_t data[3] = {0};
    if (bus_.writeRead(addr_, &reg, 1, data, 3)) {
        int32_t raw = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
        temp = 20.0f + (static_cast<float>(raw % 300) / 10.0f);
        return true;
    }
    return false;
}
