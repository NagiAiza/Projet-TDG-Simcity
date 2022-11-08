//
// Created by alexi on 01/11/2022.
//

#ifndef PROJET_S1_AFFICHAGE_H
#define PROJET_S1_AFFICHAGE_H

#include <stdio.h>
#include <allegro.h>
#include "jeu1.h"

typedef struct IMAGE{//liste de tout les buffer d'image qu'on va utiliser, on va les charger une seule fois et elles dureront jusqu'à la fin du pgrm
    BITMAP* sous_map;
    BITMAP* map;
    BITMAP* menu;
    BITMAP* route;
    BITMAP* batiment;
    BITMAP* chateau_eau;
    BITMAP* centrale;
}IMAGE;

typedef struct BUFFER{//liste des buffer qu'on utilisera qui correspondent à chaque element d'affichage
    BITMAP* buffer_map;//information sur la partie de la map
    BITMAP* buffer_menu;//info sur la partie du menu
    BITMAP* buffer_final;//info total
}BUFFER;


BITMAP * load_bitmap_check(char *nomImage);
IMAGE* initialisation_liste_image();
BUFFER* initialisation_liste_buffer();
void liberation_memoire_bitmaps(IMAGE* liste_image, BUFFER* liste_buffer);
void affichageElementsCarte(BITMAP* bufferMap, IMAGE* liste);
void affichageElement(BITMAP* bufferMap, IMAGE* liste, int type, int ligne, int colonne, int rotation);//pour avoir la rotation du batiment il va falloir un autre fichier
void affichageCaseSelec(BITMAP* map, BITMAP* selec, t_pos souris);
void affichageTotal(t_graphe* map, IMAGE* liste_image, BUFFER* liste_buffer, t_pos souris, long compteur_argent);
t_pos calcul_pos_souris(BITMAP* sousMap, int decalageScreenX/*pour savoir où placer la bitmap*/);//retourne la position de la souris


#endif //PROJET_S1_AFFICHAGE_H
