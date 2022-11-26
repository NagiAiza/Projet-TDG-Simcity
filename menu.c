//
// Created by Estelle on 07/11/2022.
//

#include "menu.h"
#include "Affichage.h"

void suite_menu()
{
    BITMAP * stitch_bonjour;
    stitch_bonjour = load_bitmap_check("stitch_bonjour.bmp");
    while (!key[KEY_SPACE])
    {
        draw_sprite(screen,stitch_bonjour,0,0);
    }
}


int menu()
{
    //declaration BITMAP devant recevoir l'image

    // SuperCool
    BITMAP* debut_projet;
    // barre de chargement
    BITMAP* chargement[17];
    char tableauChargement[14];
    // COMMENCER/CONTINUER/QUITTER
    BITMAP* image;
    BITMAP* prop1;
    BITMAP* prop2;
    BITMAP* prop3;
    // BILOU/CAPITALISTE
    BITMAP* Image;
    BITMAP* prop4;
    BITMAP* prop5;
    // MAP
    BITMAP* test_ecran;
    //regle
    BITMAP* regle;

    // pour voir le pointeur de la souris
    show_mouse(screen);

    debut_projet=load_bitmap_check("debut_projet.bmp");


    ///Tableau d'image
    for (int i=0 ; i<=16 ; i++)
    {
        printf ("--> tg %d\n",i);
        sprintf(tableauChargement, "chargement%d.bmp",i);
        chargement[i]=load_bitmap(tableauChargement,NULL);
        //blindage
        if(!chargement[i])
        {
            allegro_message("pas pu trouver %s", tableauChargement);
            exit(EXIT_FAILURE);
        }
    }

    // Chargement de l'image (l'allocation a lieu en même temps)
    image=load_bitmap_check("mon_image.bmp");

    prop1=load_bitmap_check("prop1.bmp");

    prop2=load_bitmap_check("prop2.bmp");

    prop3=load_bitmap_check("prop3.bmp");

    Image=load_bitmap_check("mon_image2.bmp");

    prop4=load_bitmap_check("prop4.bmp");

    prop5=load_bitmap_check("prop5.bmp");

    regle=load_bitmap_check("regle.bmp");

    test_ecran=load_bitmap_check("test_ecran.bmp");


    blit(debut_projet,screen,0,0,SCREEN_W-debut_projet->w,SCREEN_H-debut_projet->h, debut_projet->w, debut_projet->h);
    rest(2000);

    printf ("ruig^rgh\n");
    //affichage d'un tableau d'image
    for (int i=1 ; i<16 ; i++)
    {
        printf("%d\n",i);
        blit(chargement[i],screen,0,0, (SCREEN_W-chargement[i]->w)/2, (SCREEN_H-chargement[i]->h)/2, chargement[i]->w, chargement[i]->h);
        rest(100);
    }
    blit(chargement[16],screen,0,0,SCREEN_W-chargement[16]->w,SCREEN_H-chargement[16]->h, chargement[16]->w, chargement[16]->h);
    rest(1000);

    //boucle
    while (!key[KEY_ESC]) {
        //Affichage
        blit(image, screen, 0, 0, SCREEN_W - image->w, SCREEN_H - image->h, image->w, image->h);

        // afficher coordonnées de la souris (%4d = format numérique largeur fixe sur 4 caractères)
        //textprintf_ex(screen, font, 60, 300, makecol(0, 255, 0), makecol(0, 0, 0), "%4d %4d", mouse_x, mouse_y);

        if (mouse_x >= 391 && mouse_x <= 607 && mouse_y >= 87 && mouse_y <= 154) {
            blit(prop1, screen, 0, 0, SCREEN_W - prop1->w, SCREEN_H - prop1->h, prop1->w, prop1->h);
            rest(100);
            if (mouse_b & 1) {
                printf("COMMENCER\n");
                rest(500);
                while (!key[KEY_ESC]) {
                    //textprintf_ex(screen,font,60,300,makecol(0,255,0),makecol(0,0,0),"%4d %4d",mouse_x,mouse_y);

                    blit(Image, screen, 0, 0, SCREEN_W - Image->w, SCREEN_H - Image->h, Image->w, Image->h);
                    if (mouse_x >= 227 && mouse_x <= 444 && mouse_y >= 290 && mouse_y <= 357) {
                        blit(prop4, screen, 0, 0, SCREEN_W - prop4->w, SCREEN_H - prop4->h, prop4->w, prop4->h);
                        rest(100);
                        if (mouse_b & 1) {

                            printf("communiste\n");
                            rest(500);
                            suite_menu();
                            return 1;
                        }
                    } else if (mouse_x >= 582 && mouse_x <= 800 && mouse_y >= 290 && mouse_y <= 357) {
                        blit(prop5, screen, 0, 0, SCREEN_W - prop5->w, SCREEN_H - prop5->h, prop5->w, prop5->h);
                        rest(100);
                        if (mouse_b & 1) {
                            printf("capitaliste\n");
                            rest(500);
                            return 2;
                        }
                    }

                }

            }

        } else if (mouse_x >= 391 && mouse_x <= 607 && mouse_y >= 293 && mouse_y <= 361) {
            blit(prop2, screen, 0, 0, SCREEN_W - prop2->w, SCREEN_H - prop2->h, prop2->w, prop2->h);
            rest(100);
            if (mouse_b & 1) {
                printf("CONTINUER\n");
                rest(500);
                ///METTRE LE SOUS PROGRAMME DE SAUVEGARDE POUR RECUP LA PARTIE EN COURS
            }
        } else if (mouse_x >= 391 && mouse_x <= 607 && mouse_y >= 500 && mouse_y <= 566) {
            blit(prop3, screen, 0, 0, SCREEN_W - prop3->w, SCREEN_H - prop3->h, prop3->w, prop3->h);
            rest(100);
            if (mouse_b & 1) {
                printf("QUITTER\n");
                rest(500);
                exit(EXIT_SUCCESS);
            }
        } else if (mouse_x >= 947 && mouse_x <= 999 && mouse_y >= 23 && mouse_y <= 79) {
            blit(regle, screen, 0, 0, SCREEN_W - regle->w, SCREEN_H - regle->h, regle->w, regle->h);
            rest(100);
            if (mouse_b & 1) {
                printf("regle\n");
                rest(500);
            }

        }
    }
    ///FREE TTE LES BITMAPS
    destroy_bitmap(debut_projet);
        for (int i=1 ; i<=16 ; i++)
        {
            destroy_bitmap(chargement[i]);
        }
    destroy_bitmap(image);
    destroy_bitmap(prop1);
    destroy_bitmap(prop2);
    destroy_bitmap(prop3);
    destroy_bitmap(Image);
    destroy_bitmap(prop4);
    destroy_bitmap(prop5);


    return 0;
}

void ecran_victoire_saboteur(SAMPLE* music, int* volume)
{
    // Buffer
    BITMAP *page;
    BITMAP*decor;
    BITMAP *dirigeable;


    play_sample(music, 255, 0, 1000, PLAYMODE_LOOP);




    // Position de l'écran réel dans le repère du décor
    int screenx,screeny;

    // buffer
    page=create_bitmap(SCREEN_W,SCREEN_H);
    clear_bitmap(page);

    // charger image de fond
    decor=load_bitmap("nuage.bmp",NULL);
    if (!decor)
    {
        allegro_message("pas pu trouver images/grandfond_decor.bmp");
        exit(EXIT_FAILURE);
    }

    // initialisation du scrolling en haut à gauche du décor
    screenx=0;
    screeny=900;

    // créer le dirigeable et la cible
    dirigeable=load_bitmap("dirigeable.bmp",NULL);

    printf("ici\n");

    while (!key[KEY_SPACE] && !(mouse_b & 1))
    {
        //scroller le decor
        screeny-=5;

        // contrôle des bords : on retourne au debut de l'image lorsque les acteurs arrivent a la fin du decor
        if ( (screeny<0) )
            screeny=900;


        // EFFACER BUFFER EN APPLIQUANT UNE PARTIE DU DECOR (TAILLE DE L'ECRAN)
        blit(decor,page,screenx,screeny,0,0,SCREEN_W,SCREEN_H);

        draw_sprite(page, dirigeable, 300, 200);


        // afficher tout ça à l'écran
        blit(page,screen,0,0,0,0,SCREEN_W,SCREEN_H);

        // pause
        rest(10);
    }
    destroy_bitmap(decor);
    destroy_bitmap(page);
    destroy_bitmap(dirigeable);
}
