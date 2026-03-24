#ifndef SCORE_H
#define SCORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLASSEMENT 10
#define TRUE 1
#define FALSE 0

// Structures
typedef struct _Joueur {
	char *nom;
	int score;
} Joueur;

typedef struct _Noeud {
	Joueur *joueur;
	struct _Noeud *suivant;
} Noeud;

typedef struct _Liste {
	Noeud *debut;
	Noeud *fin;
	int taille;
} Liste;

// Prototypes des fonctions (pas de code ici !)
Liste *creerListe();
int estVide(Liste *liste);
Joueur* creerJoueur(char *nom, int score);
int supprimerFin(Liste *liste);
int insererFin(Liste *liste, Joueur *joueur);
int insererOrdonne(Liste *liste, Joueur *joueur);
void afficherListe(Liste *liste);
void chargerListe(char nomFichier[], Liste* liste);
int sauvegarderListe(char nomFichier[], Liste *liste);
void supprimerNoeud(Noeud *n);
void supprimerListe(Liste *liste);

#endif
