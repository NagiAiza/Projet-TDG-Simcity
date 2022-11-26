//
// Created by alexi on 04/11/2022.
//

#include "moteur_du_jeu.h"
#include "jeu2.h"
#include "time.h"
#include "jeu3.h"

void jeu(int mode, int nvlle_partie)//sous programme qui fera tourner tout le jeu
{
    srand(time(NULL));
    //initialisation des variables
    int decalageX; // Position de l'écran réel dans le repère du décor...
    long compteur_argent=500000;
    int capa_usine=0;
    int nb_habitant=0;
    int capa_eau=0;
    int exit=0;
    int attente=0;
    long temps_ancienne_partie=0;
    t_pos souris;
    souris.ligne=0;
    souris.colonne=0;

    int scroll=-1;
    int screeny=0;


    int choix, rotation=1, niv_visu=0;
    int algo_A=0;
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
        map=lecture_sauvegarde(map, &compteur_argent, &temps_ancienne_partie, &nb_habitant, &mode);
        map= distribution_eau(map);//pb avec l'eau surtout
        map= distribution_elec(map);
    }


    // Ici initialisation du scrolling en haut à gauche du décor
    //  (on peut décider de partir autrement...)
    decalageX=0;
    show_mouse(screen);
    clock_t CLK_debut=clock() / CLOCKS_PER_SEC;
    while(!key[KEY_ESC] && exit!=1)
    {
        if (key[KEY_RIGHT])   decalageX+=5;
        if (key[KEY_LEFT] )   decalageX-=5;
        if ( decalageX < 0 ) decalageX=0;
        if ( decalageX > liste_buffer->buffer_map->w - SCREEN_W +124) decalageX=liste_buffer->buffer_map->w - SCREEN_W+124;

        capa_eau=compte_eau(map);

        affichageTotal(map, liste_image, liste_buffer, souris, compteur_argent, niv_visu, capa_usine, CLK_debut, nb_habitant, capa_eau, screeny, temps_ancienne_partie);


        if(mouse_x>124 && mouse_y<640)
        {
            souris= calcul_pos_souris(liste_image->sous_map, decalageX);
        }

//L'utilisateur choisi déjà un bouton dans la barre d'outil
        if(mouse_b & 1)
        {
            if(mouse_x<124 || mouse_y>640)
            {
                choixAction(liste_buffer, liste_image, &choix);
                rest(100);
            }
        }
        scroll_map(&scroll, &niv_visu, &screeny);
        //dès qu'on a récup ce qu'il veut faire on conserve son action et en fonction de l'action on lance le sous pgrm pour l'effectuer

        map=action(map, liste_buffer, liste_image, &choix, souris, &rotation, niv_visu, &case_select, &algo_A, &compteur_argent, &capa_usine, &exit, &scroll);

        map=cycle_habitation(map, &capa_usine, &compteur_argent, &nb_habitant, capa_eau, liste_buffer, liste_image, &attente, mode, temps_ancienne_partie);

        map=remise_0_argent(map, souris);

        if(choix>=1 && choix <=5)
        {
            draw_sprite(liste_buffer->buffer_menu, liste_image->annulation_action, 25, 517);
            draw_sprite(liste_buffer->buffer_menu, liste_image->rotation, 70, 517);
        }

        affichage_info_menu(liste_buffer, liste_image);

        blit(liste_buffer->buffer_menu, liste_buffer->buffer_final, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        blit(liste_buffer->buffer_map, liste_buffer->buffer_final, decalageX, 0, 124, 0, SCREEN_W, SCREEN_H);
        blit(liste_buffer->buffer_final, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        if(attente)
        {
            rest(500);
            attente=0;
        }

    }

    //affichageGridMere(map);
    sauvegarde(compteur_argent, clock() / CLOCKS_PER_SEC+temps_ancienne_partie - CLK_debut, map, nb_habitant, mode);
    //libération de la mémoire
    show_mouse(NULL);
    liberation_memoire_bitmaps(liste_image, liste_buffer);
    liberation_donnee(map);
}
