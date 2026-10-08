#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <cstdlib>
#include "sim_sensor.hpp"

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

int main(int argc, char* argv[]) {
    int panneDebut = -1;
    int panneFin = -1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--panne") {
            if (i + 1 >= argc) {
                std::cerr << "Erreur : --panne attend une valeur debut:fin\n";
                return 1;
            }
            std::string valeur = argv[++i];
            std::size_t pos = valeur.find(':');
            if (pos == std::string::npos) {
                std::cerr << "Erreur : format attendu debut:fin\n";
                return 1;
            }
            try {
                panneDebut = std::stoi(valeur.substr(0, pos));
                panneFin = std::stoi(valeur.substr(pos + 1));
            }
            catch (const std::exception&) {
                std::cerr << "Erreur : debut et fin doivent être des nombres\n";
                return 1;
            }
        }
    }

    EtatNoeud etatActuel = EtatNoeud::Init;
    SimSensor capteur;
    Mesure mesure;
    int tick = 0;

    std::cout << "[TRANSITION] -> " << etatToString(etatActuel) << "\n";
    if (capteur.begin()) {
        etatActuel = EtatNoeud::LectureCapteur;
        std::cout << "[TRANSITION] Init -> " << etatToString(etatActuel) << "\n";
    }

    while (tick < 10) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        tick++;
        std::cout << "\n--- Tick " << tick << " ---\n";

        bool enPanne = (panneDebut != -1 && panneFin != -1 && tick >= panneDebut && tick <= panneFin);
        capteur.injecterPanne(enPanne);

        if (enPanne && etatActuel != EtatNoeud::ModePanne) {
            etatActuel = EtatNoeud::ModePanne;
            std::cout << "[TRANSITION] -> " << etatToString(etatActuel) << " (Panne injectée)\n";
        } else if (!enPanne && etatActuel == EtatNoeud::ModePanne) {
            etatActuel = EtatNoeud::LectureCapteur;
            std::cout << "[TRANSITION] -> " << etatToString(etatActuel) << " (Retour à la normale)\n";
        }

        switch (etatActuel) {
            case EtatNoeud::LectureCapteur: {
                if (capteur.read(mesure)) {
                    std::cout << "Capteur OK -> Temp: " << mesure.temp << "°C\n";
                    etatActuel = EtatNoeud::EnvoiDonnees;
                    std::cout << "[TRANSITION] LectureCapteur -> EnvoiDonnees\n";
                } else {
                    std::cout << "Erreur de lecture du capteur !\n";
                }
                break;
            }
            case EtatNoeud::EnvoiDonnees: {
                std::cout << "Envoi de la trame Modbus vers la passerelle...\n";
                etatActuel = EtatNoeud::LectureCapteur;
                std::cout << "[TRANSITION] EnvoiDonnees -> LectureCapteur\n";
                break;
            }
            case EtatNoeud::ModePanne: {
                std::cout << "Noeud en panne, aucune donnée envoyée\n";
                break;
            }
            default:
                break;
        }
    }

    return 0;
}
