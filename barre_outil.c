#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"
//
// Created by alexi on 03/11/2022.
//

#include "barre_outil.h"
#include "jeu2.h"
#include "listeRelated.h"


void choixAction(int* choix)//en fonction de l'endroit cliqué, on lui attribut une valeur
{

    if(((mouse_x>=5)&&(mouse_x<=121)&&(mouse_y>=124)&&(mouse_y<=158))&&(mouse_b&1)) //Si on clique sur la case route
    {
        *choix = 1;
    }
    if(((mouse_x>=6)&&(mouse_x<=119)&&(mouse_y>=165)&&(mouse_y<=196))&&(mouse_b&1)) //Si on clique sur case batiments
    {
        *choix = 4;
    }
    if(((mouse_x>=5)&&(mouse_x<=121)&&(mouse_y>=207)&&(mouse_y<=236))&&(mouse_b&1)) //elec
    {
        *choix = 3;
    }
    if(((mouse_x>=5)&&(mouse_x<=117)&&(mouse_y>=246)&&(mouse_y<=275))&&(mouse_b&1)) // eau
    {
        *choix = 2;
    }
    if(((mouse_x>=7)&&(mouse_x<=118)&&(mouse_y>=288)&&(mouse_y<=313))&&(mouse_b&1)) // pompiers
    {
        *choix = 5;
    }
    if(((mouse_x>=7)&&(mouse_x<=59)&&(mouse_y>=326)&&(mouse_y<=350))&&(mouse_b&1))//fleches
    {
        //pas codé
    }
    if(((mouse_x>=65)&&(mouse_x<=115)&&(mouse_y>=328)&&(mouse_y<=354))&&(mouse_b&1))//pelle
    {
        printf("supp\n");
        *choix = 10;
    }
    if(((mouse_x>=9)&&(mouse_x<=81)&&(mouse_y>=408)&&(mouse_y<=434))&&(mouse_b&1))//0
    {
        *choix = 6;
    }
    if(((mouse_x>=12)&&(mouse_x<=77)&&(mouse_y>=446)&&(mouse_y<=471))&&(mouse_b&1))//-1
    {
        *choix = 7;
    }
    if(((mouse_x>=11)&&(mouse_x<=79)&&(mouse_y>=486)&&(mouse_y<=507))&&(mouse_b&1))//-2
    {
        *choix = 8;
    }
    if(((mouse_x>=50)&&(mouse_x<=115)&&(mouse_y>=557)&&(mouse_y<=583))&&(mouse_b&1))//exit
    {
        *choix = 9;
    }
}

t_graphe* action(t_graphe* map, BUFFER* liste_buffer, IMAGE* liste_image, int* choix, t_pos souris, int* rotation, int niv_visu, t_tile** case_select, int* algo_A, long* argent_restant, int* capa_usine, int* exit, int* scroll, int* nb_habitants)
{
    int depense;
    t_tile* parcour_chemin=NULL;//tuile auxilière pour reparcourir les chemins calculé
    switch (*choix) {
        case 1://route
            if(niv_visu==0)
            {
                if(souris.ligne<35 && souris.colonne<45)
                {
                    if(*algo_A == 0)//si on n'a pas déjà appuyé pour choisir l'emplacement de départ de la route
                    {
                        draw_sprite(liste_buffer->buffer_map, liste_image->route,(SCREEN_W / 2 - 36) + souris.colonne * 14 - souris.ligne * 14,souris.colonne * 8 + souris.ligne * 8);//on affiche l'icone de la route
                        if (mouse_b & 1)
                        {
                            if (!verification_chevauchement(map, souris.ligne, souris.colonne, 1, *rotation) && placement_route(map, souris.ligne, souris.colonne) == 1)//on vérifie qu'on peut placer la route
                            {
                                *algo_A = 1;//on peut alors lancer A* continuellement
                                *case_select = map->grille[souris.ligne][souris.colonne];//on conserve la case de départ
                            }
                        }
                    }
                    else if (*algo_A == 1)
                    {
                        map = A_star(map, *case_select,map->grille[souris.ligne][souris.colonne]);//on retrace ensuite le chemin en utilisant les parents
                        parcour_chemin = map->grille[souris.ligne][souris.colonne];
                        while (parcour_chemin != NULL)
                        {
                            draw_sprite(liste_buffer->buffer_map, liste_image->route,(SCREEN_W / 2 - 36) + parcour_chemin->position.colonne * 14 - parcour_chemin->position.ligne * 14,parcour_chemin->position.colonne * 8 + parcour_chemin->position.ligne * 8);
                            parcour_chemin = parcour_chemin->parent;
                        }
                        if (mouse_b & 2)//on valide le chemin final avec le clic droit
                        {
                            if (!verification_chevauchement(map, souris.ligne, souris.colonne, 1, *rotation))
                            {
                                parcour_chemin = map->grille[souris.ligne][souris.colonne];
                                depense= calcul_depenses(1, tailleChemin(parcour_chemin));

                                if(validation_depense(depense, *argent_restant))
                                {
                                    while (parcour_chemin != NULL) {
                                        map = placementElement(map, parcour_chemin->position.ligne,parcour_chemin->position.colonne, 1, *rotation);
                                        map = remplissage_matrice_adjacence(map, parcour_chemin->position.ligne,parcour_chemin->position.colonne, 1, map->grille[parcour_chemin->position.ligne][parcour_chemin->position.colonne]);//ici
                                        parcour_chemin = parcour_chemin->parent;
                                    }
                                    *argent_restant-=depense;//on met a jour les informations
                                    map= distribution_eau(map);
                                    map= electricite(map, capa_usine);
                                }
                                *algo_A = 0;//et on sort de l'affichage A*
                            }
                        }
                    }
                }

                if(key[KEY_1] || ((mouse_x>=25)&&(mouse_x<=55)&&(mouse_y>=520)&&(mouse_y<=545))&&(mouse_b&1))//ou choisir un bouton plus judicieux
                {
                    *choix=0;//on sort du choix des actions si l'utilisateur le veut
                    *algo_A=0;//on annule A* si besoin
                }
            }
            break;
        case 2://Chateau d'eau
            if(niv_visu==0)//on blinde pour ne pouvoir poser un batiment seulement au niveau -1
            {
                if(key[KEY_1] || ((mouse_x>=25)&&(mouse_x<=55)&&(mouse_y>=520)&&(mouse_y<=545))&&(mouse_b&1))//ou choisir un bouton plus judicieux
                {
                    *choix=0;//on sort du choix des actions si l'utilisateur le veut
                }
                if(key[KEY_2] || ((mouse_x>=70)&&(mouse_x<=100)&&(mouse_y>=515)&&(mouse_y<=545))&&(mouse_b&1))
                {
                    *rotation=-*rotation;
                    rest(100);//pour eviter les rebonds
                }
                if(*rotation==1)//selon la rotation, on aura un remplissage des différentes matrices différentes
                {
                    if (souris.ligne >= 2 && souris.ligne < 35 - 1 && souris.colonne >= 2 && souris.colonne < 45 - 3)//blindage pour pas sortir de la map en fonction des deux moyens de rotation
                    {
                        draw_sprite(liste_buffer->buffer_map, liste_image->eau_fini, (SCREEN_W / 2 - 36) + (souris.colonne - 3) * 14 - (souris.ligne) * 14 , (souris.colonne - 3) * 8 + (souris.ligne) * 8 - 11);//pq le -8? jsp j'ai tatonné
                        if(mouse_b & 1)
                        {
                            if(!verification_chevauchement(map, souris.ligne, souris.colonne, 2, *rotation))
                            {
                                depense= calcul_depenses(2, 0);
                                if(validation_depense(depense, *argent_restant))
                                {
                                    map = placementElement(map, souris.ligne, souris.colonne, 2, *rotation);

                                    for (int i = -2; i < 2; i++)
                                    {
                                        for (int j = -2; j < 4; j++)
                                        {
                                            map = remplissage_matrice_adjacence(map, souris.ligne + i, souris.colonne + j,2, map->grille[souris.ligne][souris.colonne]);
                                        }
                                    }
                                    initialisation_chateau_eau(map->grille[souris.ligne][souris.colonne]);
                                    //printf("eau : %d", map->grille[souris.ligne][souris.colonne]->element->capacite);
                                    map=distribution_eau(map);
                                    *argent_restant-=depense;
                                }
                            }
                            *choix=0;//dès qu'il a posé un batiment on remet l'état du choix a neutre
                        }
                    }
                }else if (*rotation==-1)
                {
                    if (souris.ligne >= 3 && souris.ligne < 35-2 && souris.colonne >= 1 && souris.colonne < 45-2 )//blindage pour pas sortir de la map en fonction des deux moyens de rotation
                    {
                        draw_sprite_h_flip(liste_buffer->buffer_map, liste_image->eau_fini, (SCREEN_W/2-36)+(souris.colonne-3)*14-(souris.ligne)*14, (souris.colonne-3)*8+(souris.ligne)*8-11);//-3 par rapport a l'autre orientation pour palier la différence de hauteur
                        if(mouse_b & 1)
                        {
                            if (!verification_chevauchement(map, souris.ligne, souris.colonne, 2, *rotation))
                            {
                                depense= calcul_depenses(2, 0);
                                if(validation_depense(depense, *argent_restant))
                                {
                                    map = placementElement(map, souris.ligne, souris.colonne, 2, *rotation);
                                    for (int i = -3; i < 3; i++)
                                    {
                                        for (int j = -1; j < 3; j++)
                                        {
                                            map = remplissage_matrice_adjacence(map, souris.ligne + i, souris.colonne + j,2, map->grille[souris.ligne][souris.colonne]);
                                        }
                                    }
                                    initialisation_chateau_eau(map->grille[souris.ligne][souris.colonne]);
                                    //printf("eau : %d", map->grille[souris.ligne][souris.colonne]->element->capacite);
                                    map=distribution_eau(map);
                                    *argent_restant-=depense;
                                }
                            }
                            *choix=0;
                        }
                    }
                }
            }
            break;
        case 3://Electricité
            if(niv_visu==0)
            {
                if(key[KEY_1] || ((mouse_x>=25)&&(mouse_x<=55)&&(mouse_y>=520)&&(mouse_y<=545))&&(mouse_b&1))//ou choisir un bouton plus judicieux
                {
                    *choix=0;//on sort du choix des actions si l'utilisateur le veut
                }
                if(key[KEY_2] || ((mouse_x>=70)&&(mouse_x<=100)&&(mouse_y>=515)&&(mouse_y<=545))&&(mouse_b&1))
                {
                    *rotation=-*rotation;
                    rest(100);//pour eviter les rebonds
                }
                if(*rotation==1)
                {
                    if (souris.ligne >= 2 && souris.ligne < 35 - 1 && souris.colonne >= 2 && souris.colonne < 45 - 3)//blindage pour pas sortir de la map en fonction des deux moyens de rotation
                    {
                        draw_sprite(liste_buffer->buffer_map, liste_image->elec_fini, (SCREEN_W / 2 - 36) + (souris.colonne - 3) * 14 - (souris.ligne) * 14, (souris.colonne - 3) * 8 + (souris.ligne) * 8 - 22);//pq le -8? jsp j'ai tatonné
                        if(mouse_b & 1)
                        {
                            if(!verification_chevauchement(map, souris.ligne, souris.colonne, 3, *rotation))
                            {
                                depense= calcul_depenses(3, 0);
                                if(validation_depense(depense, *argent_restant))
                                {
                                    map = placementElement(map, souris.ligne, souris.colonne, 3, *rotation);

                                    for (int i = -2; i < 2; i++)
                                    {
                                        for (int j = -2; j < 4; j++)
                                        {
                                            map = remplissage_matrice_adjacence(map, souris.ligne + i, souris.colonne + j,3, map->grille[souris.ligne][souris.colonne]);
                                        }
                                    }
                                    initialisation_centrale(map->grille[souris.ligne][souris.colonne]);
                                    map= electricite(map, capa_usine);
                                    *argent_restant-=depense;
                                }
                            }
                            *choix=0;
                        }
                    }
                }else if (*rotation==-1)
                {
                    if (souris.ligne >= 3 && souris.ligne < 35-2 && souris.colonne >= 1 && souris.colonne < 45-2 )//blindage pour pas sortir de la map en fonction des deux moyens de rotation
                    {
                        draw_sprite_h_flip(liste_buffer->buffer_map, liste_image->elec_fini, (SCREEN_W/2-36)+(souris.colonne-3)*14-(souris.ligne)*14, (souris.colonne-3)*8+(souris.ligne)*8-22);//-14 par rapport a l'autre orientation pour palier la différence de hauteur
                        if(mouse_b & 1)
                        {
                            if (!verification_chevauchement(map, souris.ligne, souris.colonne, 3, *rotation))
                            {
                                depense= calcul_depenses(3, 0);
                                if(validation_depense(depense, *argent_restant))
                                {
                                    map = placementElement(map, souris.ligne, souris.colonne, 3, *rotation);
                                    for (int i = -3; i < 3; i++)
                                    {
                                        for (int j = -1; j < 3; j++)
                                        {
                                            map = remplissage_matrice_adjacence(map, souris.ligne + i, souris.colonne + j,3, map->grille[souris.ligne][souris.colonne]);
                                        }
                                    }
                                    initialisation_centrale(map->grille[souris.ligne][souris.colonne]);
                                    map= electricite(map, capa_usine);
                                    *argent_restant-=depense;
                                }
                            }
                            *choix=0;
                        }
                    }
                }
            }
            break;
        case 4:
            //bouton pour rotationné le sens de construction?
            if(niv_visu==0)
            {
                if(key[KEY_1] || ((mouse_x>=25)&&(mouse_x<=55)&&(mouse_y>=520)&&(mouse_y<=545))&&(mouse_b&1))//ou choisir un bouton plus judicieux
                {
                    *choix=0;//on sort du choix des actions si l'utilisateur le veut
                }
                if(souris.ligne>=0+1 && souris.colonne>=0+1 && souris.ligne<35-1 && souris.colonne<45-1)
                {
                    draw_sprite(liste_buffer->buffer_map, liste_image->terrain_vague, (SCREEN_W/2-36)+(souris.colonne-2)*14-(souris.ligne)*14, (souris.colonne-2)*8+(souris.ligne)*8);
                    if(mouse_b & 1)
                    {
                        if(!verification_chevauchement(map, souris.ligne, souris.colonne, 4, *rotation))
                        {
                            depense= calcul_depenses(4, 0);
                            initialisation_habitation(map, map->grille[souris.ligne][souris.colonne]);
                            if(validation_depense(depense, *argent_restant))
                            {
                                map = placementElement(map, souris.ligne, souris.colonne, 4, *rotation);

                                for(int i=-1; i<2; i++)
                                {
                                    for(int j=-1; j<2; j++)
                                    {
                                        map= remplissage_matrice_adjacence(map, souris.ligne+i, souris.colonne+j, 4, map->grille[souris.ligne][souris.colonne]);
                                    }
                                }
                                map= electricite(map, capa_usine);
                                *argent_restant-=depense;
                                map=distribution_eau(map);
                            }


                        }
                        //*choix=0; // dès qu'on a fait l'action on peut revenir a un etat neutre de choix
                    }
                }
            }
            break;
        case 5://caserne de pompier
            if(niv_visu==0)
            {
                if(key[KEY_1] || ((mouse_x>=25)&&(mouse_x<=55)&&(mouse_y>=520)&&(mouse_y<=545))&&(mouse_b&1))//ou choisir un bouton plus judicieux
                {
                    *choix=0;//on sort du choix des actions si l'utilisateur le veut
                }
                if(key[KEY_2] || ((mouse_x>=70)&&(mouse_x<=100)&&(mouse_y>=515)&&(mouse_y<=545))&&(mouse_b&1))
                {
                    *rotation=-*rotation;
                    rest(100);//pour eviter les rebonds
                }
                if(*rotation==1)
                {
                    if (souris.ligne >= 2 && souris.ligne < 35 - 1 && souris.colonne >= 2 && souris.colonne < 45 - 3)//blindage pour pas sortir de la map en fonction des deux moyens de rotation
                    {
                        draw_sprite(liste_buffer->buffer_map, liste_image->caserne_pompiers, (SCREEN_W / 2 - 36) + (souris.colonne - 3) * 14 - (souris.ligne) * 14, (souris.colonne - 3) * 8 + (souris.ligne) * 8 - 52);//pq le -8? jsp j'ai tatonné
                        if(mouse_b & 1)
                        {
                            if(!verification_chevauchement(map, souris.ligne, souris.colonne, 10, *rotation))
                            {
                                depense= calcul_depenses(*choix, 0);
                                if(validation_depense(depense, *argent_restant))
                                {
                                    map = placementElement(map, souris.ligne, souris.colonne, 10, *rotation);

                                    for (int i = -2; i < 2; i++)
                                    {
                                        for (int j = -2; j < 4; j++)
                                        {
                                            map = remplissage_matrice_adjacence(map, souris.ligne + i, souris.colonne + j,10, map->grille[souris.ligne][souris.colonne]);
                                        }
                                    }
                                    //caserne_de_pompier(map, souris.ligne , souris.colonne , *choix);

                                    *argent_restant-=depense;
                                }
                            }
                            *choix=0; // dès qu'on a fait l'action on peut revenir a un etat neutre de choix
                        }
                    }
                }else if (*rotation==-1)
                {
                    if (souris.ligne >= 3 && souris.ligne < 35-2 && souris.colonne >= 1 && souris.colonne < 45-2 )//blindage pour pas sortir de la map en fonction des deux moyens de rotation
                    {
                        draw_sprite_h_flip(liste_buffer->buffer_map, liste_image->caserne_pompiers, (SCREEN_W/2-36)+(souris.colonne-3)*14-(souris.ligne)*14, (souris.colonne-3)*8+(souris.ligne)*8-52);
                        if(mouse_b & 1)
                        {
                            if (!verification_chevauchement(map, souris.ligne, souris.colonne, 10, *rotation))
                            {
                                depense= calcul_depenses(*choix, 0);
                                if(validation_depense(depense, *argent_restant))
                                {
                                    map = placementElement(map, souris.ligne, souris.colonne, 10, *rotation);
                                    for (int i = -3; i < 3; i++)
                                    {
                                        for (int j = -1; j < 3; j++)
                                        {
                                            map = remplissage_matrice_adjacence(map, souris.ligne + i, souris.colonne + j,10, map->grille[souris.ligne][souris.colonne]);
                                        }
                                    }
                                    //caserne_de_pompier(map, souris.ligne , souris.colonne , *choix);

                                    *argent_restant-=depense;
                                }
                            }
                            *choix=0;
                        }
                    }
                }
            }
            break;
        case 6://visualisation niveau 0
            //*niv_visu=0;
            *choix=0;
            *scroll=0;
            break;
        case 7://visualisation eau -1
            //*niv_visu=1;
            *choix=0;
            *scroll=1;
            break;
        case 8://visualisation elec -2
            //*niv_visu=2;
            *choix=0;
            *scroll=2;
            break;
        case 9://sortie du jeu
            *exit=1;
            *choix=0;
            break;
        case 10:
            if(niv_visu==0)
            {

                if(key[KEY_1] || ((mouse_x>=25)&&(mouse_x<=55)&&(mouse_y>=520)&&(mouse_y<=545))&&(mouse_b&1))//ou choisir un bouton plus judicieux
                {
                    *choix=0;//on sort du choix des actions si l'utilisateur le veut
                }
                if(mouse_b & 1)
                {
                    if(souris.ligne<35 && souris.colonne<45)
                    {
                        int temp_ligne= map->grille[souris.ligne][souris.colonne]->case_mere->position.ligne;//on récupère les composantes de positions car la case mère peut changer pendant la suppresion
                        int temp_colonne= map->grille[souris.ligne][souris.colonne]->case_mere->position.colonne;
                        if (map->grille[souris.ligne][souris.colonne]->case_mere->element->type==2 || map->grille[souris.ligne][souris.colonne]->case_mere->element->type==3 || map->grille[souris.ligne][souris.colonne]->case_mere->element->type==10)
                        {
                            //printf("suppresion\n");
                            map->grille[souris.ligne][souris.colonne]->case_mere->element->type=0;//on remet le type à 0
                            if(map->grille[souris.ligne][souris.colonne]->case_mere->element->orientation==1)//selon l'orientation
                            {
                                for (int i = -2; i < 2; i++)
                                {
                                    for (int j = -2; j < 4; j++)
                                    {
                                        map = remplissage_matrice_adjacence(map, temp_ligne + i, temp_colonne + j,0, map->grille[temp_ligne + i][temp_colonne + j]);
                                    }
                                }
                            }
                            else
                            {
                                for (int i = -3; i < 3; i++)
                                {
                                    for (int j = -1; j < 3; j++)
                                    {
                                        map = remplissage_matrice_adjacence(map, temp_ligne + i, temp_colonne + j,0, map->grille[temp_ligne + i][temp_colonne + j]);
                                    }
                                }
                            }
                            map= electricite(map, capa_usine);//on  n'oublie pas de mettre a jour les chemins d'eau et d'elec
                            map=distribution_eau(map);
                            *choix=0;
                        }
                        else if(map->grille[souris.ligne][souris.colonne]->case_mere->element->type>=4 && map->grille[souris.ligne][souris.colonne]->case_mere->element->type<=9)
                        {
                            //retirer de la liste des habitations du graphe
                            map->grille[souris.ligne][souris.colonne]->case_mere->element->type=0;
                            *nb_habitants-=map->grille[souris.ligne][souris.colonne]->case_mere->element->nb_habitant;
                            //printf("case [%d][%d]\n", temp_ligne, temp_colonne);
                            for(int i=-1; i<2; i++)
                            {
                                for(int j=-1; j<2; j++)
                                {
                                    //printf("suppresion case [%d][%d]\n", temp_ligne + i, temp_colonne + j);
                                    map= remplissage_matrice_adjacence(map, temp_ligne + i, temp_colonne + j, 0, map->grille[temp_ligne + i][temp_colonne + j]);
                                }
                            }
                            map->liste_hab=enleverNoeud(map->liste_hab, map->grille[temp_ligne][temp_colonne]);//on  n'oublie pas de mettre a jour les chemins d'eau et d'elec
                            map= electricite(map, capa_usine);
                            map=distribution_eau(map);
                            *choix=0;
                        }
                    }
                }
            }
            break;
        default:
            textout_ex ( liste_buffer->buffer_final, font, "Rien de selectionne", 511, 326,makecol (255, 255, 255), -1); //texte explicatif
            break;
    }
    return map;
}
