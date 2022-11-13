//
// Created by alexi on 08/11/2022.
//

#include "listeRelated.h"
#include "jeu2.h"
#include "jeu1.h"
#include "stdio.h"

int calcul_depenses(int choix, int taille_chemin_route/*Algo Taille chemin*/)
{
    int depense=0;
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
int** RemplissageMatricePoids (int** MatricePoids, int orientation, t_pos position)
{
    if (orientation == 1)
    {
        for (int i = -2; i < 2; i++)
        {
            for (int j = -2; j < 4; j++)
            {
                MatricePoids[position.ligne][position.colonne] = 0;
            }
        }
    }
    else if (orientation == -1)
    {
        for (int i = -3; i < 3; i++)
        {
            for (int j = -1; j < 3; j++)
            {
                MatricePoids[position.ligne][position.colonne] = 0;
            }
        }
    }
    return MatricePoids;
}
int** InitialisationContours (int** matricepoids, t_graphe* map)
{
    t_liste* liste_voisin = creer();
    for (int i=0 ; i<NBLIGNE ; i++)
    {
        for (int j=0 ; j<NBCOLONNE ; j++)
        {
            if (matricepoids==2)
            {
                liste_voisin = map->grille[i][j]->voisin;
                while(liste_voisin != NULL)
                {
                    if(liste_voisin->n->element->type==1)
                    {
                        matricepoids[liste_voisin->n->position.ligne][liste_voisin->n->position.colonne]=1;
                    }
                }
            }
        }
    }
    return matricepoids;
}

int** initialisation_matrice_poids(t_graphe* map)
{
    int** matrice_poids = NULL;
    //allocation dynatmique de notre matrice d'adjacente
    matrice_poids = (int**)malloc(NBLIGNE*sizeof(int*)); //1er dimension
    for (int i=0 ; i<NBLIGNE ; i++)
    {
        matrice_poids[i] = malloc(NBCOLONNE*sizeof(int)); //2eme dimension
    }
    for(int i=0 ; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            matrice_poids[i][j]=-1;
        }
    }
    for(int i=0 ; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            if (map->grille[i][j]->element->type == 2)
            {
                //remplissage matrice poids
                RemplissageMatricePoids(matrice_poids, map->grille[i][j]->element->orientation, map->grille[i][j]->position );
            }
        }
        printf("\n");
    }
    matrice_poids=InitialisationContours(matrice_poids, map);
    return matrice_poids;
}

t_graphe* distribution_eau(t_graphe* map)
{
    int** matrice_poids = initialisation_matrice_poids(map);

}

