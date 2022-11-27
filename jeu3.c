//
// Created by Estelle on 15/11/2022.
//

#include "jeu3.h"
#include "listeRelated.h"
#include "jeu2.h"


int incendie ()
{
    //random pour incendie
    int aleatoire;
    aleatoire = rand() % 15;//aléatoire entre 0 et 14
    //printf ("nbre aleatoire --> %d\n", aleatoire);
    if(aleatoire==1)
    {
        //printf("en feu!\n");
        return 1;
    }
    else
    {
        //printf("pas en feu\n");
        return 0;
    }
}

t_graphe* dijkstra_incendie(t_graphe* map, t_tile* sommet_de_depart, t_tile* case_en_feu, int* fin_recherche, BUFFER* liste_buffer, IMAGE* liste_image)//meme dijkstra que pour l'eau mais adapté à l'incendie
{
    //printf("\nlancement dijkstra incendie\n");
    int poids_temp;
    int fin=0;
    t_tile* case_analysee,* voisin_actuel, *parcourt;
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
                //liste_ouverte=insererNoeud(liste_ouverte, map->grille[i][j]);
                liste_voisin=map->grille[i][j]->voisin;
                while(liste_voisin!=NULL)
                {
                    voisin_actuel=liste_voisin->n;
                    if(voisin_actuel->g==-1 && voisin_actuel->element->type==1)
                    {
                        voisin_actuel->g=1;//on insère les cases de route
                        //voisin_actuel->parent=map->grille[i][j]; si on veut compter la case de pompier dans le chemin

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

                if(tailleChemin(case_analysee)<=18)//si le chemin est inférieur à 18 sinon on n'est pas protégé
                {
                    //c'est cool on est protegé

                    voisin_actuel->case_mere->element->incendie=0;
                    parcourt=case_analysee;
                    printf("maison [%d][%d] protegee par caserne\n", voisin_actuel->case_mere->position.ligne, voisin_actuel->case_mere->position.colonne);
                    while(parcourt->parent!=NULL)
                    {
                        draw_sprite(liste_buffer->buffer_map, liste_image->feu, (SCREEN_W/2-36)+parcourt->position.colonne*14-parcourt->position.ligne*14, parcourt->position.colonne*8+parcourt->position.ligne*8);
                        parcourt=parcourt->parent;
                    }
                    *fin_recherche=1;
                }
                else
                {
                    printf("maison [%d][%d] non protegee par casene\n", voisin_actuel->case_mere->position.ligne, voisin_actuel->case_mere->position.colonne);
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
                if(verification_connexite_route(map, map->grille[i][j]))//on lance le dijkstra a partir d'une caserne seulement si elle est connexe a la route
                {
                    map=dijkstra_incendie(map, map->grille[i][j], case_en_feu, &fin_recherche, liste_buffer, liste_image);
                }
                else
                {
                    //printf("pas connexe\n");
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

void sauvegarde(long compteur_argent, long temps, t_graphe* map, int nb_habitant, int mode_de_jeu)
{
    t_liste2* temp;
    t_liste* temp_hab;
    int compteur;
    FILE* sauvegarde=fopen("sauvegarde.txt", "w+");

    //sauvegarde des infomations primaires
    fprintf(sauvegarde, "%d ", mode_de_jeu);
    fprintf(sauvegarde, "\n");
    fprintf(sauvegarde, "%ld ", compteur_argent);
    fprintf(sauvegarde, "\n");
    fprintf(sauvegarde,  "%ld ", temps);
    fprintf(sauvegarde, "\n");
    fprintf(sauvegarde, "%d", nb_habitant);
    fprintf(sauvegarde, "\n");

    //sauvegarde de la matrice de tile
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            fprintf(sauvegarde,  "%d ", map->grille[i][j]->element->type);//type
            fprintf(sauvegarde,  "%d ", map->grille[i][j]->element->orientation);//on garde l'orientation
            fprintf(sauvegarde, "%d ", map->grille[i][j]->case_mere->position.ligne);//on stock la position de la case mère
            fprintf(sauvegarde, "%d ", map->grille[i][j]->case_mere->position.colonne);
            if(map->grille[i][j]->element->type==2)//si c'est un chateau d'eau
            {
                fprintf(sauvegarde, "%d ", map->grille[i][j]->element->capacite);
                fprintf(sauvegarde, "%d ", map->grille[i][j]->element->couleur);
            }
            if(map->grille[i][j]->element->type>=4 && map->grille[i][j]->element->type<=9)//si c'est une habitation
            {
                fprintf(sauvegarde, "%d ", map->grille[i][j]->element->nb_habitant);
                fprintf(sauvegarde, "%d ", map->grille[i][j]->element->eau_actuelle);

                fprintf(sauvegarde, "%ld ", map->grille[i][j]->element->compteur);


                temp=map->grille[i][j]->element->chateau_approvisionnement;//information sur les chateaux alimantant la maison
                compteur=0;
                while(temp!=NULL)
                {
                    compteur++;
                    temp=temp->next;
                }
                fprintf(sauvegarde, "%d ", compteur);//pour connaitre le nombre de chiffres à lire lors de la lecture

                temp=map->grille[i][j]->element->chateau_approvisionnement;
                while(temp!=NULL)
                {
                    fprintf(sauvegarde, "%d ", temp->n->position.ligne);//on stock la position pour pouvoir regenerer la liste chainee dans le futur
                    fprintf(sauvegarde, "%d ", temp->n->position.colonne);
                    fprintf(sauvegarde, "%d ", temp->montant_distribue);
                    temp=temp->next;
                }
                fprintf(sauvegarde, "%d ", map->grille[i][j]->element->incendie);//info sur l'incendie
                fprintf(sauvegarde, "%d ", map->grille[i][j]->element->argent);//info sur l'affichage de la pièce
                fprintf(sauvegarde, "%d ", map->grille[i][j]->element->alimente);//indo sur l'alimantation
            }

            fprintf(sauvegarde, "\n");// on passe a la case suivante
        }
    }

    //sauvegarde matrice adajcence
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            fprintf(sauvegarde, "%d ", map->mat_adjacence[i][j]);
        }
        fprintf(sauvegarde, "\n");
    }

    //sauvegarde des habitations
    temp_hab=map->liste_hab;
    fprintf(sauvegarde, "%d ", taille(map->liste_hab));
    while (temp_hab!=NULL)
    {
        fprintf(sauvegarde, "%d ", temp_hab->n->position.ligne);
        fprintf(sauvegarde, "%d ", temp_hab->n->position.colonne);
        temp_hab=temp_hab->next;
    }
    fclose(sauvegarde);
}
t_graphe* lecture_sauvegarde(t_graphe* map, long* compteur_argent, long* temps, int* nb_habitant, int* mode_de_jeu)
{
    int compteur;
    int ligne_temp, colonne_temp, montant_distrib;
    FILE* sauvegarde=fopen("sauvegarde.txt", "r");
    if(sauvegarde==NULL)
    {
        printf("Erreur d'ouverture du fichier\n");
        exit(EXIT_FAILURE);
    }
    //informations primaire
    fscanf(sauvegarde, "%d", mode_de_jeu);
    fscanf(sauvegarde, "%ld", compteur_argent);
    fscanf(sauvegarde,  "%ld", temps);
    fscanf(sauvegarde, "%d", nb_habitant);

    //informations tile
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            fscanf(sauvegarde,  "%d", &map->grille[i][j]->element->type);//type
            fscanf(sauvegarde,  "%d", &map->grille[i][j]->element->orientation);//on garde l'orientation
            fscanf(sauvegarde, "%d", &ligne_temp);//on stock la position de la case mère
            fscanf(sauvegarde, "%d", &colonne_temp);
            map->grille[i][j]->case_mere=map->grille[ligne_temp][colonne_temp];
            if(map->grille[i][j]->element->type==2)//si c'est un chateau d'eau
            {
                fscanf(sauvegarde, "%d", &map->grille[i][j]->element->capacite);
                fscanf(sauvegarde, "%d", &map->grille[i][j]->element->couleur);
            }
            if(map->grille[i][j]->element->type>=4 && map->grille[i][j]->element->type<=9)//si c'est une habitation
            {
                fscanf(sauvegarde, "%d", &map->grille[i][j]->element->nb_habitant);
                fscanf(sauvegarde, "%d", &map->grille[i][j]->element->eau_actuelle);
                fscanf(sauvegarde, "%ld", &map->grille[i][j]->element->compteur);

                fscanf(sauvegarde, "%d", &compteur);
                for(int k=0; k<compteur; k++)
                {
                    fscanf(sauvegarde, "%d", &ligne_temp);//on stock la position pour pouvoir regenerer la liste chainee dans le futur
                    fscanf(sauvegarde, "%d", &colonne_temp);
                    fscanf(sauvegarde, "%d", &montant_distrib);
                    map->grille[i][j]->element->chateau_approvisionnement=insererNoeud2( map->grille[i][j]->element->chateau_approvisionnement, map->grille[ligne_temp][colonne_temp], montant_distrib);
                }

                fscanf(sauvegarde, "%d", &map->grille[i][j]->element->incendie);//info sur l'incendie
                fscanf(sauvegarde, "%d", &map->grille[i][j]->element->argent);//info sur l'affichage de la pièce
                fscanf(sauvegarde, "%d", &map->grille[i][j]->element->alimente);//indo sur l'alimantation
            }
        }
    }

    //récupération de la matrice d'adjacence
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            fscanf(sauvegarde, "%d", &map->mat_adjacence[i][j]);
        }
    }

    //recupération des habitations
    fscanf(sauvegarde, "%d", &compteur);
    for(int k=0; k<compteur; k++)
    {
        fscanf(sauvegarde, "%d", &ligne_temp);
        fscanf(sauvegarde, "%d ", &colonne_temp);
        map->liste_hab= insererNoeud(map->liste_hab, map->grille[ligne_temp][colonne_temp]);
    }

    fclose(sauvegarde);
    return map;
}

t_graphe* suppresion(t_graphe* map, t_pos souris)
{
    if(mouse_b & 1)
    {
        if (map->grille[souris.ligne][souris.colonne]->case_mere->element->type==2)
        {
            map->grille[souris.ligne][souris.colonne]->case_mere->element->type=0;
            if(map->grille[souris.ligne][souris.colonne]->case_mere->element->orientation==1)
            {
                for (int i = -2; i < 2; i++)
                {
                    for (int j = -2; j < 4; j++)
                    {
                        map = remplissage_matrice_adjacence(map, souris.ligne + i, souris.colonne + j,0, map->grille[souris.ligne][souris.colonne]);
                    }
                }
            }
            else
            {
                for (int i = -3; i < 3; i++)
                {
                    for (int j = -1; j < 3; j++)
                    {
                        map = remplissage_matrice_adjacence(map, souris.ligne + i, souris.colonne + j,0, map->grille[souris.ligne][souris.colonne]);
                    }
                }
            }

        }
    }
}