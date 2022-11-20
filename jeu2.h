//
// Created by alexi on 08/11/2022.
//

#ifndef PROJET_TDG_SIMCITY_JEU2_H
#define PROJET_TDG_SIMCITY_JEU2_H


int calcul_depenses(int choix, int taille_chemin_route/*Algo Taille chemin*/);
int validation_depense(int depense, long argent_restant);

t_graphe* initialisation_distance_chateau (t_graphe* map, t_tile* case_chateau);
t_graphe* ecriture_fichier_eau(t_graphe* map, t_tile* case_arrive);
void maj_capacite(t_tile* chateau_eau, t_tile* maison);//tester avec ça sinon utiliser un double pointeur
t_graphe* dijkstra(t_graphe* map, t_tile* sommet_de_depart);
t_graphe* distribution_eau(t_graphe* map);
t_graphe* reinitialisation_eau(t_graphe* map);


#endif //PROJET_TDG_SIMCITY_JEU2_H
