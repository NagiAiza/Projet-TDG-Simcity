#include <stdio.h>
#include <allegro.h>
#include "moteur_du_jeu.h"



void initialisation_allegro() {    ///Initialisation pour Allegro
    allegro_init();
    install_keyboard();
    install_mouse();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);

    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,1024,678,0,0)!=0) {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
}

int main() {
    initialisation_allegro();
    jeu();
    allegro_exit();
    return 0;
}END_OF_MAIN();

//il reste (partie placement des batiments):
// 1/ faire les batiments de taille 4x6
// 2/ gérer le chevauchement des batiments
// 3/ gérer la création de la structure de données nouvellement posé
