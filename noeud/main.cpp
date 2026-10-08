#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>
#include <cstdint>
#include <string>
#include "sim_i2c_bus.hpp"
#include "bme280.hpp"

//================================================ TP4

struct GpioRegs {                      // même disposition qu'un vrai périphérique
    volatile uint32_t IDR;               // entrées
    volatile uint32_t ODR;               // sorties
};
GpioRegs gpioSim{};                    // sur cible : reinterpret_cast<GpioRegs*>(0x40020000)
GpioRegs* const GPIO = &gpioSim;
constexpr uint32_t LED = 1u << 5;

volatile std::sig_atomic_t appui = 0;
extern "C" void onBouton(int) { appui = 1; }   // « ISR » : une seule écriture

std::atomic<bool> heartbeatAlive{true};

void afficherBarrePWM(float pourcentage) {
    int totalBars = 10;
    int activeBars = static_cast<int>((pourcentage / 100.0f) * totalBars);
    if (activeBars > totalBars) activeBars = totalBars;
    if (activeBars < 0) activeBars = 0;

    std::cout << "[";
    for (int i = 0; i < totalBars; ++i) {
        std::cout << (i < activeBars ? "#" : "-");
    }
    std::cout << "] " << static_cast<int>(pourcentage) << " %";
}


//================================================ TP5

uint8_t calculerCRC8(const uint8_t* data, size_t length) {
    uint8_t crc = 0x00;
    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; ++j) {
            if (crc & 0x80) crc = (crc << 1) ^ 0x07;
            else crc <<= 1;
        }
    }
    return crc;
}

int main(int argc, char* argv[]) {
    bool modeBinaire = false;
    bool traceI2c = false;
    int tickBlocage = -1;

    // Analyse des arguments en ligne de commande 
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--binaire") {
            modeBinaire = true;
        } else if (arg == "--trace-i2c") {
            traceI2c = true;
        } else if (arg == "--bloquer-a" && i + 1 < argc) {
            tickBlocage = std::stoi(argv[i + 1]);
        }
    }

    struct sigaction sa{};
    sa.sa_handler = onBouton;
    sigaction(SIGUSR1, &sa, nullptr);

    // Lancement du Watchdog (TP4)
    std::thread watchdog([]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (!heartbeatAlive) {
                std::cerr << "[watchdog] aucun battement depuis 3 s : abort\n";
                std::abort();
            }
            heartbeatAlive = false;
        }
    });
    watchdog.detach();

    // Initialisation du bus I2C et du capteur BME280 (TP5)
    SimI2cBus bus(traceI2c);
    Bme280 capteur(bus);

    if (!capteur.identifier()) {
        std::cerr << "[Erreur] Capteur BME280 introuvable sur le bus I2C !\n";
        return 1;
    }

    int tick = 0;

    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        tick++;

        heartbeatAlive = true;

        if (tickBlocage != -1 && tick >= tickBlocage) {
            std::cout << "[t=" << tick << "] Simulation d'un blocage du nœud...\n";
            while (true) { std::this_thread::sleep_for(std::chrono::milliseconds(500)); }
        }

        if (appui) {
            appui = 0;
            GPIO->ODR ^= LED;
        }

        // Lecture de la température (TP5)
        float temp = 0.0f;
        if (!capteur.readTemperature(temp)) {
            continue;
        }

        if (modeBinaire) {
            uint8_t sync = 0xAA;
            uint8_t crc = calculerCRC8(reinterpret_cast<const uint8_t*>(&temp), sizeof(temp));

            std::cout.write(reinterpret_cast<char*>(&sync), 1);
            std::cout.write(reinterpret_cast<const char*>(&temp), sizeof(temp));
            std::cout.write(reinterpret_cast<char*>(&crc), 1);
            std::cout.flush();
        } else {
            // Affichage texte PWM et LED (TP4)
            float pwmRapport = ((temp - 20.0f) / 30.0f) * 100.0f;
            if (pwmRapport < 0.0f) pwmRapport = 0.0f;
            if (pwmRapport > 100.0f) pwmRapport = 100.0f;

            bool ledEtat = (GPIO->ODR & LED) != 0;
            std::cout << "[t=" << tick << "] Capteur OK -> Temp: " << temp << "°C | LED " << (ledEtat ? "ON " : "OFF") << "  PWM ";
            afficherBarrePWM(pwmRapport);
            std::cout << "\n";
        }
    }

    return 0;
}