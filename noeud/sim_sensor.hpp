#pragma once
#include "isensor.hpp"

class SimSensor final : public ISensor {
  float t_ = 20.0f;
  bool panne_ = false;
public:
  void injecterPanne(bool p) { panne_ = p; }   // pilotable depuis la console
  bool begin() override { return true; }
  bool read(Mesure& m) override {
    if (panne_) return false;
    t_ += 0.1f;
    m = { t_, 45.0f, 1013.0f, 0 };
    return true;
  }
};