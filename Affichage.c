//
// Created by alexi on 01/11/2022.
//

#include "Affichage.h"
#include "barre_outil.h"

//simplification du code pour load une bitmap avec une erreur si pb
BITMAP * load_bitmap_check(char *nomImage){
    BITMAP *bmp;
    bmp=load_bitmap(nomImage,NULL);
    if (!bmp)
    {
        allegro_message("pas pu trouver %s",nomImage);
        exit(EXIT_FAILURE);
    }
    return bmp;
}


//affichage de la case selectionné, pas utile en soit mais ça permettra de bien placer les sprites avec la formule
void affichageCaseSelec(BITMAP* map, BITMAP* selec, t_pos souris)//R correspond à la colonne, B à la ligne
{
    draw_sprite(map, selec, (SCREEN_W/2-36)+souris.colonne*14-souris.ligne*14, souris.colonne*8+souris.ligne*8);
}



void affichageTotal()
{
    int choix;
    int screenx; // Position de l'écran réel dans le repère du décor...
    t_pos souris;
    souris.ligne=0;
    souris.colonne=0;

    BITMAP* bufferMenu;//Buffer regroupant les informations du menu

    BITMAP* sousMap;//pour la selection des cases, les 2 maps sont superposées, celle là on ne la verra pas
    BITMAP *selectionCase;//sprite d'une case sélectionnée
    BITMAP *bufferMap;//buffer 2 pour les infos sur la map reste stocké, buffer pour le jeu

    BITMAP* bufferFinal;//buffer d'affichage total

    BITMAP* map;//map en elle meme
    BITMAP* menu;


    menu= load_bitmap_check("test_ecran.bmp");
    map= load_bitmap_check("damierTestProvisoire.bmp");


    bufferMenu= create_bitmap(900, 700);
    clear_bitmap(bufferMenu);

    bufferMap= create_bitmap(1120, 640);
    clear_bitmap(bufferMap);

    bufferFinal= create_bitmap(SCREEN_W, SCREEN_H);
    clear_bitmap(bufferFinal);

    sousMap= load_bitmap_check("damierFond.bmp");
    selectionCase= load_bitmap_check("tuileBaseSelec.bmp");


    // Ici initialisation du scrolling en haut à gauche du décor
    //  (on peut décider de partir autrement...)
    screenx=0;
    printf("position %d\n", SCREEN_W/2-36);
    while (!key[KEY_ESC])
    {
        clear(bufferMenu);
        if (key[KEY_RIGHT])   screenx+=5;
        if (key[KEY_LEFT] )   screenx-=5;

        if ( screenx < 0 ) screenx=0;
        if ( screenx > bufferMap->w - SCREEN_W +124) screenx=bufferMap->w - SCREEN_W+124;

        blit(menu, bufferMenu, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        //blit(map, buffer2, 0, 0, 0, 0, map->w, map->h);

        draw_sprite(bufferMap, map, 0, 0);

        affichageCaseSelec(bufferMap, selectionCase, souris);

        textprintf_ex(bufferMap,font,10,10,makecol(0,255,0),makecol(0,0,0),"%4d %4d",mouse_x,mouse_y);
        textprintf_ex(bufferMap,font,10,20,makecol(0,255,0),makecol(0,0,0),"case[%d][%d]",souris.ligne,souris.colonne);
        blit(bufferMenu, bufferFinal, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        blit(bufferMap, bufferFinal, screenx, 0, 124, 0, SCREEN_W, SCREEN_H);

        if(mouse_b & 1)
        {
            if(mouse_x>124 && mouse_y<640)
            {
                souris= calcul_pos_souris(sousMap, screenx);
            }
            else
            {
                action(bufferFinal);
            }
        }

        show_mouse(bufferFinal);

        blit(bufferFinal, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        clear_bitmap(bufferMap);
    }
    show_mouse(NULL);

//Liberation de donnees
    destroy_bitmap(map);
    destroy_bitmap(menu);
    destroy_bitmap(bufferMap);
    destroy_bitmap(bufferMenu);
    destroy_bitmap(bufferFinal);
    destroy_bitmap(selectionCase);
    destroy_bitmap(sousMap);

}