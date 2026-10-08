package com.passerelle;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Passerelle {

    private static int validFrames = 0;
    private static int rejectedFrames = 0;
    private static int resyncCount = 0;


    // Méthode pour lire l'utilisation mémoire RSS : TP4
    private static void lireVmRSS() {
        try (BufferedReader reader = new BufferedReader(new FileReader("/proc/self/status"))) {
            String ligne;
            while ((ligne = reader.readLine()) != null) {
                if (ligne.startsWith("VmRSS:")) {
                    System.out.println("-> Consommation mémoire (VmRSS) : " + ligne.replaceAll("\\s+", " ").trim());
                    return;
                }
            }
        } catch (IOException e) {
            System.err.println("Impossible de lire /proc/self/status (environnement non-Linux ?) : " + e.getMessage());
        }
    }

    // ================================================= TP5
    private static int calculerCRC8(byte[] data, int offset, int length) {
        int crc = 0x00;
        for (int i = offset; i < offset + length; i++) {
            crc ^= (data[i] & 0xFF);
            for (int j = 0; j < 8; j++) {
                if ((crc & 0x80) != 0) {
                    crc = ((crc << 1) ^ 0x07) & 0xFF;
                } else {
                    crc = (crc << 1) & 0xFF;
                }
            }
        }
        return crc;
    }


    public static void main(String[] args) {
        // ================================================== TP1
        /*Scanner scanner = new Scanner(System.in);
        System.out.println("Entrez deux octets hexadécimaux :");

        if (scanner.hasNext()) {
            String highHex = scanner.next();
            String lowHex = scanner.hasNext() ? scanner.next() : "00";

            int high = Integer.parseInt(highHex, 16);
            int low = Integer.parseInt(lowHex, 16);

            int valeur16bits = (high << 8) | low;

            System.out.println("Valeur 16 bits décimale : " + valeur16bits);
        }
        scanner.close();*/


        // ================================================== TP2
        /*EtatPasserelle etat = EtatPasserelle.ECOUTE;
        System.out.println("Passerelle démarrée. État : " + etat);
        
        Mesure m = new Mesure(21.5f, 40.0f, 1013.2f, System.currentTimeMillis());
        System.out.println("Mesure reçue : " + m);*/


        //=================================================== TP3
        /*int nbMesures = 100000;

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
        System.out.println("Temps double[] : " + (fin2 - debut2) + " ms");*/
    


    //================================================================ TP4

        /*Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            System.out.println("\n[Passerelle] Arrêt détecté, nettoyage et bilan mémoire :");
            lireVmRSS();
        }));

        System.out.println("Passerelle Java active. En attente de trames... (Appuyez sur Ctrl+C pour quitter)");

        // Boucle de simulation de la passerelle
        try {
            while (true) {
                Thread.sleep(2000);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }*/

        //================================================================ TP5


        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            System.out.println("\ntrames valides : " + validFrames + 
                               "   rejetées (CRC) : " + rejectedFrames + 
                               "   resynchronisations : " + resyncCount);
        }));

        try {
            byte[] frame = new byte[6];
            while (true) {
                int b = System.in.read();
                if (b == -1) break;

                if ((b & 0xFF) == 0xAA) {
                    frame[0] = (byte) b;
                    int lues = System.in.readNBytes(frame, 1, 5);
                    if (lues < 5) break; 

                    int crcRecu = frame[5] & 0xFF;
                    int crcCalcule = calculerCRC8(frame, 1, 4);

                    if (crcRecu == crcCalcule) {
                        validFrames++;
                    } else {
                        rejectedFrames++;
                    }
                } else {
                    resyncCount++;
                }
            }
        } catch (IOException e) {
            System.err.println("Erreur de lecture : " + e.getMessage());
        }

        System.out.println("trames valides : " + validFrames + ", rejetées (CRC) : " + rejectedFrames + ", resynchronisations : " + resyncCount);
    }
    
}
