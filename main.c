#include "jeu.h"

int main(void) {
    initialiserComposants();
    initialiserPolice();
    chargerSurfaces();
    choisirNiveau(1);
    chargerNiveau(nomNiveau);

    // Initialisation liste chainée (fonctions de score.h)
    liste_scores = creerListe();
    chargerListe("ranking.txt", liste_scores);

    int boucle = 0;
    int activerGriffe = 0;

    while (boucle == 0) { // Boucle principale du menu
        while (SDL_PollEvent(&evenement)) {
            if(evenement.type == SDL_QUIT) {
                boucle = 1;
            } else if(evenement.type == SDL_MOUSEMOTION) {
                if((evenement.motion.x >= 535 && evenement.motion.x <= 670) && (evenement.motion.y >= 240 && evenement.motion.y <= 290)) {
                    activerGriffe = 1; rectGriffe.x = 465; rectGriffe.y = 225;
                } else if((evenement.motion.x >= 535 && evenement.motion.x <= 741) && (evenement.motion.y >= 316 && evenement.motion.y <= 365)) {
                    activerGriffe = 1; rectGriffe.x = 465; rectGriffe.y = 300;
                } else if((evenement.motion.x >= 535 && evenement.motion.x <= 844) && (evenement.motion.y >= 390 && evenement.motion.y <= 438)) {
                    activerGriffe = 1; rectGriffe.x = 465; rectGriffe.y = 375;
                } else if((evenement.motion.x >= 535 && evenement.motion.x <= 693) && (evenement.motion.y >= 470 && evenement.motion.y <= 518)) {
                    activerGriffe = 1; rectGriffe.x = 465; rectGriffe.y = 455;
                } else if((evenement.motion.x >= 535 && evenement.motion.x <= 613) && (evenement.motion.y >= 559 && evenement.motion.y <= 600)) {
                    activerGriffe = 1; rectGriffe.x = 465; rectGriffe.y = 540;
                } else {
                    activerGriffe = 0;
                }
            } else if(evenement.type == SDL_MOUSEBUTTONDOWN) {
                if((evenement.button.x >= 535 && evenement.button.x <= 670) && (evenement.button.y >= 240 && evenement.button.y <= 290)) {
                    if(!insererNom()) boucle = 1;
                    else if(!jeu()) boucle = 1;

                    nombreVies = 6;
                    zombiesMorts = 0;
                    tailleHorde = 5;
                    morts = 0;
                    score = 0;
                } else if((evenement.button.x >= 535 && evenement.button.x <= 741) && (evenement.button.y >= 316 && evenement.button.y <= 365)) {
                    if(!instructions()) boucle = 1;
                } else if((evenement.button.x >= 535 && evenement.button.x <= 844) && (evenement.button.y >= 390 && evenement.button.y <= 438)) {
                    if(!afficherDeveloppeurs()) boucle = 1;
                } else if((evenement.button.x >= 535 && evenement.button.x <= 693) && (evenement.button.y >= 470 && evenement.button.y <= 518)) {
                    if(!records()) boucle = 1;
                } else if((evenement.button.x >= 535 && evenement.button.x <= 613) && (evenement.button.y >= 559 && evenement.button.y <= 600)) {
                    boucle = 1;
                }
            }
        }

        SDL_BlitSurface(fondMenu, NULL, ecran, NULL);
        if(activerGriffe) SDL_BlitSurface(griffe, NULL, ecran, &rectGriffe);
        SDL_UpdateWindowSurface(fenetre);
    }

    sauvegarderListe("ranking.txt", liste_scores);
    supprimerListe(liste_scores);

    SDL_DestroyWindow(fenetre);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
