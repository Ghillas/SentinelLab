#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>
#include <cstdint>
#include <string>

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

int main(int argc, char* argv[]) {
    int tickBlocage = -1;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--bloquer-a" && i + 1 < argc) {
            tickBlocage = std::stoi(argv[i + 1]);
        }
    }

    struct sigaction sa{};
    sa.sa_handler = onBouton;
    sigaction(SIGUSR1, &sa, nullptr);

    std::thread watchdog([]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            if (!heartbeatAlive) {
                std::cerr << "[watchdog] aucun battement depuis 3 s : abort\n";
                std::abort();
            }
            heartbeatAlive = false;
        }
    });
    watchdog.detach();

    int tick = 0;
    float temperatureSimulee = 25.0f; 

    std::cout << "[t=" << tick << "] Noeud démarré (PID: " << getpid() << ")\n";

    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        tick++;

        heartbeatAlive = true;

        if (tickBlocage != -1 && tick >= tickBlocage) {
            std::cout << "[t=" << tick << "] Simulation d'un blocage du noeud...\n";
            while (true) { 
                std::this_thread::sleep_for(std::chrono::seconds(1)); // Boucle infinie bloquante
            }
        }

        if (appui) {
            appui = 0;
            GPIO->ODR ^= LED;
        }

        bool ledAllumee = (GPIO->ODR & LED) != 0;

        if (ledAllumee) {
            temperatureSimulee += 0.5f;
        } else {
            temperatureSimulee -= 0.5f; 
        }

        // Bornes strictes sans dépassement
        if (temperatureSimulee > 50.0f) temperatureSimulee = 50.0f;
        if (temperatureSimulee < 20.0f) temperatureSimulee = 20.0f;

        /*temperatureSimulee += 0.5f;
        if (temperatureSimulee > 50.0f) temperatureSimulee = 20.0f;
*/
        float pwmRapport = ((temperatureSimulee - 20.0f) / 30.0f) * 100.0f;
        if (pwmRapport < 0.0f) pwmRapport = 0.0f;
        if (pwmRapport > 100.0f) pwmRapport = 100.0f;

        bool ledEtat = (GPIO->ODR & LED) != 0;
        std::cout << "[t=" << tick << "] IRQ bouton -> LED " << (ledEtat ? "ON " : "OFF") << "  PWM ";
        afficherBarrePWM(pwmRapport);
        std::cout << "\n";
    }

    return 0;
}