//
// Created by alexi on 04/11/2022.
//

#include "moteur_du_jeu.h"

void jeu()//sous programme qui fera tourner tout le jeu
{
    //initialisation des variables
    int decalageX; // Position de l'écran réel dans le repère du décor...
    t_pos souris;
    souris.ligne=0;
    souris.colonne=0;
    int choix, rotation=1;

    //initialisation des bitmaps
    IMAGE* liste_image=initialisation_liste_image();
    BUFFER* liste_buffer=initialisation_liste_buffer();

    //generation du fichier
    initialisationElementCarte();//genere le fichier

    //génération de la map
    t_graphe* map=makeGrid();



    // Ici initialisation du scrolling en haut à gauche du décor
    //  (on peut décider de partir autrement...)
    decalageX=0;

    while(!key[KEY_ESC])
    {
        if (key[KEY_RIGHT])   decalageX+=5;
        if (key[KEY_LEFT] )   decalageX-=5;
        if ( decalageX < 0 ) decalageX=0;
        if ( decalageX > liste_buffer->buffer_map->w - SCREEN_W +124) decalageX=liste_buffer->buffer_map->w - SCREEN_W+124;


        affichageTotal(map, liste_image, liste_buffer, souris);

        if(mouse_x>124 && mouse_y<640)
        {
            souris= calcul_pos_souris(liste_image->sous_map, decalageX);
        }

//L'utilisateur choisi déjà un bouton dans la barre d'outil
        if(mouse_b & 1)
        {
            if(mouse_x<124 || mouse_y>640)
            {
                choix = choixAction();
                rest(100);
            }
        }

        //dès qu'on a récup ce qu'il veut faire on conserve son action et en fonction de l'action on lance le sous pgrm pour l'effectué
        map=action(map, liste_buffer, liste_image, &choix, souris, &rotation);


        blit(liste_buffer->buffer_menu, liste_buffer->buffer_final, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        blit(liste_buffer->buffer_map, liste_buffer->buffer_final, decalageX, 0, 124, 0, SCREEN_W, SCREEN_H);
        show_mouse(liste_buffer->buffer_final);
        blit(liste_buffer->buffer_final, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }



    //libération de la mémoire
    show_mouse(NULL);
    liberation_memoire_bitmaps(liste_image, liste_buffer);
    liberation_donnee(map);
}
