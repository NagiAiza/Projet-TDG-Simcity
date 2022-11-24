//
// Created by Estelle on 15/11/2022.
//

#ifndef PROJET_S1_TDG_JEU3_H
#define PROJET_S1_TDG_JEU3_H

#include "stdio.h"
#include "stdlib.h"
#include "allegro.h"
#include "jeu1.h"


#define NBLIGNE 35
#define NBCOLONNE 45

void caserne_de_pompier(t_graphe* map, int a, int b, int choix);
t_graphe* remplissage_matrice_caserne(t_graphe* g, int ligne, int colonne, int type);//rempli la matrice caserne qui permet de s'avoir l'influence de la caserne de pompier
void incendie (t_graphe* map);
int nbre_maison (t_graphe* map);


#endif //PROJET_S1_TDG_JEU3_H
