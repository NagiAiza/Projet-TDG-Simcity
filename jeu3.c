//
// Created by Estelle on 15/11/2022.
//

#include "jeu3.h"


t_graphe* remplissage_matrice_caserne(t_graphe* g, int ligne, int colonne, int type)
{
    g->mat_adj_caserne[ligne][colonne]=type;
    return g;
}

void caserne_de_pompier(t_graphe* map, int a, int b, int choix)
{
    for (int i = -10; i < 10; i++)
    {
        for (int j = -10; j < 10; j++)
        {
            if ( a+i <= 35 && b+j <= 45)
            {
                map = remplissage_matrice_caserne(map, a+i, b+j ,choix);
            }
        }
    }

    //affichage :

    /*printf ("\nMAT ADJ CASERNE : \n\n");
    for (int i = 0; i<NBLIGNE ; i++)
    {
        for (int j=0 ; j<NBCOLONNE ; j++)
        {
            printf ("%d ", map->mat_adj_caserne[i][j]);
        }
        printf ("\n");
    }

    printf ("\n\n\n");*/

    //FAIRE AFFICHAGE MAP CASERNE GENRE UN ETAGE AVEC LA ZONE QUE PREND LA CASERNE OU QUAND ON SELECT LA CASERNE VOIR CASE INFECTE

}

int nbre_maison (t_graphe* map)
{
    int nbreMaison = 0;
    for (int i=0 ; i<NBLIGNE ; i++)
    {
        for (int j=0 ; j<NBCOLONNE ; j++)
        {
            if (map->mat_adjacence[i][j] == 4 && map->mat_adjacence[i-1][j] == 4 && map->mat_adjacence[i+1][j] == 4 && map->mat_adjacence[i][j-1] == 4 && map->mat_adjacence[i][j+1] == 4)
            {
                nbreMaison++;
            }
        }
    }
    return nbreMaison;
}

void incendie (t_graphe* map)
{
    srand(time(NULL));

    int temp = 0;
    int nbreMaison = 0;
    //determiner toute les maisons du graphe adjacent
    nbreMaison = nbre_maison(map);

    printf ("nombre de maison : %d\n", nbreMaison);

    t_pos* tabMaison = (t_pos*) calloc(2,sizeof(t_pos));

    for (int i=0 ; i<NBLIGNE ; i++)
    {
        for (int j=0 ; j<NBCOLONNE ; j++)
        {//map->grille[i][j]->element->type==4 && map->grille[i][j]->element->type>=6 && map->grille[i][j]->element->type<=9
            if (map->mat_adjacence[i][j] == 4 && map->mat_adjacence[i-1][j] == 4 && map->mat_adjacence[i+1][j] == 4 && map->mat_adjacence[i][j-1] == 4 && map->mat_adjacence[i][j+1] == 4)
            {
                printf ("%d / %d\n", i,j);
                tabMaison[temp].ligne=i;
                tabMaison[temp].colonne=j;
                temp++;
            }
        }
    }
    int aleatoire=0;
    for (int i = 0 ; i<nbreMaison ; i++)
    {
        aleatoire = rand() % 4 + 1;
        printf ("nbre aleatoire --> %d\n", aleatoire);

        if (aleatoire == 1 )
        {
            printf ("maison %d a un incendie !", i);
            if (map->mat_adj_caserne[tabMaison[i].ligne][tabMaison[i].colonne] == 5)
            {
                printf ("--------->  MAISON PROTEGE\n");
            }
            else
            {
                printf ("-------- c la desh --------\n");
            }
        }
    }
}

void evolution (t_graphe* map)
{
    t_batiment* TabBatiment = realloc(TabBatiment, sizeof(t_batiment));


}


