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

    // Calcul CRC8 : TP5
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
        System.out.println("[TP1] Test conversion 16 bits");
        if (scanner.hasNextLine() && scanner.hasNext()) {
            try {
                String highHex = scanner.next();
                String lowHex = scanner.hasNext() ? scanner.next() : "00";
                int high = Integer.parseInt(highHex, 16);
                int low = Integer.parseInt(lowHex, 16);
                int valeur16bits = (high << 8) | low;
                System.out.println("[TP1] Valeur 16 bits décimale : " + valeur16bits);
            } catch (Exception ignored) {}
        }
*/
        // ================================================== TP2
        //System.out.println("[TP2] Initialisation des structures de données (Mesure / États)...");
        
        // Mesure m = new Mesure(21.5f, 40.0f, 1013.2f, System.currentTimeMillis());

        // ================================================== TP3 (Benchmark ArrayList vs double[])
        int nbMesures = 100_000;
        System.out.println("[TP3] Comparaison performances mémoire (" + nbMesures + " éléments) :");

        long debut1 = System.currentTimeMillis();
        List<Double> list = new ArrayList<>(nbMesures);
        for (int i = 0; i < nbMesures; i++) {
            list.add(20.5 + i * 0.001); // Correction de l'opérateur manquant
        }
        long fin1 = System.currentTimeMillis();
        System.out.println("  Temps ArrayList : " + (fin1 - debut1) + " ms");

        long debut2 = System.currentTimeMillis();
        double[] array = new double[nbMesures];
        for (int i = 0; i < nbMesures; i++) {
            array[i] = 20.5 + i * 0.001;
        }
        long fin2 = System.currentTimeMillis();
        System.out.println("  Temps double[]  : " + (fin2 - debut2) + " ms");

        // ================================================== TP4 & TP5 : Shutdown Hook Unique
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            System.out.println("\n--- [Passerelle] Arrêt détecté (Bilan final) ---");
            System.out.println("Trames valides      : " + validFrames);
            System.out.println("Trames rejetées(CRC): " + rejectedFrames);
            System.out.println("Resynchronisations  : " + resyncCount);
            lireVmRSS(); 
        }));

        // ================================================== TP5 : Boucle principale de réception binaire
        System.out.println("\n[TP5] Passerelle active. En attente de flux binaire (0xAA)...");

        try {
            byte[] frame = new byte[6];
            while (true) {
                int b = System.in.read();
                if (b == -1) break; 

                // Resynchronisation sur l'octet magique 0xAA (TP5)
                if ((b & 0xFF) == 0xAA) {
                    frame[0] = (byte) b;
                    int lues = System.in.readNBytes(frame, 1, 5);
                    if (lues < 5) break; 

                    int crcRecu = frame[5] & 0xFF;
                    int crcCalcule = calculerCRC8(frame, 1, 4);

                    if (crcRecu == crcCalcule) {
                        validFrames++;
                        // Extraction de la température (4 premiers octets du payload, little/big endian selon le C++)
                        float temp = ByteBuffer.wrap(frame, 1, 4).order(ByteOrder.LITTLE_ENDIAN).getFloat();
                        System.out.printf("[Reçu] Température : %.2f °C (CRC OK)%n", temp);
                    } else {
                        rejectedFrames++;
                    }
                } else {
                    resyncCount++;
                }
            }
        } catch (IOException e) {
            System.err.println("Erreur de lecture du flux : " + e.getMessage());
        }
    }
}

