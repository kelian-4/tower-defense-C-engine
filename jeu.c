#include "jeu.h"

SDL_Surface *load_image_map(const char *file)
{
  char buf[150];
  int m_id = (niveauActuel / 3) + 1;
  if (m_id > 3)
    m_id = 3;
  sprintf(buf, "assets/Maps/map%d/%s", m_id, file);
  return IMG_Load(buf);
}

void reload_map_textures()
{
  if (fond)
    SDL_FreeSurface(fond);
  fond = load_image_map("fundo3.png");
  if (zero)
    SDL_FreeSurface(zero);
  zero = load_image_map("0.png");
  if (un)
    SDL_FreeSurface(un);
  un = load_image_map("1.png");
  if (deux)
    SDL_FreeSurface(deux);
  deux = load_image_map("2.png");
  if (trois)
    SDL_FreeSurface(trois);
  trois = load_image_map("3.png");
  if (quatre)
    SDL_FreeSurface(quatre);
  quatre = load_image_map("4.png");
  if (cinq)
    SDL_FreeSurface(cinq);
  cinq = load_image_map("5.png");
  if (six)
    SDL_FreeSurface(six);
  six = load_image_map("6.png");
  if (sept)
    SDL_FreeSurface(sept);
  sept = load_image_map("7.png");
  if (huit)
    SDL_FreeSurface(huit);
  huit = load_image_map("8.png");
  if (neuf)
    SDL_FreeSurface(neuf);
  neuf = load_image_map("9.png");
  if (dix)
    SDL_FreeSurface(dix);
  dix = load_image_map("10.png");
  if (onze)
    SDL_FreeSurface(onze);
  onze = load_image_map("11.png");
  if (douze)
    SDL_FreeSurface(douze);
  douze = load_image_map("12.png");
  if (treize)
    SDL_FreeSurface(treize);
  treize = load_image_map("13.png");
  if (quatorze)
    SDL_FreeSurface(quatorze);
  quatorze = load_image_map("14.png");

  for (int i = 0; i < NB_ZOMBIES; i++)
  {
    if (z1[i].surface)
      SDL_FreeSurface(z1[i].surface);
    z1[i].surface = load_image_map("z1.png");
  }
  for (int i = 0; i < NB_TOURS; i++)
  {
    if (t1[i].surface)
      SDL_FreeSurface(t1[i].surface);
    t1[i].surface = load_image_map("torre.png");
    for (int j = 0; j < NB_TIRS; j++)
    {
      if (t1[i].tirs[j].surface)
        SDL_FreeSurface(t1[i].tirs[j].surface);
      t1[i].tirs[j].surface = load_image_map("tiro1.png");
    }
  }
}

/*
 * reset_level_entities — Réinitialise toutes les entités du niveau.
 * Détruit toutes les tours, projectiles et zombies.
 * Conserve : score, vies, niveauActuel.
 */
static void reset_level_entities()
{
  /* 1. Désactiver toutes les tours et leurs projectiles */
  for (int k = 0; k < NB_TOURS; k++)
  {
    t1[k].etat = DESACTIVEE;
    t1[k].cibleActuelle = -1;
    t1[k].porteeTir = 150;
    for (int j = 0; j < NB_TIRS; j++)
      t1[k].tirs[j].etat = EN_ATTENTE;
  }
  engine.active_towers = 0;

  /* 2. Marquer tous les zombies comme morts */
  for (int k = 0; k < NB_ZOMBIES; k++)
  {
    z1[k].etat = MORT;
    z1[k].resistance = 0;
  }

  /* 3. Libérer les slots du niveau courant */
  if (niveauActuel >= 0 && niveauActuel < 10)
  {
    for (int s = 0; s < niveaux[niveauActuel].nbSlotsTotal; s++)
    {
      niveaux[niveauActuel].slots[s].estOccupe = 0;
      niveaux[niveauActuel].slots[s].indexTour = -1;
    }
  }
}

int jeu()
{
  int boucle = 0;
  int i = 0;

  engine.currentState = LEVEL_ACTIVE;
  engine.player_score = score;
  engine.player_vies = nombreVies;
  engine.niveauActuel = 0;
  engine.active_towers = 0;
  niveauActuel = 0;

  initialiserNiveaux();
  tailleHorde = niveaux[niveauActuel].nbZombiesBase;
  qteTours = niveaux[niveauActuel].maxToursConstructibles;

  chargerSprite();
  initialiserEtatZombies();
  initialiserEtatTours();
  initialiserEtatTirs();

  int placerHorde = 0, pauseHorde = 0;
  int activerSelection = 0, mute = 0;
  int spawnTimer = 0; /* Timer indépendant pour l'apparition des zombies */

  rectNbVies.x = 90;
  rectNbVies.y = 603;
  rectMorts.x = 245;
  rectMorts.y = 603;
  rectScore.x = 375;
  rectScore.y = 603;
  rectAudio.x = 793;
  rectAudio.y = 412;
  rectTC.x = 610;
  rectTC.y = 603;
  rectBarre.x = 640;
  rectBarre.y = 603;
  rectTD.x = 660;
  rectTD.y = 603;

  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
  musique = Mix_LoadMUS("Musica/thriller.mp3");
  Mix_PlayMusic(musique, -1);

  SDL_Rect rectSouris;
  int x, y;

  while (boucle == 0)
  {
    if (engine.currentState == LEVEL_TRANSITION)
    {
      play_level_transition(engine.niveauActuel);
      engine.currentState = LEVEL_REINIT;
      continue;
    }
    else if (engine.currentState == LEVEL_REINIT)
    {
      if (engine.niveauActuel < 9)
      {
        /* Passage au niveau suivant (niveaux 1 à 9, index 0 à 8) */
        engine.niveauActuel++;
        niveauActuel = engine.niveauActuel;

        /* Sécurité : ne jamais dépasser l'index 9 du tableau niveaux[10] */
        if (niveauActuel > 9)
          niveauActuel = 9;

        tailleHorde = niveaux[niveauActuel].nbZombiesBase;
        qteTours = niveaux[niveauActuel].maxToursConstructibles;
        resistance_globale = niveaux[niveauActuel].hpZombieBase;

        /* Réinitialisation complète des entités — page blanche */
        reset_level_entities();
        i = 0;
        spawnTimer = 0;
        placerHorde = 0;
        pauseHorde = 0;

        /* Changement de map tous les 3 niveaux (niveaux 4, 7, 10) */
        if (niveauActuel % 3 == 0)
        {
          reload_map_textures();
          chargerNiveau(nomNiveau);
        }
      }
      else
      {
        /* Niveau 9 (dernier) terminé → écran de victoire finale */
        Mix_PauseMusic();
        SDL_BlitSurface(gagne, NULL, ecran, NULL);
        SDL_UpdateWindowSurface(fenetre);
        SDL_Delay(3000);
        Joueur *j_win = creerJoueur(nj, score);
        insererOrdonne(liste_scores, j_win);
        boucle = 1;
      }
      engine.currentState = LEVEL_ACTIVE;
      continue;
    }
    if (SDL_PollEvent(&evenement))
    {
      if (evenement.type == SDL_QUIT)
      {
        return 0;
      }
      else if (evenement.type == SDL_MOUSEMOTION)
      {
        if (activerSelection)
        {
          SDL_GetMouseState(&x, &y);
          rectSouris.x = x;
          rectSouris.y = y;
        }
      }
      else if (evenement.type == SDL_MOUSEBUTTONDOWN)
      {
        if ((evenement.button.x >= 779 && evenement.button.x <= 858) &&
            (evenement.button.y >= 30 && evenement.button.y <= 70))
        {
          placerHorde = 1; // Commencer
        }
        else if ((evenement.button.x >= 779 && evenement.button.x <= 858) &&
                 (evenement.button.y >= 79 && evenement.button.y <= 119))
        {
          placerHorde = 0;
          pauseHorde = 1; // Pausar
        }
        else if ((evenement.button.x >= 779 && evenement.button.x <= 858) &&
                 (evenement.button.y >= 129 && evenement.button.y <= 179))
        {
          Joueur *j1 = creerJoueur(nj, score);
          insererOrdonne(liste_scores, j1);
          boucle = 1;
          Mix_PauseMusic(); // Quitter
        }
        else if ((evenement.button.x >= 770 && evenement.button.x <= 810) &&
                 (evenement.button.y >= 210 && evenement.button.y <= 240))
        {
          rectSelection.x = 770;
          rectSelection.y = 200;
          activerSelection = TRUE;
        }
        else if ((evenement.button.x >= 0 && evenement.button.x <= 760) &&
                 (evenement.button.y >= 0 && evenement.button.y <= 600) &&
                 (activerSelection == TRUE))
        {
          int indexSlot = -1;
          if (verifierClicSlot(rectSouris, &niveaux[niveauActuel],
                               &indexSlot))
          {
            int slotX = niveaux[niveauActuel].slots[indexSlot].x;
            int slotY = niveaux[niveauActuel].slots[indexSlot].y;

            if (niveaux[niveauActuel].slots[indexSlot].estOccupe == 0)
            {
              if (engine.active_towers <
                  niveaux[niveauActuel].maxToursConstructibles)
              {
                initialiserTour(engine.active_towers, slotX, slotY);
                niveaux[niveauActuel].slots[indexSlot].estOccupe = 1;
                niveaux[niveauActuel].slots[indexSlot].indexTour =
                    engine.active_towers;
                engine.active_towers++;
              }
            }
          }
          else
          {
            // Upgrade Check
            for (int k = 0; k < niveaux[niveauActuel].nbSlotsTotal; k++)
            {
              SDL_Rect rectSlot = {niveaux[niveauActuel].slots[k].x,
                                   niveaux[niveauActuel].slots[k].y,
                                   TAILLE_TUILE + 10, TAILLE_TUILE + 10};
              if (collision(rectSouris, rectSlot) &&
                  niveaux[niveauActuel].slots[k].estOccupe == 1)
              {
                upgrade_tower(&t1[niveaux[niveauActuel].slots[k].indexTour]);
                break;
              }
            }
          }
          activerSelection = FALSE;
        }
        else if ((evenement.button.x >= 790 && evenement.button.x <= 842) &&
                 (evenement.button.y >= 420 && evenement.button.y <= 460))
        {
          if (mute == 0)
          {
            mute = 1;
            Mix_PauseMusic();
          }
          else
          {
            mute = 0;
            Mix_ResumeMusic();
          }
        }
      }
    }

    SDL_BlitSurface(fond, NULL, ecran, NULL);

    if (i < tailleHorde && placerHorde)
    {
      if (i == 0 || spawnTimer >= 60)
      {
        initialiserZombie(i);
        i++;
        spawnTimer = 0;
      }
      else
      {
        spawnTimer++;
      }
    }

    render_map(niveauActuel);
    afficherTours();

    /* Les zombies marchent et sont affichés dès que la vague est lancée,
     * indépendamment des tours. Le tir des tours est conditionnel. */
    if (placerHorde)
    {
      if (pauseHorde == 0)
      {
        marcherZombie2();
        controlerImage();
      }
      afficherZombies();

      if (aTour())
      {
        ciblerZombies();
        initialiserTir();
        tirer();
        tuerZombie(z1);
      }

      if (sansZombie() && i == tailleHorde)
      {
        printf("[DEBUG] Vague terminee ! Niveau %d passe.\n", niveauActuel);
        if (engine.niveauActuel < 9)
        {
          engine.currentState = LEVEL_TRANSITION;
        }
      }
      verifierVies();
    }

    if (pauseHorde && !placerHorde)
    {
      afficherZombies();
    }

    if (activerSelection)
    {
      SDL_BlitSurface(selection, NULL, ecran, &rectSelection);
      if (!limitesSelection(rectSouris))
        SDL_BlitSurface(souris, NULL, ecran, &rectSouris);
    }

    sprintf(nv, "%d", nombreVies);
    sprintf(n_morts, "%d", zombiesMorts);
    sprintf(vetScore, "%d", score);
    sprintf(tc, "%d", engine.active_towers);
    sprintf(td, "%d", NB_TOURS);

    nbVies_surf = TTF_RenderText_Solid(police, nv, couleurTexte);
    surfMorts = TTF_RenderText_Solid(police, n_morts, couleurTexte);
    surfScore = TTF_RenderText_Solid(police, vetScore, couleurTexte);
    toursPlacees = TTF_RenderText_Solid(police, tc, couleurTexte);
    barre = TTF_RenderText_Solid(police, "/", couleurTexte);
    toursDisponibles = TTF_RenderText_Solid(police, td, couleurTexte);

    SDL_BlitSurface(nbVies_surf, NULL, ecran, &rectNbVies);
    SDL_BlitSurface(surfMorts, NULL, ecran, &rectMorts);
    SDL_BlitSurface(surfScore, NULL, ecran, &rectScore);
    SDL_BlitSurface(toursPlacees, NULL, ecran, &rectTC);
    SDL_BlitSurface(barre, NULL, ecran, &rectBarre);
    SDL_BlitSurface(toursDisponibles, NULL, ecran, &rectTD);

    if (mute)
      SDL_BlitSurface(audio, NULL, ecran, &rectAudio);

    SDL_UpdateWindowSurface(fenetre);

    if (nombreVies == 0)
    {
      if (!sansZombie())
      {
        Joueur *j1 = creerJoueur(nj, score);
        insererOrdonne(liste_scores, j1);
        Mix_PauseMusic();
        if (gameOver())
          boucle = 1;
        else
          return 0;
      }
    }
    else if (nombreVies > 0 && (tailleHorde == NB_ZOMBIES && sansZombie()))
    {
      Mix_PauseMusic();
      SDL_BlitSurface(gagne, NULL, ecran, NULL);
      SDL_UpdateWindowSurface(fenetre);
      SDL_Delay(3000);
      Joueur *j1 = creerJoueur(nj, score);
      insererOrdonne(liste_scores, j1);
      boucle = 1;
    }
  }
  return 1;
}

// Les fonctions utilitaires (traduction des fonctions métier)
void initialiserComposants()
{
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Init(SDL_INIT_TIMER);
  IMG_Init(IMG_INIT_PNG);
  if (TTF_Init() < 0)
    printf("Erreur Initialisation TTF_Init %s\n", TTF_GetError());
}

TTF_Font *chargerPolice(const char *fichier, int taille)
{
  TTF_Font *f = TTF_OpenFont(fichier, taille);
  if (!f)
    printf("Probleme chargement police ! %s\n", TTF_GetError());
  return f;
}

void initialiserPolice()
{
  atexit(TTF_Quit);
  police = chargerPolice("Fonts/fonte1.ttf", 30);
  couleurTexte.r = 139;
  couleurTexte.g = 0;
  couleurTexte.b = 0;
}

void chargerSurfaces()
{
  fenetre =
      SDL_CreateWindow("Tower Defense", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1080, 640, SDL_WINDOW_OPENGL);
  ecran = SDL_GetWindowSurface(fenetre);
  fond = load_image_map("fundo3.png");
  fondMenu = load_image_map("fundoMenu2.png");
  selection = load_image_map("selecao.png");
  souris = load_image_map("selecao.png");
  developpeurs = load_image_map("desenvolvedores3.png");
  fondScore = load_image_map("menuRanking3.png");
  fondGameOver = load_image_map("gameOver.png");
  fondInstructions = load_image_map("instrucoes.png");
  gagne = load_image_map("ganhou.png");
  fondEntrerNom = load_image_map("fundoColocarNome.png");

  zero = load_image_map("0.png");
  un = load_image_map("1.png");
  deux = load_image_map("2.png");
  trois = load_image_map("3.png");
  quatre = load_image_map("4.png");
  cinq = load_image_map("5.png");
  six = load_image_map("6.png");
  sept = load_image_map("7.png");
  huit = load_image_map("8.png");
  neuf = load_image_map("9.png");
  dix = load_image_map("10.png");
  onze = load_image_map("11.png");
  douze = load_image_map("12.png");
  treize = load_image_map("13.png");
  quatorze = load_image_map("14.png");
  griffe = load_image_map("mao_com_sangue.png");
  audio = load_image_map("mute.png");

  for (int i = 0; i < NB_ZOMBIES; i++)
    z1[i].surface = load_image_map("z1.png");
  for (int i = 0; i < NB_TOURS; i++)
  {
    t1[i].surface = load_image_map("torre.png");
    for (int j = 0; j < NB_TIRS; j++)
    {
      t1[i].tirs[j].surface = load_image_map("tiro1.png");
    }
  }
}

int controlerAffichageTir(SDL_Rect rect)
{
  if (rect.x < 0 || rect.y < 0 || rect.x > 760 || rect.y > 600)
    return 1;
  else
    return 0;
}

void controlerPositionTour(SDL_Rect rect)
{
  if (rect.x < 0)
    rect.x = 0;
  else if (rect.x > 760)
    rect.x = 760;
  else if (rect.y < 0)
    rect.y = 0;
  else if (rect.y > 600)
    rect.y = 600;
}

void chargerNiveau(char *nomFichier)
{
  FILE *fichier = fopen(nomFichier, "r");
  if (fichier == NULL)
  {
    printf("Erreur de chargement du txt \n.");
    exit(1);
  }
  for (int i = 0; i < NB_LIGNES; i++)
  {
    for (int j = 0; j < NB_COLONNES; j++)
    {
      fscanf(fichier, "%d", &carte[i][j]);
    }
  }
  fclose(fichier);
  autoTile();
}

// Supprime afficherCarte() car on utilise render_map

void choisirNiveau(int option)
{
  switch (option)
  {
  case 1:
    nomNiveau = "Fases/matriz.txt";
    break;
  default:
    break;
  }
}

void initialiserEtatTirs()
{
  for (int i = 0; i < NB_TOURS; i++)
  {
    for (int j = 0; j < NB_TIRS; j++)
    {
      t1[i].tirs[j].etat = EN_ATTENTE;
    }
  }
}

void initialiserEtatZombies()
{
  for (int i = 0; i < NB_ZOMBIES; i++)
  {
    z1[i].etat = MORT;
    z1[i].resistance = 0;
  }
}

void initialiserZombie(int i)
{
  z1[i].rect.x = 0;
  z1[i].rect.y = 80;
  z1[i].etat = VIVANT;
  z1[i].resistance = niveaux[niveauActuel].hpZombieBase;
  z1[i].routeId = rand() % niveaux[niveauActuel].nbChemins;
}

void initialiserEtatTours()
{
  for (int i = 0; i < NB_TOURS; i++)
  {
    t1[i].etat = DESACTIVEE;
    t1[i].porteeTir = 150;
    t1[i].cibleActuelle = -1;
  }
}

void initialiserTour(int i, int x, int y)
{
  t1[i].rect.x = x;
  t1[i].rect.y = y;
  t1[i].etat = ACTIVEE;
  t1[i].cibleActuelle = -1;
  t1[i].cooldown = 60;
  t1[i].cooldownTimer =
      60; /* Délai initial : la tour attend 1 cycle avant le 1er tir */
  t1[i].damage = 10;
  t1[i].coutUpgrade = 50;
  t1[i].level = 1;
  for (int j = 0; j < NB_TIRS; j++)
  {
    t1[i].tirs[j].rect.x = x;
    t1[i].tirs[j].rect.y = y + 10;
  }
}

void marcherZombie2()
{
  for (int i = 0; i < tailleHorde; i++)
  {
    if (z1[i].etat == VIVANT)
    {
      if (z1[i].rect.x < 165)
      {
        z1[i].surface = load_image_map("z1lado.png");
        z1[i].rect.x++;
      }
      else if (z1[i].routeId >= 1 && z1[i].rect.y == 80 &&
               z1[i].rect.x >= 165 && z1[i].rect.x < 405)
      {
        z1[i].surface = load_image_map("z1lado.png");
        z1[i].rect.x++;
      }
      else if (z1[i].rect.x == 165 && z1[i].rect.y < 360)
      {
        z1[i].surface = load_image_map("z1descendo.png");
        z1[i].rect.y++;
      }
      else if (z1[i].routeId >= 3 && z1[i].rect.y == 360 &&
               z1[i].rect.x >= 165 && z1[i].rect.x < 720)
      {
        z1[i].surface = load_image_map("z1lado.png");
        z1[i].rect.x++;
        if (z1[i].rect.x == 720)
          z1[i].etat = MORT;
      }
      else if (z1[i].rect.y == 360 && z1[i].rect.x >= 165 &&
               z1[i].rect.x < 405)
      {
        z1[i].surface = load_image_map("z1lado.png");
        z1[i].rect.x++;
      }
      else if (z1[i].routeId >= 2 && z1[i].rect.y == 360 &&
               z1[i].rect.x >= 405 && z1[i].rect.x < 605)
      {
        z1[i].surface = load_image_map("z1lado.png");
        z1[i].rect.x++;
      }
      else if (z1[i].rect.x == 405 && z1[i].rect.y > 118)
      {
        z1[i].surface = load_image_map("z1subindo.png");
        z1[i].rect.y--;
      }
      else if (z1[i].routeId >= 1 && z1[i].rect.x == 405 &&
               z1[i].rect.y >= 80 && z1[i].rect.y < 118)
      {
        z1[i].surface = load_image_map("z1descendo.png");
        z1[i].rect.y++;
      }
      else if (z1[i].rect.y == 118 && z1[i].rect.x >= 405 &&
               z1[i].rect.x < 605)
      {
        z1[i].surface = load_image_map("z1lado.png");
        z1[i].rect.x++;
      }
      else if (z1[i].rect.x == 605 && z1[i].rect.y < 480)
      {
        z1[i].surface = load_image_map("z1descendo.png");
        z1[i].rect.y++;
      }
      else if (z1[i].rect.y == 480 && z1[i].rect.x >= 605 &&
               z1[i].rect.x < 720)
      {
        z1[i].surface = load_image_map("z1lado.png");
        z1[i].rect.x++;
      }
      else if (z1[i].rect.x == 720)
      {
        z1[i].etat = MORT;
      }
    }
  }
}

void controlerImage()
{
  for (int i = 0; i < tailleHorde; i++)
  {
    z1[i].image_actuelle++;
    if (z1[i].image_actuelle == 2)
      z1[i].image_actuelle = 0;
  }
}

void afficherZombies()
{
  for (int i = 0; i < tailleHorde; i++)
  {
    if (z1[i].etat == VIVANT)
      SDL_BlitSurface(z1[i].surface, &z1[i].sprites[z1[i].image_actuelle],
                      ecran, &z1[i].rect);
  }
}

void afficherTours()
{
  // Indicateur visuel pour les emplacements (slots)
  for (int k = 0; k < niveaux[niveauActuel].nbSlotsTotal; k++)
  {
    if (niveaux[niveauActuel].slots[k].estOccupe == 0)
    {
      SDL_Rect r = {niveaux[niveauActuel].slots[k].x,
                    niveaux[niveauActuel].slots[k].y, TAILLE_TUILE - 4,
                    TAILLE_TUILE - 4};
      SDL_FillRect(ecran, &r, SDL_MapRGB(ecran->format, 255, 255, 0)); // Jaune
    }
    else if (niveaux[niveauActuel].slots[k].estOccupe == 2)
    {
      SDL_Rect r = {niveaux[niveauActuel].slots[k].x,
                    niveaux[niveauActuel].slots[k].y, TAILLE_TUILE - 4,
                    TAILLE_TUILE - 4};
      SDL_FillRect(
          ecran, &r,
          SDL_MapRGB(ecran->format, 100, 100,
                     100)); // Gris = Rocher/Zone bloquée par la génération
    }
  }

  for (int i = 0; i < engine.active_towers; i++)
  {
    if (t1[i].etat == ACTIVEE)
      SDL_BlitSurface(t1[i].surface, NULL, ecran, &t1[i].rect);
  }
}

void chargerSprite()
{
  for (int i = 0; i < NB_ZOMBIES; i++)
  {
    z1[i].sprites[0].x = 7;
    z1[i].sprites[0].y = 0;
    z1[i].sprites[0].w = 40;
    z1[i].sprites[0].h = 80;
    z1[i].sprites[1].x = 47;
    z1[i].sprites[1].y = 0;
    z1[i].sprites[1].w = 40;
    z1[i].sprites[1].h = 80;
    z1[i].image_actuelle = 0;
  }
}

int tirHorsEcran(SDL_Rect tir)
{
  if (tir.x <= 0 || tir.x >= 760 || tir.y >= 600 || tir.y <= 0)
    return TRUE;
  return FALSE;
}

void tirer()
{
  int x, y;
  for (int i = 0; i < engine.active_towers; i++)
  {
    if (t1[i].etat == ACTIVEE)
    {
      x = t1[i].rect.x;
      y = t1[i].rect.y;
      int cible = t1[i].cibleActuelle;
      int aCibleValide = (cible != -1 && z1[cible].etat == VIVANT);

      for (int j = 0; j < NB_TIRS; j++)
      {
        if (t1[i].tirs[j].etat == TIRE)
        {
          if (!tirHorsEcran(t1[i].tirs[j].rect) && aCibleValide)
          {
            t1[i].tirs[j].rect.x += (z1[cible].rect.x - x) * 0.15;
            t1[i].tirs[j].rect.y += (z1[cible].rect.y - y) * 0.15;
            SDL_BlitSurface(t1[i].tirs[j].surface, NULL, ecran,
                            &t1[i].tirs[j].rect);
          }
          else
          {
            t1[i].tirs[j].rect.x = x;
            t1[i].tirs[j].rect.y = y + 10;
            t1[i].tirs[j].etat = DESACTIVE;
          }
        }
      }
    }
  }
}

int collision(SDL_Rect rect1, SDL_Rect rect2)
{
  if (rect2.x + rect2.w < rect1.x)
    return 0;
  if (rect2.x > rect1.x + rect1.w)
    return 0;
  if (rect2.y + rect2.h < rect1.y)
    return 0;
  if (rect2.y > rect1.y + rect1.h)
    return 0;
  return 1;
}

void tuerZombie(ZOMBIE *zombie)
{
  for (int i = 0; i < tailleHorde; i++)
  {
    if (zombie[i].etat == VIVANT)
    {
      for (int j = 0; j < engine.active_towers; j++)
      {
        if (t1[j].etat == ACTIVEE)
        {
          for (int k = 0; k < NB_TIRS; k++)
          {
            if (t1[j].tirs[k].etat == TIRE)
            {
              if (collision(zombie[i].rect, t1[j].tirs[k].rect))
              {
                if (zombie[i].resistance == 0)
                {
                  zombie[i].etat = MORT;
                  score += (zombie[i].resistance / 100) + 3;
                  zombiesMorts++;
                  morts++;
                  if (nombreVies > 0 && morts == tailleHorde)
                  {
                    nombreVies++;
                    morts = 0;
                  }
                }
                else
                {
                  // Amplification des dégâts des tours (au lieu de redescendre
                  // juste de 1, descendre de 2 ou 3)
                  zombie[i].resistance = zombie[i].resistance - t1[j].damage;
                  if (zombie[i].resistance < 0)
                    zombie[i].resistance = 0;
                }
              }
            }
          }
        }
      }
    }
  }
}

int limitesSelection(SDL_Rect souris_r)
{
  if ((souris_r.x > 760) || (souris_r.y > 600))
    return TRUE;
  return FALSE;
}

int aTour()
{
  for (int i = 0; i < engine.active_towers; i++)
  {
    if (t1[i].etat == ACTIVEE)
      return TRUE;
  }
  return FALSE;
}

float distance(SDL_Rect zumb, SDL_Rect tour)
{
  return sqrt(pow(zumb.y - tour.y, 2) + pow(zumb.x - tour.x, 2));
}

void initialiserTir()
{
  for (int i = 0; i < engine.active_towers; i++)
  {
    if (t1[i].etat == ACTIVEE)
    {
      if (t1[i].cooldownTimer > 0)
        t1[i].cooldownTimer--;

      if (t1[i].cooldownTimer <= 0)
      {
        for (int j = 0; j < NB_TIRS; j++)
        {
          if (t1[i].tirs[j].etat == EN_ATTENTE)
          {
            t1[i].tirs[j].etat = TIRE;
            t1[i].cooldownTimer = t1[i].cooldown;
            break;
          }
        }
      }
      for (int j = 0; j < NB_TIRS; j++)
      {
        if (t1[i].tirs[j].etat == DESACTIVE)
        {
          t1[i].tirs[j].rect.x = t1[i].rect.x;
          t1[i].tirs[j].rect.y = t1[i].rect.y + 10;
          t1[i].tirs[j].etat = EN_ATTENTE;
        }
      }
    }
  }
}

void verifierVies()
{
  for (int i = 0; i < tailleHorde; i++)
  {
    if (z1[i].etat == VIVANT && z1[i].rect.x == 719 && z1[i].rect.y == 480)
      nombreVies--;
  }
}

int gameOver()
{
  int boucle = 0;
  SDL_BlitSurface(fondGameOver, NULL, ecran, NULL);
  while (boucle == 0)
  {
    while (SDL_PollEvent(&evenement))
    {
      if (evenement.type == SDL_QUIT)
        return 0;
      else if (evenement.type == SDL_MOUSEBUTTONDOWN)
      {
        if ((evenement.button.x >= 100 && evenement.button.x <= 175) &&
            (evenement.button.y >= 100 && evenement.button.y <= 140))
          return 1;
      }
    }
    SDL_UpdateWindowSurface(fenetre);
  }
  return 0;
}

int records()
{
  rectTexte1.x = 250;
  rectTexte1.y = 210;
  rectTexte2.x = 550;
  rectTexte2.y = 210;
  SDL_BlitSurface(fondScore, NULL, ecran, NULL);

  Noeud *paux = liste_scores->debut;
  while (paux != NULL)
  {
    sprintf(pb, "%d", paux->joueur->score);
    texte1 = TTF_RenderText_Solid(police, paux->joueur->nom, couleurTexte);
    texte2 = TTF_RenderText_Solid(police, pb, couleurTexte);
    SDL_BlitSurface(texte1, NULL, ecran, &rectTexte1);
    SDL_BlitSurface(texte2, NULL, ecran, &rectTexte2);
    SDL_UpdateWindowSurface(fenetre);
    rectTexte1.y += 40;
    rectTexte2.y += 40;
    paux = paux->suivant;
  }

  int boucle = 0;
  while (boucle == 0)
  {
    while (SDL_PollEvent(&evenement))
    {
      if (evenement.type == SDL_QUIT)
        return 0;
      else if (evenement.type == SDL_MOUSEBUTTONDOWN)
      {
        if ((evenement.button.x >= 100 && evenement.button.x <= 175) &&
            (evenement.button.y >= 100 && evenement.button.y <= 140))
          return 1;
      }
    }
    SDL_UpdateWindowSurface(fenetre);
  }
  return 0;
}

int insererNom()
{
  int indice = 0;
  char message[] = "Entrer votre pseudo:";
  SDL_Surface *ttfMessage, *ttfNom, *ttfTrace;
  SDL_Rect posMsg = {300, 200, 0, 0}, pos = {290, 300, 0, 0},
           posTrace = {290, 305, 0, 0};
  char nomJoueur_temp[30] = "";
  int boucle = 0;

  while (boucle == 0)
  {
    while (SDL_PollEvent(&evenement))
    {
      if (evenement.type == SDL_QUIT)
        return 0;
      else if (evenement.type == SDL_KEYDOWN)
      {
        if (evenement.key.keysym.sym == SDLK_ESCAPE)
          return 0;
        else if (evenement.key.keysym.sym == SDLK_BACKSPACE)
        {
          if (indice > 0)
          {
            indice--;
            nomJoueur_temp[indice] = '\0';
          }
        }
        else if (evenement.key.keysym.sym >= SDLK_a &&
                 evenement.key.keysym.sym <= SDLK_z)
        {
          if (indice < 29)
          {
            nomJoueur_temp[indice] = evenement.key.keysym.sym;
            indice++;
            nomJoueur_temp[indice] = '\0';
          }
        }
      }
      else if (evenement.type == SDL_MOUSEBUTTONDOWN)
      {
        if ((evenement.button.x >= 700 && evenement.button.x <= 760) &&
            (evenement.button.y >= 530 && evenement.button.y <= 570))
        {
          strcpy(nj, nomJoueur_temp);
          return 1;
        }
      }
    }
    SDL_BlitSurface(fondEntrerNom, NULL, ecran, NULL);
    ttfMessage = TTF_RenderText_Blended(police, message, couleurTexte);
    ttfNom = TTF_RenderText_Blended(police, nomJoueur_temp, couleurTexte);
    ttfTrace =
        TTF_RenderText_Blended(police, "____________________", couleurTexte);
    SDL_BlitSurface(ttfMessage, NULL, ecran, &posMsg);
    SDL_BlitSurface(ttfNom, NULL, ecran, &pos);
    SDL_BlitSurface(ttfTrace, NULL, ecran, &posTrace);
    SDL_UpdateWindowSurface(fenetre);
  }
  return 0;
}

int afficherDeveloppeurs()
{
  int boucle = 0;
  while (boucle == 0)
  {
    while (SDL_PollEvent(&evenement))
    {
      if (evenement.type == SDL_QUIT)
        return 0;
      else if (evenement.type == SDL_MOUSEBUTTONDOWN)
      {
        if ((evenement.button.x >= 700 && evenement.button.x <= 780) &&
            (evenement.button.y >= 530 && evenement.button.y <= 570))
          return 1;
      }
    }
    SDL_BlitSurface(developpeurs, NULL, ecran, NULL);
    SDL_UpdateWindowSurface(fenetre);
  }
  return 0;
}

void ciblerZombies()
{
  for (int i = 0; i < engine.active_towers; i++)
  {
    if (t1[i].etat == ACTIVEE)
    {
      // Vérifier si la cible actuelle est toujours valide
      int cible = t1[i].cibleActuelle;
      if (cible != -1)
      {
        if (z1[cible].etat == MORT ||
            distance(t1[i].rect, z1[cible].rect) > t1[i].porteeTir)
        {
          t1[i].cibleActuelle = -1; // Cible perdue (morte ou hors portée)
        }
      }

      // Chercher une nouvelle cible si aucune
      if (t1[i].cibleActuelle == -1)
      {
        for (int j = 0; j < tailleHorde; j++)
        {
          if (z1[j].etat == VIVANT &&
              distance(t1[i].rect, z1[j].rect) <= t1[i].porteeTir)
          {
            t1[i].cibleActuelle = j;
            break; // Se fige sur le premier zombie à portée !
          }
        }
      }
    }
  }
}

int tourSurChemin(SDL_Rect souris_r)
{
  for (int i = 0; i < NB_LIGNES; i++)
  {
    for (int j = 0; j < NB_COLONNES; j++)
    {
      if (collision(champ[i][j], souris_r) == 1)
      {
        if (carte[i][j] >= 1 && carte[i][j] <= 6)
          return TRUE;
      }
    }
  }
  return FALSE;
}

int sansZombie()
{
  for (int i = 0; i < tailleHorde; i++)
  {
    if (z1[i].etat == VIVANT)
      return FALSE;
  }
  return TRUE;
}

void augmenterResistance()
{
  resistance_globale += 100;
  for (int i = 0; i < tailleHorde; i++)
    z1[i].resistance = resistance_globale;
}

int instructions()
{
  int boucle = 0;
  SDL_BlitSurface(fondInstructions, NULL, ecran, NULL);
  while (boucle == 0)
  {
    while (SDL_PollEvent(&evenement))
    {
      if (evenement.type == SDL_QUIT)
        return 0;
      else if (evenement.type == SDL_MOUSEBUTTONDOWN)
      {
        if ((evenement.button.x >= 100 && evenement.button.x <= 178) &&
            (evenement.button.y >= 100 && evenement.button.y <= 140))
          return 1;
      }
    }
    SDL_UpdateWindowSurface(fenetre);
  }
  return 0;
}

void initialiserNiveaux()
{
  int hpBase[10] = {100, 150, 200, 300, 400, 550, 750, 1000, 1400, 2200};
  int nbZombies[10] = {5, 10, 15, 20, 25, 30, 35, 40, 50, 60};
  int maxTours[10] = {4, 4, 4, 5, 5, 5, 6, 6, 6, 7};
  int chemins[10] = {1, 1, 2, 2, 2, 3, 3, 3, 4, 4};
  int typeTour[10] = {1, 1, 2, 2, 2, 3, 3, 3, 4, 4};

  int coordX[7] = {120, 200, 320, 440, 520, 560, 640};
  int coordY[7] = {120, 200, 320, 240, 160, 360, 440};

  for (int k = 0; k < 10; k++)
  {
    niveaux[k].numeroNiveau = k + 1;
    niveaux[k].nbZombiesBase = (int)(nbZombies[k] * 1.2);
    if (niveaux[k].nbZombiesBase < 5)
      niveaux[k].nbZombiesBase = 5;
    niveaux[k].hpZombieBase = (int)(hpBase[k] * 0.7);
    niveaux[k].maxToursConstructibles = maxTours[k];
    niveaux[k].nbChemins = chemins[k];
    niveaux[k].typeTourDebloquee = typeTour[k];
    niveaux[k].nbSlotsTotal = maxTours[k];
    for (int s = 0; s < niveaux[k].nbSlotsTotal; s++)
    {
      niveaux[k].slots[s].x = coordX[s];
      niveaux[k].slots[s].y = coordY[s];
      niveaux[k].slots[s].estOccupe = 0;
      niveaux[k].slots[s].typeTour = 1;
    }
  }
}

int verifierClicSlot(SDL_Rect souris, LEVEL *niveau, int *indexSlot)
{
  for (int k = 0; k < niveau->nbSlotsTotal; k++)
  {
    SDL_Rect rectSlot = {niveau->slots[k].x, niveau->slots[k].y,
                         TAILLE_TUILE + 10, TAILLE_TUILE + 10};
    if (collision(souris, rectSlot))
    {
      if (niveau->slots[k].estOccupe == 0)
      {
        *indexSlot = k;
        return 1;
      }
    }
  }
  return 0;
}

void play_level_transition(int niv)
{
  SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
  char message[50];
  sprintf(message, "NIVEAU SUIVANT ! :  (Niveau %d)", niv + 1);
  SDL_Surface *texte = TTF_RenderText_Solid(police, message, couleurTexte);
  SDL_Rect posTexte = {250, 300, 0, 0};
  SDL_BlitSurface(texte, NULL, ecran, &posTexte);
  SDL_UpdateWindowSurface(fenetre);
  Mix_PauseMusic();
  SDL_Delay(3500);
  Mix_ResumeMusic();
  SDL_FreeSurface(texte);
}

void upgrade_tower(TOUR *t)
{
  if (engine.player_score >= t->coutUpgrade)
  {
    engine.player_score -= t->coutUpgrade;
    score = engine.player_score;
    t->damage *= 2;
    t->porteeTir += 30;
    t->cooldown = (t->cooldown * 2) / 3; // +33% fire rate
    if (t->cooldown < 10)
      t->cooldown = 10;
    t->coutUpgrade *= 2;
    t->level++;
  }
}

void autoTile()
{
  for (int y = 0; y < NB_LIGNES; y++)
  {
    for (int x = 0; x < NB_COLONNES; x++)
    {
      if (carte[y][x] != 0 && carte[y][x] != 10 && carte[y][x] != 11 &&
          carte[y][x] != 12 && carte[y][x] != 13 && carte[y][x] != 14)
      {
        int haut = (y > 0 && carte[y - 1][x] != 0);
        int bas = (y < NB_LIGNES - 1 && carte[y + 1][x] != 0);
        int gauche = (x > 0 && carte[y][x - 1] != 0);
        int droite = (x < NB_COLONNES - 1 && carte[y][x + 1] != 0);

        if (gauche && droite && !haut && !bas)
          carte[y][x] = 1;
        else if (haut && bas && !gauche && !droite)
          carte[y][x] = 2;
        else if (bas && droite && !haut && !gauche)
          carte[y][x] = 3;
        else if (bas && gauche && !haut && !droite)
          carte[y][x] = 4;
        else if (haut && droite && !bas && !gauche)
          carte[y][x] = 5;
        else if (haut && gauche && !bas && !droite)
          carte[y][x] = 6;
        else
          carte[y][x] = 1;
      }
    }
  }
}
