//
// Created by alexi on 02/11/2022.
//

#include "listeRelated.h"

t_liste *creer(void)
{
    return NULL;
}

t_liste *insererNoeud(t_liste *liste, t_tile *n) //inserer noeud debut de la liste dans la liste
{
    t_liste *nouv = (t_liste *)malloc(sizeof(t_liste));
    nouv->n = n;

    nouv->next = liste;
    nouv->prev=NULL;
    if(liste!=NULL)
    {
        liste->prev=nouv;
    }
    return nouv;
}

t_liste *enleverNoeud(t_liste *liste, t_tile *n)
{
    t_liste *actuel, *prev;
    actuel = liste;

    if (liste == NULL)// si la liste est vide on retourne NULL
    {
        return NULL;
    }

    prev = NULL;
    while (actuel)
    {
        if (actuel->n->position.colonne == n->position.colonne && actuel->n->position.ligne == n->position.ligne)
        {
            if (prev == NULL)
            {
                // On supprime un element de début de liste
                liste = liste->next; // on actualise le début de la liste
                free(actuel);
                actuel = liste;
                continue;
            } else
            {
                //Ou un element de milieu ou fin de liste
                prev->next = actuel->next;
                free(actuel);
                actuel = prev;
                continue;
            }
        }
        // On passe a l'element suivant
        prev = actuel;
        actuel = actuel->next;
    }
    return liste;
}

int existe(t_liste *l, t_tile *n)
{
    //printf("entree dans existe\n");
    t_liste *aux = l;
    while (aux != NULL)
    {

        if ((aux->n->position.colonne == n->position.colonne) && (aux->n->position.ligne == n->position.ligne))
        {
            return 1;
        }
        aux = aux->next;
    }
    return 0;
}


/*t_tile* rechercherPlusPetit(t_liste *l)
{
    t_liste *aux = l->next;
    double petitF = l->n->f;//on récupère la première valeur
    t_tile *petit = l->n;//on récupère le noeud concerné
    while (aux != NULL)
    {
        if (aux->n->f < petitF)
        {
            petitF = aux->n->f;//pour récupérer la valeur
            petit = aux->n;//pour récupérer le noeud
        }
        aux = aux->next;
    }
    return petit;//on retourne finalement le plus petit noeud de la liste
}*/

t_liste* insertion_en_triant(t_liste* liste, t_tile* noeud)//pour file de priorité
{
    t_liste* nouv=(t_liste*)malloc(sizeof(t_liste));
    nouv->n=noeud;
    if(estVide(liste))
    {
        nouv->next=liste;
        nouv->prev=NULL;
        return nouv;
    }
    else
    {
        t_liste* temp=liste;
        if((nouv->n->f)<(temp->n->f))//insertion en début de liste
        {
            nouv->next=temp;
            nouv->prev=NULL;
            temp->prev=nouv;
            return nouv;
        }
        else
        {
            while(temp->next!=NULL)//on recherche l'emplacement où mettre le maillon
            {
                temp=temp->next;//vérifier le bon fonctionnement
                if((nouv->n->f)<(temp->n->f))//insertion en mileu de liste
                {
                    nouv->next=temp;
                    nouv->prev=temp->prev;
                    temp->prev->next=nouv;
                    temp->prev=nouv;
                    return liste;
                }
            }
            //insertion en fin de liste
            temp->next=nouv;
            nouv->prev=temp;
            nouv->next=NULL;
            return liste;
        }
    }
}

t_liste* enlever_noeud_debut(t_liste* liste, t_tile** noeud_a_conserver)//enlever en debut + mettre l'adresse du noeud à conserveer
{
    t_liste* temp=liste;
    *noeud_a_conserver=temp->n;
    liste=liste->next;
    free(temp);
    return liste;
}

t_liste* actualisation(t_liste* l, t_tile* noeud_a_retrier)//si jamais la valeur f du noeud change
{
    t_liste* temp=l;
    //on doit sortir le noeud de la liste après l'avoir trouvé
    while(temp->n!=noeud_a_retrier)
    {
        temp=temp->next;
    }
    temp->prev->next=temp->next;
    if(temp->next!=NULL)
    {
        temp->next->prev=temp->prev;
    }

    //et on le retrie ensuite

    l= insertion_en_triant(l, noeud_a_retrier);

    return l;
}


int taille(t_liste *l)
{
    int cont = 0;
    t_liste *aux = NULL;
    for (aux = l; aux != NULL; aux = aux->next)
    {
        cont++;
    }
    return cont;
}

int tailleChemin(t_tile* n)
{
    int taille=0;
    t_tile* aux = n;
    while(aux->parent!=NULL)
    {
        taille++;
        aux=aux->parent;
    }
    return taille;
}

int estVide(t_liste *l)
{
    if( l == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void afficherListe(t_liste *l)//servira pour débugger
{
    t_liste *aux = l;
    while (aux != NULL)
    {
        printf("case [%d][%d]\n", aux->n->position.ligne, aux->n->position.colonne);
        aux = aux->next;
    }
    printf("\n\n");
}

void liberer(t_liste *l)
{
    t_liste *aux;
    while (l != NULL)
    {
        aux = l->next;
        free(l);
        l = aux;
    }
}