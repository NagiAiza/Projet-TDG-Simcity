//
// Created by alexi on 02/11/2022.
//

#ifndef PROJET_S1_LISTERELATED_H
#define PROJET_S1_LISTERELATED_H

#include "jeu1.h"

typedef struct liste
{
    t_tile* n;
    struct liste* next;
    struct liste* prev;
}t_liste;

t_liste *creer(void);
t_liste *insererNoeud(t_liste *liste, t_tile *n); //inserer noeud dans la liste
t_liste *enleverNoeud(t_liste *liste, t_tile *n);
int existe(t_liste *l, t_tile *n);
//t_tile* rechercherPlusPetit(t_liste *l);
int taille(t_liste *l);
int tailleChemin(t_tile* n);
int estVide(t_liste *l);



#endif //PROJET_S1_LISTERELATED_H
