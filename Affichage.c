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
    liste->map= load_bitmap_check("map.bmp");
    liste->sous_map= load_bitmap_check("damierFond.bmp");
    liste->route = load_bitmap_check("route.bmp");
    liste->batiment = load_bitmap_check("batiment.bmp");
    liste->chateau_eau = load_bitmap_check("chateau_eau.bmp");
    liste->centrale = load_bitmap_check("centrale.bmp");
    liste->canalisation = load_bitmap_check("canalisation.bmp");
    liste->reseau= load_bitmap_check("reseau.bmp");
    liste->case_selec = load_bitmap_check("tuileBaseSelec.bmp");

    //VRAIS SPRITE
    liste->eau_fini= load_bitmap_check("eau-fini.bmp");
    liste->elec_fini= load_bitmap_check("elec-fini.bmp");
    liste->terrain_vague= load_bitmap_check("terrain-vague.bmp");
    liste->cabane=load_bitmap_check("cabanefinie.bmp");
    liste->maison=load_bitmap_check("maison-finie.bmp");
    liste->immeuble=load_bitmap_check("immeuble-fini.bmp");
    liste->gratte_ciel= load_bitmap_check("gratte-ciel-fini.bmp");
    liste->ruine=load_bitmap_check("ruines-finie.bmp");
    liste->caserne_pompiers=load_bitmap_check("pompiers.bmp");
    liste->piece=load_bitmap_check("piece.bmp");
    liste->flamme = load_bitmap_check("flamme.bmp");
    liste->feu= load_bitmap_check("feu.bmp");

    liste->barre_outils=load_bitmap_check("barre_outils.bmp");//same que menu à changer!
    liste->info_eau=load_bitmap_check("info_eau.bmp");
    liste->info_elec=load_bitmap_check("info_elec.bmp");
    liste->info_monnaie=load_bitmap_check("info_monnaie.bmp");
    liste->info_habitants=load_bitmap_check("info_habitants.bmp");
    //liste->info_deplacer=load_bitmap_check("info_deplacer.bmp");
    //liste->info_supprimer=load_bitmap_check("info_supprimer.bmp");
    liste->nuage=load_bitmap_check("nuage.bmp");
    liste->annulation_action= load_bitmap_check("croix_rouge.bmp");
    liste->rotation= load_bitmap_check("symbole_rotation.bmp");
    return liste;
}

BUFFER* initialisation_liste_buffer()//meme chose avec les buffer
{
    BUFFER* liste=(BUFFER*)malloc(sizeof(BUFFER));
    liste->buffer_menu=create_bitmap(1024, 740);
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
    destroy_bitmap(liste_image->route);
    destroy_bitmap(liste_image->batiment);
    destroy_bitmap(liste_image->sous_map);
    destroy_bitmap(liste_image->chateau_eau);
    destroy_bitmap(liste_image->centrale);
    destroy_bitmap(liste_image->canalisation);
    destroy_bitmap(liste_image->reseau);
    destroy_bitmap(liste_image->case_selec);

    destroy_bitmap(liste_image->eau_fini);
    destroy_bitmap(liste_image->elec_fini);
    destroy_bitmap(liste_image->terrain_vague);
    destroy_bitmap(liste_image->cabane);
    destroy_bitmap(liste_image->maison);
    destroy_bitmap(liste_image->immeuble);
    destroy_bitmap(liste_image->gratte_ciel);
    destroy_bitmap(liste_image->ruine);
    destroy_bitmap(liste_image->caserne_pompiers);
    destroy_bitmap(liste_image->piece);
    destroy_bitmap(liste_image->flamme);
    destroy_bitmap(liste_image->feu);

    destroy_bitmap(liste_image->info_eau);
    destroy_bitmap(liste_image->info_elec);
    destroy_bitmap(liste_image->info_monnaie);
    destroy_bitmap(liste_image->info_habitants);
    destroy_bitmap(liste_image->annulation_action);
    destroy_bitmap(liste_image->rotation);

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
                draw_sprite(bufferMap, liste->eau_fini, (SCREEN_W / 2 - 36) + (colonne - 3) * 14 - (ligne) * 14, (colonne - 3) * 8 + (ligne) * 8 - 11);//pq le -8? jsp j'ai tatonné
            }
            else if(rotation==-1)
            {
                draw_sprite_h_flip(bufferMap, liste->eau_fini, (SCREEN_W/2-36)+(colonne-3)*14-(ligne)*14, (colonne-3)*8+(ligne)*8-11);
            }
            break;
        case 3://centrale
            if(rotation==1)
            {
                draw_sprite(bufferMap, liste->elec_fini, (SCREEN_W / 2 - 36) + (colonne - 3) * 14 - (ligne) * 14, (colonne - 3) * 8 + (ligne) * 8 - 22);//pq le -8? jsp j'ai tatonné
            }
            else if(rotation==-1)
            {
                draw_sprite_h_flip(bufferMap, liste->elec_fini, (SCREEN_W/2-36)+(colonne-3)*14-(ligne)*14, (colonne-3)*8+(ligne)*8-22);
            }
            //Chateau Deau;
            break;
        case 4://habitation
            draw_sprite(bufferMap, liste->terrain_vague, (SCREEN_W/2-36)+(colonne-2)*14-ligne*14, (colonne-2)*8+ligne*8);
            break;
        case 5://habitation de stade 1
            draw_sprite(bufferMap, liste->cabane, (SCREEN_W/2-36)+(colonne-2)*14-ligne*14, (colonne-2)*8+ligne*8-11);//-11 parce que l'image est 11 pixel plus grande que le sprite de base
            break;
        case 6://habitation de stade 2
            draw_sprite(bufferMap, liste->maison, (SCREEN_W/2-36)+(colonne-2)*14-ligne*14, (colonne-2)*8+ligne*8-27);
            break;
        case 7://habitation de stade 3
            draw_sprite(bufferMap, liste->immeuble, (SCREEN_W/2-36)+(colonne-2)*14-ligne*14, (colonne-2)*8+ligne*8-36);
            break;
        case 8://habitation de stade 4
            draw_sprite(bufferMap, liste->gratte_ciel, (SCREEN_W/2-36)+(colonne-2)*14-ligne*14, (colonne-2)*8+ligne*8-87);
            break;
        case 9://habitation au stade de ruine
            draw_sprite(bufferMap, liste->ruine, (SCREEN_W/2-36)+(colonne-2)*14-ligne*14, (colonne-2)*8+ligne*8);
            break;
        case 10:
            if(rotation==1)
            {
                draw_sprite(bufferMap, liste->caserne_pompiers, (SCREEN_W / 2 - 36) + (colonne - 3) * 14 - (ligne) * 14, (colonne - 3) * 8 + (ligne) * 8 - 52);//pq le -8 - 44? jsp j'ai tatonné
            }
            else if(rotation==-1)
            {
                draw_sprite_h_flip(bufferMap, liste->caserne_pompiers, (SCREEN_W/2-36)+(colonne-3)*14-(ligne)*14, (colonne-3)*8+(ligne)*8-52);
            }
            break;
        default:
            break;
    }
}


void affichage_level_0(BUFFER* liste_buffer, IMAGE* liste_image, t_graphe* map)//on pourra peut etre rajouter des obstacles du style montagne etc...
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
            if(map->grille[i][j]->element->type>=4 && map->grille[i][j]->element->type<=9)
            {
                if(map->grille[i][j]->element->argent==1)
                {
                    draw_sprite(liste_buffer->buffer_map, liste_image->piece, (SCREEN_W/2-36)+(map->grille[i][j]->position.colonne-3)*14-map->grille[i][j]->position.ligne*14, (map->grille[i][j]->position.colonne-2)*8+map->grille[i][j]->position.ligne*8-11);
                }
            }
        }
    }
    fclose(rotation_element_map);
    fclose(elementMap);
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
                        putpixel(temp, j, i, makecol(0, 0, chateau->element->couleur*30));
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
            textprintf_ex(liste_buffer->buffer_map,font,(SCREEN_W/2-36)+(colonne-3)*14-(ligne)*14+35,(colonne-3)*8+(ligne)*8+26,makecol(255,255,255),-1,"%d/%d",chateau->element->capacite,5000);
            break;
        default:
            break;
    }
}


void affichage_habitation(BUFFER* liste_buffer, IMAGE* liste_image, t_tile* habitation)//pour le niveau -1
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
    if(habitation->element->eau_actuelle<habitation->element->nb_habitant)
    {
        pourcentage_non_alimente=1-(float)habitation->element->eau_actuelle/(float)habitation->element->nb_habitant;
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
                            putpixel(temp, k, j, makecol(0,0, makecol(0,0,liste_chateau->n->element->couleur*30)));
                        }
                        else
                        {

                            if(i<compteur-1)
                            {
                                putpixel(temp, k, j, makecol(0,0, makecol(0,0,liste_chateau->n->element->couleur*30)));
                            }
                            else
                            {
                                putpixel(temp, k, j, makecol(0,0, makecol(255,0,0)));
                            }
                        }
                    }
                }
            }
            if(liste_chateau!=NULL)
            {
                liste_chateau=liste_chateau->next;
            }
        }

        draw_sprite(liste_buffer->buffer_map, temp, (SCREEN_W/2-36)+(habitation->position.colonne-2)*14-habitation->position.ligne*14, (habitation->position.colonne-2)*8+habitation->position.ligne*8);
    }
    destroy_bitmap(temp);//a enlever si bug
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
                else if(type>=4 && type <=9)
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

void affichage_level_2(t_graphe* map, IMAGE* liste_image, BUFFER* liste_buffer)
{
    FILE* elementMap=fopen("element_map.txt", "r");//pour la nouvelle partie tout les elements sont à 0
    FILE* rotation_element_map=fopen("rotation_element_map.txt", "r");// la rotation indique dans quel sens on doit mettre le batiment sur la carte et donc la place qu'il prend
    FILE* map_elec=fopen("map_elec.txt", "r");//pour la nouvelle partie tout les elements sont à 0


    BITMAP* temp= create_bitmap(liste_image->batiment->w, liste_image->batiment->h);
    int reseau=0;
    int rgb, r, b;
    int type=0;
    int rotation=0;
    for(int i=0; i<NBLIGNE; i++)
    {
        for(int j=0; j<NBCOLONNE; j++)
        {
            fscanf(elementMap, "%d", &type);
            fscanf(rotation_element_map, "%d", &rotation);
            fscanf(map_elec, "%d", &reseau);
            if(reseau==1)
            {

                draw_sprite(liste_buffer->buffer_map, liste_image->reseau, (SCREEN_W/2-36)+j*14-i*14, j*8+i*8);
            }
            else
            {
                switch (type) {
                    case 1://route
                        draw_sprite(liste_buffer->buffer_map, liste_image->route, (SCREEN_W/2-36)+j*14-i*14, j*8+i*8);
                        break;
                    case 3://chateau eau
                        if(rotation==1)
                        {
                            draw_sprite(liste_buffer->buffer_map, liste_image->centrale, (SCREEN_W / 2 - 36) + (j - 3) * 14 - (i) * 14, (j - 3) * 8 + (i) * 8 - 8);//pq le -8? jsp j'ai tatonné
                        }
                        else if(rotation==-1)
                        {
                            draw_sprite_h_flip(liste_buffer->buffer_map, liste_image->centrale, (SCREEN_W/2-36)+(j-3)*14-(i)*14, (j-3)*8+(i)*8-8);
                        }
                        textprintf_ex(liste_buffer->buffer_map,font,(SCREEN_W/2-36)+(j-3)*14-(i)*14+35,(j-3)*8+(i)*8+26,makecol(255,255,255),-1,"%d/%d",map->grille[i][j]->element->capacite,5000);
                        break;
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                        if(map->grille[i][j]->element->alimente==0)
                        {
                            draw_sprite(liste_buffer->buffer_map, liste_image->batiment, (SCREEN_W/2-36)+(j-2)*14-i*14, (j-2)*8+i*8);
                        }
                        else
                        {
                            for(int k=0; k<temp->h; k++)
                            {
                                for(int l=0; l<temp->w; l++)
                                {
                                    rgb= getpixel(liste_image->batiment, l, k);
                                    r=getr(rgb);
                                    b=getb(rgb);
                                    if(r==255 && b==255)
                                    {
                                        putpixel(temp, l, k, rgb);
                                    }
                                    else
                                    {
                                        putpixel(temp, l, k, makecol(0,0, makecol(222,222,0)));
                                    }
                                }
                            }
                            draw_sprite(liste_buffer->buffer_map, temp, (SCREEN_W/2-36)+(j-2)*14-i*14, (j-2)*8+i*8);
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
    destroy_bitmap(temp);//a enlever si bug
    fclose(map_elec);
    fclose(rotation_element_map);
    fclose(elementMap);
}


void affichageTotal(t_graphe* map, IMAGE* liste_image, BUFFER* liste_buffer, t_pos souris, long compteur_argent, int niveau_visu, int capa_usine, clock_t CLK_debut, int compteur_hab,  int capa_eau, int screeny)//doit etre independant du jeu en lui meme mais affiche toute les données nécéssaire à l'utilisateur
{
    clear_bitmap(liste_buffer->buffer_menu);
    clear_bitmap(liste_buffer->buffer_map);


    blit(liste_image->barre_outils, liste_buffer->buffer_menu, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    textprintf_ex(liste_buffer->buffer_menu,font,19,605,makecol(0,0,0),-1,"%ld$",compteur_argent);
    textprintf_ex(liste_buffer->buffer_menu,font,560,658,makecol(0,0,0),-1,"%d",capa_usine);
    textprintf_ex(liste_buffer->buffer_menu,font,873,657,makecol(0,0,0),-1,"%d",capa_eau);
    textprintf_ex(liste_buffer->buffer_menu,font,237,658,makecol(0,0,0),-1,"%d",compteur_hab);
    textprintf_ex(liste_buffer->buffer_menu,font,47,73,makecol(0,0,0),-1,"%ld sec",clock()/CLOCKS_PER_SEC-CLK_debut);


    blit(liste_image->nuage,liste_buffer->buffer_map,0,screeny,0,0,1500,SCREEN_H);
    draw_sprite(liste_buffer->buffer_map, liste_image->map, 0, 0);

    textprintf_ex(liste_buffer->buffer_map,font,10,10,makecol(0,255,0),makecol(0,0,0),"%4d %4d",mouse_x,mouse_y);
    textprintf_ex(liste_buffer->buffer_map,font,10,20,makecol(0,255,0),makecol(0,0,0),"case[%d][%d]",souris.ligne,souris.colonne);
    textprintf_ex(liste_buffer->buffer_map,font,10,30,makecol(0,255,0),makecol(0,0,0),"niveau %d",niveau_visu);
    if(niveau_visu==0)
    {
        affichage_level_0(liste_buffer, liste_image, map);
    }
    else if (niveau_visu==1)
    {
        affichage_level_1(map, liste_image, liste_buffer);
    }
    else if(niveau_visu==2)
    {
        affichage_level_2(map, liste_image, liste_buffer);
    }

}

void affichage_info_menu(BUFFER* liste_buffer, IMAGE* liste_image)
{
    ///passer la souris sur la case compteur habitants pour afficher le nom de la case
    if(((mouse_x>=164)&&(mouse_x<=295)&&(mouse_y>=647)&&(mouse_y<=672)))
    {
        //textprintf_ex(liste_buffer->buffer_menu,font,10,20,makecol(0,0,0),makecol(255,255,255),"bouton selec");
        blit(liste_image->info_habitants, liste_buffer->buffer_menu,0, 0,  201, 654, 100, 13);
    }
    ///passer la souris sur la case compteur eau pour afficher le nom de la case
    if(((mouse_x>=798)&&(mouse_x<=931)&&(mouse_y>=648)&&(mouse_y<=672)))
    {
        blit(liste_image->info_eau, liste_buffer->buffer_menu,0, 0,  833, 649, 100, 13);
    }
    ///passer la souris sur la case compteur elec pour afficher le nom de la case
    if(((mouse_x>=482)&&(mouse_x<=613)&&(mouse_y>=647)&&(mouse_y<=672)))
    {
        blit(liste_image->info_elec, liste_buffer->buffer_menu,0, 0,  521, 651, 100, 13);
    }
    ///passer la souris sur la case compteur monaie pour afficher le nom de la case
    if(((mouse_x>=4)&&(mouse_x<=108)&&(mouse_y>=592)&&(mouse_y<=612)))
    {
        blit(liste_image->info_monnaie, liste_buffer->buffer_menu,0, 0,  15, 600, 100, 13);
    }
    ///passer la souris sur la case compteur habitants pour afficher le nom de la case
    if(((mouse_x>=288)&&(mouse_x<=420)&&(mouse_y>=671)&&(mouse_y<=702)))
    {
        blit(liste_image->info_habitants, liste_buffer->buffer_menu,0, 0,  288, 705, 100, 13);
    }
}


