#include <stdio.h>
#include "plateau.h"


int sauvegardePartie(partie*pa)
{
	int i;
	int max = (pa->p->n)*(pa->p->n)
	FILE *save = fopen("save.txt","rt");
	
	if (save == NULL)
	{
		printf("la sauvegarde n'existe pas, création d'un sauvegarde vide.\n");
		save = fopen ("save.txt","wt");
	}
	else
	{
		printf("mise à jour de la sauvegarde.\n");
		save = fopen ("save.txt","wt")
	}
	fprintf(save,"%s,%s,%d \n",pa->nomJoueur1,pa->nomJoueur2,pionJoueurCourant);
	
	for(i=0;i<max;i++)
		fprintf(save," %d ",pa->p->tableau[i])
	
	
	fclose(save);
		
}

partie * chargementPartie()
{



}

int menu(partie *pa)
{
	
	
	
	
}