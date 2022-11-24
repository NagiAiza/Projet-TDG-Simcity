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


int choixAction(BUFFER* liste_buffer,IMAGE*liste_image);
t_graphe* action(t_graphe* map, BUFFER* liste_buffer, IMAGE* liste_image, int* choix, t_pos souris, int* rotation, int* niv_visu, t_tile** case_select, int* algo_A, long* argent_restant, int* capa_usine);

#endif //PROJET_S1_BARRE_OUTIL_H
