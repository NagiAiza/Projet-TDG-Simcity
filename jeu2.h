//
// Created by alexi on 08/11/2022.
//

#ifndef PROJET_TDG_SIMCITY_JEU2_H
#define PROJET_TDG_SIMCITY_JEU2_H

#include "Affichage.h"
//j
int calcul_depenses(int choix, int taille_chemin_route/*Algo Taille chemin*/);
int validation_depense(int depense, long argent_restant);

t_graphe* initialisation_distance_chateau (t_graphe* map, t_tile* case_chateau);
t_graphe* ecriture_fichier_eau(t_graphe* map, t_tile* case_arrive);
void maj_capacite(t_tile* chateau_eau, t_tile* maison);//tester avec ça sinon utiliser un double pointeur
t_graphe* dijkstra(t_graphe* map, t_tile* sommet_de_depart);
t_graphe* distribution_eau(t_graphe* map);
t_graphe* reinitialisation_eau(t_graphe* map);

t_graphe* electricite(t_graphe* map, int* capa_usine);
int verification_connexite_route(t_graphe* map, t_tile* case_actu);

int validation_evolution_communiste(t_tile* batiment, int* nb_habitant, int compteur_eau);
int validation_evolution_capitaliste(t_graphe* map, t_tile* batiment, int* nb_habitant);//evolue forcément peut importe les conditions
t_graphe* cycle_habitation(t_graphe* map, int* capa_usine, long* compteur_argent, int* nb_habitant, int compteur_eau, BUFFER* liste_buffer, IMAGE* liste_image, int* attente, int mode, int temps_ancienne_partie);

t_graphe* ecriture_fichier_elec(t_graphe* map, t_tile* case_arrive);
t_graphe* BFS(t_graphe* map, t_tile* sommet_depart);
t_graphe* distribution_elec(t_graphe* map);

t_graphe* remise_0_argent(t_graphe* map, t_pos souris);


int compte_eau(t_graphe* map);

#endif //PROJET_TDG_SIMCITY_JEU2_H
