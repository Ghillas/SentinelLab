# Rapport de Performances et Mesures (TP3)

---

## 1. Noeud Embarqué (C++)

Le noeud intègre un tampon circulaire de taille fixe pour mémoriser l'historique des relevés du capteur.

Un accès hors limites a été volontairement injecté dans un pointeur de test (`ptr[10]` sur un bloc alloué de 5 entiers) pour valider la détection d'anomalies à l'exécution.

* **Rapport de sortie ASan :**
  
  =================================================================
  ==43369==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x503000000068
  READ of size 4 at 0x503000000068 thread T0
      #0 main /home/user/SentinelLab/noeud/test_asan.cpp:8
  
  0x503000000068 is located 20 bytes after 20-byte region [0x503000000040,0x503000000054)
  SUMMARY: AddressSanitizer: heap-buffer-overflow test_asan.cpp:8 in main
  =================================================================

Valgrind : 

	Après correction et nettoyage du code de test, une exécution de la boucle complète du noeud sous Valgrind (sans ASan) a donné le résultat suivant :

	HEAP SUMMARY :

	    in use at exit: 0 bytes in 0 blocks

	    total heap usage: 2 allocs, 2 frees, 74,752 bytes allocated

	ERROR SUMMARY : 0 errors from 0 contexts (Aucune fuite ni erreur mémoire détectée en régime permanent).

---

## 2. Passerelle Java (Benchmark de performance)

Un comparatif a été réalisé en traitant **100 000 mesures** car **1 000 000** de mesures provoque un **OutOfMemoryError** pour le test avec ArrayList<Double>.

ArrayList<Double> :  **19 ms** 
double[] : **3 ms**
