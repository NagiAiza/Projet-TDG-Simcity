//
// Created by alexi on 04/11/2022.
//

#include "moteur_du_jeu.h"

void jeu()//sous programme qui fera tourner tout le jeu
{
    //initialisation des variables
    int decalageX; // Position de l'écran réel dans le repère du décor...
    long compteur_argent=500000;
    t_pos souris;
    souris.ligne=0;
    souris.colonne=0;

    int choix, rotation=1;
    int action_en_cours=0;
    int algo_A=0;
    t_tile* case_select;
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


        affichageTotal(map, liste_image, liste_buffer, souris, compteur_argent);

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
        map=action(map, liste_buffer, liste_image, &choix, souris, &rotation, &action_en_cours, &case_select, &algo_A, &compteur_argent);


        blit(liste_buffer->buffer_menu, liste_buffer->buffer_final, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        blit(liste_buffer->buffer_map, liste_buffer->buffer_final, decalageX, 0, 124, 0, SCREEN_W, SCREEN_H);
        show_mouse(liste_buffer->buffer_final);
        blit(liste_buffer->buffer_final, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);


    }
   for(int i=0 ; i<NBLIGNE; i++)
   {
       for(int j=0; j<NBCOLONNE; j++)
       {
           printf("%d ", map->mat_adjacence[i][j]);
       }
       printf("\n");
   }
    //libération de la mémoire
    show_mouse(NULL);
    liberation_memoire_bitmaps(liste_image, liste_buffer);
    //liberation_donnee(map);
}
