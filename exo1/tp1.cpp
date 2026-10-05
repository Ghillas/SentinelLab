#include <iostream>
#include <vector>
#include <string>

using namespace std;

double moyenne(vector<int> notes) {
    double total = 0;
    unsigned int lenght = notes.size();
    if (lenght == 0) {
        return 0.0;
    }
    for(unsigned i = 0; i < lenght; i++) {
        total += notes[i];
    }
    return total / lenght;
}

int minNotes(vector<int> notes) {
    unsigned int lenght = notes.size();
    if (lenght == 0) {
        return 0;
    } else {
        int min = notes[0];
        for(unsigned i = 1; i < lenght; i++) {
            if (notes[i] < min) {
                min = notes[i];
            }
        }
        return min;
    }
}

int maxNotes(vector<int> notes) {
    unsigned int lenght = notes.size();
    if (lenght == 0) {
        return 0;
    } else {
        int max = notes[0];
        for(unsigned i = 1; i < lenght; i++) {
            if (notes[i] > max) {
                max = notes[i];
            }
        }
        return max;
    }
}

string mention(int note) {
    switch(note) {
        case 0 ... 9:
            return "Ajourné";
        case 10 ... 11:
            return "Passable";
        case 12 ... 13:
            return "Assez bien";
        case 14 ... 15:
            return "Bien";
        case 16 ... 20:
            return "Très bien";
        default:
            return "Note invalide";
    }
}

int nombreAdmis(vector<int> notes) {
    unsigned int lenght = notes.size();
    int count = 0;
    for(unsigned i = 0; i < lenght; i++) {
        if (notes[i] >= 10) {
            count++;
        }
    }
    return count;
}

vector<int> inverser(vector<int> notes) {
    unsigned int lenght = notes.size();
    unsigned int i = 0;
    while(i < lenght / 2) {
        int temp = notes[i];
        notes[i] = notes[lenght - 1 - i];
        notes[lenght - 1 - i] = temp;
        i++;
    }
    return notes;
}

vector<int> noteAdmis(vector<int> notes) {
    vector<int> notesAdmis;
    unsigned int lenght = notes.size();
    for(unsigned i = 0; i < lenght; i++) {
        if (notes[i] >= 10) {
            notesAdmis.push_back(notes[i]);
        }
    }
    return notesAdmis;
}

int main() {
    vector <int> notes = {12, 7, 15, 9, 18, 10, 4, 14};
    cout << moyenne(notes) << endl;
    cout << minNotes(notes) << endl;
    cout << maxNotes(notes) << endl;
    cout << mention(18) << endl;
    cout << nombreAdmis(notes) << endl;
    cout << "Notes inversées : ";
    vector<int> notesInverses = inverser(notes);
    for(unsigned i = 0; i < notesInverses.size(); i++) {
        cout << notesInverses[i] << " ";
    } 
    cout << endl;
    vector<int> notesAdmis = noteAdmis(notes);
    cout << "Notes admis : ";
    for(unsigned i = 0; i < notesAdmis.size(); i++) {
        cout << notesAdmis[i] << " ";
    }
    cout << endl;

    return 0;
}