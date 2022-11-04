#include <stdio.h>
#include <allegro.h>
#include "Affichage.h"


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

int main() {//il n'y aura rien dans le main, tout se fait dans les sous prgm
    initialisation_allegro();

    t_graphe* map=makeGrid();

    affichageTotal(map);
    allegro_exit();
    return 0;
}END_OF_MAIN();
