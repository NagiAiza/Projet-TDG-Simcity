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


typedef struct batiment{
    t_pos position;
    int type; //si c'est une route, une habitation, un batiment
    int orientation;//connaitre l'orientation du bat 1 ou 2
    // si c'est une habitation
    int stade;//niveau d'évolution
    int nb_habitant;
    int compteur;//timer à la création de l'habitation

    //si c'est un chateau d'eau
    int capacite;
}t_batiment;

typedef struct tile{//pour faire la map on fait un tableau à 2 dimensions de tile
    t_batiment* element;//quel element est dessus (maison, chateau d'eau, centrale etc)
    int numConnexite;
    t_pos position;

    //pour algo A*
    int g;
    int h;
    int f;
    struct tile* parent;
    struct liste* voisin;//on initialise les voisins de chaque case en début de programme
}t_tile;//rajouter un emplacement qui permette de differencier l'affichage du truc en globalité

typedef struct graphe{
    t_tile*** grille;
    int** mat_adjacence;//vrai matrice d'adjacence avec toute les cases qui sont remplis pour les batiments
}t_graphe;


t_graphe* makeGrid();
t_graphe* initialiserGrille(t_graphe* g); //premiere initialisation a faire
void initialisationElementCarte();
void majFichierPlacementElement(t_graphe* g);
t_graphe* placementElement(t_graphe* g, int ligne, int colonne, int type, int rotation);
t_graphe* remplissage_matrice_adjacence(t_graphe* g, int ligne, int colonne, int type, int rotation);
void rajouterVoisin(t_tile* spot, t_tile ***map, int colonne, int ligne);
void initialiserVoisin(t_tile*** map, int ligne, int colonne);//pour donner une liste de voisins à chaque noeuds
void liberation_donnee(t_graphe* g);


int placement_route(t_graphe* map, int ligne, int colonne);
int verification_chevauchement(t_graphe* map, int ligne, int colonne, int choix, int rotation);


#endif //PROJET_S1_JEU1_H
