# SentinelLab

-O0 : 

   text	   data	    bss	    dec	    hex	filename
   5787	    664	    280	   6731	   1a4b	noeud


-O2 :

   text	   data	    bss	    dec	    hex	filename
   2772	    664	    280	   3716	    e84	noeud


-Os :

   text	   data	    bss	    dec	    hex	filename
   2894	    672	    280	   3846	    f06	noeud





## Compiler TP1

$ cd noeud
$ cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
$ ./build/noeud
$ size build/noeud

$ cd ../passerelle
$ mvn compile
$ echo "01 90" | java -jar target/passerelle.jar --decode




## Compiler TP2



$ cd noeud
$ cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
$ ./build/noeud --panne 5:7



## Compiler TP3



$ cd noeud
$ cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
$ ./build/noeud --duree 600 --stats

$ cd ../passerelle
$ mvn compile
$ java -Xmx16m -XX:StartFlightRecording=filename=p.jfr -cp target/classes/ com.passerelle.Passerelle --bench



## Compiler TP4



$ cd noeud
$ cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
$ ./build/noeud &
$ kill -USR1 4242
$ ./superviseur.sh ./build/noeud --bloquer-a 20



## Compiler TP5



$ cd noeud
$ cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
$ ./build/noeud --trace-i2c | head -1
$ ./build/noeud --binaire | xxd | head -1

$ cp ../passerelle
$ mvn compile
$ cd ..
$ ./noeud/build/noeud --binaire | ./noeud/build/bitflip 0.001 | java -cp passerelle/target/classes/ com.passerelle.Passerelle 

CTRL+C => trames valides : 598   rejetées (CRC) : 2   resynchronisations : 2


