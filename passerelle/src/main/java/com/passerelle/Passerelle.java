package com.passerelle;

import java.util.Scanner;

public class Passerelle {
    public static void main(String[] args) {
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
    }
}
