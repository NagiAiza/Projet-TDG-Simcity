//
// Created by alexi on 02/11/2022.
//
#include "jeu1.h"
#include "listeRelated.h"
#include "math.h"


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

int detectionChangementDeCase(t_pos old_souris, t_pos new_souris)//peut etre l'utiliser pour le placement de la route et ainsi éviter que le calcul de chemin se fasse h24?
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
            g->grille[i][j] = (t_tile*)malloc(1*sizeof(t_tile));//Verifier la ligne peut etre une erreur
            g->grille[i][j]->position.colonne = j;
            g->grille[i][j]->position.ligne = i;

            g->grille[i][j]->f = 0;
            g->grille[i][j]->g = 0;
            g->grille[i][j]->h = 0;

            g->grille[i][j]->f = g->grille[i][j]->g + g->grille[i][j]->h;

            g->grille[i][j]->voisin = NULL;

            g->grille[i][j]->parent = NULL;

            g->grille[i][j]->element=(t_batiment*)calloc(1, sizeof(t_batiment));
            g->grille[i][j]->element->position.ligne=i;
            g->grille[i][j]->element->position.colonne=j;
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
    int i = spot->position.ligne;// on récupère les coordonnées
    int j = spot->position.colonne;

    if (i < ligne - 1)//on vérifie qu'on n'est pas sur les bords de la map
        spot->voisin = insererNoeud(spot->voisin, map[i + 1][j]);//et on rajoute les noeuds voisins en tant que voisin dans une liste du noeuds donné
    if (i > 0)
        spot->voisin = insererNoeud(spot->voisin, map[i - 1][j]);
    if (j < colonne - 1)
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


/*
« the memory allocation using malloc() is not de-allocated on its own. So, “free()” method is
 used to de-allocate the memory. But the free() method is not compulsory to use. If free() is
 not used in a program the memory allocated using malloc() will be de-allocated after completion
 of the execution of the program (included program execution time is relatively small and the
 program ends normally). »
 */

void liberation_donnee(t_graphe* g)//ne marche pas mais n'est pas forcément utile
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
    //dans cette verion on ne peut placer la route que sur une case étant déjà une route
    if(map->grille[ligne][colonne]->element->type==1)//si l'element est une route
    {
        return 1;
    }
    else
    {
        printf("on ne peut pas placer la route\n");
        return 0;
    }

    //Dans cette version on ne peut placer la route que sur une case voisine a une route
    /*t_liste* voisin=map->grille[ligne][colonne]->voisin;
    while(voisin!=NULL)
    {
        if(voisin->n->element->type==1)
        {
            return 1;
        }
        voisin=voisin->next;
    }
    printf("on ne peut pas placer la route\n");
    return 0;*/

}

int verification_chevauchement(t_graphe* map, int ligne, int colonne, int choix, int rotation)//rajouter un changement de couleur du sprite, genre en rouge pour dire que y a un chevauchement?
{
    int verif=0;
    switch (choix) {
        case 1 ://route 1x1
            if(map->mat_adjacence[ligne][colonne]>1){
                verif=1;
            }
            break;
        case 2 :
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
        case 3 ://c'est pour les bat 4x6
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
        case 4:
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
        default:
            break;
    }

    if(verif==1)
    {
        printf("impossible il ya chevauchement\n");
    }
    return verif;
}

///algorithme A*

int estArrive(t_tile *actuel, t_tile *arrive)
{
    if((actuel->position.colonne == arrive->position.colonne) && (actuel->position.ligne == arrive->position.ligne))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


int distance(t_pos a, t_pos b)
{
    int d = abs(a.colonne - b.colonne) + abs(a.ligne - b.ligne);//valeur absolu pour éviter d'avoir des problèmes avec une différence de coordoonées négative
    //printf("Vi: %i ",abs(a.x - b.x) + abs(a.y - b.y));
    //printf(" Vd: %d ",d);
    return d;
}

int heuristic(t_tile *a, t_tile *b)
{
    int h = round(distance((a->position), (b->position)));//arrondi le nombre pour ne pas avoir de float
    //printf("Vh: %d\n",h);
    return h;
}

t_graphe* A_star(t_graphe* g, t_tile* depart, t_tile* arrive/*position souris*/)
{
    int tempG;//pour une maj temporaire de cette composante
    t_tile* case_actuelle,*voisin_actuel;
    t_liste* liste_voisin;
    t_liste* liste_ouverte=creer();//liste des noeuds à analyser
    t_liste* liste_ferme=creer();//liste des noeuds analysés qu'on ne touchera plus
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            g->grille[i][j]->parent=NULL;//on n'oublie pas de dire que les noeuds n'ont pas de prédécésseur au debut de l'algo
        }
    }

    liste_ouverte= insertion_en_triant(liste_ouverte, depart);

    while(!estVide(liste_ouverte))
    {
        liste_ouverte= enlever_noeud_debut(liste_ouverte, &case_actuelle);
        liste_ferme= insererNoeud(liste_ferme, case_actuelle);

        if (existe(liste_ferme, arrive))
        {
            break;
        }

        liste_voisin=case_actuelle->voisin;//on récupère les voisins du noeud en cours d'analyse et on analyse
        while(liste_voisin!=NULL)
        {

            voisin_actuel=liste_voisin->n;

            if(!existe(liste_ferme, voisin_actuel) && g->mat_adjacence[voisin_actuel->position.ligne][voisin_actuel->position.colonne]<=1)//si le voisin n'est pas déjà dans la liste fermé et si on peut posser qqc
            {
                tempG=case_actuelle->g+ heuristic(voisin_actuel, case_actuelle);
                if(!existe(liste_ouverte, voisin_actuel))
                {
                    //on actualise toute les variables de calcul de noeuds
                    voisin_actuel->g=tempG;
                    voisin_actuel->h= heuristic(voisin_actuel, arrive);
                    voisin_actuel->f=voisin_actuel->g+voisin_actuel->h;
                    voisin_actuel->parent=case_actuelle;
                    liste_ouverte= insertion_en_triant(liste_ouverte, voisin_actuel);
                }
                else if (tempG<voisin_actuel->g)
                {
                    //on actualise toute les variables de calcul de noeuds
                    voisin_actuel->g=tempG;
                    voisin_actuel->h= heuristic(voisin_actuel, arrive);
                    voisin_actuel->f=voisin_actuel->g+voisin_actuel->h;
                    voisin_actuel->parent=case_actuelle;
                    actualisation(liste_ouverte, voisin_actuel);
                }
            }
            liste_voisin=liste_voisin->next;
        }
    }
    liberer(liste_ouverte);
    liberer(liste_ferme);
    return g;
}

