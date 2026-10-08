package com.passerelle;

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
    }
}
