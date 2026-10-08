#!/bin/bash

# Récupération de la commande passée en argument (ex: ./build/noeud --bloquer-a 20)
COMMANDE="$@"
RESTART_COUNT=0

while true; do
    echo "[superviseur] Lancement de la cible : $COMMANDE"
    $COMMANDE
    
    EXIT_CODE=$?
    RESTART_COUNT=$((RESTART_COUNT + 1))
    
    echo "[superviseur] Le processus s'est arrêté (code $EXIT_CODE)."
    echo "[superviseur] redémarrage n°$RESTART_COUNT"
    sleep 1
done
