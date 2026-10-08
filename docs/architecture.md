# Architecture du Projet Modbus (TP2)

Ce document présente l'architecture logicielle du projet, incluant la modélisation orientée objet des composants (C++ et Java) ainsi que leurs machines à états respectives.

---

## 1. Structure des classes

### A. Côté Noeud (C++)
Le noeud utilise une interface pure (ISensor) implémentée par un capteur (SimSensor) qui simule le comportement d'un vrai matériel.


    class ISensor {
        <<interface>>
        +begin() bool
        +read(Mesure& out) bool
    }
    class SimSensor {
        -float t_
        -bool panne_
        +injecterPanne(bool p)
        +begin() bool
        +read(Mesure& out) bool
    }
    class Mesure {
        +float temp
        +float hum
        +float press
        +uint32_t t_ms
    }
    ISensor <|.. SimSensor : Implémente
    SimSensor ..> Mesure : Utilise
    
    
### B. Coté Passerelle (Java)

La passerelle utilise un record pour les données et une interface pour structurer la lecture.

    class Sensor {
        <<interface>>
        +begin() boolean
        +read(Mesure[] out) boolean
    }
    class Mesure {
        +float temp
        +float hum
        +float press
        +long tMs
    }
    class AppPasserelle {
        +main(String[] args)
    }
    Sensor <|.. AppPasserelle : Implémente / Utilise
    AppPasserelle ..> Mesure : Crée
    
    
    
## 2. Machines à états

### A. Machine à états du Nœud (C++)

Gère le cycle de vie du noeud embarqué, de l'initialisation jusqu'à la gestion des pannes injectées par la ligne de commande (--panne debut:fin).

Diagramme d'état
    [*] --> Init
    Init --> LectureCapteur : Succès begin()
    LectureCapteur --> EnvoiDonnees : read() OK
    EnvoiDonnees --> LectureCapteur : Trame envoyée
    LectureCapteur --> ModePanne : Injection --panne active
    ModePanne --> LectureCapteur : Fin de panne

### B. Machine à états de la Passerelle (Java)

Gère la réception des trames Modbus brutes sur l'entrée standard et la conversion des données.

Diagramme d'état
    [*] --> ECOUTE
    ECOUTE --> TRAITEMENT : Réception octets hexadécimaux
    TRAITEMENT --> ECOUTE : Affichage valeur 16 bits
    ECOUTE --> ERREUR : Trame invalide / Erreur de format
    ERREUR --> ECOUTE : Réinitialisation du buffer
