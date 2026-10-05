#include <iostream>

using namespace std;

class AfficheurConsole : public Afficheur {
    public :
        void afficher(const string& nom, double valeur, const string& unite) override {
            cout << nom << valeur << unite << endl;
        }
}