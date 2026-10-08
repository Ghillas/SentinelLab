// crc16.hpp : CRC-16/MODBUS (polynôme 0xA001 réfléchi, valeur initiale 0xFFFF).
// La table de 256 entrées est calculée PAR LE COMPILATEUR (consteval) :
// elle est rangée en mémoire morte, aucun calcul au démarrage.
#pragma once
#include <array>
#include <cstdint>
#include <span>

namespace modbus {

consteval std::array<uint16_t, 256> tableCrc() {
  std::array<uint16_t, 256> t{};
  // TODO C1 : pour chaque i de 0 à 255, partir de crc = i et appliquer 8 fois : si le bit 0 vaut 1, crc = (crc >> 1) ^ 0xA001, sinon crc = crc >> 1
  for (uint16_t i = 0; i < 256; ++i) {
    uint16_t crc = i;
    for (int j = 0; j < 8; ++j) {
      if (crc & 1) {
        crc = (crc >> 1) ^ 0xA001;
      } else {
        crc >>= 1;
      }
    }
    t[i] = crc;
  }
  return t;
}

inline constexpr std::array<uint16_t, 256> TABLE_CRC = tableCrc();

// CRC d'une suite d'octets. constexpr : utilisable aussi à la compilation.
constexpr uint16_t crc16(std::span<const uint8_t> octets) {
  uint16_t crc = 0xFFFF;
  // TODO C2 : pour chaque octet o, crc = (crc >> 8) ^ TABLE_CRC[(crc ^ o) & 0xFF]
  for (uint8_t o : octets) {
    crc = (crc >> 8) ^ TABLE_CRC[(crc ^ o) & 0xFF];
  }
  return crc;
}

// TODO C2 : vérifier À LA COMPILATION l'exemple de la spécification : 01 03 00 00 00 0A -> CRC 0xCDC5 (octets C5 CD)
static_assert(crc16(std::array<uint8_t, 6>{
        0x01, 0x03, 0x00, 0x00, 0x00, 0x0A
    }) == 0xCDC5);

}  // namespace modbus
