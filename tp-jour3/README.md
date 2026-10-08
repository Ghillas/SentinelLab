Question TP-Jour3 :


## TP1 : Modbus

1. Pourquoi l’esclave reste-t-il muet au lieu de renvoyer une exception quand le CRC est faux ?

Car si plusieurs esclave reçoivent une trame corrompue ou qui ne leur est pas destinée, et qu'ils répondent tous pour signaler l'erreur, cela peut créer des collisions et surcharger le bus.


2. Le maître ne peut pas savoir combien d’octets lire avant d’avoir reçu l’en-tête. Comment `lireReponse()` déduit-elle la longueur ? Que se passerait-il si le bruit touchait l’octet de fonction ?

La fonction lireReponse() déduit la longueur de la trame, car les trames suivent le même format, avec les premiers octets pour l'adresse de l'esclave et le code de fonction. Ensuite selon le code de fonction, l'octet suivant indique le nombre d'octet de données, permettant au maître de savoir combien d'octet il doit lire pour récupérer les données, enfin, les 2 dernier octets sont pour le CRC. La longueur totale peut donc être calculer par le maître en temps réél. Si le bruit touchait l'octet de fonction le calcul du CRC ne correspondra plus aux données reçues, donc la trame sera rejetée.


3. Quelle différence concrète, dans le binaire produit, entre `consteval` et `constexpr` pour `tableCrc()` ?

consteval impose que la fonction soit évaluée à la compilation. Si le compilateur ne peut pas calculer la table à l'avance, il va provoquer une erreur de compilation. A l'inverse, constexpr, peut reporter le calcul à l'exécution. Avec consteval, on a donc la certitude que la table est stockée directement dans le binaire produit.


4. Réécrire `traiter()` avec une classe de base `IEquipement` à méthodes virtuelles. Qu’y gagne-t-on, qu’y perd-on ?

Ce qu'on gagne : On peut stocker différents types d'équipements dans une même collection sans connaitre leur type au moment de la compilation grâce au polymorphisme.

Ce qu'on y perd : On augmente la compléxité, et on peut perdre légèrement en performance égalements.
    
   
5. Pourquoi la lecture de la sortie de l’esclave est-elle faite dans un thread séparé, plutôt que directement avec `InputStream.read()` dans la transaction ?

Car le flux de communication avec l'esclace est asynnchrone et imprévisible, par exemple si l'esclave ne répond pas et que le maître fait un read() sur le thread principale, il peut se retrouver bloqué à attendre une réponse de l'esclave. 


## TP2 : Embouteillage


1. Pourquoi faut-il deux sémaphores **et** un mutex dans `FileBornee` ? Que se passe-t-il avec un seul mutex et une attente active ?


Car il y a un sémaphore qui gère l'espace disponible dans la file pour les producteurs, et un sémaphore qui gère les éléments disponible dans la file pour les consommateurs. Le mutex lui gère l'accès au ressources partagées et garantit l'exclusion mutuelle.


2. Quelle différence entre `std::latch` et `std::barrier` ? Pourquoi a-t-on besoin des deux dans la ligne ?

std::latch permet de savoir quand tous les postes son prêts avant de démarrer, alors que std::barrier permet au différent postes de se synchroniser, notamment pour le changement de format.


3. Avec une alimentation à 20 ms et un remplissage à 30 ms, combien de bouteilles attendent sur le premier convoyeur en régime établi ? Que changerait un convoyeur de 100 places ?

Le poste d'alimentation produit une bouteille toutes les 20 ms, mais le remplissage prend 30 ms par bouteille. Le remplissage est le goulot d'étranglement. Par conséquent, le premier convoyeur va se remplir immédiatement jusqu'à atteindre sa capacité maximale (4 bouteilles), puis l'alimentation sera bloquée en attendant qu'une place se libère. Un convoyeur de 100 places permettrait d'avoir un plus grand espace de stockage, mais cela ne changerait pas le goulot d'étranglement que représente le remplissage.


4. Pourquoi la fonction de fin de lot peut-elle modifier `debutLot` sans verrou ?

Car debutLot est modifier directement par le std::barrier, le barrier étant un point de synchronisation, aucun thread ne peut accéder à débutLot.


5. Dans quel cas un thread virtuel n’apporte-t-il rien par rapport à un thread de plateforme ?

Dans le cas ou un thread virtuel consomme 100% du processeur, il n'apporterait rien par rapport à un thread de plateforme






















