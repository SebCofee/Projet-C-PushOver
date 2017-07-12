#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "struct.h"
#include "plato.h"
#include "jeu.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    FONCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Fonction sauvegardant la partie passée en paramètre
	Retourne 0 en cas de problème, 1 sinon.
*/

int sauvegardePartie (partie *pa)
{

	FILE* save = fopen("sauvegarde.txt", "w+t");
	int i, j;


	if (save == NULL)	// Si ouverture non effectuée on affiche un message d'erreur
	{
		printf("\nImpossible d'ouvrir le fichier de sauvegarde");
		fclose(save);
		return 0;
	}

//////////////

	fprintf(save, "%s%s%d%d", pa->nomJoueur1, pa->nomJoueur2, pa->pionJoueurCourant, pa->p->n);


  for(i=0; i < pa->p->n; i++)
  {
		for(j=0; j < pa->p->n; j++)
		{
			fprintf(save, "%d", getCase(pa->p, i, j));
		}
  }

//////////////

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Fonction chargeant la partie en cours.
	Retourne l'adresse de la partie nouvellement créée.

	Si aucune partie n'a été sauvegardée, une nouvelle partie est créée.
*/

partie * chargementPartie()
{
	partie *pa = NULL;
	FILE* save = NULL;
	int i, j;
	//int curseur;
	int val = 0; // pour récupérer les valeurs du tableau
	int ent = 0; // pour récupérer la taille du tableau

	////

	save = fopen("sauvegarde.txt", "r+t"); 

/*	// la fonction fseek place le curseur à la fin du fichier, donc s'il n'y a rien écrit elle doit renvoyer 0 (position curseur)
	curseur = fseek(save, 0, SEEK_END);
                       
	rewind(save); // remettre le curseur au début du fichier*/


	// Si fichier inexistant ou vide on ferme le fichier puis on créer la partie
	if((save == NULL) || !feof(save))
	{
		printf("\nImpossible d'ouvrir le fichier de sauvegarde -- création nouvelle partie\n");
		fclose(save);
		pa = creerPartie();
		return pa;
	}	



//////////////

// Si l'ouverture et la vérification du contenu sont ok on alloue de la mémoire pour la partie et le plateau et on récupère les données
	pa = malloc(sizeof(partie));

	fscanf(save, "%s%s%d%d", pa->nomJoueur1, pa->nomJoueur2, &pa->pionJoueurCourant, &ent);

	pa->p = creerPlateau(ent);

//////////////

  for(i=0; i < pa->p->n; i++)
  {
		for(j=0; j < pa->p->n; j++)
		{
			fscanf(save, "%d", &val);
			setCase(pa->p, i, j, val);
		}
  }

//////////////

	fclose(save);

	return pa;
}
