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

typedef struct liste2{
    t_tile* n;
    int montant_distribue;
    struct liste2* next;
}t_liste2;

t_liste *creer(void);
t_liste *insererNoeud(t_liste *liste, t_tile *n); //inserer noeud dans la liste
t_liste *enleverNoeud(t_liste *liste, t_tile *n);
int existe(t_liste *l, t_tile *n);
//t_tile* rechercherPlusPetit(t_liste *l);
int taille(t_liste *l);
int tailleChemin(t_tile* n);
int estVide(t_liste *l);
void afficherListe(t_liste *l);//servira pour débugger
void liberer(t_liste *l);


t_liste* insertion_en_triant(t_liste* liste, t_tile* noeud);//pour file de priorité
t_liste* insertion_en_triant2(t_liste* liste, t_tile* noeud);
t_liste* enlever_noeud_debut(t_liste* liste, t_tile** noeud_a_conserver);
t_liste* actualisation(t_liste* l, t_tile* noeud_a_retrier);
t_liste* insererNoeudFin(t_liste* liste, t_tile* n);//enfiler

//LISTE 2
t_liste2 *creer2(void);
t_liste2 *insererNoeud2(t_liste2 *liste, t_tile *n,int montant_distribue);//inserer noeud debut de la liste dans la liste
t_liste2* vider_liste(t_liste2* liste);


#endif //PROJET_S1_LISTERELATED_H
