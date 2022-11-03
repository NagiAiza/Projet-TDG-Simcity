//
// Created by alexi on 01/11/2022.
//

#ifndef PROJET_S1_AFFICHAGE_H
#define PROJET_S1_AFFICHAGE_H

#include <stdio.h>
#include <allegro.h>
#include "jeu1.h"

BITMAP * load_bitmap_check(char *nomImage);
void affichageCaseSelec(BITMAP* map, BITMAP* selec, t_pos souris);
void affichageTotal();

#endif //PROJET_S1_AFFICHAGE_H
