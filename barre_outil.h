//
// Created by alexi on 03/11/2022.
//

#ifndef PROJET_S1_BARRE_OUTIL_H
#define PROJET_S1_BARRE_OUTIL_H

#include "stdio.h"
#include "stdlib.h"
#include "allegro.h"
#include "Affichage.h"
#include "jeu1.h"


int choixAction();
t_graphe* action(t_graphe* map, BUFFER* liste_buffer, IMAGE* liste_image, int* choix, t_pos souris, int* rotation, int* action_en_cours, t_tile** case_select, int* algo_A);

#endif //PROJET_S1_BARRE_OUTIL_H