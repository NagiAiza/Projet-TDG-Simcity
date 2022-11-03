//
// Created by alexi on 02/11/2022.
//
#include "jeu1.h"
#include "listeRelated.h"

t_pos calcul_pos_souris(BITMAP* sousMap, int decalageScreenX/*pour savoir où placer la bitmap*/)//retourne la position de la souris
{
    t_pos souris;
    souris.ligne=getb(getpixel(sousMap, mouse_x+decalageScreenX-124, mouse_y));
    souris.colonne=getr(getpixel(sousMap, mouse_x+decalageScreenX-124, mouse_y));
    return souris;
}

t_tile* associerCaseSouris(t_graphe* map, t_pos souris)
{
    if(souris.ligne<35 && souris.colonne<45)
    {
        return map->tab_case[souris.ligne][souris.colonne];
    }
    else
    {
        return NULL;
    }
}

int detectionChangementDeCase(t_pos old_souris, t_pos new_souris)
{
    if(new_souris.ligne<35 && new_souris.colonne<45)
    {
        if(old_souris.colonne!=new_souris.colonne || old_souris.ligne!=new_souris.ligne)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

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

void placementRoute()
{
    //mettre a jour le fichier
    //est ce qu'on ferait pas un placement en A*?
}

void placementBatiment()
{

}