#include "jeu.h"

GameEngine engine = {LEVEL_ACTIVE, 0, 0, 0, 0};

SDL_Window  *fenetre = NULL;
SDL_Surface *ecran = NULL;
SDL_Surface *fond = NULL;
SDL_Surface *fondMenu = NULL;
SDL_Surface *fondEntrerNom = NULL;
SDL_Event evenement;

SDL_Surface *fondScore = NULL;
SDL_Surface *texte1 = NULL;
SDL_Surface *texte2 = NULL;
SDL_Rect rectTexte1;
SDL_Rect rectTexte2;
TTF_Font *police = NULL;
SDL_Color couleurTexte;

SDL_Surface *selection = NULL;
SDL_Surface *souris = NULL;
SDL_Surface *vies = NULL;
SDL_Surface *nbVies_surf = NULL;
SDL_Surface *surfMorts = NULL;
SDL_Surface *surfScore = NULL;
SDL_Surface *fondGameOver = NULL;
SDL_Surface *fondInstructions = NULL;
SDL_Surface *gagne = NULL;
SDL_Surface *audio = NULL;
SDL_Surface *toursPlacees = NULL;
SDL_Surface *toursDisponibles = NULL;
SDL_Surface *barre = NULL;

SDL_Surface *zero=NULL, *un=NULL, *deux=NULL, *trois=NULL, *quatre=NULL;
SDL_Surface *cinq=NULL, *six=NULL, *sept=NULL, *huit=NULL, *neuf=NULL;
SDL_Surface *dix=NULL, *onze=NULL, *douze=NULL, *treize=NULL, *quatorze=NULL;
SDL_Surface *griffe=NULL;
SDL_Surface *developpeurs=NULL;

SDL_Rect rectSelection;
SDL_Rect rectGriffe;
SDL_Rect champ[NB_LIGNES][NB_COLONNES];
SDL_Rect rectVies;
SDL_Rect rectNbVies;
SDL_Rect rectMorts;
SDL_Rect rectScore;
SDL_Rect rectAudio;
SDL_Rect rectTC;
SDL_Rect rectTD;
SDL_Rect rectBarre;

Mix_Music *musique = NULL;

int xZombie, yZombie;
int carte[NB_LIGNES][NB_COLONNES];
int tailleHorde = 0;
int qteTours = 0;
int nombreVies = 6;
int zombiesMorts = 0;
int score = 0;
int resistance_globale = 0;
int morts = 0;
int n = 1;
char *nomNiveau;
char nj[30];
char nv[4];
char pb[16];
char n_morts[4];
char vetScore[4];
char tc[4];
char td[4];

Liste *liste_scores = NULL;

ZOMBIE z1[NB_ZOMBIES];
TOUR t1[NB_TOURS];
LEVEL niveaux[10];
int niveauActuel = 0;
