//
// Created by alexi on 02/11/2022.
//
#include "jeu1.h"
#include "listeRelated.h"


t_tile* associerCaseSouris(t_graphe* map, t_pos souris)
{
    if(souris.ligne<35 && souris.colonne<45)
    {
        return map->grille[souris.ligne][souris.colonne];
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


t_graphe* initialiserGrille(t_graphe* g) //premiere initialisation a faire
{
    int i, j;
    for (i = 0; i < NBLIGNE; i++)
    {
        for (j = 0; j < NBCOLONNE; j++)
        {
            g->grille[i][j] = (t_tile*)malloc(sizeof(t_tile));
            g->grille[i][j]->position.colonne = i;
            g->grille[i][j]->position.ligne = j;

            g->grille[i][j]->f = 0;
            g->grille[i][j]->g = 0;
            g->grille[i][j]->h = 0;

            g->grille[i][j]->f = g->grille[i][j]->g + g->grille[i][j]->h;

            g->grille[i][j]->voisin = NULL;

            g->grille[i][j]->parent = NULL;

            g->grille[i][j]->element=(t_batiment*)calloc(1, sizeof(t_batiment));
        }
    }
    g->grille[17][0]->element->type=1;//emplacement de départ de la route
    initialiserVoisin(g->grille, NBLIGNE, NBCOLONNE);
    return g;
}

t_graphe* makeGrid()//penser a crer la libération de données
{
    t_graphe* g=(t_graphe*) malloc(sizeof(t_graphe));
    g->grille=(t_tile***)malloc(sizeof(t_tile**)*NBLIGNE);
    for(int i=0; i<NBCOLONNE; i++)
    {
        g->grille[i] = (t_tile**) malloc(NBCOLONNE*sizeof(t_tile*));
    }
    g= initialiserGrille(g);

    g->mat_adjacence=(int **) calloc(NBLIGNE,sizeof(int*));
    for(int i=0; i<NBLIGNE; i++)
    {
        g->mat_adjacence[i]=(int*)calloc(NBCOLONNE,sizeof(int));
    }

    return g;
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

void majFichierPlacementElement(t_graphe* g)// ou mettre rotation directement dans la structure?
{

    FILE* elementCarte= fopen("element_map.txt", "w");
    FILE* rotation_element_carte= fopen("rotation_element_map.txt", "w");

    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            fprintf(elementCarte, "%d ", g->grille[i][j]->element->type);
            fprintf(rotation_element_carte, "%d ", g->grille[i][j]->element->orientation);
        }
        fprintf(elementCarte, "\n");
        fprintf(rotation_element_carte, "\n");
    }

    fclose(elementCarte);
    fclose(rotation_element_carte);
}

t_graphe* placementElement(t_graphe* g, int ligne, int colonne, int type, int rotation)
{
    //faire en fonction des différentes taille de machin
    g->grille[ligne][colonne]->element->type=type;
    g->grille[ligne][colonne]->element->orientation=rotation;
    majFichierPlacementElement(g);
    return g;
}

t_graphe* remplissage_matrice_adjacence(t_graphe* g, int ligne, int colonne, int type, int rotation)//à remplir en fonction de la rotation du bail
{
    g->mat_adjacence[ligne][colonne]=type;
    return g;
}


void initialisationElementCarte()
{
    FILE* elementCarte= fopen("element_map.txt", "w+");
    FILE* rotation_element_map=fopen("rotation_element_map.txt", "w+");
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            if(i!=17 || j!=0)
            {
                fprintf(elementCarte, "%d ", 0);//aucun element n'est sur la carte SAUF 1 route
            }
            else
            {
                fprintf(elementCarte, "%d ", 1);//aucun element n'est sur la carte SAUF 1 route a l'emplacement [17][0]
            }
            fprintf(rotation_element_map, "%d ", 1);
        }
        fprintf(elementCarte, "\n");
        fprintf(rotation_element_map, "\n");
    }
    fclose(rotation_element_map);
    fclose(elementCarte);
}


void liberation_donnee(t_graphe* g)//ne marche pas
{
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            free(g->grille[i][j]->element);
            free(g->grille[i][j]);
        }
        //free(g->grille[i]);
        free(g->mat_adjacence[i]);
    }
    //free(g->grille);
    free(g->mat_adjacence);
    free(g);

}

int placement_route(t_graphe* map, int ligne, int colonne)
{
    t_liste* voisin=map->grille[ligne][colonne]->voisin;
    while(voisin!=NULL)
    {
        if(voisin->n->element->type==1)
        {
            return 1;
        }
        voisin=voisin->next;
    }
    printf("on ne peut pas placer la route\n");
    return 0;
}

int verification_chevauchement(t_graphe* map, int ligne, int colonne, int choix, int rotation)//rajouter un changement de couleur du sprite, genre en rouge pour dire que y a un chevauchement?
{
    int verif=0;
    switch (choix) {
        case 1 ://route 1x1
            if(map->mat_adjacence[ligne][colonne]!=0){
                verif=1;
            }
            break;
        case 2 :
            for(int i=-1; i<2; i++)
            {
                for(int j=-1; j<2; j++)
                {
                    if(map->mat_adjacence[ligne+i][colonne+j]!=0)
                    {
                        verif=1;
                    }
                }
            }
            break;
        default ://a coder mais c'est pour les bat 4x6
            if(rotation==1)
            {
                for(int i=-2; i<2; i++)
                {
                    for(int j=-2; j<4; j++)
                    {
                        if(map->mat_adjacence[ligne+i][colonne+j]!=0)
                        {
                            verif=1;
                        }
                    }
                }
            }
            else if(rotation==-1)
            {
                for(int i = -3; i < 3; i++)
                {
                    for(int j = -1; j < 3; j++)
                    {
                        if(map->mat_adjacence[ligne+i][colonne+j]!=0)
                        {
                            verif=1;
                        }
                    }
                }
            }

            break;
    }
    if(verif==1)
    {
        printf("impossible il ya chevauchement\n");
    }
    return verif;
}
