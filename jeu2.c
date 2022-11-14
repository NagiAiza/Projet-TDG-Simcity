//
// Created by alexi on 08/11/2022.
//

#include "jeu2.h"
#include "jeu1.h"
#include "stdio.h"

int calcul_depenses(int choix, int taille_chemin_route/*Algo Taille chemin*/)
{
    int depense=0;
    switch (choix) {
        case 1://achat d'une route
            depense = 10*taille_chemin_route;
            break;
        case 2://achat d'un chateau d'eau
            depense = 100000;
            break;
        case 3:
            depense = 100000;
            break;
        case 4:
            depense = 1000;
            break;
        default:
            depense = 0;
            break;
    }
    return depense;
}

int validation_depense(int depense, long argent_restant)
{
    if(argent_restant-depense>=0)
    {
        return 1;
    }
    else
    {
        printf("pas d'argent\n");
        return 0;
    }
}

//--------------------------------------------------------------------
///horloge
/*void setTimeout(int milliseconds)
{
    if (milliseconds <= 0) {
        fprintf(stderr, "Count milliseconds for timeout is less or equal to 0\n");
        return;
    }

    //chaque seconde
    int milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;

    int end = milliseconds_since + milliseconds;
    do {
        milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    } while (milliseconds_since <= end);
}
void horloge()
{

    int compteur= 0;
    int seconde=0;
    int minute=0;
    int heure=0;

    int xseconde=90;
    int yseconde=25;

    do {
        do
        {

            // on affiche l'horloge
            printf("%d:%d:%d", heure,minute,seconde);

            //textprintf_ex(page, font, 476, 176, makecol(255, 100, 200),-1, "%d",seconde);
//           if(seconde==0)
//           {
//              draw_sprite(page,horloge0,xseconde, yseconde);
//           }
//           if(seconde==1)
//           {
//              draw_sprite(page,horloge1,xseconde, yseconde);
//           }

            //le compteur s'incrémente chaque seconde
            setTimeout(1000);
            // on incrémente les secondes et le compteur
            seconde++;
            compteur++;
            //on efface la ligne pour re ecrire au meme endroit
            system("CLS");
            if(seconde==60) //les minutes s'incrémentes toutes les 60 secondes
            {
                minute++;//on incremente les minutes
            }
            if(minute==60) //les heures s'incrémentes toutes les 60 minutes
            {
                heure++;//on incremente les heures
            }
            if(seconde==60) //on repart à 0
            {
                seconde=0;
            }
            if(minute==60) //on repart à 0
            {
                minute=0;
            }
            if(heure==60) //on repart à 0
            {
                heure=0;
            }

        }while (seconde < 60);//une minute

    } while (compteur <= 7200);//tourne pendant deux heures

    ///-----
    // Lancer allegro et le mode graphique
    allegro_init();
    // Pour disposer du clavier
    install_keyboard();
    // pour disposer de la souris
    install_mouse();

    // Permet d'installer le clavier et de vérifier l'installation
    if(install_keyboard() == -1)
        allegro_message("ERREUR CLAVIER");

    // Initialisation et choix du mode graphique
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN,1024,768,0,0)!=0)
    {
        allegro_message("probleme gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);
    }


    /// Déclaration des pointeurs sur BITMAP devant recevoir les images
    // Buffer
    BITMAP *page;
    BITMAP *decor;
    BITMAP *horloge0;
    BITMAP *horloge1;
    int done;
    // buffer
    page=create_bitmap(SCREEN_W,SCREEN_H);
    clear_bitmap(page);

    // charger images de fond
    decor=load_bitmap("images/test_ecran.bmp",NULL);
    horloge0=load_bitmap("images/horloge/0.bmp",NULL);
    horloge1=load_bitmap("images/horloge/1.bmp",NULL);

    // pour voir le pointeur de la souris
    show_mouse(screen);

    if (!decor)
    {
        allegro_message("pas pu trouver les images");
        exit(EXIT_FAILURE);
    }
    while (done!=1)
    {

        //sortie de l'ecran si on appuie sur la touche s
        while (key[KEY_S])
        {
            done=1;
            destroy_bitmap(page);
        }
        // effacer buffer en appliquant décor  (pas de clear_bitmap)
        blit(decor,page,0,0,0,0,SCREEN_W,SCREEN_H);

        // afficher coordonnées de la souris (%4d = format numérique largeur fixe sur 4 caractères)
        textprintf_ex(page,font,1,750,makecol(0,255,0),makecol(0,0,0),"%4d %4d",mouse_x,mouse_y);


        // afficher tout ça à l'écran
        blit(page,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }
    ///-----
}
void allegro()
{
    printf("test!!");
    // Lancer allegro et le mode graphique
    allegro_init();
    // Pour disposer du clavier
    install_keyboard();
    // pour disposer de la souris
    install_mouse();


    int xseconde=90;
    int yseconde=25;

    // Permet d'installer le clavier et de vérifier l'installation
    if(install_keyboard() == -1)
        allegro_message("ERREUR CLAVIER");

    // Initialisation et choix du mode graphique
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN,1024,768,0,0)!=0)
    {
        allegro_message("probleme gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    /// Déclaration des pointeurs sur BITMAP devant recevoir les images
    // Buffer
    BITMAP *page;
    BITMAP *decor;
    BITMAP *horloge0;
    BITMAP *horloge1;
    int done;
    // buffer
    page=create_bitmap(SCREEN_W,SCREEN_H);
    clear_bitmap(page);

    // charger images de fond
    decor=load_bitmap("images/test_ecran.bmp",NULL);
    horloge0=load_bitmap("images/horloge/0.bmp",NULL);
    horloge1=load_bitmap("images/horloge/1.bmp",NULL);

    // pour voir le pointeur de la souris
    show_mouse(screen);

    if (!decor)
    {
        allegro_message("pas pu trouver les images");
        exit(EXIT_FAILURE);
    }
    while (done!=1)
    {

        //sortie de l'ecran si on appuie sur la touche s
        while (key[KEY_S])
        {
            done=1;
            destroy_bitmap(page);
        }
        // effacer buffer en appliquant décor  (pas de clear_bitmap)
        blit(decor,page,0,0,0,0,SCREEN_W,SCREEN_H);

        // afficher coordonnées de la souris (%4d = format numérique largeur fixe sur 4 caractères)
        textprintf_ex(page,font,1,750,makecol(0,255,0),makecol(0,0,0),"%4d %4d",mouse_x,mouse_y);


        // afficher tout ça à l'écran
        blit(page,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }
}*/


//gestion de l'eau
int** RemplissageMatricePoids (int** MatricePoids, int orientation, t_pos position)
{
    if (orientation == 1)
    {
        for (int i = -2; i < 2; i++)
        {
            for (int j = -2; j < 4; j++)
            {
                MatricePoids[position.ligne][position.colonne] = 0;
            }
        }
    }
    else if (orientation == -1)
    {
        for (int i = -3; i < 3; i++)
        {
            for (int j = -1; j < 3; j++)
            {
                MatricePoids[position.ligne][position.colonne] = 0;
            }
        }
    }
    return MatricePoids;
}
int** InitialisationContours (int** matricepoids, t_graphe* map)
{
    t_liste* liste_voisin = creer();
    for (int i=0 ; i<NBLIGNE ; i++)
    {
        for (int j=0 ; j<NBCOLONNE ; j++)
        {
            if (matricepoids==2)
            {
                liste_voisin = map->grille[i][j]->voisin;
                while(liste_voisin != NULL)
                {
                    if(liste_voisin->n->element->type==1)
                    {
                        matricepoids[liste_voisin->n->position.ligne][liste_voisin->n->position.colonne]=1;
                    }
                }
            }
        }
    }
    return matricepoids;
}

int** initialisation_matrice_poids(t_graphe* map)
{
    int** matrice_poids = NULL;
    //allocation dynatmique de notre matrice d'adjacente
    matrice_poids = (int**)malloc(NBLIGNE*sizeof(int*)); //1er dimension
    for (int i=0 ; i<NBLIGNE ; i++)
    {
        matrice_poids[i] = malloc(NBCOLONNE*sizeof(int)); //2eme dimension
    }
    for(int i=0 ; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            matrice_poids[i][j]=-1;
        }
    }
    for(int i=0 ; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            if (map->grille[i][j]->element->type == 2)
            {
                //remplissage matrice poids
                RemplissageMatricePoids(matrice_poids, map->grille[i][j]->element->orientation, map->grille[i][j]->position );
            }
        }
        printf("\n");
    }
    matrice_poids=InitialisationContours(matrice_poids, map);
    return matrice_poids;
}

t_graphe* distribution_eau(t_graphe* map)
{
    int** matrice_poids = initialisation_matrice_poids(map);

}

