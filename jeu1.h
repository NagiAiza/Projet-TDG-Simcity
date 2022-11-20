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
    t_pos position;//elle est déjà dans tile, est ce que je l'utilise quand meme?
    int type; //1 si route, 2 si chateau d'eau, 3 si elec, 4 a 9 les maisons !!!
    int orientation;//connaitre l'orientation du bat 1 ou 2

    // si c'est une habitation
    int stade;//niveau d'évolution
    int nb_habitant;
    int eau_actuelle;
    int compteur;//timer à la création de l'habitation

    //si c'est un chateau d'eau
    int capacite;
}t_batiment;

typedef struct tile{//pour faire la map on fait un tableau à 2 dimensions de tile
    t_batiment* element;//quel element est dessus (maison, chateau d'eau, centrale etc)
    int numConnexite;
    t_pos position;

    //pour algo A*
    int g;//distance a partir du départ
    int h;
    int f;
    struct tile* parent;//pour garder le prédécesseur de chaque case et ainsi retracer le chemin à la fin
    struct liste* voisin;//on initialise les voisins de chaque case en début de programme

    struct tile* case_mere;//donne la case principale du bat
}t_tile;//rajouter un emplacement qui permette de differencier l'affichage du truc en globalité

typedef struct graphe{
    t_tile*** grille;
    int** mat_adjacence;//vraie matrice d'adjacence avec toutes les cases qui sont remplies pour les batiments
    int** mat_chemin_eau;//matrice pour repertorier les chemins en eau
}t_graphe;


t_graphe* makeGrid();//initialisation de la grille
t_graphe* initialiserGrille(t_graphe* g); //premiere initialisation a faire
void initialisationElementCarte();//initialisation du fichier des elements de carte avec une route de départ
void majFichierPlacementElement(t_graphe* g);//mise à jour du fichier des elements à chaque fois qu'on fait une modification sur la map
t_graphe* placementElement(t_graphe* g, int ligne, int colonne, int type, int rotation);//mets à jour les fichiers des elements et de rotation
t_graphe* remplissage_matrice_adjacence(t_graphe* g, int ligne, int colonne, int type, t_tile* case_mere);//rempli la matrice d'adjacence qui permet de savoir quelle place totale prend les elements
void rajouterVoisin(t_tile* spot, t_tile ***map, int colonne, int ligne);//initialise les voisins de chaque case de la grille
void initialiserVoisin(t_tile*** map, int ligne, int colonne);//pour donner une liste de voisins à chaque noeuds
void liberation_donnee(t_graphe* g);// pas forcément utile


int placement_route(t_graphe* map, int ligne, int colonne);
int verification_chevauchement(t_graphe* map, int ligne, int colonne, int choix, int rotation);

void initialisation_habitation(t_tile* case_hab);
void initialisation_chateau_eau(t_tile* case_chateau);

///algo A*
int distance(t_pos a, t_pos b);
int heuristic(t_tile *a, t_tile *b);
t_graphe* A_star(t_graphe* g, t_tile* depart, t_tile* arrive/*position souris*/);


#endif //PROJET_S1_JEU1_H
