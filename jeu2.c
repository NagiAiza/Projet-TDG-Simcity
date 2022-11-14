//
// Created by alexi on 08/11/2022.
//

#include "listeRelated.h"
#include "jeu2.h"
#include "jeu1.h"
#include "stdio.h"

int calcul_depenses(int choix, int taille_chemin_route/*Algo Taille chemin*/)
{
    int depense;
    switch (choix) {
        case 1://achat d'une route
            depense = 10*taille_chemin_route;
            break;
        case 2://achat d'un chateau d'eau
            depense = 100000;
            break;
        case 3:
            depense = 100000;
            break;
        case 4:
            depense = 1000;
            break;
        default:
            depense = 0;
            break;
    }
    return depense;
}

int validation_depense(int depense, long argent_restant)
{
    if(argent_restant-depense>=0)
    {
        return 1;
    }
    else
    {
        printf("pas d'argent\n");
        return 0;
    }
}


//gestion de l'eau

t_graphe* initialisation_distance_chateau (t_graphe* map, t_tile* case_chateau)
{
    if (case_chateau->element->orientation == 1)
    {
        for (int i = -2; i < 2; i++)
        {
            for (int j = -2; j < 4; j++)
            {
                map->grille[case_chateau->position.ligne+i][case_chateau->position.colonne+j]->g = 0;
            }
        }
    }
    else if (case_chateau->element->orientation == -1)
    {
        for (int i = -3; i < 3; i++)
        {
            for (int j = -1; j < 3; j++)
            {
                map->grille[case_chateau->position.ligne+i][case_chateau->position.colonne+j]->g = 0;
            }
        }
    }
    return map;
}

/*t_graphe* initialisation_contour_chateau (t_graphe* map)
{
    t_liste* liste_voisin;
    for (int i=0 ; i<NBLIGNE ; i++)
    {
        for (int j=0 ; j<NBCOLONNE ; j++)
        {
            if (map->grille[i][j]->g==2)
            {
                liste_voisin = map->grille[i][j]->voisin;
                while(liste_voisin != NULL)
                {
                    if(liste_voisin->n->element->type==1)//si l'element est une route, alors on a une route adjacente au chateau d'eau et on initialise sa distance à 1
                    {
                        map->grille[liste_voisin->n->position.ligne][liste_voisin->n->position.colonne]->g=1;
                    }
                    liste_voisin=liste_voisin->next;
                }
            }
        }
    }
    return map;
}*/

/*
 * on lance un parcours de tableau des elements, des qu'on tombe sur un chateau d'eau on initialise les cases de distances
 * et
 * on lance un dijkstra a partir de chaque chateau d'eau, si -1 on change la taille, si la nouvelle taille est inférieure on change la taille
 * quand on insère un element dans la liste on lui dit que sa distance est égale à celui de son prédécesseur +1
 * dès qu'on trouve une maison, on retrace le chemin tout de suite en enlevant la quantité d'eau qui a été distribué
 * , ça nous donnera une matrice avec la taille des différents chemin
 * et ensuite penser a modifier le fichier du niveau -1 pour l'affichage des chemins
 */

t_graphe* distribution_eau(t_graphe* map)
{
    for(int i=0 ; i<NBLIGNE; i++)//parcours de toute les cases du tableau pour trouver les chateaux d'eau
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            if (map->grille[i][j]->element->type == 2)//dès qu'on a trouver on lance dijkstra pour ensuite trouver les chemins menant aux dif bat
            {
                //lancement du dijkstra

                //majFichier-1
            }
        }
    }
}

void dijkstra(t_graphe* map, t_tile* sommet_de_depart)
{
    int poids_temp;
    t_tile* case_analysee,* voisin_actuel;
    t_liste* liste_voisin;
    t_liste* liste_ouverte=creer();
    t_liste* liste_ferme=creer();//pas forcement utile cette ligne

    //initialisation de la matrice des poids
    for(int i=0 ; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            map->grille[i][j]->g=-1;
            map->grille[i][j]->parent=NULL;
        }
    }
    map=initialisation_distance_chateau(map, map->grille[sommet_de_depart->position.ligne][sommet_de_depart->position.colonne]);

    //insertion dans la liste ouverte
    for(int i=0 ; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            if(map->grille[i][j]->g==0)
            {
                insererNoeud(liste_ouverte, map->grille[i][j]);
            }
        }
    }

    //debut de l'analyse
    while(!estVide(liste_ouverte) && sommet_de_depart->element->capacite!=NULL)//tant qu'il reste des noeuds à analyser et que le chateau d'eau peut encore distribuer de l'eau
    {
        liste_ouverte = enlever_noeud_debut(liste_ouverte, &case_analysee);
        liste_ferme = insererNoeud(liste_ferme, case_analysee);

        liste_voisin=case_analysee->voisin;
        while(liste_voisin!=NULL || sommet_de_depart->element->capacite>0)
        {
            voisin_actuel=liste_voisin->n;
            poids_temp=case_analysee->g+ heuristic(case_analysee, voisin_actuel);
            if(voisin_actuel->element->type>=4 && voisin_actuel->element->type<=10)//le voisin est une habitation
            {
                if(voisin_actuel->case_mere->element->compteur>0)//si il reste des habitants qui doivent être alimenté en eau
                {
                    //faire toute les maj sur l'habitation en fonction de l'eau distrib
                    //retracer le chemin et le mettre sur la map -1
                }

            }
            if((poids_temp<voisin_actuel->g || voisin_actuel->g!=-1) && voisin_actuel->element->type==1)//si la nouvelle valeur de distance est inférieure à l'ancienne on actualise la distance et si c'est une route
            {
                voisin_actuel->g=poids_temp;
                voisin_actuel->parent=case_analysee;
                if(existe(liste_ouverte, voisin_actuel))
                {
                    liste_ouverte= actualisation(liste_ouverte, voisin_actuel);//on actualise le sommet pour le remettre à la bonne place
                }
            }
            if(!existe(liste_ouverte, voisin_actuel) && !existe(liste_ferme, voisin_actuel) && voisin_actuel->element->type==1)//on verifie que le sommet n'est pas dans la liste ouverte et fermé et que c'est une route
            {
                liste_ouverte= insertion_en_triant(liste_ouverte, voisin_actuel);//dans ce cas on l'insère dans la liste
            }
            liste_voisin=liste_voisin->next;
        }
    }
}
//la forme est faite, il faut maintenant verifier les conditions et mettre à jour le niveau -1 à chaque fois que l'on tombe sur une maison
