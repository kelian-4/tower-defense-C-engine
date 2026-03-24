#include "jeu.h"
#include "map_manager.h"

/*
 * render_map — Affiche la carte selon la matrice carte[NB_LIGNES][NB_COLONNES].
 *
 * GUIDE DE MODIFICATION DES IDs
 * Modifiez les case ci-dessous pour changer l'image associée à un ID :
 *   ID 0  : Herbe / sol de base       -> zero
 *   ID 1  : Route horizontale droite  -> un
 *   ID 2  : Route verticale droite    -> deux
 *   ID 3  : Virage bas-droite         -> trois
 *   ID 4  : Virage bas-gauche         -> quatre
 *   ID 5  : Virage haut-droite        -> cinq
 *   ID 6  : Virage haut-gauche        -> six
 *   ID 7  : Arbre                     -> sept
 *   ID 8  : Bordure haute A           -> huit
 *   ID 9  : Bordure haute B           -> neuf
 *   ID 10 : Bordure haute C           -> dix
 *   ID 11 : Bordure haute D           -> onze
 *   ID 12 : Rocher                    -> douze
 *   ID 13 : Buisson                   -> treize
 *   ID 14 : Herbes hautes             -> quatorze
 */
void render_map(int levelID) {
    (void)levelID;
    for (int i = 0; i < NB_LIGNES; i++) {
        for (int j = 0; j < NB_COLONNES; j++) {
            champ[i][j].x = j * TAILLE_TUILE;
            champ[i][j].y = i * TAILLE_TUILE;
            champ[i][j].w = TAILLE_TUILE;
            champ[i][j].h = TAILLE_TUILE;

            SDL_Surface *s = zero;
            switch (carte[i][j]) {
                case 0:  s = zero;      break;
                case 1:  s = un;        break;
                case 2:  s = deux;      break;
                case 3:  s = trois;     break;
                case 4:  s = quatre;    break;
                case 5:  s = cinq;      break;
                case 6:  s = six;       break;
                case 7:  s = sept;      break;
                case 8:  s = huit;      break;
                case 9:  s = neuf;      break;
                case 10: s = dix;       break;
                case 11: s = onze;      break;
                case 12: s = douze;     break;
                case 13: s = treize;    break;
                case 14: s = quatorze;  break;
                default: s = zero;      break;
            }
            if (s)
                SDL_BlitSurface(s, NULL, ecran, &champ[i][j]);
        }
    }
}
