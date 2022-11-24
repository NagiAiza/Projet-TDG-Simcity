#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"
//
// Created by alexi on 03/11/2022.
//

#include "barre_outil.h"
#include "jeu2.h"
#include "listeRelated.h"


int choixAction(BUFFER* liste_buffer, IMAGE*liste_image)
{
    ///passer la souris sur la case compteur habitants pour afficher le nom de la case
    if(((mouse_x>=164)&&(mouse_x<=295)&&(mouse_y>=647)&&(mouse_y<=672)))
    {
        //textprintf_ex(liste_buffer->buffer_menu,font,10,20,makecol(0,0,0),makecol(255,255,255),"bouton selec");
        blit(liste_image->info_habitants, liste_buffer->buffer_menu,0, 0,  201, 654, 100, 13);
    }
    ///passer la souris sur la case compteur eau pour afficher le nom de la case
    if(((mouse_x>=798)&&(mouse_x<=931)&&(mouse_y>=648)&&(mouse_y<=672)))
    {
        blit(liste_image->info_eau, liste_buffer->buffer_menu,0, 0,  833, 649, 100, 13);
    }
    ///passer la souris sur la case compteur elec pour afficher le nom de la case
    if(((mouse_x>=482)&&(mouse_x<=613)&&(mouse_y>=647)&&(mouse_y<=672)))
    {
        blit(liste_image->info_elec, liste_buffer->buffer_menu,0, 0,  521, 651, 100, 13);
    }
    ///passer la souris sur la case compteur monaie pour afficher le nom de la case
    if(((mouse_x>=4)&&(mouse_x<=108)&&(mouse_y>=592)&&(mouse_y<=612)))
    {
        blit(liste_image->info_monnaie, liste_buffer->buffer_menu,0, 0,  15, 600, 100, 13);
    }
    ///passer la souris sur la case compteur habitants pour afficher le nom de la case
    if(((mouse_x>=288)&&(mouse_x<=420)&&(mouse_y>=671)&&(mouse_y<=702)))
    {
        blit(liste_image->info_habitants, liste_buffer->buffer_menu,0, 0,  288, 705, 100, 13);
    }

    if(((mouse_x>=5)&&(mouse_x<=121)&&(mouse_y>=124)&&(mouse_y<=158))&&(mouse_b&1)) //Si on clique sur la case route
    {
        return 1;
    }
    if(((mouse_x>=6)&&(mouse_x<=119)&&(mouse_y>=165)&&(mouse_y<=196))&&(mouse_b&1)) //Si on clique sur case batiments
    {
        return 4;
    }
    if(((mouse_x>=5)&&(mouse_x<=121)&&(mouse_y>=207)&&(mouse_y<=236))&&(mouse_b&1)) //elec
    {
        return 3;
    }
    if(((mouse_x>=5)&&(mouse_x<=117)&&(mouse_y>=246)&&(mouse_y<=275))&&(mouse_b&1)) // eau
    {
        return 2;
    }
    if(((mouse_x>=7)&&(mouse_x<=118)&&(mouse_y>=288)&&(mouse_y<=313))&&(mouse_b&1)) // pompiers
    {
        return 5;
    }
    if(((mouse_x>=7)&&(mouse_x<=59)&&(mouse_y>=326)&&(mouse_y<=350))&&(mouse_b&1))//fleches
    {

    }
    if(((mouse_x>=65)&&(mouse_x<=115)&&(mouse_y>=328)&&(mouse_y<=354))&&(mouse_b&1))//pelle
    {

    }
    if(((mouse_x>=9)&&(mouse_x<=81)&&(mouse_y>=408)&&(mouse_y<=434))&&(mouse_b&1))//0
    {
        return 6;
    }
    if(((mouse_x>=12)&&(mouse_x<=77)&&(mouse_y>=446)&&(mouse_y<=471))&&(mouse_b&1))//-1
    {
        return 7;
    }
    if(((mouse_x>=11)&&(mouse_x<=79)&&(mouse_y>=486)&&(mouse_y<=507))&&(mouse_b&1))//-2
    {
        return 8;
    }
    if(((mouse_x>=50)&&(mouse_x<=115)&&(mouse_y>=557)&&(mouse_y<=583))&&(mouse_b&1))//exit
    {
        return 9;
    }
    return 0;
    return 0;

}

t_graphe* action(t_graphe* map, BUFFER* liste_buffer, IMAGE* liste_image, int* choix, t_pos souris, int* rotation, int* niv_visu, t_tile** case_select, int* algo_A, long* argent_restant, int* capa_usine)
{
    int depense;
    t_tile* parcour_chemin=NULL;//tuile auxilière pour reparcourir les chemins calculé
    switch (*choix) {
        case 1://route
            if(souris.ligne<35 && souris.colonne<45)
            {
                if(*algo_A == 0)
                {
                    draw_sprite(liste_buffer->buffer_map, liste_image->route,(SCREEN_W / 2 - 36) + souris.colonne * 14 - souris.ligne * 14,souris.colonne * 8 + souris.ligne * 8);
                    if (mouse_b & 1)
                    {
                        if (!verification_chevauchement(map, souris.ligne, souris.colonne, 1, *rotation) && placement_route(map, souris.ligne, souris.colonne) == 1)
                        {
                            *algo_A = 1;
                            *case_select = map->grille[souris.ligne][souris.colonne];
                            printf("Case selec\n");
                        }
                    }
                }
                else if (*algo_A == 1)//calcul de chemin à chaque fois qu'on change de case c'est peut etre mieux
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
                                *argent_restant-=depense;
                                map= distribution_eau(map);
                                map= electricite(map, capa_usine);
                            }
                            *algo_A = 0;//et on sort de l'affichage A*
                        }
                    }
                }
            }

            if(key[KEY_1])//ou choisir un bouton plus judicieux
            {
                *choix=0;//on sort du choix des actions si l'utilisateur le veut
                *algo_A=0;
            }
            break;
        case 2://Chateau d'eau
            if(key[KEY_1])//ou choisir un bouton plus judicieux
            {
                *choix=0;//on sort du choix des actions si l'utilisateur le veut
            }
            if(key[KEY_2])
            {
                *rotation=-*rotation;
                rest(100);//pour eviter les rebonds
            }
            if(*rotation==1)
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
                        *choix=0;
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
            break;
        case 3://Electricité
            if(key[KEY_1])//ou choisir un bouton plus judicieux
            {
                *choix=0;//on sort du choix des actions si l'utilisateur le veut
            }
            if(key[KEY_2])
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
            break;
        case 4:
            //habitation 3x3 à changer
            //bouton pour rotationné le sens de construction? + blindage pour ne pas superposer avec une autre construction
            if(key[KEY_1])//ou choisir un bouton plus judicieux
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
                    *choix=0; // dès qu'on a fait l'action on peut revenir a un etat neutre de choix
                }
            }
            break;
        case 5://caserne de pompier
            textout_ex ( liste_buffer->buffer_final, font, "Action 5", 511, 326,makecol (255, 255, 255), -1); //texte explicatif
            break;
        case 6://visualisation niveau 0
            *niv_visu=0;
            *choix=0;
            break;
        case 7://visualisation eau -1
            *niv_visu=1;
            *choix=0;
            break;
        case 8://visualisation elec -2
            *niv_visu=2;
            *choix=0;
            break;
        case 9:
            textout_ex ( liste_buffer->buffer_final, font, "Action 9", 511, 326,makecol (255, 255, 255), -1); //texte explicatif
            break;
        default:
            textout_ex ( liste_buffer->buffer_final, font, "Rien de selectionne", 511, 326,makecol (255, 255, 255), -1); //texte explicatif
            break;
    }
    return map;
}
#pragma clang diagnostic pop