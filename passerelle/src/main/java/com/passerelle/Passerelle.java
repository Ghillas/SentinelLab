package com.passerelle;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Passerelle {
    public static void main(String[] args) {
        // ================================================== TP1
        Scanner scanner = new Scanner(System.in);
        System.out.println("Entrez deux octets hexadécimaux :");

        if (scanner.hasNext()) {
            String highHex = scanner.next();
            String lowHex = scanner.hasNext() ? scanner.next() : "00";

            int high = Integer.parseInt(highHex, 16);
            int low = Integer.parseInt(lowHex, 16);

            int valeur16bits = (high << 8) | low;

            System.out.println("Valeur 16 bits décimale : " + valeur16bits);
        }
        scanner.close();


        // ================================================== TP2
        EtatPasserelle etat = EtatPasserelle.ECOUTE;
        System.out.println("Passerelle démarrée. État : " + etat);
        
        Mesure m = new Mesure(21.5f, 40.0f, 1013.2f, System.currentTimeMillis());
        System.out.println("Mesure reçue : " + m);


        //=================================================== TP3
        int nbMesures = 100000;

        System.out.println("Test avec ArrayList<Double>");
        long debut1 = System.currentTimeMillis();
        List<Double> list = new ArrayList<>(nbMesures);
        for (int i = 0; i < nbMesures; i++) {
            list.add(20.5 + i * 0.001);
        }
        long fin1 = System.currentTimeMillis();
        System.out.println("Temps ArrayList : " + (fin1 - debut1) + " ms");

        System.out.println("Test avec double[]");
        long debut2 = System.currentTimeMillis();
        double[] array = new double[nbMesures];
        for (int i = 0; i < nbMesures; i++) {
            array[i] = 20.5 + i * 0.001;
        }
        long fin2 = System.currentTimeMillis();
        System.out.println("Temps double[] : " + (fin2 - debut2) + " ms");
    }
}
