//
// Created by alexi on 08/11/2022.
//

#include "listeRelated.h"
#include "jeu2.h"
#include "jeu1.h"
#include "stdio.h"

int calcul_depenses(int choix, int taille_chemin_route/*Algo Taille chemin*/)
{
    int depense;
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

//--------------------------------------------------------------------
///horloge
///horloge
/*
void horloge(int CLK_depuis_debut_prog,int* seconde,int* minute,int* heure)
{

//    printf("%d",CLK_depuis_debut_prog);
//    system("CLS");

    *seconde=CLK_depuis_debut_prog%60;
    if(*seconde==60)
    {
        *minute++;
        *seconde=0;
    }
    if(*minute%60==0)
    {
        *heure++;
    }

}
int main()
{
    clock_t CLK;
    int heure=0;
    int min=0;
    int sec=0;
    //allegro();
    //horloge();
    do
    {
        CLK=clock() / CLOCKS_PER_SEC;
        horloge((int)CLK,&sec,&min, &heure);
        printf("%d:%d:%d",heure,min,sec);
        system("CLS");
    } while (1);//tourne pendant deux heures



}
END_OF_MAIN();
}*/


//gestion de l'eau

t_graphe* initialisation_distance_chateau (t_graphe* map, t_tile* case_chateau)
{
    if (case_chateau->element->orientation == 1)
    {
        for (int i = -2; i < 2; i++)
        {
            for (int j = -2; j < 4; j++)
            {
                map->grille[case_chateau->position.ligne+i][case_chateau->position.colonne+j]->g = 0;
            }
        }
    }
    else if (case_chateau->element->orientation == -1)
    {
        for (int i = -3; i < 3; i++)
        {
            for (int j = -1; j < 3; j++)
            {
                map->grille[case_chateau->position.ligne+i][case_chateau->position.colonne+j]->g = 0;
            }
        }
    }
    return map;
}

/*t_graphe* initialisation_contour_chateau (t_graphe* map)
{
    t_liste* liste_voisin;
    for (int i=0 ; i<NBLIGNE ; i++)
    {
        for (int j=0 ; j<NBCOLONNE ; j++)
        {
            if (map->grille[i][j]->g==2)
            {
                liste_voisin = map->grille[i][j]->voisin;
                while(liste_voisin != NULL)
                {
                    if(liste_voisin->n->element->type==1)//si l'element est une route, alors on a une route adjacente au chateau d'eau et on initialise sa distance à 1
                    {
                        map->grille[liste_voisin->n->position.ligne][liste_voisin->n->position.colonne]->g=1;
                    }
                    liste_voisin=liste_voisin->next;
                }
            }
        }
    }
    return map;
}*/

/*
 * on lance un parcours de tableau des elements, des qu'on tombe sur un chateau d'eau on initialise les cases de distances
 * et
 * on lance un dijkstra a partir de chaque chateau d'eau, si -1 on change la taille, si la nouvelle taille est inférieure on change la taille
 * quand on insère un element dans la liste on lui dit que sa distance est égale à celui de son prédécesseur +1
 * dès qu'on trouve une maison, on retrace le chemin tout de suite en enlevant la quantité d'eau qui a été distribué
 * , ça nous donnera une matrice avec la taille des différents chemin
 * et ensuite penser a modifier le fichier du niveau -1 pour l'affichage des chemins
 */

t_graphe* ecriture_fichier_eau(t_graphe* map, t_tile* case_arrive)
{
    t_tile* temp=case_arrive;

    while(temp!=NULL)
    {
        map->mat_chemin_eau[temp->position.ligne][temp->position.colonne]=1;
        temp=temp->parent;
    }
    FILE* map_eau= fopen("map_eau.txt", "w");
    if(map_eau==NULL)
    {
        printf("Erreur d'ouverture du fichier\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            fprintf(map_eau, "%d ", map->mat_chemin_eau[i][j]);
        }
        fprintf(map_eau, "\n");
    }
    fclose(map_eau);
    return map;
}

void maj_capacite(t_tile* chateau_eau, t_tile* maison)//tester avec ça sinon utiliser un double pointeur
{
    int nb_habitant_restant=maison->element->nb_habitant-maison->element->eau_actuelle;
    if(chateau_eau->element->capacite>=nb_habitant_restant)
    {
        chateau_eau->element->capacite-=nb_habitant_restant;
        maison->element->eau_actuelle+=nb_habitant_restant;
        maison->element->chateau_approvisionnement= insererNoeud2(maison->element->chateau_approvisionnement, chateau_eau, nb_habitant_restant);
    }
    else
    {
        maison->element->eau_actuelle+=chateau_eau->element->capacite;
        maison->element->chateau_approvisionnement= insererNoeud2(maison->element->chateau_approvisionnement, chateau_eau, chateau_eau->element->capacite);
        chateau_eau->element->capacite=0;
    }
}

t_graphe* dijkstra(t_graphe* map, t_tile* sommet_de_depart)
{
    printf("\nlancement dijkstra\n");
    int poids_temp;
    t_tile* case_analysee,* voisin_actuel;
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
                liste_ouverte=insererNoeud(liste_ouverte, map->grille[i][j]);
                liste_voisin=map->grille[i][j]->voisin;
                while(liste_voisin!=NULL)
                {
                    voisin_actuel=liste_voisin->n;
                    if(voisin_actuel->g==-1 && voisin_actuel->element->type==1)
                    {
                        voisin_actuel->g=1;
                        //voisin_actuel->parent=map->grille[i][j];
                        liste_ouverte= insererNoeud(liste_ouverte, voisin_actuel);
                    }
                    liste_voisin=liste_voisin->next;
                }
            }
        }
    }

    //debut de l'analyse
    while(!estVide(liste_ouverte) && sommet_de_depart->element->capacite>0)//tant qu'il reste des noeuds à analyser et que le chateau d'eau peut encore distribuer de l'eau
    {
        liste_ouverte = enlever_noeud_debut(liste_ouverte, &case_analysee);
        liste_ferme = insererNoeud(liste_ferme, case_analysee);
        //printf("noeud actuel [%d][%d]\n", case_analysee->position.ligne, case_analysee->position.colonne);
        liste_voisin=case_analysee->voisin;
        //afficherListe(liste_ferme);pb de boucle infini dans la liste ferme a voir si le temps
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

            if(voisin_actuel->case_mere->element->type>=4 && voisin_actuel->case_mere->element->type<=10)//le voisin est une habitation pb ici
            {
                if(voisin_actuel->case_mere->element->eau_actuelle<voisin_actuel->case_mere->element->nb_habitant)//si il reste des habitants qui doivent être alimenté en eau
                {
                    //voisin_actuel->parent=case_analysee; on met cette ligne si on veut que la case de la maison soit prise dans le chemin
                    printf("maison trouve\n");
                    //printf("case [%d][%d] -> parent [%d][%d]\n", voisin_actuel->position.ligne, voisin_actuel->position.colonne, voisin_actuel->parent->position.ligne, voisin_actuel->parent->position.colonne);
                    //faire toute les maj sur l'habitation en fonction de l'eau distrib
                    maj_capacite(sommet_de_depart, voisin_actuel->case_mere);
                    //retracer le chemin et le mettre sur la map -1
                    printf("compteur eau maison [%d][%d] : %d\n", voisin_actuel->case_mere->position.ligne, voisin_actuel->case_mere->position.colonne, voisin_actuel->case_mere->element->eau_actuelle);
                    temp =voisin_actuel->case_mere->element->chateau_approvisionnement;
                    printf("Approvisionnement : \n");
                    while(temp!=NULL)
                    {
                        printf("chateau [%d][%d] : %d\n", temp->n->position.ligne, temp->n->position.colonne, temp->montant_distribue);
                        temp=temp->next;
                    }
                    map=ecriture_fichier_eau(map, case_analysee);//on part de la route adajacente à la maison
                }

            }
            liste_voisin=liste_voisin->next;
        }
    }
    return map;
}
//la forme est faite, il faut maintenant verifier les conditions et mettre à jour le niveau -1 à chaque fois que l'on tombe sur une maison

t_graphe* reinitialisation_eau(t_graphe* map)
{
    int couleur=1;
    for(int i=0 ; i<NBLIGNE; i++)//parcours de toute les cases du tableau pour trouver les chateaux d'eau
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            map->mat_chemin_eau[i][j]=0;
            if(map->grille[i][j]->element->type == 2)
            {
                map->grille[i][j]->element->capacite=5000;//valeur max
                map->grille[i][j]->element->couleur=couleur;
                couleur++;
            }
            else if(map->grille[i][j]->element->type>=4 && map->grille[i][j]->element->type<=10)
            {
                map->grille[i][j]->element->eau_actuelle=0;
                map->grille[i][j]->element->chateau_approvisionnement= vider_liste(map->grille[i][j]->element->chateau_approvisionnement);
            }
        }
    }
    FILE* map_eau=fopen("map_eau.txt", "w+");
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            fprintf(map_eau, "%d ", map->mat_chemin_eau[i][j]);
        }
        fprintf(map_eau, "\n");
    }
    fclose(map_eau);
    return map;
}

t_graphe* distribution_eau(t_graphe* map)
{
    map= reinitialisation_eau(map);
    for(int i=0 ; i<NBLIGNE; i++)//parcours de toute les cases du tableau pour trouver les chateaux d'eau
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            if (map->grille[i][j]->element->type == 2)//dès qu'on a trouver on lance dijkstra pour ensuite trouver les chemins menant aux dif bat
            {
                printf("chateau d'eau\n");
                map=dijkstra(map, (map->grille[i][j]));
            }
        }
    }

    return map;
}
///Usines electricite (num 3 sur la map)
t_graphe* electricite(t_graphe* map, int* capa_usine)
{
    int compteur_usine=0;
    int elec_distrib=0;
    int verif_route;
    for(int i=0 ; i<NBLIGNE; i++)//parcours de toute les cases du tableau pour trouver les usines elec
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            if (map->grille[i][j]->element->type == 3)//dès qu'on a trouvé on la compte && on verifie si elle est connecté a la route
            {
                verif_route= verification_connexite_route(map, map->grille[i][j]);
                if(verif_route==1)
                {
                    compteur_usine++;
                }
            }
        }
    }
    //On calcul la capacite totale de la ville
    *capa_usine=(compteur_usine*5000);

    for (int i = 0; i < NBLIGNE; i++)//parcours de toute les cases du tableau pour trouver les habitations
    {
        for (int j = 0; j < NBCOLONNE; j++) {
            if ((map->grille[i][j]->element->type >= 4) && (map->grille[i][j]->element->type <=9) )//dès qu'on a trouve une habitation
            {
                verif_route= verification_connexite_route(map, map->grille[i][j]);
                if(verif_route==1)
                {
                    if(map->grille[i][j]->element->nb_habitant < *capa_usine)
                    {
                        //on note le batiment trouve comme etant alimente
                        map->grille[i][j]->element->alimente=1;
                        //on soustrait le nombre d'habitants de cette habitation a la capacite totale de la ville
                        *capa_usine = (*capa_usine - map->grille[i][j]->element->nb_habitant);
                        elec_distrib+=map->grille[i][j]->element->nb_habitant;
                    }
                }
            }
        }
    }
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            if(map->grille[i][j]->element->type==2)
            {
                verif_route= verification_connexite_route(map, map->grille[i][j]);
                if(verif_route==1)
                {
                    if(elec_distrib>map->grille[i][j]->element->capacite)
                    {
                        map->grille[i][j]->element->capacite=0;
                        elec_distrib-=map->grille[i][j]->element->capacite;
                    }
                    else if(elec_distrib>0)
                    {
                        map->grille[i][j]->element->capacite-=elec_distrib;
                        elec_distrib=0;
                    }
                }
            }
        }
    }
    return map;
}


int verification_connexite_route(t_graphe* map, t_tile* case_actu)
{
    int verif_route=0;
    t_liste* voisin_case;
    if(case_actu->element->type==3)
    {
        if(case_actu->element->orientation==1)
        {
            for(int i=-2; i<2; i++)
            {
                for(int j=-2; j<4; j++)
                {
                    voisin_case=map->grille[case_actu->position.ligne+i][case_actu->position.colonne+j]->voisin;
                    while(voisin_case!=NULL)
                    {
                        if(voisin_case->n->element->type==1)
                        {
                            verif_route=1;
                        }
                        voisin_case=voisin_case->next;
                    }
                }
            }
        }
        else if(case_actu->element->orientation==-1)
        {
            for(int i = -3; i < 3; i++)
            {
                for(int j = -1; j < 3; j++)
                {
                    voisin_case=map->grille[case_actu->position.ligne+i][case_actu->position.colonne+j]->voisin;
                    while(voisin_case!=NULL)
                    {
                        if(voisin_case->n->element->type==1)
                        {
                            verif_route=1;
                        }
                        voisin_case=voisin_case->next;
                    }
                }
            }
        }

    }
    else if(case_actu->element->type>=4 && case_actu->element->type<=10)
    {
        for(int i=-1; i<2; i++)
        {
            for(int j=-1; j<2; j++)
            {
                voisin_case=map->grille[case_actu->position.ligne+i][case_actu->position.colonne+j]->voisin;
                while(voisin_case!=NULL)
                {
                    if(voisin_case->n->element->type==1)
                    {
                        verif_route=1;
                    }
                    voisin_case=voisin_case->next;
                }
            }
        }
    }
    return verif_route;

}