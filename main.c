#include <stdio.h>
#include <allegro.h>
#include "menu.h"
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
    //variable qui reçoit le mode souhaite
    int Mode_de_Jeu;

    ///importation de la musique de fond
    SAMPLE* music=load_sample("musique.wav");
    int volume=10; //40=max
    if(music==NULL){
        printf("Erreur le dossier << musique >> (format .wav) n'a pas ete trouvee");
        volume = 0;
    }

    initialisation_allegro();


    //Sous-programme menu
    //Mode_de_Jeu = menu(); //si 1 communiste si 2 capitaliste si 0 nada
    ecran_victoire_saboteur(music, volume);
    //Sous-programme jeu
    jeu();
    allegro_exit();
    return 0;
}END_OF_MAIN();

//il reste (partie placement des batiments):
//2.2/si chevauchement changer la couleur du sprite en rouge ou teinte de rouge
//3/ gérer la création de la structure de données nouvellement posé

