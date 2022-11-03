//
// Created by alexi on 02/11/2022.
//

#ifndef PROJET_S1_JEU1_H
#define PROJET_S1_JEU1_H

#include "stdio.h"
#include "stdlib.h"

#define NBLIGNE 35
#define NBCOLONNE 45

typedef struct pos{
    int ligne;
    int colonne;
}t_pos;

typedef struct tile{//pour faire la map on fait un tableau à 2 dimensions de tile
    int element;//quel element est dessus (maison, chateau d'eau, centrale etc)
    int connexite;
    t_pos position;

    //pour algo A*
    int g;
    int h;
    int f;

    struct tile* parent;
    struct liste* voisin;//on initialise les voisins de chaque case en début de programme
}t_tile;



typedef struct route{
    int tailleX, tailleY;
}t_route;

typedef struct habitation{
    int tailleX, tailleY;
    int type;
}t_habitation;

t_tile*** makeGrid();
void rajouterVoisin(t_tile* spot, t_tile ***map, int colonne, int ligne);
void initialiserVoisin(t_tile*** map, int ligne, int colonne);//pour donner une liste de voisins à chaque noeuds


#endif //PROJET_S1_JEU1_H
