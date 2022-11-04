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
    int choix;

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

        if(mouse_b & 1)
        {

            if(mouse_x<124 || mouse_y>640)
            {
                choix = action(liste_buffer->buffer_final);
                rest(100);
            }
        }
        if(choix==1)
        {
            if(mouse_x>124 && mouse_y<640)
            {
                souris= calcul_pos_souris(liste_image->sous_map, decalageX);
            }
            if(souris.ligne<35 && souris.colonne<45)
            {
                draw_sprite(liste_buffer->buffer_map, liste_image->route, (SCREEN_W/2-36)+souris.colonne*14-souris.ligne*14, souris.colonne*8+souris.ligne*8);
                if(mouse_b & 1)
                {
                    map = placementElement(map, souris.ligne, souris.colonne, choix);
                    choix=0;

                }
            }
        }
        blit(liste_buffer->buffer_menu, liste_buffer->buffer_final, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        blit(liste_buffer->buffer_map, liste_buffer->buffer_final, decalageX, 0, 124, 0, SCREEN_W, SCREEN_H);
        show_mouse(liste_buffer->buffer_final);
        blit(liste_buffer->buffer_final, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }



    //libération de la mémoire
    show_mouse(NULL);
    liberation_memoire_bitmaps(liste_image, liste_buffer);
}
