//
// Created by alexi on 08/11/2022.
//

#include "listeRelated.h"
#include "Affichage.h"
#include "jeu2.h"
#include "jeu1.h"
#include "stdio.h"
#include "jeu3.h"

int calcul_depenses(int choix, int taille_chemin_route)//le deuxième paramètre c'est seulement pour la depense de route
{
    int depense;
    switch (choix) {
        case 1://achat d'une route
            depense = 10*taille_chemin_route;
            break;
        case 2://achat d'un chateau d'eau
        case 3://achat d'une centrale
            depense = 100000;
            break;
        case 4:
            depense = 1000;
            break;
        case 5:
            depense = 10000;
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

void maj_capacite(t_tile* chateau_eau, t_tile* maison)
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
    //printf("\nlancement dijkstra\n");
    int poids_temp;
    t_tile* case_analysee,* voisin_actuel;
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
            //printf("case :[%d][%d] de type %d\n", voisin_actuel->position.ligne, voisin_actuel->position.colonne, voisin_actuel->element->type);
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

            if(voisin_actuel->case_mere->element->type>=4 && voisin_actuel->case_mere->element->type<=9)//le voisin est une habitation pb ici
            {
                if(voisin_actuel->case_mere->element->eau_actuelle<voisin_actuel->case_mere->element->nb_habitant)//si il reste des habitants qui doivent être alimenté en eau
                {
                    //voisin_actuel->parent=case_analysee; on met cette ligne si on veut que la case de la maison soit prise dans le chemin
                    //printf("maison trouve\n");
                    //printf("case [%d][%d] -> parent [%d][%d]\n", voisin_actuel->position.ligne, voisin_actuel->position.colonne, voisin_actuel->parent->position.ligne, voisin_actuel->parent->position.colonne);
                    //faire toute les maj sur l'habitation en fonction de l'eau distrib
                    maj_capacite(sommet_de_depart, voisin_actuel->case_mere);
                    //retracer le chemin et le mettre sur la map -1
                    //printf("compteur eau maison [%d][%d] : %d\n", voisin_actuel->case_mere->position.ligne, voisin_actuel->case_mere->position.colonne, voisin_actuel->case_mere->element->eau_actuelle);
                    /*temp =voisin_actuel->case_mere->element->chateau_approvisionnement;
                    //printf("Approvisionnement : \n");
                    while(temp!=NULL)
                    {
                        printf("chateau [%d][%d] : %d\n", temp->n->position.ligne, temp->n->position.colonne, temp->montant_distribue);
                        temp=temp->next;
                    }*/
                    map=ecriture_fichier_eau(map, case_analysee);//on part de la route adajacente à la maison
                }
                if(voisin_actuel->case_mere->element->type==4)
                {
                    voisin_actuel->case_mere->element->eau_actuelle=1;
                    voisin_actuel->case_mere->element->chateau_approvisionnement= insererNoeud2(voisin_actuel->case_mere->element->chateau_approvisionnement, sommet_de_depart, voisin_actuel->case_mere->element->capacite);
                    sommet_de_depart->element->capacite-=1;
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
    struct liste* liste_hab=map->liste_hab;
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

    while(liste_hab!=NULL)
    {

        verif_route= verification_connexite_route(map, liste_hab->n);
        if(verif_route==1)
        {
            if(liste_hab->n->element->nb_habitant <= *capa_usine && *capa_usine!=0)
            {
                //on note le batiment trouve comme etant alimente
                liste_hab->n->element->alimente=1;
                //on soustrait le nombre d'habitants de cette habitation a la capacite totale de la ville
                *capa_usine = (*capa_usine - liste_hab->n->element->nb_habitant);
                elec_distrib+=liste_hab->n->element->nb_habitant;
            }
            else
            {
                liste_hab->n->element->alimente=0;
            }
        }
        liste_hab=liste_hab->next;
    }

    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            if(map->grille[i][j]->element->type==3)
            {
                map->grille[i][j]->element->capacite=5000;
                verif_route= verification_connexite_route(map, map->grille[i][j]);
                if(verif_route==1)
                {
                    if(elec_distrib>5000)
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
    map= distribution_elec(map);
    return map;
}

t_graphe* ecriture_fichier_elec(t_graphe* map, t_tile* case_arrive)
{
    t_tile* temp=case_arrive;

    while(temp!=NULL)
    {
        map->mat_chemin_elec[temp->position.ligne][temp->position.colonne]=1;
        temp=temp->parent;
    }
    FILE* map_elec= fopen("map_elec.txt", "w");
    if(map_elec==NULL)
    {
        printf("Erreur d'ouverture du fichier\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            fprintf(map_elec, "%d ", map->mat_chemin_elec[i][j]);
        }
        fprintf(map_elec, "\n");
    }
    fclose(map_elec);
    return map;
}



t_graphe* BFS(t_graphe* map, t_tile* sommet_depart)
{
    t_tile* noeud;
    for(int i=0; i<NBLIGNE; i++) //initialisation du bfs
    {
        for(int j=0; j<NBCOLONNE; j++) //initialisation du bfs
        {
            map->grille[i][j]->parent=NULL; //initialisation du bfs
        }
    }

    t_liste* liste_ouverte=creer(); //liste des elem qu'on modifie
    t_liste* liste_sommet_plus_toucher=creer(); //liste pour noircir
    t_liste* liste_temporaire; //liste des sommets qu'on étudie

//initialisation du BFS en fonction de l'orientation du bat
    if (sommet_depart->element->orientation == 1)
    {
        for (int i = -2; i < 2; i++)
        {
            for (int j = -2; j < 4; j++)
            {
                liste_temporaire=map->grille[sommet_depart->position.ligne+i][sommet_depart->position.colonne+j]->voisin;
                while(liste_temporaire!=NULL)
                {
                   if(liste_temporaire->n->element->type==1 && !existe(liste_ouverte, liste_temporaire->n)) //si route
                   {
                       liste_ouverte=insererNoeudFin(liste_ouverte,liste_temporaire->n);//on insère
                       liste_temporaire->n->parent=map->grille[sommet_depart->position.ligne+i][sommet_depart->position.colonne+j];
                   }
                    liste_temporaire=liste_temporaire->next;
                }
            }
        }
    }
    else if (sommet_depart->element->orientation == -1)
    {
        for (int i = -3; i < 3; i++)
        {
            for (int j = -1; j < 3; j++)
            {
                liste_temporaire=map->grille[sommet_depart->position.ligne+i][sommet_depart->position.colonne+j]->voisin;
                while(liste_temporaire!=NULL)
                {
                    if(liste_temporaire->n->element->type==1) //si route
                    {
                        liste_ouverte=insererNoeudFin(liste_ouverte,liste_temporaire->n); //on insère
                    }
                    liste_temporaire=liste_temporaire->next;
                }
            }
        }
    }
    while(!estVide(liste_ouverte))// parcours de la liste
    {

        liste_ouverte=enlever_noeud_debut(liste_ouverte, &noeud);
        liste_sommet_plus_toucher= insererNoeud(liste_sommet_plus_toucher, noeud);//pour noircir
        liste_temporaire=noeud->voisin;
        //printf("case de depart [%d][%d]:\n", noeud->position.ligne, noeud->position.colonne);

        while(liste_temporaire!=NULL)
        {
            if(liste_temporaire->n->case_mere->element->type>=4 && liste_temporaire->n->case_mere->element->type<=9 && liste_temporaire->n->case_mere->element->alimente==1)
            {
                map= ecriture_fichier_elec(map, noeud);

            }
            if(!existe(liste_ouverte,liste_temporaire->n)   && !existe(liste_sommet_plus_toucher,liste_temporaire->n)  && liste_temporaire->n->element->type==1)
            {
                liste_ouverte=insererNoeudFin(liste_ouverte,liste_temporaire->n);
                //afficherListe(liste_ouverte);
                liste_temporaire->n->parent=noeud;
            }
            liste_temporaire=liste_temporaire->next;
        }
    }
    return map;
}

t_graphe* distribution_elec(t_graphe* map)
{
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            map->mat_chemin_elec[i][j]=0;
        }
    }
    for(int i=0 ; i<NBLIGNE; i++)//parcours de toute les cases du tableau pour trouver les chateaux d'eau
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            if (map->grille[i][j]->element->type == 3)//dès qu'on a trouver on lance dijkstra pour ensuite trouver les chemins menant aux dif bat
            {
                map=BFS(map, (map->grille[i][j]));
            }
        }
    }

    return map;
}
int verification_connexite_route(t_graphe* map, t_tile* case_actu)
{
    int verif_route=0;
    t_liste* voisin_case;
    if(case_actu->element->type==3 || case_actu->element->type==2 || case_actu->element->type==10)//si c'est un chateau d'eau/une centrale/une caserne
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

int validation_evolution_communiste(t_tile* batiment, int* nb_habitant, int compteur_eau)//renvoie 1 quand il y a un changement sur la map, 0 quand y a rien
{
    switch (batiment->element->type) {
        case 4:
            //printf("evolution\n");
            if(batiment->element->alimente==1 && batiment->element->eau_actuelle==1)//on met 1 d'eau sur la batiment pour dire que'il y a possibilité d'amélioration
            {
                batiment->element->type++;
                batiment->element->nb_habitant=10;
                *nb_habitant+=10;
                return 1;
            }
            else
            {
                return 0;
            }
        case 5:
            //printf("evolution\n");

            if(batiment->element->alimente==1 && batiment->element->incendie!=1)
            {
                if(((float)batiment->element->eau_actuelle/(float)batiment->element->nb_habitant==1) && batiment->element->incendie!=1 )//si l'habitation est alimenté a plus de 90% en eau elle s'améliore
                {

                    batiment->element->type++;
                    batiment->element->nb_habitant=50;
                    *nb_habitant+=40;
                    return 1;
                }
                else if((float)batiment->element->eau_actuelle/(float)batiment->element->nb_habitant>0)//0% car si on est a 0 personne n'est alimenté et donc ça passe a l'état de ruine
                {
                    return 0;//il se passe rien
                }
                else
                {
                    batiment->element->type=9;//on passe a l'état de ruine
                    batiment->element->nb_habitant=0;
                    *nb_habitant-=10;
                    return 1;

                }
            }
            else
            {
                batiment->element->type=9;//on passe a l'état de ruine
                batiment->element->nb_habitant=0;
                *nb_habitant-=10;
                return 1;

            }
        case 6:
            //printf("evolution\n");

            if(batiment->element->incendie!=1)
            {

                if(batiment->element->alimente==1)
                {
                    if((float)batiment->element->eau_actuelle/(float)batiment->element->nb_habitant==1)//si l'habitation est alimenté a plus de 90% en eau elle s'améliore
                    {
                        batiment->element->type++;
                        batiment->element->nb_habitant=100;
                        *nb_habitant+=50;
                        return 1;
                    }
                    else if((float)batiment->element->eau_actuelle/(float)batiment->element->nb_habitant>0.20)//20% car en dessous on est à 10 habitants alimentés en eau donc regression
                    {
                        return 0;//il se passe rien
                    }
                    else
                    {
                        batiment->element->type--;//on passe a l'état de cabane
                        batiment->element->nb_habitant=10;
                        *nb_habitant-=40;
                        return 1;
                    }
                }
                else
                {
                    batiment->element->type--;//on passe a l'état de cabane
                    batiment->element->nb_habitant=10;
                    *nb_habitant-=40;
                    return 1;
                }
            }
            else
            {
                batiment->element->type=9;//on passe a l'état de ruine
                batiment->element->nb_habitant=0;
                batiment->element->incendie=0;

                *nb_habitant-=50;
                return 1;
            }
        case 7:
            //printf("evolution\n");

            if(batiment->element->incendie!=1)
            {
                if(batiment->element->alimente==1)
                {
                    if((float)batiment->element->eau_actuelle/(float)batiment->element->nb_habitant==1)//si l'habitation est alimenté a plus de 90% en eau elle s'améliore
                    {
                        batiment->element->type++;
                        batiment->element->nb_habitant=1000;
                        *nb_habitant+=900;
                        return 1;

                    }
                    else if((float)batiment->element->eau_actuelle/(float)batiment->element->nb_habitant>0.50)//50% car en dessous on est à 50 habitants alimentés en eau donc regression
                    {
                        return 0;//il se passe rien
                    }
                    else
                    {
                        batiment->element->type--;//on passe a l'état de maison
                        batiment->element->nb_habitant=50;
                        *nb_habitant-=50;
                        return 1;
                    }
                }
                else
                {
                    batiment->element->type--;//on passe a l'état de maison
                    batiment->element->nb_habitant=50;
                    *nb_habitant-=50;
                    return 1;
                }
            }
            else
            {
                batiment->element->type=9;//on passe a l'état de ruine
                batiment->element->nb_habitant=0;
                batiment->element->incendie=0;

                *nb_habitant-=100;
                return 1;
            }
        case 8:
            //printf("evolution\n");
            if(batiment->element->incendie!=1)
            {
                if(batiment->element->alimente==1)
                {
                    if((float)batiment->element->eau_actuelle/(float)batiment->element->nb_habitant>0.10)//10% car en dessous on est à 100 habitants alimentés en eau donc regression
                    {
                        return 0;//il se passe rien
                    }
                    else
                    {
                        batiment->element->type--;//on passe a l'état d'immeuble
                        batiment->element->nb_habitant=100;
                        *nb_habitant-=900;
                        return 1;
                    }
                }
                else
                {
                    batiment->element->type--;//on passe a l'état d'immeuble
                    batiment->element->nb_habitant=100;
                    *nb_habitant-=900;
                    return 1;
                }
            }
            else
            {
                batiment->element->type=9;//on passe a l'état de ruine
                batiment->element->nb_habitant=0;
                batiment->element->incendie=0;
                *nb_habitant-=1000;
                return 1;
            }
        case 9:
            //printf("evolution\n");

            /*if(batiment->element->alimente==1)
            {
                batiment->element->type=5;
                batiment->element->nb_habitant=10;
                *nb_habitant+=10;
                return 1;
            }
            else
            {
                return 0;
            }*/
            break;
        default:
            return 0;
    }
    return 0;
}

int validation_evolution_capitaliste(t_graphe* map, t_tile* batiment, int* nb_habitant)//evolue forcément peut importe les conditions
{
    switch (batiment->element->type) {
        case 4:
            if(verification_connexite_route(map, batiment))
            {
                batiment->element->type++;
                batiment->element->nb_habitant=10;
                *nb_habitant+=10;
                return 1;
            }
            else
            {
                return 0;
            }
        case 5:
            if(batiment->element->incendie!=1)
            {
                batiment->element->type++;
                batiment->element->nb_habitant=50;
                *nb_habitant+=40;
                return 1;
            }
            else
            {
                batiment->element->type=9;//on passe a l'état de ruine
                batiment->element->nb_habitant=0;
                *nb_habitant-=10;
                return 1;
            }
        case 6:
            if(batiment->element->incendie!=1)
            {
                batiment->element->type++;
                batiment->element->nb_habitant=100;
                *nb_habitant+=50;
                return 1;
            }
            else
            {
                batiment->element->type=9;//on passe a l'état de ruine
                batiment->element->nb_habitant=0;
                batiment->element->incendie=0;
                *nb_habitant-=50;
                return 1;
            }
        case 7:
            if(batiment->element->incendie!=1)
            {
                batiment->element->type++;
                batiment->element->nb_habitant=1000;
                *nb_habitant+=900;
                return 1;
            }
            else
            {
                batiment->element->type=9;//on passe a l'état de ruine
                batiment->element->nb_habitant=0;
                batiment->element->incendie=0;
                *nb_habitant-=100;
                return 1;
            }
        case 8:
            if(batiment->element->incendie!=1)
            {
                return 0;
            }
            else
            {
                batiment->element->type=9;//on passe a l'état de ruine
                batiment->element->nb_habitant=0;
                batiment->element->incendie=0;
                *nb_habitant-=1000;
                return 1;
            }
        case 9:
        default:
            return 0;
    }
}

t_graphe* cycle_habitation(t_graphe* map, int* capa_usine, long* compteur_argent, int* nb_habitant, int compteur_eau, BUFFER* liste_buffer, IMAGE* liste_image, int* attente, int mode, int temps_ancienne_partie)
{
    int changement;
    //parcours du tableau des maisons au lieu du parcours de toute la map
    t_liste* parcours_habitation=map->liste_hab;
    while(parcours_habitation!=NULL)
    {
        if(clock()/CLOCKS_PER_SEC + temps_ancienne_partie -parcours_habitation->n->element->compteur==5)//si on a fait un cycle
        {
            if(parcours_habitation->n->element->type>4 && parcours_habitation->n->element->type<9)
            {
                parcours_habitation->n->element->incendie=incendie();
                parcours_habitation->n->element->argent=1;
            }
            if(parcours_habitation->n->element->incendie==1)
            {
                *attente=1;
                draw_sprite(liste_buffer->buffer_map, liste_image->flamme, (SCREEN_W/2-36)+(map->grille[parcours_habitation->n->position.ligne][parcours_habitation->n->position.colonne]->position.colonne-2)*14-map->grille[parcours_habitation->n->position.ligne][parcours_habitation->n->position.colonne]->position.ligne*14, (map->grille[parcours_habitation->n->position.ligne][parcours_habitation->n->position.colonne]->position.colonne-5)*8+map->grille[parcours_habitation->n->position.ligne][parcours_habitation->n->position.colonne]->position.ligne*8-11);
                map= gestion_incendie(map, parcours_habitation->n, liste_buffer, liste_image);
            }
            //sous progrm pour générer l'incendie après la gestion de l'incendie comme ça on laisse un cycle entier à l'utilisateur pour éteindre l'incendie si besoin
            parcours_habitation->n->element->compteur=clock()/CLOCKS_PER_SEC + temps_ancienne_partie;
            *compteur_argent=*compteur_argent+parcours_habitation->n->element->nb_habitant;
            if(mode==1)
            {
                changement=validation_evolution_communiste(parcours_habitation->n, nb_habitant, compteur_eau);
            }
            else
            {
                changement=validation_evolution_capitaliste(map, parcours_habitation->n, nb_habitant);
            }
            if(changement==1)
            {
                map=distribution_eau(map);
                map=electricite(map, capa_usine);
            }
            parcours_habitation->n->element->incendie=0;
        }
        parcours_habitation=parcours_habitation->next;
    }
    majFichierPlacementElement(map);
    return map;
}

int compte_eau(t_graphe* map)
{
    int compteur_eau=0;
    for(int i = 0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            if(map->grille[i][j]->element->type==2)
            {
                if(verification_connexite_route(map, map->grille[i][j]))
                {
                    compteur_eau+=map->grille[i][j]->element->capacite;
                }
            }
        }
    }
    return compteur_eau;
}

t_graphe* remise_0_argent(t_graphe* map, t_pos souris)
{
    if(mouse_b & 1)
    {
        if(souris.ligne!=255 || souris.colonne!=255)
        {
            if(map->mat_adjacence[souris.ligne][souris.colonne]>=4 && map->mat_adjacence[souris.ligne][souris.colonne]<=9)
            {
                map->grille[souris.ligne][souris.colonne]->case_mere->element->argent=0;
            }
        }


    }
    return map;
}