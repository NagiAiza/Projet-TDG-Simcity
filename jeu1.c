//
// Created by alexi on 02/11/2022.
//
#include "jeu1.h"
#include "math.h"
#include "listeRelated.h"


t_tile* associerCaseSouris(t_graphe* map, t_pos souris)//pas utilse dans le projet mais ça renvoyait la case exacte du tableau
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
            g->grille[i][j] = (t_tile*)calloc(1, sizeof(t_tile));//Verifier la ligne peut etre une erreur
            if(g->grille[i][j]==NULL)
            {
                printf("Erreur maloc\n");
                exit(EXIT_FAILURE);
            }
            g->grille[i][j]->position.colonne = j;
            g->grille[i][j]->position.ligne = i;

            g->grille[i][j]->case_mere = g->grille[i][j];

            g->grille[i][j]->element=(t_batiment*)calloc(1, sizeof(t_batiment));
            g->grille[i][j]->element->position.ligne=i;
            g->grille[i][j]->element->position.colonne=j;
            g->grille[i][j]->element->incendie=0;
        }
    }
    g->grille[17][0]->element->type=1;//emplacement de départ de la route
    initialiserVoisin(g->grille, NBLIGNE, NBCOLONNE);
    return g;
}

t_graphe* makeGrid()//penser a crer la libération de données
{
    t_graphe* g=(t_graphe*) calloc(1, sizeof(t_graphe));

    g->grille=(t_tile***)calloc(NBLIGNE, sizeof(t_tile**));

    for(int i=0; i<NBLIGNE; i++)
    {
        g->grille[i] = (t_tile**) calloc (NBCOLONNE, sizeof(t_tile*));
    }
    g= initialiserGrille(g);//pour initialiser toute les cases individuellement

    //initialisation des diverses matrices
    g->mat_adjacence=(int **) calloc(NBLIGNE,sizeof(int*));
    g->mat_chemin_eau=(int **) calloc(NBLIGNE,sizeof(int*));
    g->mat_chemin_elec=(int **) calloc(NBLIGNE,sizeof(int*));
    for(int i=0; i<NBLIGNE; i++)
    {
        g->mat_adjacence[i]=(int*)calloc(NBCOLONNE,sizeof(int));
        g->mat_chemin_eau[i]=(int*) calloc(NBCOLONNE,sizeof(int));
        g->mat_chemin_elec[i]=(int*) calloc(NBCOLONNE,sizeof(int));
    }
    g->mat_adjacence[17][0]=1;//pour avoir la première route posée
    //création de la liste des habitants
    g->liste_hab=creer();
    return g;
}

void affichageGridMere(t_graphe* g)//a servi a debugger, affiche toute les cases et leurs case mère
{
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            if( g->grille[i][j]->case_mere==NULL)
            {
                printf("pas de case mere\n");
                exit(EXIT_FAILURE);
            }
            printf("case [%d][%d] mere-> [%d][%d] type %d\n", i, j, g->grille[i][j]->case_mere->position.ligne, g->grille[i][j]->case_mere->position.colonne,  g->grille[i][j]->case_mere->element->type);
        }
    }
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

void majFichierPlacementElement(t_graphe* g)// ou mettre rotation directement dans la structure? on a fait les 2
{

    FILE* elementCarte= fopen("element_map.txt", "w");
    FILE* rotation_element_carte= fopen("rotation_element_map.txt", "w");

    for(int i=0; i<NBLIGNE; i++)//maj à partir de la matrice du graphe
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
    g->grille[ligne][colonne]->element->type=type;//on place l'élement mère de l'élement
    g->grille[ligne][colonne]->element->orientation=rotation;
    majFichierPlacementElement(g);//^puis on met à jour le fichier des elements
    return g;
}

t_graphe* remplissage_matrice_adjacence(t_graphe* g, int ligne, int colonne, int type, t_tile* case_mere)//à remplir en fonction de la rotation du bail
{
    g->mat_adjacence[ligne][colonne]=type;
    g->grille[ligne][colonne]->case_mere=case_mere;//on n'oublie pas de désigner la case mere
    return g;
}


void initialisationElementCarte()//
{
    FILE* elementCarte= fopen("element_map.txt", "w+");
    FILE* rotation_element_map=fopen("rotation_element_map.txt", "w+");
    FILE* map_eau=fopen("map_eau.txt", "w+");
    FILE* map_elec=fopen("map_elec.txt", "w+");

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
            fprintf(map_eau, "%d ", 0);
            fprintf(map_elec, "%d ", 0);
        }
        fprintf(elementCarte, "\n");
        fprintf(rotation_element_map, "\n");
        fprintf(map_eau, "\n");
        fprintf(map_elec, "\n");
    }
    fclose(rotation_element_map);
    fclose(elementCarte);
    fclose(map_eau);
    fclose(map_elec);
}


/*
« the memory allocation using malloc() is not de-allocated on its own. So, “free()” method is
 used to de-allocate the memory. But the free() method is not compulsory to use. If free() is
 not used in a program the memory allocated using malloc() will be de-allocated after completion
 of the execution of the program (included program execution time is relatively small and the
 program ends normally). »
 */

void liberation_donnee(t_graphe* g)//n'est pas forcément utile (voir ci dessus)
{
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            free(g->grille[i][j]->element);
            free(g->grille[i][j]);
        }
        free(g->grille[i]);
        free(g->mat_adjacence[i]);
        free(g->mat_chemin_elec[i]);
        free(g->mat_chemin_eau[i]);
    }
    free(g->grille);
    free(g->mat_adjacence);
    free(g->mat_chemin_elec);
    free(g->mat_chemin_eau);
    free(g);

}

int placement_route(t_graphe* map, int ligne, int colonne)
{
    t_liste* liste_voisin;
    //dans cette verion on ne peut placer la route que sur une case étant déjà une route
    if(map->grille[ligne][colonne]->element->type==1)//si l'element est une route
    {
        return 1;
    }
    else
    {
        liste_voisin=map->grille[ligne][colonne]->voisin;//ou que un de ses voisin est une route
        while(liste_voisin!=NULL)
        {
            if(liste_voisin->n->element->type==1)
            {
                return 1;
            }
            liste_voisin=liste_voisin->next;
        }
        printf("on ne peut pas placer la route\n");//si l'utilisateur veut créer une route non connexe à la route de base
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
    switch (choix) {//on fonction de l'orientation de chaque batiment (surtout les 4x6)
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
        case 10:
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
        default:
            break;
    }

    /*if(verif==1)
    {
        printf("impossible il ya chevauchement\n");
    }*/
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
            g->grille[i][j]->g=-1;//ces trois lignes sont pas utiles mais plus par précuations
            g->grille[i][j]->h=-1;
            g->grille[i][j]->f=-1;
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

///Fonctions pour initiliser les batiments à leur pose
void initialisation_habitation(t_graphe* map, t_tile* case_hab)
{
    case_hab->element->nb_habitant=0;
    case_hab->element->eau_actuelle=0;
    case_hab->element->alimente=0;
    case_hab->element->compteur=clock()/CLOCKS_PER_SEC;//on détermine le moment de pose
    case_hab->element->chateau_approvisionnement=creer2();
    map->liste_hab= insererNoeud(map->liste_hab, case_hab);//on insere l'habitation dans la liste des habitations du graphe
}

void initialisation_chateau_eau(t_tile* case_chateau)
{
    case_chateau->element->capacite=5000;//on initialise à 5000 de capacité
}

void initialisation_centrale(t_tile* case_elec)
{
    case_elec->element->capacite=5000;//on initialise à 5000 de capacité
}
