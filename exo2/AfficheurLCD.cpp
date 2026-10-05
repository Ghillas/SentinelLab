#include <iostream>

using namespace std;

class AfficheurLCD : public Afficheur {
    public :
        void afficher(const string& nom, double valeur, const string& unite) override {
            cout << nom << valeur << unite << endl;
        }
}


int main() {
    vector<unique_ptr<Capteur>> capteurs;
    capteurs.push_back(make_unique<CapteurTemperature>());
    capteurs.push_back(make_unique<CapteurHumidite>());
    capteurs.push_back(make_unique<CapteurLuminosite>());

    // 2. Instanciation des afficheurs via l'interface (polymorphisme)
    unique_ptr<Afficheur> affConsole = make_unique<AfficheurConsole>();
    unique_ptr<Afficheur> affLCD = make_unique<AfficheurLCD>();

    // 3. Test de l'afficheur Console
    cout << "--- Affichage sur Console ---\n";
    affConsole->effacer();
    for (const auto& c : capteurs) {
        double val = c->lire();
        affConsole->afficher(c->getNom(), val, c->getUnite());
    }

    cout << "\n--- Affichage sur ecran LCD simule (16x2) ---\n";
    affLCD->effacer();
    for (const auto& c : capteurs) {
        double val = c->lire();
        affLCD->afficher(c->getNom(), val, c->getUnite());
    }
    affLCD->effacer();
    return 0;
}