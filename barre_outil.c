//
// Created by alexi on 03/11/2022.
//

#include "barre_outil.h"
#include "jeu2.h"
#include "listeRelated.h"

int choixAction()
{
    if(((mouse_x>=6)&&(mouse_x<=39)&&(mouse_y>=90)&&(mouse_y<=126))&&(mouse_b&1)) //Si on clique sur la premiere case
    {
        return 1;
    }
    if(((mouse_x>=44)&&(mouse_x<=78)&&(mouse_y>=88)&&(mouse_y<=122))&&(mouse_b&1)) //Si on clique sur la deuxieme case
    {
        return 2;
    }
    if(((mouse_x>=87)&&(mouse_x<=119)&&(mouse_y>=91)&&(mouse_y<=123))&&(mouse_b&1)) //3eme case
    {
        return 3;
    }
    if(((mouse_x>=7)&&(mouse_x<=37)&&(mouse_y>=138)&&(mouse_y<=170))&&(mouse_b&1))
    {
        return 4;
    }
    if(((mouse_x>=45)&&(mouse_x<=77)&&(mouse_y>=137)&&(mouse_y<=170))&&(mouse_b&1))
    {
        return 5;
    }
    if(((mouse_x>=85)&&(mouse_x<=119)&&(mouse_y>=137)&&(mouse_y<=172))&&(mouse_b&1))
    {
        return 6;
    }
    if(((mouse_x>=6)&&(mouse_x<=37)&&(mouse_y>=182)&&(mouse_y<=218))&&(mouse_b&1))
    {
        return 7;
    }
    if(((mouse_x>=45)&&(mouse_x<=75)&&(mouse_y>=182)&&(mouse_y<=214))&&(mouse_b&1))
    {
        return 8;
    }
    if(((mouse_x>=86)&&(mouse_x<=119)&&(mouse_y>=182)&&(mouse_y<=215))&&(mouse_b&1))//9eme case
    {
        return 9;
    }
    return 0;
}

t_graphe* action(t_graphe* map, BUFFER* liste_buffer, IMAGE* liste_image, int* choix, t_pos souris, int* rotation, int* niv_visu, t_tile** case_select, int* algo_A, long* argent_restant)
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
                    draw_sprite(liste_buffer->buffer_map, liste_image->chateau_eau, (SCREEN_W / 2 - 36) + (souris.colonne - 3) * 14 - (souris.ligne) * 14, (souris.colonne - 3) * 8 + (souris.ligne) * 8 - 8);//pq le -8? jsp j'ai tatonné
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
                    draw_sprite_h_flip(liste_buffer->buffer_map, liste_image->chateau_eau, (SCREEN_W/2-36)+(souris.colonne-3)*14-(souris.ligne)*14, (souris.colonne-3)*8+(souris.ligne)*8-8);
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
                    draw_sprite(liste_buffer->buffer_map, liste_image->centrale, (SCREEN_W / 2 - 36) + (souris.colonne - 3) * 14 - (souris.ligne) * 14, (souris.colonne - 3) * 8 + (souris.ligne) * 8 - 8);//pq le -8? jsp j'ai tatonné
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
                    draw_sprite_h_flip(liste_buffer->buffer_map, liste_image->centrale, (SCREEN_W/2-36)+(souris.colonne-3)*14-(souris.ligne)*14, (souris.colonne-3)*8+(souris.ligne)*8-8);
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
                draw_sprite(liste_buffer->buffer_map, liste_image->batiment, (SCREEN_W/2-36)+(souris.colonne-2)*14-(souris.ligne)*14, (souris.colonne-2)*8+(souris.ligne)*8);
                if(mouse_b & 1)
                {
                    if(!verification_chevauchement(map, souris.ligne, souris.colonne, 4, *rotation))
                    {
                        depense= calcul_depenses(4, 0);
                        initialisation_habitation(map->grille[souris.ligne][souris.colonne]);
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
        case 7://visualisation eau
            *niv_visu=1;
            *choix=0;
            break;
        case 8://visualisation elec
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