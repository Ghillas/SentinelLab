#include <string>

class Capteur {
    protected :
        string: nom;
        string: unite;
        double: derniereValeur;
    public: 
    Capteur(string n, string u) : nom(n), unite(u) {}
        virtual double lire() = 0;
        string getNom() {
            return this -> nom;
        }

        string getUnite() {
            return (*this).unite
        }

        double getDerniereValeur() {
            return this -> derniereValeur;
        }

        virtual ~Capteur() {

        }
}