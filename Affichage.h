//
// Created by alexi on 01/11/2022.
//

#ifndef PROJET_S1_AFFICHAGE_H
#define PROJET_S1_AFFICHAGE_H

#include <stdio.h>
#include <allegro.h>

BITMAP * load_bitmap_check(char *nomImage);
void affichageCaseSelec(BITMAP* map, BITMAP* selec, int R, int B);//R correspond à la colonne, B à la ligne
BITMAP* affichageMap(BITMAP* bufferMap);
BITMAP* affichageMenu(BITMAP* bufferMenu);
void affichageTotal();

#endif //PROJET_S1_AFFICHAGE_H
