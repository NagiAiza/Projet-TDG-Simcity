# Projet-TDG-Simcity

tout est dans la branche master actuellement, faudra que je regarde comment on merge dans le main


structures utilisés, modulable à souhait, juste mettez ici quand vous modifiez et prévenez

typedef struct pos{
    int ligne;
    int colonne;
}t_pos;

typedef struct tile{//pour faire la map on fait un tableau à 2 dimensions de tile
    int element;//quel element est dessus (maison, chateau d'eau, centrale etc)
    int connexite;
    t_pos position;

    //pour algo A*
    int g;
    int h;
    int f;

    struct tile* parent;
    struct liste* voisin;//on initialise les voisins de chaque case en début de programme
}t_tile;



typedef struct route{
    int tailleX, tailleY;
}t_route;

typedef struct habitation{
    int tailleX, tailleY;
    int type;
}t_habitation;

typedef struct liste
{
    t_tile* n;
    struct liste* next;
    struct liste* prev;
}t_liste;
