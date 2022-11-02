//
// Created by alexi on 01/11/2022.
//

#include "Affichage.h"

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
void affichageCaseSelec(BITMAP* map, BITMAP* selec, int R, int B)//R correspond à la colonne, B à la ligne
{
    draw_sprite(map, selec, (SCREEN_W/2-36)+R*14-B*14, R*8+B*8);
}

BITMAP* affichageMenu(BITMAP* bufferMenu)
{
    BITMAP* menu;
    menu= load_bitmap_check("test_ecran.bmp");
    blit(menu, bufferMenu, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    destroy_bitmap(menu);
    return bufferMenu;

}

BITMAP* affichageMap(BITMAP* bufferMap)
{
    BITMAP* map;//map en elle meme

    map= load_bitmap_check("damierTestProvisoire.bmp");

    draw_sprite(bufferMap, map, 0, 0); //pb d'affichage

//libération de données
    destroy_bitmap(map);

    return bufferMap;

}

void affichageTotal()
{
    int screenx; // Position de l'écran réel dans le repère du décor...

    BITMAP* bufferMenu;//Buffer regroupant les informations du menu

    BITMAP* sousMap;//pour la selection des cases, les 2 maps sont superposées, celle là on ne la verra pas
    BITMAP *selectionCase;//sprite d'une case sélectionnée
    BITMAP *bufferMap;//buffer 2 pour les infos sur la map reste stocké, buffer pour le jeu

    BITMAP* bufferFinal;//buffer d'affichage total



    bufferMenu= create_bitmap(900, 700);
    clear_bitmap(bufferMenu);

    bufferMap= create_bitmap(1120, 640);
    clear_bitmap(bufferMap);

    bufferFinal= create_bitmap(SCREEN_W, SCREEN_H);
    clear_bitmap(bufferFinal);

    sousMap= load_bitmap_check("damierFond.bmp");
    selectionCase= load_bitmap_check("tuileBaseSelec.bmp");


    int pixel, r, b;//pour récupérer les informations de la sous map rouge pour la colonne et bleu pour la longueur
    r=0;
    b=0;

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

        //blit(map, buffer2, 0, 0, 0, 0, map->w, map->h);
        show_mouse(bufferFinal);
        if(mouse_b & 1)
        {
            pixel= getpixel(sousMap, mouse_x+screenx-124, mouse_y);
            r=getr(pixel);
            b=getb(pixel);
            rest(100);
        }


        bufferMap= affichageMap(bufferMap);
        bufferMenu = affichageMenu(bufferMenu);

        affichageCaseSelec(bufferMap, selectionCase, r, b);


        textprintf_ex(bufferMap,font,10,10,makecol(0,255,0),makecol(0,0,0),"%4d %4d",mouse_x,mouse_y);
        textprintf_ex(bufferMap,font,10,20,makecol(0,255,0),makecol(0,0,0),"case[%d][%d]",b,r);
        blit(bufferMenu, bufferFinal, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        blit(bufferMap, bufferFinal, screenx, 0, 124, 0, SCREEN_W, SCREEN_H);
        if(((mouse_x>=6)&&(mouse_x<=39)&&(mouse_y>=90)&&(mouse_y<=126))&&(mouse_b&1)) //Si on clique sur la premiere case
        {
            textout_ex ( bufferFinal, font, "Action 1", 511, 326,makecol (0, 0, 0), -1); //texte explicatif
        }
        if(((mouse_x>=44)&&(mouse_x<=78)&&(mouse_y>=88)&&(mouse_y<=122))&&(mouse_b&1)) //Si on clique sur la deuxieme case
        {
            textout_ex ( bufferFinal, font, "Action 2", 511, 326,makecol (0, 0, 0), -1); //texte explicatif
        }
        if(((mouse_x>=87)&&(mouse_x<=119)&&(mouse_y>=91)&&(mouse_y<=123))&&(mouse_b&1)) //3eme case
        {
            textout_ex ( bufferFinal, font, "Action 3", 511, 326,makecol (0, 0, 0), -1); //texte explicatif
        }
        if(((mouse_x>=7)&&(mouse_x<=37)&&(mouse_y>=138)&&(mouse_y<=170))&&(mouse_b&1))
        {
            textout_ex ( bufferFinal, font, "Action 4", 511, 326,makecol (0, 0, 0), -1); //texte explicatif
        }
        if(((mouse_x>=45)&&(mouse_x<=77)&&(mouse_y>=137)&&(mouse_y<=170))&&(mouse_b&1))
        {
            textout_ex ( bufferFinal, font, "Action 5", 511, 326,makecol (0, 0, 0), -1); //texte explicatif
        }
        if(((mouse_x>=85)&&(mouse_x<=119)&&(mouse_y>=137)&&(mouse_y<=172))&&(mouse_b&1))
        {
            textout_ex ( bufferFinal, font, "Action 6", 511, 326,makecol (0, 0, 0), -1); //texte explicatif
        }
        if(((mouse_x>=6)&&(mouse_x<=37)&&(mouse_y>=182)&&(mouse_y<=218))&&(mouse_b&1))
        {
            textout_ex ( bufferFinal, font, "Action 7", 511, 326,makecol (0, 0, 0), -1); //texte explicatif
        }
        if(((mouse_x>=45)&&(mouse_x<=75)&&(mouse_y>=182)&&(mouse_y<=214))&&(mouse_b&1))
        {
            textout_ex ( bufferFinal, font, "Action 8", 511, 326,makecol (0, 0, 0), -1); //texte explicatif
        }
        if(((mouse_x>=86)&&(mouse_x<=119)&&(mouse_y>=182)&&(mouse_y<=215))&&(mouse_b&1))//9eme case
        {
            textout_ex ( bufferFinal, font, "Action 9", 511, 326,makecol (0, 0, 0), -1); //texte explicatif
        }
        show_mouse(bufferFinal);
        blit(bufferFinal, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }
    show_mouse(NULL);

//Liberation de donnees
    destroy_bitmap(bufferMap);
    destroy_bitmap(bufferMenu);
    destroy_bitmap(bufferFinal);
    destroy_bitmap(selectionCase);
    destroy_bitmap(sousMap);

}