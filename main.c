#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>
#include <stdbool.h>
#define MAX_LONGUEUR_MOT 100

void generer_lettres(char lettres[], int longueur) {
	char choix;
	int i;
	for (i = 0; i < longueur; i++) {
		char consonnes[] = {'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'X', 'Y', 'Z'};
		char voyelles[] = {'A', 'E', 'I', 'O', 'U'};
		do {
			printf("Caractère %d : Consonne (c/C) ou Voyelle (v/V) = ", i + 1);
			scanf(" %c", &choix);
		} while (choix != 'C' && choix != 'V'&& choix != 'c' && choix != 'v');

		if (choix == 'C'||choix == 'c') {
			lettres[i] = consonnes[rand() % 21];
		} else {
			lettres[i] = voyelles[rand() % 5];
		}
	}

	printf("\n\n     ");
	int j;
	for (j = 0; j < longueur; j++) {
		printf("%c    ", lettres[j]);
	}
}

int mot_valide(char *mot, char *mots_trouves[]) {
	int i;
	for ( i = 0; mots_trouves[i] != NULL; i++) {
		if (strcmp(mot, mots_trouves[i]) == 0) {
			return 1;
		}
	}
	return 0;
}
void trierMots(char **mots, int taille) {

	int i;
	for(i = 0; i < taille - 1; i++) {
		int j;
		for(j = 0; j < taille - i - 1; j++) {
			if (strcmp(mots[j], mots[j + 1]) > 0) {
				// Échanger les mots
				char *temp = mots[j];
				mots[j] = mots[j + 1];
				mots[j + 1] = temp;
			}
		}
	}
}

int sum(int arr[], int size) {
	int total = 0;
	int i;
	for ( i = 0; i < size; i++) {
		total += arr[i];
	}
	return total;
}

size_t taille_mots_trouves;
size_t taille_max = 0;
int main() {
	setlocale(LC_ALL, ""); // pour afficher les caractères spéciaux dans la console
	srand(time(NULL));

	// Chargement du dictionnaire
	FILE *fp = fopen("dictionnaire(1).txt", "r");

	int nb_mots = 0;
	char *dictionnaire[50000];

	if (fp == NULL) {
		printf("Erreur lors de l'ouverture du fichier dictionnaire.txt\n");
		return 1;
	}
	char *mots_trouves[50000];
	char mot[MAX_LONGUEUR_MOT];
	while (fscanf(fp, "%s", mot) == 1) {
		dictionnaire[nb_mots] = strdup(mot);  // Utilisation de strdup pour allouer de la mémoire et copier la chaîne
		nb_mots++;

		if (nb_mots >= 50000) {
			printf("Trop de mots dans le dictionnaire (limite atteinte :");
			fclose(fp);
			return 1;
		}
	}

	// Boucle de jeu
	int mode_jeu;
	printf("       J E U   D E   M O T S     ");
	printf("\n1- Jouer à un\n2- Jouer à un sous une contrainte de temps\n3- Jouer à deux\n0- quitter\n");

	do {
		printf("                      Votre choix = ");
		scanf("%d", &mode_jeu);
	} while (mode_jeu < 0 || mode_jeu > 3);

	// Jouer contre l'ordinateur sans contrainte du temps
	if (mode_jeu == 1) {
		int longueur_mot;
		int score=0;
		printf("Tapez la longueur (7/8/9) du mot à retrouver : ");
		scanf("%d", &longueur_mot);
		while(longueur_mot<7|| longueur_mot>9) {
			printf("La taille que vous avez saisie est erronée!\n");
			printf("Tapez la longueur (7/8/9) du mot à retrouver : ");
			scanf("%d", &longueur_mot);
		}
		char lettres[longueur_mot];
		generer_lettres(lettres, longueur_mot);
		int taille_max;
		trouver_mots(dictionnaire, nb_mots, lettres, mots_trouves, &taille_max, &taille_mots_trouves);


		//printf("start");//test
//		int l;
//		for (l = 0; mots_trouves[l] != NULL && l < 10; l++) {
//			printf(" helloooooooo %s\n", mots_trouves[l]);
//		}
		//printf("fin");
		int tentatives =longueur_mot;
		int c=0;
		int max=0;
		int gameOver = 0;
		while (tentatives >0&& !gameOver) {
			c++;
			char mot[MAX_LONGUEUR_MOT];
			printf("\n%d-       ",c);
			scanf("%s",mot) ;

			// Convertir la chaîne en minuscules
			int i = 0;
			while (mot[i]) {
				mot[i] = tolower((unsigned char)mot[i]);
				i++;
			}

			if (mot_valide(mot,mots_trouves)) {
				if (strlen(mot) == taille_max) {
					printf("Vous gagnez! Le mot que vous avez saisi est correct et est le plus long.\n");
					score = score + strlen(mot);
					int i = 0;
					while (mot[i]) {
						mot[i] = toupper((unsigned char)mot[i]);
						i++;
					}
					printf("%s(%d)\n", mot, strlen(mot));
					printf("     votre score est %d\n", score);
					if (score > max) {
						max = score;
					}
					gameOver = 1;
				} else {
					printf("Le mot que vous avez saisi est correct mais n'est pas le plus long.\n");
				}
			} else {
				tentatives--;
				printf("Le mot que vous avez saisi est incorrect!\n");
			}
		}

		if (tentatives == 0&& !gameOver) {
			printf("Vous avez perdu !\n");
			int choice;
			printf("1-Afficher Le/Les mot(s) le(s) plus long(s)\n2-Afficher tous les mots possibles\n ");

			do {
				printf("                    Votre choix = ");
				scanf("%d", &choice);
			} while(choice!=1 & choice!=2);
			if (choice==1) {

			} else {
				int i;
				for ( i = 0; i < taille_mots_trouves; i++) {
					printf("%s(%d)\n", mots_trouves[i], strlen(mots_trouves[i]));
				}
			}

			printf("Votre score est %d\n", max);
		}
	}

	//jouer contre l'ordinateur avec contrainte du temps
	else if (mode_jeu == 2) {
		int longueur_mot;
		int score = 0;
		printf("Tapez la longueur (7/8/9) du mot à retrouver : ");
		scanf("%d", &longueur_mot);

		while (longueur_mot < 7 || longueur_mot > 9) {
			printf("La taille que vous avez saisie est erronée!\n");
			printf("Tapez la longueur (7/8/9) du mot à retrouver : ");
			scanf("%d", &longueur_mot);
		}

		char lettres[longueur_mot];
		generer_lettres(lettres, longueur_mot);
		int taille_max;
		trouver_mots(dictionnaire, nb_mots, lettres, mots_trouves, &taille_max, &taille_mots_trouves);

//		int l; // test
//		printf("strt");
//		for (l = 0; mots_trouves[l] != NULL && l < 10; l++) {
//			printf(" helloooooooo %s\n", mots_trouves[l]);
//		}

		int time_limit = longueur_mot * longueur_mot;  // Set the time limit based on longueur_mot
		time_t start_time, current_time;
		time(&start_time);

		int tentatives = 0;
		int gameOver = 0;

		while ((current_time - start_time) < time_limit && !gameOver) {
			time(&current_time);  // Mettre à jour le temps actuel à l'intérieur de la boucle

			tentatives++;  // Incrémentation du nombre de tentatives à l'intérieur de la boucle

			char mot[MAX_LONGUEUR_MOT];
			printf("\n%d-       ", tentatives);
			scanf("%s", mot);

			int i = 0;
			while (mot[i]) {
				mot[i] = tolower((unsigned char)mot[i]);
				i++;
			}

			if (mot_valide(mot, mots_trouves)) {
				if (strlen(mot) == taille_max) {
					printf("Vous gagnez! Le mot que vous avez saisi est correct et est le plus long.\n");
					score = score + strlen(mot);
					int i = 0;
					while (mot[i]) {
						mot[i] = toupper((unsigned char)mot[i]);
						i++;
					}
					printf("%s(%d)\n", mot, strlen(mot));
					printf("     votre score est %d\n", score);
					gameOver = 1;
				} else {
					printf("Le mot que vous avez saisi est correct mais n'est pas le plus long.\n");
				}
			} else {
				printf("Le mot que vous avez saisi est incorrect!\n");
			}
		}

		// À la fin du jeu, affichez les choix
		int choice;
		printf("1-Afficher Le/Les mot(s) le(s) plus long(s)\n2-Afficher tous les mots possibles\n ");

		do {
			printf("Votre choix = ");
			scanf("%d", &choice);
		} while (choice != 1 && choice != 2);

		if (choice == 1) {
			// Afficher Le/Les mot(s) le(s) plus long(s)
			// Ajoutez ici la logique pour afficher les mots les plus longs
		} else {
			printf("Mots possibles :\n");
			int i;
			for ( i = 0; i < taille_mots_trouves; i++) {
				printf("%s\n", mots_trouves[i]);
			}
		}

		printf("Votre score est %d\n", score);
	}
//jouer à deux
	else if (mode_jeu == 3) {
		int scores_manche_joueur1[100] = {0};
		int scores_manche_joueur2[100] = {0};
		int t;
		int manche = 1;
		char continuer;

		do {
			printf("\nMANCHE n %d\n", manche++);
			printf("Tapez la taille (nombre pair) du mot le plus long à retrouver = ");
			scanf("%d", &t);

			while (t % 2 != 0) {
				printf("La taille doit être pair. Réessayez : ");
				scanf("%d", &t);
			}

			char lettres[t];
			generer_lettres(lettres, t);

			char *mots_trouves_joueur1[10000];
			char *mots_trouves_joueur2[10000];

			int taille_max = 0;
			size_t taille_mots_trouves_joueur1 = 0;
			size_t taille_mots_trouves_joueur2 = 0;

			int score_joueur1 = 0;
			int score_joueur2 = 0;

			int tentatives = t / 2;

			int joueur_actuel = 1; // 1 pour Joueur 1, 2 pour Joueur 2

			while (tentatives > 0) {
				printf("\n%d-", t / 2 + 1 - tentatives);
				if (joueur_actuel == 1) {
					printf(" Joueur 1, ");
				} else {
					printf(" Joueur 2, ");
				}

				char mot[MAX_LONGUEUR_MOT];
				printf("entrez un mot : ");
				scanf("%s", mot);

				// Convertir la chaîne en minuscules
				int i = 0;
				while (mot[i]) {
					mot[i] = tolower((unsigned char)mot[i]);
					i++;
				}

				char **mots_trouves_actuel;
				size_t *taille_mots_trouves_actuel;
				int *score_actuel;

				if (joueur_actuel == 1) {
					mots_trouves_actuel = mots_trouves_joueur1;
					taille_mots_trouves_actuel = &taille_mots_trouves_joueur1;
					score_actuel = &score_joueur1;
				} else {
					mots_trouves_actuel = mots_trouves_joueur2;
					taille_mots_trouves_actuel = &taille_mots_trouves_joueur2;
					score_actuel = &score_joueur2;
				}

				if (!mot_valide(mot, mots_trouves_joueur1) && !mot_valide(mot, mots_trouves_joueur2)) {
					*taille_mots_trouves_actuel = 0;
					trouver_mots(dictionnaire, nb_mots, lettres, mots_trouves_actuel, &taille_max, taille_mots_trouves_actuel);
					trierMots(mots_trouves_actuel, *taille_mots_trouves_actuel);

					if (*taille_mots_trouves_actuel > 0) {
						*score_actuel = strlen(mots_trouves_actuel[0]);
						int i = 0;
						while (mots_trouves_actuel[0][i]) {
							mots_trouves_actuel[0][i] = toupper((unsigned char)mots_trouves_actuel[0][i]);
							i++;
						}
						printf("\nLe mot que vous avez saisi est correct mais ");
						if (*score_actuel == t) {
							printf("c'est le plus long! Félicitations!\n");
						} else {
							printf("n'est pas le plus long!\n");
						}
						printf("Le score de Joueur %d est %d.\n", joueur_actuel, *score_actuel);
					} else {
						printf("\nLe mot que vous avez saisi est incorrect!\n");
					}

					joueur_actuel = (joueur_actuel == 1) ? 2 : 1; // Changer de joueur
					tentatives--;
				} else {
					printf("\nLe mot que vous avez saisi est correct mais ");
					if (joueur_actuel == 1) {
						printf("vous l'avez déjà tapé!\n");
					} else {
						printf("Joueur 1 l'a déjà tapé!\n");
					}
				}
			}

			// Afficher les mots les plus longs
			printf("\n1- Afficher le/les mot(s) le/les plus long(s)\n");
			printf("2- Afficher tous les mots possibles\n");

			int choix;
			printf("                   Votre choix = ");
			scanf("%d", &choix);

			if (choix == 1) {
				// Afficher le/les mot(s) le/les plus long(s)
				printf("Mot(s) le/les plus long(s) :\n");
				int i;
				for ( i = 0; i < taille_mots_trouves_joueur1; i++) {
					printf("%s(%d)\n", mots_trouves_joueur1[i], strlen(mots_trouves_joueur1[i]));
				}

				// Afficher le gagnant de la manche
				if (score_joueur1 > score_joueur2) {
					printf("Joueur 1 a remporté la manche n°%d!\n", manche - 1);
				} else if (score_joueur2 > score_joueur1) {
					printf("Joueur 2 a remporté la manche n°%d!\n", manche - 1);
				} else {
					printf("Match nul pour la manche n°%d!\n", manche - 1);
				}
			} else if (choix == 2) {
				// Afficher tous les mots possibles
				printf("Mots possibles :\n");
				int i;
				for ( i = 0; i < taille_mots_trouves_joueur1; i++) {
					printf("%s(%d)\n", mots_trouves_joueur1[i], strlen(mots_trouves_joueur1[i]));
				}
			}

			// Demander si les joueurs veulent continuer
			printf("\nVoulez-vous continuer? Répondez par 'o' si oui et 'n' sinon = ");
			scanf(" %c", &continuer);

		} while (continuer == 'o' || continuer == 'O');
		// Afficher le score final
		printf("\nScore Final\t\t\t\tJ1\t\tJ2");
		printf("\n------------------------------------------------------------");
		int i;
		for ( i = 1; i < manche; i++) {
			printf("\nScore Manche %d\t\t\t\t%d\t\t%d", i, scores_manche_joueur1[i-1], scores_manche_joueur2[i-1]);
		}

		printf("\n------------------------------------------------------------");

		int score_final_joueur1 = sum(scores_manche_joueur1, manche - 1);
		int score_final_joueur2 = sum(scores_manche_joueur2, manche - 1);

		printf("\nScore Final\t\t\t\t%d\t\t%d", score_final_joueur1, score_final_joueur2);

// Déclarer le vainqueur
		if (score_final_joueur1 > score_final_joueur2) {
			printf("\n\nJ1 est le vainqueur!\n");
		} else if (score_final_joueur2 > score_final_joueur1) {
			printf("\n\nJ2 est le vainqueur!\n");
		} else {
			printf("\n\nMatch nul!\n");
		}
		return 0;
	}

//quitter
	else if (mode_jeu == 0) {
		printf("Au Revoir");
	}

	int i;
	for (i = 0; i < nb_mots; i++) {
		free(dictionnaire[i]);
	}

	fclose(fp);
	return 0;
}


void trouver_mots(char **dictionnaire, int nb_mots, char *lettres, char **mots_trouves, int *taille_max, size_t *taille_mots_trouves) {
	// Allocation mémoire pour le mot courant
	char *mot_courant = (char *)malloc((*taille_max) * sizeof(char));

	// Parcourir toutes les combinaisons de lettres possibles
	size_t i;
	for ( i = 0; i < (1 << strlen(lettres)); i++) {
		// Construire le mot courant
		size_t taille_mot_courant = 0;
		size_t j;
		for ( j = 0; j < strlen(lettres); j++) {
			if ((i & (1 << j)) != 0) {
				// Ajouter la lettre à mot_courant
				mot_courant[taille_mot_courant++] = lettres[j];
			}
		}

		// Ajouter le caractère de fin de chaîne
		mot_courant[taille_mot_courant] = '\0';

		// Vérifier si le mot courant est dans le dictionnaire
		int mot_valide = 0;
		int k;
		while (k < nb_mots && !mot_valide) {
			if (strcmp(mot_courant, dictionnaire[k]) == 0) {
				mot_valide = 1;
			}
			k++;
		}

		// Si le mot est valide, l'ajouter au tableau de mots_trouves
		if (mot_valide) {
			// Vérifier si la taille actuelle est suffisante
			if (*taille_mots_trouves >= *taille_max) {
				*taille_max *= 2;
				mots_trouves = (char **)realloc(mots_trouves, (*taille_max) * sizeof(char *));
			}

			// Allouer de la mémoire pour le mot trouvé
			mots_trouves[*taille_mots_trouves] = (char *)malloc((taille_mot_courant + 1) * sizeof(char));

			// Copier le mot courant dans le tableau de mots_trouves
			strcpy(mots_trouves[*taille_mots_trouves], mot_courant);

			// Incrémenter la taille des mots_trouves
			(*taille_mots_trouves)++;
		}
	}

	// Trier les mots trouvés
	trierMots(mots_trouves, *taille_mots_trouves);

	// Libérer la mémoire allouée pour le mot courant
	free(mot_courant);
}

