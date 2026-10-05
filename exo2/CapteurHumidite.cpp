#include <iostream>
#include <cstdlib>

class CapteurHumidite : public Capteur {

    public:
        CapteurHumidite(string nom, string unite) : Capteur(nom,unite) {}
        double lire() override = (30 + rand( % 60)) / 100;
}