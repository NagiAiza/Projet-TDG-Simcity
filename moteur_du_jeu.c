//
// Created by alexi on 04/11/2022.
//

#include "moteur_du_jeu.h"
#include "jeu2.h"
#include "time.h"
#include "jeu3.h"

void jeu(int* mode, int nvlle_partie)//sous programme qui fera tourner tout le jeu
{
    printf("Debut du jeu\n");
    srand(time(NULL));
    //initialisation des variables
    int decalageX; // Position de l'écran réel dans le repère du décor...
    long compteur_argent=500000;
    int capa_usine=0;
    int nb_habitant=0;
    int capa_eau;
    int exit=0;
    int attente=0;
    long temps_ancienne_partie=0;//pour récupérer le timer si on continue une partie
    t_pos souris;
    souris.ligne=0;
    souris.colonne=0;

    int scroll=-1;
    int screeny=0;


    int choix, rotation=1, niv_visu=0;
    int algo_A=0;//pour savoir si on doit lancer l'algo A*
    t_tile* case_select;
    //initialisation des bitmaps
    IMAGE* liste_image=initialisation_liste_image();
    BUFFER* liste_buffer=initialisation_liste_buffer();





    //génération de la map
    t_graphe* map=makeGrid();

    //generation du fichier
    if(nvlle_partie==1)
    {
        initialisationElementCarte();//genere le fichier
    }
    else//pb on n'a pas la connexite
    {
        map=lecture_sauvegarde(map, &compteur_argent, &temps_ancienne_partie, &nb_habitant, mode);
        map= distribution_eau(map);//pb avec l'eau surtout
        map= distribution_elec(map);
    }


    // Ici initialisation du scrolling en haut à gauche du décor
    //  (on peut décider de partir autrement...)
    decalageX=0;
    show_mouse(screen);
    clock_t CLK_debut=clock() / CLOCKS_PER_SEC;
    while(!key[KEY_ESC] && exit!=1)//fin du jeu si on appuie sur exit ou si on appuie sur echap
    {
        if (key[KEY_RIGHT])   decalageX+=5;//pour décaler la map si on veut voir une autre partie
        if (key[KEY_LEFT] )   decalageX-=5;
        if ( decalageX < 0 ) decalageX=0;//blindage
        if ( decalageX > liste_buffer->buffer_map->w - SCREEN_W +124) decalageX=liste_buffer->buffer_map->w - SCREEN_W+124;

        capa_eau=compte_eau(map);

        affichageTotal(map, liste_image, liste_buffer, souris, compteur_argent, niv_visu, capa_usine, CLK_debut, nb_habitant, capa_eau, screeny, temps_ancienne_partie);//on affiche toute les infos à l'écran


        if(mouse_x>124 && mouse_y<640)
        {
            souris= calcul_pos_souris(liste_image->sous_map, decalageX);//pour determiner l'emplacement de la souris sur la map
        }

//L'utilisateur choisi déjà un bouton dans la barre d'outil
        if(mouse_b & 1)
        {
            if(mouse_x<124 || mouse_y>640)
            {
                choixAction(&choix);//pour savoir où le joueur a cliqué
                rest(100);
            }
        }
        scroll_map(&scroll, &niv_visu, &screeny);
        //dès qu'on a récup ce qu'il veut faire on conserve son action et en fonction de l'action on lance le sous pgrm pour l'effectuer

        map=action(map, liste_buffer, liste_image, &choix, souris, &rotation, niv_visu, &case_select, &algo_A, &compteur_argent, &capa_usine, &exit, &scroll);//gère l'action choisi par l'utilisateur

        map=cycle_habitation(map, &capa_usine, &compteur_argent, &nb_habitant, capa_eau, liste_buffer, liste_image, &attente, *mode, temps_ancienne_partie);//gestion des cycles des habitations

        map=remise_0_argent(map, souris);//si on appuie sur une pièce on fait disparaitre la pièce

        if(choix>=1 && choix <=5)//si il doit poser un batiment, on affiche les sprites qui permettront d'annuler l'action ou de faire tourner le batiment
        {
            draw_sprite(liste_buffer->buffer_menu, liste_image->annulation_action, 25, 517);
            draw_sprite(liste_buffer->buffer_menu, liste_image->rotation, 70, 517);
        }

        affichage_info_menu(liste_buffer, liste_image);//on affiche les infos du menu nécessaire

        blit(liste_buffer->buffer_menu, liste_buffer->buffer_final, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        blit(liste_buffer->buffer_map, liste_buffer->buffer_final, decalageX, 0, 124, 0, SCREEN_W, SCREEN_H);
        blit(liste_buffer->buffer_final, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        if(attente)//si il y a un incendie on attend un peu le temps de visualiser le chemin
        {
            rest(500);
            attente=0;
        }

    }

    //affichageGridMere(map);

    sauvegarde(compteur_argent, clock() / CLOCKS_PER_SEC+temps_ancienne_partie - CLK_debut, map, nb_habitant, *mode);//on sauvegarde le jeu pour une utilisation ultérieure
    printf("Sauvegarde effectuee\n");


    //libération de la mémoire
    show_mouse(NULL);
    liberation_memoire_bitmaps(liste_image, liste_buffer);
    liberation_donnee(map);
    printf("Fin du jeu\n");
}
