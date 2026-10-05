#include <iostream>
#include <cstdlib>

class CapteurTemperature: public Capteur {

    public:
        CapteurTemperature(string nom, string unite) : Capteur(nom,unite) {}
        double lire() override = 15 + rand() % 21;
}