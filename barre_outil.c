//
// Created by alexi on 03/11/2022.
//

#include "barre_outil.h"

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

int action(BITMAP* bufferFinal)
{
    int choix=choixAction();
    switch (choix) {
        case 1:
            textout_ex ( bufferFinal, font, "Action 1", 511, 326,makecol (255, 255, 255), -1); //texte explicatif
            break;
        case 2:
            textout_ex ( bufferFinal, font, "Action 2", 511, 326,makecol (255, 255, 255), -1); //texte explicatif
            break;
        case 3:
            textout_ex ( bufferFinal, font, "Action 3", 511, 326,makecol (255, 255, 255), -1); //texte explicatif
            break;
        case 4:
            textout_ex ( bufferFinal, font, "Action 4", 511, 326,makecol (255, 255, 255), -1); //texte explicatif
            break;
        case 5:
            textout_ex ( bufferFinal, font, "Action 5", 511, 326,makecol (255, 255, 255), -1); //texte explicatif
            break;
        case 6:
            textout_ex ( bufferFinal, font, "Action 6", 511, 326,makecol (255, 255, 255), -1); //texte explicatif
            break;
        case 7:
            textout_ex ( bufferFinal, font, "Action 7", 511, 326,makecol (255, 255, 255), -1); //texte explicatif
            break;
        case 8:
            textout_ex ( bufferFinal, font, "Action 8", 511, 326,makecol (255, 255, 255), -1); //texte explicatif
            break;
        case 9:
            textout_ex ( bufferFinal, font, "Action 9", 511, 326,makecol (255, 255, 255), -1); //texte explicatif
            break;
        default:
            textout_ex ( bufferFinal, font, "Rien de selectionne", 511, 326,makecol (255, 255, 255), -1); //texte explicatif
            break;
    }
    return choix;
}