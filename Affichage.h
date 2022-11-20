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
    BITMAP* canalisation;
    BITMAP* batiment;
    BITMAP* chateau_eau;
    BITMAP* centrale;
    BITMAP* case_selec;
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
void affichage_level_0(BUFFER* liste_buffer, IMAGE* liste_image);//on pourra peut etre rajouter des obstacles du style montagne etc..
void affichage_level_1(t_graphe* map, IMAGE* liste_image, BUFFER* liste_buffer);
void affichageElement(BITMAP* bufferMap, IMAGE* liste, int type, int ligne, int colonne, int rotation);//pour avoir la rotation du batiment il va falloir un autre fichier
void affichageCaseSelec(BUFFER* liste_buffer, IMAGE* liste_image, t_pos souris);
void affichage_element_eau(BUFFER* liste_buffer, IMAGE* liste, int type, int ligne, int colonne, int rotation, t_tile* chateau);
void affichage_habitation(BUFFER* liste_buffer, IMAGE* liste_image, t_tile* habitation);
void affichageTotal(t_graphe* map, IMAGE* liste_image, BUFFER* liste_buffer, t_pos souris, long compteur_argent, int niveau_visu);
t_pos calcul_pos_souris(BITMAP* sousMap, int decalageScreenX/*pour savoir où placer la bitmap*/);//retourne la position de la souris


#endif //PROJET_S1_AFFICHAGE_H
