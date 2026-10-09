#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <cstdlib>
#include <csignal>
#include <atomic>
#include <cstdint>
#include "ring_buffer.hpp"
#include "sim_i2c_bus.hpp"
#include "bme280.hpp"

// Structure de mesure (TP2/TP3)
struct Mesure {
    float temp;
    uint32_t timestamp;
};

// Etats du noeud (TP2)
enum class EtatNoeud {
    Init,
    LectureCapteur,
    EnvoiDonnees,
    ModePanne
};

std::string etatToString(EtatNoeud e) {
    switch(e) {
        case EtatNoeud::Init: return "INIT";
        case EtatNoeud::LectureCapteur: return "LECTURE_CAPTEUR";
        case EtatNoeud::EnvoiDonnees: return "ENVOI_DONNEES";
        case EtatNoeud::ModePanne: return "MODE_PANNE";
    }
    return "INCONNU";
}

// Registres GPIO virtuels (TP4)
struct GpioRegs {
    volatile uint32_t IDR;
    volatile uint32_t ODR;
};
GpioRegs gpioSim{};
GpioRegs* GPIO = &gpioSim;
constexpr uint32_t LED = 1u << 5;

// Gestion du bouton via signal (TP4)
volatile std::sig_atomic_t appui = 0;
extern "C" void onBouton(int) { appui = 1; }

// Watchdog (TP4)
std::atomic<bool> heartbeatAlive{true};

// Affichage de la barre PWM (TP4)
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

// Calcul CRC8 (TP5)
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
    int panneDebut = -1;
    int panneFin = -1;

    // Analyse des arguments en ligne de commande
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--binaire") {
            modeBinaire = true;
        } else if (arg == "--trace-i2c") {
            traceI2c = true;
        } else if (arg == "--bloquer-a" && i + 1 < argc) {
            tickBlocage = std::stoi(argv[++i]);
        } else if (arg == "--panne" && i + 1 < argc) {
            std::string valeur = argv[++i];
            std::size_t pos = valeur.find(':');
            if (pos != std::string::npos) {
                try {
                    panneDebut = std::stoi(valeur.substr(0, pos));
                    panneFin = std::stoi(valeur.substr(pos + 1));
                } catch (const std::exception&) {
                    std::cerr << "Erreur format --panne debut:fin\n";
                    return 1;
                }
            }
        }
    }

    // Enregistrement du signal bouton (TP4)
    struct sigaction sa{};
    sa.sa_handler = onBouton;
    sigaction(SIGUSR1, &sa, nullptr);

    // Lancement du Watchdog anti-blocage (TP4)
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

    EtatNoeud etatActuel = EtatNoeud::Init;
    std::cout << "[TRANSITION] -> " << etatToString(etatActuel) << "\n";

    // Initialisation du bus I2C et du capteur BME280 (TP5)
    SimI2cBus bus(traceI2c);
    Bme280 capteur(bus);

    if (!capteur.identifier()) {
        std::cerr << "[Erreur] Capteur BME280 introuvable sur le bus I2C !\n";
        return 1;
    }

    etatActuel = EtatNoeud::LectureCapteur;
    std::cout << "[TRANSITION] Init -> " << etatToString(etatActuel) << "\n";

    RingBuffer<Mesure, 64> historiqueMesures; // TP3
    int tick = 0;

    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        tick++;

        heartbeatAlive = true; // Battement de cœur OK pour le watchdog

        // Simulation de blocage volontaire (TP4)
        if (tickBlocage != -1 && tick >= tickBlocage) {
            std::cout << "[t=" << tick << "] Simulation d'un blocage du nœud...\n";
            while (true) { std::this_thread::sleep_for(std::chrono::milliseconds(500)); }
        }

        // Gestion de l'appui bouton (TP4)
        if (appui) {
            appui = 0;
            GPIO->ODR ^= LED;
        }

        // Vérification de l'injection de panne (TP2)
        bool enPanne = (panneDebut != -1 && panneFin != -1 && tick >= panneDebut && tick <= panneFin);
        if (enPanne && etatActuel != EtatNoeud::ModePanne) {
            etatActuel = EtatNoeud::ModePanne;
            std::cout << "[TRANSITION] -> " << etatToString(etatActuel) << " (Panne injectée)\n";
        } else if (!enPanne && etatActuel == EtatNoeud::ModePanne) {
            etatActuel = EtatNoeud::LectureCapteur;
            std::cout << "[TRANSITION] -> " << etatToString(etatActuel) << " (Retour à la normale)\n";
        }

        // Exécution de la machine à états (TP2 / TP3 / TP5)
        switch (etatActuel) {
            case EtatNoeud::LectureCapteur: {
                float temp = 0.0f;
                if (capteur.readTemperature(temp)) {
                    Mesure m{temp, static_cast<uint32_t>(tick)};
                    historiqueMesures.push(m); // Stockage TP3

                    etatActuel = EtatNoeud::EnvoiDonnees;
                } else {
                    std::cerr << "[Erreur] Lecture capteur BME280 échouée\n";
                }
                break;
            }
            case EtatNoeud::EnvoiDonnees: {
                float temp = 0.0f;
                if (capteur.readTemperature(temp)) {
                    if (modeBinaire) {
                        // Envoi trame binaire TP5 (--binaire)
                        uint8_t sync = 0xAA;
                        uint8_t crc = calculerCRC8(reinterpret_cast<const uint8_t*>(&temp), sizeof(temp));

                        std::cout.write(reinterpret_cast<char*>(&sync), 1);
                        std::cout.write(reinterpret_cast<const char*>(&temp), sizeof(temp));
                        std::cout.write(reinterpret_cast<char*>(&crc), 1);
                        std::cout.flush();
                    } else {
                        // Affichage texte normal (TP4)
                        float pwmRapport = ((temp - 20.0f) / 30.0f) * 100.0f;
                        if (pwmRapport < 0.0f) pwmRapport = 0.0f;
                        if (pwmRapport > 100.0f) pwmRapport = 100.0f;

                        bool ledEtat = (GPIO->ODR & LED) != 0;
                        std::cout << "[t=" << tick << "] Temp: " << temp << "°C | Buffer: " << historiqueMesures.size() 
                                  << " | LED " << (ledEtat ? "ON " : "OFF") << " | PWM ";
                        afficherBarrePWM(pwmRapport);
                        std::cout << "\n";
                    }
                }
                etatActuel = EtatNoeud::LectureCapteur;
                break;
            }
            case EtatNoeud::ModePanne: {
                std::cout << "[t=" << tick << "] Mode Panne actif : aucune donnée traitée.\n";
                break;
            }
            default:
                break;
        }
    }

    return 0;
}