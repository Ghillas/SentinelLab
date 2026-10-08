#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

using namespace std::chrono;
constexpr auto PERIODE = milliseconds{500};

int main() {
  bool led = false;
  auto prochain = steady_clock::now();
  for (uint32_t tick = 0; tick < 20; ++tick) {
    led = !led;
    std::cout << "[" << tick << "] LED " << (led ? "ON " : "off") << '\n';
    prochain += PERIODE;                    // échéance absolue : pas de dérive
    std::this_thread::sleep_until(prochain);
  }
}


/*#include <iostream>
#include <chrono>
#include <thread>

int main() {
    while (true) {
        std::cout << "Noeud actif " << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}*/
