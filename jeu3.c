//
// Created by Estelle on 15/11/2022.
//

#include "jeu3.h"
#include "listeRelated.h"
#include "jeu2.h"


/*t_graphe* remplissage_matrice_caserne(t_graphe* g, int ligne, int colonne, int type)
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
/*
    //FAIRE AFFICHAGE MAP CASERNE GENRE UN ETAGE AVEC LA ZONE QUE PREND LA CASERNE OU QUAND ON SELECT LA CASERNE VOIR CASE INFECTE
    return map;
}*/

int incendie ()
{
    //random pour incendie
    int aleatoire;
    aleatoire = rand() % 15;
    printf ("nbre aleatoire --> %d\n", aleatoire);
    if(aleatoire==1)
    {
        printf("en feu!\n");
        return 1;
    }
    else
    {
        printf("pas en feu\n");
        return 0;
    }
}

t_graphe* dijkstra_incendie(t_graphe* map, t_tile* sommet_de_depart, t_tile* case_en_feu, int* fin_recherche, BUFFER* liste_buffer, IMAGE* liste_image)
{
    printf("\nlancement dijkstra incendie\n");
    int poids_temp;
    int fin=0;
    t_tile* case_analysee,* voisin_actuel, *parcourt;
    t_liste* liste_voisin;
    t_liste* liste_ouverte=creer();
    t_liste* liste_ferme=creer();//pas forcement utile cette ligne
    t_liste2* temp;
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
                //liste_ouverte=insererNoeud(liste_ouverte, map->grille[i][j]);
                liste_voisin=map->grille[i][j]->voisin;
                while(liste_voisin!=NULL)
                {
                    voisin_actuel=liste_voisin->n;
                    if(voisin_actuel->g==-1 && voisin_actuel->element->type==1)
                    {
                        voisin_actuel->g=1;//on insère les cases de route
                        //voisin_actuel->parent=map->grille[i][j];

                        liste_ouverte= insererNoeud(liste_ouverte, voisin_actuel);

                    }
                    liste_voisin=liste_voisin->next;
                }
            }
        }
    }

    //debut de l'analyse
    while(!estVide(liste_ouverte) && fin==0)//tant qu'il reste des noeuds à analyser et que le chateau d'eau peut encore distribuer de l'eau
    {
        liste_ouverte = enlever_noeud_debut(liste_ouverte, &case_analysee);
        liste_ferme = insererNoeud(liste_ferme, case_analysee);

        liste_voisin=case_analysee->voisin;
        while(liste_voisin!=NULL)
        {
            voisin_actuel=liste_voisin->n;
            //printf("case :[%d][%d]\n", voisin_actuel->position.ligne, voisin_actuel->position.colonne);
            poids_temp=case_analysee->g+ heuristic(case_analysee, voisin_actuel);


            if(!existe(liste_ferme, voisin_actuel) && voisin_actuel->element->type==1)
            {
                if(!existe(liste_ouverte, voisin_actuel))//on verifie que le sommet n'est pas dans la liste ouverte et fermé et que c'est une route
                {
                    //printf("insertion\n");
                    voisin_actuel->g=poids_temp;
                    voisin_actuel->parent=case_analysee;
                    liste_ouverte= insertion_en_triant2(liste_ouverte, voisin_actuel);//dans ce cas on l'insère dans la liste
                }
                else
                {
                    if((poids_temp<voisin_actuel->g && voisin_actuel->g!=-1))//si la nouvelle valeur de distance est inférieure à l'ancienne on actualise la distance et si c'est une route
                    {
                        voisin_actuel->g=poids_temp;
                        voisin_actuel->parent=case_analysee;
                        if(existe(liste_ouverte, voisin_actuel))
                        {
                            //printf("actu\n");
                            liste_ouverte = actualisation(liste_ouverte, voisin_actuel);//on actualise le sommet pour le remettre à la bonne place
                        }
                    }
                }

            }

            if(voisin_actuel->case_mere==case_en_feu)//le voisin est une habitation pb ici
            {
                //verifier taille chemin

                if(tailleChemin(case_analysee)<=18)
                {
                    //c'est cool on est protegé

                    voisin_actuel->case_mere->element->incendie=0;
                    parcourt=case_analysee;
                    while(parcourt->parent!=NULL)
                    {
                        draw_sprite(liste_buffer->buffer_map, liste_image->feu, (SCREEN_W/2-36)+parcourt->position.colonne*14-parcourt->position.ligne*14, parcourt->position.colonne*8+parcourt->position.ligne*8);
                        parcourt=parcourt->parent;
                    }
                    *fin_recherche=1;
                }
                else
                {
                    printf("non protege par cette casene\n");
                    *fin_recherche=0;
                }
                fin=1;
            }
            liste_voisin=liste_voisin->next;
        }
    }
    return map;
}

t_graphe* gestion_incendie(t_graphe* map, t_tile* case_en_feu, BUFFER* liste_buffer, IMAGE* liste_image)
{
    int fin_recherche=0;
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            if (map->grille[i][j]->element->type==10)
            {
                //On lance le A* pour l'incendie
                if(verification_connexite_route(map, map->grille[i][j]))
                {
                    printf("connexe\n");
                    map=dijkstra_incendie(map, map->grille[i][j], case_en_feu, &fin_recherche, liste_buffer, liste_image);
                }
                else
                {
                    printf("pas connexe\n");
                }
            }
            if(fin_recherche)
            {
                break;
            }
        }
        if(fin_recherche)
        {
            break;
        }
    }
    return map;
}
