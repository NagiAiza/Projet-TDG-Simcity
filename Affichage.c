//
// Created by alexi on 01/11/2022.
//

#include "Affichage.h"
#include "listeRelated.h"

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

IMAGE* initialisation_liste_image()//on initialise une seule fois les bitmaps en début de prgm
{
    IMAGE* liste=(IMAGE*)malloc(sizeof(IMAGE));
    liste->menu= load_bitmap_check("test_ecran.bmp");
    liste->map= load_bitmap_check("damierTestProvisoire.bmp");
    liste->sous_map= load_bitmap_check("damierFond.bmp");
    liste->route = load_bitmap_check("route.bmp");
    liste->batiment = load_bitmap_check("batiment.bmp");
    liste->chateau_eau = load_bitmap_check("chateau_eau.bmp");
    liste->centrale = load_bitmap_check("centrale.bmp");
    liste->canalisation = load_bitmap_check("canalisation.bmp");
    liste->case_selec = load_bitmap_check("tuileBaseSelec.bmp");
    return liste;
}

BUFFER* initialisation_liste_buffer()//meme chose avec les buffer
{
    BUFFER* liste=(BUFFER*)malloc(sizeof(BUFFER));
    liste->buffer_menu=create_bitmap(900, 700);
    clear_bitmap(liste->buffer_menu);

    liste->buffer_map= create_bitmap(1120, 640);
    clear_bitmap(liste->buffer_map);

    liste->buffer_final= create_bitmap(SCREEN_W, SCREEN_H);
    clear_bitmap(liste->buffer_final);

    return liste;
}

void liberation_memoire_bitmaps(IMAGE* liste_image, BUFFER* liste_buffer)//on n'oublie pas de libérer la mémoire
{
    destroy_bitmap(liste_image->map);
    destroy_bitmap(liste_image->menu);
    destroy_bitmap(liste_image->route);
    destroy_bitmap(liste_image->batiment);
    destroy_bitmap(liste_image->sous_map);
    destroy_bitmap(liste_image->chateau_eau);
    destroy_bitmap(liste_image->centrale);
    destroy_bitmap(liste_image->canalisation);
    destroy_bitmap(liste_image->case_selec);
    free(liste_image);
    destroy_bitmap(liste_buffer->buffer_map);
    destroy_bitmap(liste_buffer->buffer_menu);
    destroy_bitmap(liste_buffer->buffer_final);
    free(liste_buffer);
}

t_pos calcul_pos_souris(BITMAP* sousMap, int decalageScreenX/*pour savoir où placer la bitmap*/)//retourne la position de la souris en fonction de notre niveau de scroll
{
    t_pos souris;
    souris.ligne=getb(getpixel(sousMap, mouse_x+decalageScreenX-124, mouse_y));
    souris.colonne=getr(getpixel(sousMap, mouse_x+decalageScreenX-124, mouse_y));
    return souris;
}

//affichage de la case selectionné, pas utile en soit mais ça permettra de bien placer les sprites avec la formule
void affichageCaseSelec(BUFFER* liste_buffer, IMAGE* liste_image, t_pos souris)//R correspond à la colonne, B à la ligne
{
    draw_sprite(liste_buffer->buffer_map, liste_image->case_selec, (SCREEN_W/2-36)+souris.colonne*14-souris.ligne*14, souris.colonne*8+souris.ligne*8);
}

void affichageElement(BITMAP* bufferMap, IMAGE* liste, int type, int ligne, int colonne, int rotation)//pour avoir la rotation du batiment il va falloir un autre fichier qui a l'emplacement de notre batiment mettra un 1 ou -1 en fonction du sens de rotation
{
    switch (type) {
        case 1://route
            draw_sprite(bufferMap, liste->route, (SCREEN_W/2-36)+colonne*14-ligne*14, colonne*8+ligne*8);
            break;
        case 2://chateau eau
            if(rotation==1)
            {
                draw_sprite(bufferMap, liste->chateau_eau, (SCREEN_W / 2 - 36) + (colonne - 3) * 14 - (ligne) * 14, (colonne - 3) * 8 + (ligne) * 8 - 8);//pq le -8? jsp j'ai tatonné
            }
            else if(rotation==-1)
            {
                draw_sprite_h_flip(bufferMap, liste->chateau_eau, (SCREEN_W/2-36)+(colonne-3)*14-(ligne)*14, (colonne-3)*8+(ligne)*8-8);
            }
            break;
        case 3://centrale
            if(rotation==1)
            {
                draw_sprite(bufferMap, liste->centrale, (SCREEN_W / 2 - 36) + (colonne - 3) * 14 - (ligne) * 14, (colonne - 3) * 8 + (ligne) * 8 - 8);//pq le -8? jsp j'ai tatonné
            }
            else if(rotation==-1)
            {
                draw_sprite_h_flip(bufferMap, liste->centrale, (SCREEN_W/2-36)+(colonne-3)*14-(ligne)*14, (colonne-3)*8+(ligne)*8-8);
            }
            //Chateau Deau;
            break;
        case 4://habitation
            draw_sprite(bufferMap, liste->batiment, (SCREEN_W/2-36)+(colonne-2)*14-ligne*14, (colonne-2)*8+ligne*8);
            break;
        case 5://habitation de stade 1
            break;
        case 6://habitation de stade 2
            break;
        case 7://habitation de stade 3
            break;
        case 8://habitation de stade 4
            break;
        case 9://habitation au stade de ruine
            break;
        default:
            break;
    }
}

void affichage_element_eau(BUFFER* liste_buffer, IMAGE* liste, int type, int ligne, int colonne, int rotation, t_tile* chateau)
{
    int rgb;
    int r, b;
    BITMAP* temp= create_bitmap(liste->chateau_eau->w, liste->chateau_eau->h);
    switch (type) {
        case 1://route
            draw_sprite(liste_buffer->buffer_map, liste->route, (SCREEN_W/2-36)+colonne*14-ligne*14, colonne*8+ligne*8);
            break;
        case 2://chateau eau
            for(int i=0; i<temp->h; i++)
            {
                for(int j=0; j<temp->w; j++)
                {
                    rgb= getpixel(liste->chateau_eau, j, i);
                    r=getr(rgb);
                    b=getb(rgb);
                    if(r==255 && b==255)
                    {
                        putpixel(temp, j, i, rgb);
                    }
                    else
                    {
                        putpixel(temp, j, i, makecol(0, 0, chateau->element->couleur*50));
                    }
                }
            }
            if(rotation==1)
            {
                draw_sprite(liste_buffer->buffer_map, temp, (SCREEN_W / 2 - 36) + (colonne - 3) * 14 - (ligne) * 14, (colonne - 3) * 8 + (ligne) * 8 - 8);//pq le -8? jsp j'ai tatonné
            }
            else if(rotation==-1)
            {
                draw_sprite_h_flip(liste_buffer->buffer_map, temp, (SCREEN_W/2-36)+(colonne-3)*14-(ligne)*14, (colonne-3)*8+(ligne)*8-8);
            }
            break;
        default:
            break;
    }
}

void affichage_level_0(BUFFER* liste_buffer, IMAGE* liste_image)//on pourra peut etre rajouter des obstacles du style montagne etc...
{
    //peut etre 2 fichiers texte, un pour l'affichage, l'autre pour les données de la map
    //parce que avec ça on va avoir un problème pour l'affichage des batiments > 1x1
    //j'ai trouvé!
    //on part de la coordonnées de départ de la souris et on donne le point de départ du sprite en fonction de la case du fichier indiquant le type de batiment, on regarde la rotation et on affiche l'élement en fonction de ça
    FILE* elementMap=fopen("element_map.txt", "r");//pour la nouvelle partie tout les elements sont à 0
    FILE* rotation_element_map=fopen("rotation_element_map.txt", "r");// la rotation indique dans quel sens on doit mettre le batiment sur la carte et donc la place qu'il prend
    int type=0;
    int rotation=0;
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            fscanf(elementMap, "%d", &type);
            fscanf(rotation_element_map, "%d", &rotation);
            affichageElement(liste_buffer->buffer_map, liste_image, type, i, j, rotation);
        }
    }
    fclose(rotation_element_map);
    fclose(elementMap);
}


void affichage_habitation(BUFFER* liste_buffer, IMAGE* liste_image, t_tile* habitation)//il manque la pourcentage non alimente
{
    BITMAP* temp= create_bitmap(liste_image->batiment->w, liste_image->batiment->h);
    int compteur=0;
    int rgb;
    int r;
    int b;
    float pourcentage[50];
    pourcentage[0]=0;
    float pourcentage_non_alimente=0;
    int non_alimente=0;
    if(habitation->element->eau_actuelle!=habitation->element->nb_habitant)
    {
        pourcentage_non_alimente=(float)habitation->element->eau_actuelle/(float)habitation->element->nb_habitant;
        non_alimente=1;
    }
    t_liste2* liste_chateau=habitation->element->chateau_approvisionnement;
    while(liste_chateau!=NULL)
    {
        compteur++;
        pourcentage[compteur]=(float)liste_chateau->montant_distribue/(float)habitation->element->nb_habitant+pourcentage[compteur-1];
        liste_chateau=liste_chateau->next;
    }
    if(non_alimente==1 && compteur!=0)
    {
        compteur++;
        pourcentage[compteur]=pourcentage_non_alimente+pourcentage[compteur-1];
        printf("pourcentage non alimente : %f\n", pourcentage[compteur]*100);
    }
    pourcentage[compteur+1]=1;
    if(compteur==0)
    {
        draw_sprite(liste_buffer->buffer_map, liste_image->batiment, (SCREEN_W/2-36)+(habitation->position.colonne-2)*14-habitation->position.ligne*14, (habitation->position.colonne-2)*8+habitation->position.ligne*8);
    }
    else
    {
        liste_chateau=habitation->element->chateau_approvisionnement;
        for(int i=0; i<compteur; i++)
        {
            for(int j=0; j<temp->h; j++)
            {
                for(int k=(int)(pourcentage[i]*temp->w); k<(int)(pourcentage[i+1]*temp->w); k++)
                {
                    //recuperer le pixel, si il est magenta on le met sur la bitmap temporaire, sinon si il est rouge on change la couleur
                    rgb= getpixel(liste_image->batiment, k, j);
                    r=getr(rgb);
                    b=getb(rgb);
                    if(r==255 && b==255)
                    {
                        putpixel(temp, k, j, rgb);
                    }
                    else
                    {
                        if(non_alimente==0)
                        {
                            putpixel(temp, k, j, makecol(0,0, makecol(0,0,liste_chateau->n->element->couleur*50)));
                        }
                        else
                        {

                            /*if(i<compteur-1)
                            {
                                putpixel(temp, k, j, makecol(0,0, makecol(0,0,liste_chateau->n->element->couleur*50)));
                            }
                            else
                            {*/
                                putpixel(temp, k, j, makecol(0,0, makecol(255,0,0)));
                            //}
                        }
                    }
                }
            }
            liste_chateau=liste_chateau->next;
        }

        draw_sprite(liste_buffer->buffer_map, temp, (SCREEN_W/2-36)+(habitation->position.colonne-2)*14-habitation->position.ligne*14, (habitation->position.colonne-2)*8+habitation->position.ligne*8);
    }
}

void affichage_level_1(t_graphe* map, IMAGE* liste_image, BUFFER* liste_buffer)
{
    FILE* elementMap=fopen("element_map.txt", "r");//pour la nouvelle partie tout les elements sont à 0
    FILE* rotation_element_map=fopen("rotation_element_map.txt", "r");// la rotation indique dans quel sens on doit mettre le batiment sur la carte et donc la place qu'il prend
    FILE* map_eau=fopen("map_eau.txt", "r");//pour la nouvelle partie tout les elements sont à 0

    int canalisation=0;
    int type=0;
    int rotation=0;
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            fscanf(elementMap, "%d", &type);
            fscanf(rotation_element_map, "%d", &rotation);
            fscanf(map_eau, "%d", &canalisation);
            if(canalisation==1)
            {

                draw_sprite(liste_buffer->buffer_map, liste_image->canalisation, (SCREEN_W/2-36)+j*14-i*14, j*8+i*8);
            }
            else
            {
                if(type==1 || type ==2)
                {
                    affichage_element_eau(liste_buffer, liste_image, type, i, j, rotation, map->grille[i][j]);
                }
                else if(type>=4 && type <=10)
                {
                    affichage_habitation(liste_buffer, liste_image, map->grille[i][j]);
                }
            }
        }
    }

    fclose(map_eau);
    fclose(rotation_element_map);
    fclose(elementMap);
}


void affichageTotal(t_graphe* map, IMAGE* liste_image, BUFFER* liste_buffer, t_pos souris, long compteur_argent, int niveau_visu, int capa_usine)//doit etre independant du jeu en lui meme mais affiche toute les données nécéssaire à l'utilisateur
{
        clear_bitmap(liste_buffer->buffer_menu);
        clear_bitmap(liste_buffer->buffer_map);


        blit(liste_image->menu, liste_buffer->buffer_menu, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        textprintf_ex(liste_buffer->buffer_menu,font,10,645,makecol(0,0,0),-1,"%ld$",compteur_argent);
        textprintf_ex(liste_buffer->buffer_menu,font,750,645,makecol(0,0,0),-1,"%d",capa_usine);


        draw_sprite(liste_buffer->buffer_map, liste_image->map, 0, 0);

        textprintf_ex(liste_buffer->buffer_map,font,10,10,makecol(0,255,0),makecol(0,0,0),"%4d %4d",mouse_x,mouse_y);
        textprintf_ex(liste_buffer->buffer_map,font,10,20,makecol(0,255,0),makecol(0,0,0),"case[%d][%d]",souris.ligne,souris.colonne);
        textprintf_ex(liste_buffer->buffer_map,font,10,30,makecol(0,255,0),makecol(0,0,0),"niveau %d",niveau_visu);
        if(niveau_visu==0)
        {
            affichage_level_0(liste_buffer, liste_image);
        }
        else if (niveau_visu==1)
        {
            affichage_level_1(map, liste_image, liste_buffer);
        }

}

