#include <iostream>

using namespace std;

class Led {
    private : 
        bool allumee;
        int pin;
    public :

        Led(int p) : pin(p), allumee(false) {}

        ~Led() {
            allumee = false;
            cout << "[LED pin " << this -> pin << "] liberre" << endl;
        }

        void allumer() {
            allumee = true;
            cout << "[LED pin " << this -> pin << "] ON" << endl;
        }
        void eteindre() {
            allumee = false;
            cout << "[LED pin " << this -> pin << "] OFF" << endl;
        }
        void basculer() {
            if (allumee) {
                eteindre();
            } else {
                allumer();
            }
        }
        void estAllumee() const {
            return allumee;
        }
};


int main() {
    Led l1 = Led(5);
    return 0;
}