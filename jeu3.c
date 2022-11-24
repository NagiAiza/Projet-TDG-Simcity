//
// Created by Estelle on 15/11/2022.
//

#include "jeu3.h"
#include "listeRelated.h"


t_graphe* remplissage_matrice_caserne(t_graphe* g, int ligne, int colonne, int type)
{
    g->mat_adj_caserne[ligne][colonne]=type;
    return g;
}

t_graphe* caserne_de_pompier(t_graphe* map, int a, int b, int choix)//verifier la connexité a la route
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

void incendie (t_graphe* map)
{
    //si il y a un incendie on change l'état incendie de la maison a 1, si jusqu'au prochain cycle il n'est pas protégé alors ruine
    t_liste* parcourt=map->liste_hab;

    int aleatoire=0;

    while(parcourt!=NULL)
    {
        aleatoire = rand() % 4 + 1;
        printf ("nbre aleatoire --> %d\n", aleatoire);

        if (aleatoire == 1 )
        {
            printf ("maison [%d][%d] a un incendie !", parcourt->n->position.ligne, parcourt->n->position.colonne);
            if (map->mat_adj_caserne[parcourt->n->position.ligne][parcourt->n->position.colonne] == 5)
            {
                printf ("--------->  MAISON PROTEGE\n");
            }
            else
            {
                printf ("-------- c la desh --------\n");
            }
        }
        parcourt=parcourt->next;
    }
}


