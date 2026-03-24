#ifndef JEU_H
#define JEU_H

#include "map_manager.h"
#include "score.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NB_TIRS 5
#define TAILLE_TUILE 40
#define NB_LIGNES 14
#define NB_COLONNES 19
#define NB_ZOMBIES 100
#define NB_TOURS 8
#define VIVANT 1
#define MORT 0
#define ACTIVEE 1
#define DESACTIVEE 0
#define TIRE 2
#define EN_ATTENTE 1
#define DESACTIVE 0

#define TRUE 1
#define FALSE 0

typedef enum { LEVEL_ACTIVE, LEVEL_TRANSITION, LEVEL_REINIT } GameState;

typedef struct {
  GameState currentState;
  int player_score;
  int player_vies;
  int niveauActuel;
  int active_towers;
} GameEngine;

// Structs
typedef struct _TIR {
  int etat;
  SDL_Surface *surface;
  SDL_Rect rect;
  double angle;
  int portee;
  int xVel;
  int yVel;
} TIR;

typedef struct _TOUR {
  int etat; // activée ou désactivée
  SDL_Surface *surface;
  SDL_Rect rect;
  SDL_Rect aux;
  TIR tirs[NB_TIRS];
  double angle;
  int porteeTir;
  float xVel;
  float yVel;
  int cibleActuelle;
  int damage;
  int cooldown;
  int cooldownTimer;
  int coutUpgrade;
  int level;
} TOUR;

typedef struct _ZOMBIE {
  int etat; // mort ou vivant
  SDL_Surface *surface;
  SDL_Rect rect;
  SDL_Rect sprites[2];
  int xVel;
  int yVel;
  int image_actuelle;
  int resistance;
  int routeId;
} ZOMBIE;

typedef struct _SLOT_TOUR {
  int x;
  int y;
  int estOccupe;
  int typeTour;
  int indexTour;
} SLOT_TOUR;

typedef struct _LEVEL {
  int numeroNiveau;
  int nbZombiesBase;
  int hpZombieBase;
  int maxToursConstructibles;
  int nbChemins;
  int typeTourDebloquee;
  int nbSlotsTotal;
  SLOT_TOUR slots[25];
} LEVEL;

// Variables globales (déclarées en extern pour éviter les redéfinitions)
extern GameEngine engine;
extern SDL_Window *fenetre;
extern SDL_Surface *ecran;
extern SDL_Surface *fond;
extern SDL_Surface *fondMenu;
extern SDL_Surface *fondEntrerNom;
extern SDL_Event evenement;

extern SDL_Surface *fondScore;
extern SDL_Surface *texte1;
extern SDL_Surface *texte2;
extern SDL_Rect rectTexte1;
extern SDL_Rect rectTexte2;
extern TTF_Font *police;
extern SDL_Color couleurTexte;

extern SDL_Surface *selection;
extern SDL_Surface *souris;
extern SDL_Surface *vies;
extern SDL_Surface *nbVies_surf;
extern SDL_Surface *surfMorts;
extern SDL_Surface *surfScore;
extern SDL_Surface *fondGameOver;
extern SDL_Surface *fondInstructions;
extern SDL_Surface *gagne;
extern SDL_Surface *audio;
extern SDL_Surface *toursPlacees;
extern SDL_Surface *toursDisponibles;
extern SDL_Surface *barre;

// Surfaces tuiles décor
extern SDL_Surface *zero, *un, *deux, *trois, *quatre, *cinq, *six, *sept;
extern SDL_Surface *huit, *neuf, *dix, *onze, *douze, *treize, *quatorze;
extern SDL_Surface *griffe;
extern SDL_Surface *developpeurs;

extern SDL_Rect rectSelection;
extern SDL_Rect rectGriffe;
extern SDL_Rect champ[NB_LIGNES][NB_COLONNES];
extern SDL_Rect rectVies;
extern SDL_Rect rectNbVies;
extern SDL_Rect rectMorts;
extern SDL_Rect rectScore;
extern SDL_Rect rectAudio;
extern SDL_Rect rectTC;
extern SDL_Rect rectTD;
extern SDL_Rect rectBarre;

extern Mix_Music *musique;

// Variables de logique
extern int xZombie, yZombie;
extern int carte[NB_LIGNES][NB_COLONNES];
extern int tailleHorde;
extern int qteTours;
extern int nombreVies;
extern int zombiesMorts;
extern int score;
extern int resistance_globale;
extern int morts;
extern int n;
extern char *nomNiveau;
extern char nj[30];
extern char nv[4];
extern char pb[16];
extern char n_morts[4];
extern char vetScore[4];
extern char tc[4];
extern char td[4];

extern Liste *liste_scores; // Provient de score.h

extern ZOMBIE z1[NB_ZOMBIES];
extern TOUR t1[NB_TOURS];

extern LEVEL niveaux[10];
extern int niveauActuel;

// Prototypes
float distance(SDL_Rect, SDL_Rect);
int jeu();
int verifierClicSlot(SDL_Rect, LEVEL *, int *);
int records();
int aTour();
int afficherDeveloppeurs();
int insererNom();
int controlerAffichageTir(SDL_Rect);
int tirHorsEcran(SDL_Rect);
int collision(SDL_Rect, SDL_Rect);
int limitesSelection(SDL_Rect);
int tourSurChemin(SDL_Rect);
int sansZombie();
int gameOver();
int instructions();
void tirer();
void chargerNiveau(char *);
void controlerPositionTour(SDL_Rect);
void initialiserComposants();
void initialiserPolice();
void chargerSurfaces();
void initialiserNiveaux();
void choisirNiveau(int);
void initialiserZombie(int);
void initialiserEtatTours();
void initialiserEtatTirs();
void initialiserEtatZombies();
void initialiserTour(int, int, int);
void marcherZombie2();
void controlerImage();
void afficherZombies();
void afficherTours();
void ciblerZombies();
void chargerSprite();
void tuerZombie(ZOMBIE *);
void play_level_transition(int);
void initialiserTir();
void verifierVies();
void augmenterResistance();
TTF_Font *chargerPolice(const char *, int);
void upgrade_tower(TOUR *t);
void autoTile();

#endif
