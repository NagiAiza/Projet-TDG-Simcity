//
// Created by alexi on 02/11/2022.
//

#include "jeu1.h"
#include "listeRelated.h"

t_tile*** makeGrid()
{
    t_tile*** grille=(t_tile***)malloc(sizeof(t_tile**)*NBLIGNE);
    for(int i=0; i<NBCOLONNE; i++)
    {
        grille[i] = (t_tile**) malloc(NBCOLONNE*sizeof(t_tile*));
    }
    return grille;
}

void initialiserGrille(t_tile*** grid) //premiere initialisation a faire
{
    FILE* pf=NULL;
    pf=fopen("./map.txt","r");
    if(pf==NULL)
    {
        printf("erreur d'ouverture fichier\n");
    }
    int i, j, random;
    for (i = 0; i < NBLIGNE; i++)
    {
        for (j = 0; j < NBCOLONNE; j++)
        {
            grid[i][j] = (t_tile*)malloc(sizeof(t_tile));
            grid[i][j]->position.colonne = i;
            grid[i][j]->position.ligne = j;

            grid[i][j]->f = 0;
            grid[i][j]->g = 0;
            grid[i][j]->h = 0;

            grid[i][j]->f = grid[i][j]->g + grid[i][j]->h;

            grid[i][j]->voisin = NULL;

            grid[i][j]->parent = NULL;

            fscanf(pf,"%d ",&grid[i][j]->element);
        }
    }
    fclose(pf);
    pf=NULL;
}

void majFichierMap(t_tile*** grid)
{

}

void rajouterVoisin(t_tile* spot, t_tile ***map, int colonne, int ligne)//les arretes existes que entre les voisins
{
    int i = spot->position.colonne;// on récupère les coordonnées
    int j = spot->position.ligne;

    if (i < colonne - 1)//on vérifie qu'on n'est pas sur les bords de la map
        spot->voisin = insererNoeud(spot->voisin, map[i + 1][j]);//et on rajoute les noeuds voisins en tant que voisin dans une liste du noeuds donné
    if (i > 0)
        spot->voisin = insererNoeud(spot->voisin, map[i - 1][j]);
    if (j < ligne - 1)
        spot->voisin = insererNoeud(spot->voisin, map[i][j + 1]);
    if (j > 0)
        spot->voisin = insererNoeud(spot->voisin, map[i][j - 1]);
}

void initialiserVoisin(t_tile*** map, int ligne, int colonne)//pour donner une liste de voisins à chaque noeuds
{
    for (int i = 0; i < ligne; i++)//on parcours toute la grille
    {
        for (int j = 0; j < colonne; j++)
        {
            rajouterVoisin(map[i][j], map, colonne, ligne);
        }
    }
}