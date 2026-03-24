#ifndef PATHFINDER_H
#define PATHFINDER_H

typedef struct {
    int x; // colonne
    int y; // ligne
} PathPoint;

extern PathPoint current_path[500];
extern int current_path_len;

void generer_chemin(int niveauID);

#endif
