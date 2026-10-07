// gpio.hpp : registres GPIO simulés et opérations sur les bits.
// Sur une vraie carte, seule la ligne qui définit GPIO change :
//   inline GpioRegs* const GPIO = reinterpret_cast<GpioRegs*>(0x40020000);
#pragma once
#include <cstdint>

struct GpioRegs {
  volatile uint32_t IDR;   // 0x00 : entrées (lecture)
  volatile uint32_t ODR;   // 0x04 : sorties (lecture / écriture)
};

extern GpioRegs gpio_sim;            // défini dans noeud.cpp
inline GpioRegs* const GPIO = &gpio_sim;

constexpr uint32_t PIN_LED = 5;      // LED d'état sur la broche 5

// Les quatre opérations de base. La lecture-modification-écriture est écrite
// en clair (r = r | m) : elle n'est PAS atomique.
inline void bit_set(volatile uint32_t& r, uint32_t n) {
  // TODO B1 : mettre le bit n à 1
  r |= (1UL << n);
  //r = r | (1u << n);
}
inline void bit_clear(volatile uint32_t& r, uint32_t n) {
  // TODO B1 : mettre le bit n à 0
  r &= ~(1UL << n);
}
inline void bit_toggle(volatile uint32_t& r, uint32_t n) {
  // TODO B1 : inverser le bit n
  r ^= (1UL << n);
}
inline bool bit_test(const volatile uint32_t& r, uint32_t n) {
  // TODO B1 : renvoyer true si le bit n vaut 1
  //(void)r; (void)n; return false;
  return (r & (1UL << n)) != 0;
}
