#include "score.h"

Liste *creerListe() {
	Liste *liste = (Liste *) malloc(sizeof(Liste));
	if(liste != NULL) {
		liste->debut = NULL;
		liste->fin = NULL;
		liste->taille = 0;
	}
	return liste;
}

int estVide(Liste *liste) {
	if(liste->taille == 0) return TRUE;
	else return FALSE;
}

Joueur* creerJoueur(char nom[], int score) {
	Joueur *joueur = (Joueur *) malloc(sizeof(Joueur));
	if(joueur != NULL) {
		joueur->nom = malloc(60 * sizeof(char));
		strcpy(joueur->nom, nom);
		joueur->score = score;
		return joueur;
	} else {
		printf("Impossible d'allouer de la mémoire\n");
		return NULL;
	}
}

int supprimerFin(Liste *liste) {
	Noeud *prec = liste->debut;
	Noeud *aux = liste->debut;
	if(!estVide(liste)) {
		while(aux->suivant != NULL) {
			prec = aux;
			aux = aux->suivant;
		}
		prec->suivant = NULL;
		liste->fin = prec; 
		liste->taille--;
		return TRUE;
	} else {
		return FALSE;
	}
}

int insererFin(Liste *liste, Joueur *joueur) {
	Noeud *pnouveau = (Noeud*) malloc(sizeof(Noeud));
	if(pnouveau != NULL) {
		pnouveau->joueur = joueur;
		pnouveau->suivant = NULL;
		if(liste->debut == NULL) {
			liste->debut = pnouveau;		
		} else {
			liste->fin->suivant = pnouveau;
		}
		liste->fin = pnouveau;
		liste->taille++;
	    return TRUE;
	} else {
		return FALSE;
	}	
}

int insererOrdonne(Liste *liste, Joueur *joueur) {
	Noeud *pnouveau = (Noeud*) malloc(sizeof(Noeud));
	if(pnouveau != NULL) {
		pnouveau->joueur = joueur;
		if(estVide(liste)) {
			insererFin(liste, joueur);
		} else if(liste->debut->joueur->score < pnouveau->joueur->score) {
			Noeud *aux = liste->debut;
			aux->suivant = liste->debut->suivant;
			pnouveau->suivant = aux;
			liste->debut = pnouveau;
			liste->taille++;
		} else {
			Noeud *aux1 = liste->debut;
			Noeud *aux2 = liste->debut->suivant;
			while(aux2 != NULL && aux2->joueur->score > pnouveau->joueur->score) {
				aux1 = aux2;
				aux2 = aux2->suivant;
			}
			pnouveau->suivant = aux2;
			aux1->suivant = pnouveau;
			liste->taille++;
		}
		if(liste->taille > CLASSEMENT) {
			supprimerFin(liste);
		}
	    return TRUE;
	} else {
		return FALSE;
	}
}

void afficherListe(Liste *liste) {
	Noeud *paux = liste->debut;
	if(!estVide(liste)) {
		while(paux != NULL) {
			printf("Nom: %s || Score: %d\n", paux->joueur->nom, paux->joueur->score);
			paux = paux->suivant;	
		}
	}
}

void chargerListe(char nomFichier[], Liste* liste) {
	FILE *fichier = fopen(nomFichier, "r");
	if(fichier == NULL) {
		printf("Fichier introuvable\n");
	} else {
		char nom[30];
		int score;
		while(!feof(fichier)) {
			if(fscanf(fichier, "%s", nom) != EOF) {
				fscanf(fichier, "%d", &score);
				Joueur *joueur = creerJoueur(nom, score);
				insererFin(liste, joueur);
			} else {
				break;
			}
		}
	}
}

int sauvegarderListe(char nomFichier[], Liste *liste) {
	Noeud *aux = liste->debut;
	FILE *fichier = fopen(nomFichier, "w");
	if(fichier == NULL) {
		printf("Problème avec le fichier !\n");
		return FALSE;
	}
	while(aux != NULL) {
		fprintf(fichier, "%s\t%d\n", aux->joueur->nom, aux->joueur->score);
		aux = aux->suivant;
	}
	fclose(fichier);
	return TRUE;
}

void supprimerNoeud(Noeud *n) {
	free(n->joueur->nom);
	free(n->joueur);
	free(n);
}

void supprimerListe(Liste *liste) {
	Noeud *aux;
	Noeud *supprimer = liste->debut;
	while(liste->taille > 0) {
		aux = supprimer->suivant;
		supprimerNoeud(supprimer);
		supprimer = aux;
		liste->taille = liste->taille - 1;
	}
	free(liste);
}
