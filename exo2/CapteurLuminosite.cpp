#include <iostream>
#include <cstdlib>

class CapteurLuminosite : public Capteur {

    public:
        CapteurLuminosite(string nom, string unite) : Capteur(nom,unite) {}
        double lire() override = rand() % 1001;
}