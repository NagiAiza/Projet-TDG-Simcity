//
// Created by Estelle on 15/11/2022.
//

#ifndef PROJET_S1_TDG_JEU3_H
#define PROJET_S1_TDG_JEU3_H

#include "stdio.h"
#include "stdlib.h"
#include "allegro.h"
#include "jeu1.h"
#include "Affichage.h"



//t_graphe* caserne_de_pompier(t_graphe* map, int a, int b, int choix);
//t_graphe* remplissage_matrice_caserne(t_graphe* g, int ligne, int colonne, int type);//rempli la matrice caserne qui permet de s'avoir l'influence de la caserne de pompier
int incendie ();
t_graphe* dijkstra_incendie(t_graphe* map, t_tile* sommet_de_depart, t_tile* case_en_feu, int* fin_recherche, BUFFER* liste_buffer, IMAGE* liste_image);
t_graphe* gestion_incendie(t_graphe* map, t_tile* case_en_feu, BUFFER* liste_buffer, IMAGE* liste_image);

void sauvegarde(long compteur_argent, long temps, t_graphe* map, int nb_habitant);

t_graphe* lecture_sauvegarde(t_graphe* map, long* compteur_argent, long* temps, int* nb_habitant);


#endif //PROJET_S1_TDG_JEU3_H
