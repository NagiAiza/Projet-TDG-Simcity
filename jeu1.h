//
// Created by alexi on 02/11/2022.
//

#ifndef PROJET_S1_JEU1_H
#define PROJET_S1_JEU1_H

#include "stdio.h"
#include "stdlib.h"
#include "allegro.h"


#define NBLIGNE 35
#define NBCOLONNE 45

typedef struct pos{
    int ligne;
    int colonne;
}t_pos;


typedef struct tile{//pour faire la map on fait un tableau à 2 dimensions de tile
    int element;//quel element est dessus (maison, chateau d'eau, centrale etc)
    int numConnexite;
    t_pos position;

    //pour algo A*
    int g;
    int h;
    int f;
    struct tile* parent;
    struct liste* voisin;//on initialise les voisins de chaque case en début de programme
}t_tile;

typedef struct graphe{
    t_tile*** tab_case;
    int** mat_adjacence;
}t_graphe;



typedef struct route{
    t_pos position;
}t_route;

typedef struct batiment{
    t_pos position;
    int type; //par exemple 1 pour le chateau d'eau, 2 pour la centrale
    int capacite;
}t_chateauEau;

typedef struct habitation{
    t_pos position;
    int stade;
    int nbHabitant;
}t_habitation;

t_tile*** makeGrid();
void rajouterVoisin(t_tile* spot, t_tile ***map, int colonne, int ligne);
void initialiserVoisin(t_tile*** map, int ligne, int colonne);//pour donner une liste de voisins à chaque noeuds
t_pos calcul_pos_souris(BITMAP* sousMap, int decalageScreenX/*pour savoir où placer la bitmap*/);//retourne la position de la souris



#endif //PROJET_S1_JEU1_H
