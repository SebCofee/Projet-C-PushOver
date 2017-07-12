#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <termios.h>
#include <unistd.h>


// couleurs du terminal
  typedef enum
  {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
  } COULEUR_TERMINAL;


//======DEFINITION STRUCTURE=======//

typedef struct{
int *tableau;	// tableau = tableau d'entiers DYNAMIQUE
int n; 			// n = nb de cases par lignes&col
}plateau;


void clear_terminal()
{
  printf("\033[2J");
  printf("\033[0;0H");
}

int color_printf(COULEUR_TERMINAL fg, COULEUR_TERMINAL bg, const char * format, ...)
{
  int res;
  va_list args;
  printf("\x1B[1;%02d;%02dm", fg + 30, bg + 40);
  va_start(args, format);
  res = vprintf(format, args);
  va_end(args);
  printf("\x1B[00m");
  return res;
}


//======INITIATION DU PLATEAU======//


void initPlateau (plateau *P)
{
	int i;
	int *ma_case = P->tableau;
	for(i=0;i < P->n ; i++){
		*ma_case = 0;
		ma_case ++;

	}
}
 //=====CREATION DU PLATEAU=====//
plateau* creerPlateau (int tailleP){		// tailleP=n

	plateau *P;
	assert(tailleP>=2 && tailleP<=8);	//vérificat° si  2<= tailleP <= 8
	P = malloc(sizeof(plateau));		// on alloue dynamiquement des cases mémoires pour P
	P->n=tailleP;
	P->tableau  = malloc((P->n * P->n) * sizeof(int)); // ici on alloue dynamiquement (DANS P) des cases mémoires pour le champ tableau. Chaque case aura une taille de 4 octets, or 															nous avons n*n cases. Donc l'espace mémoire total réservé pour le champ tableau dans P est : nombre de cases * taille en octet 															d'une case.
	initPlateau (P); // fonction pour ne pas avoir de pion
	return P;
}
//=====DESTRUCTION DU PLATEAU=====//
void detruirePlateau (plateau *P){
assert(P != NULL);

// 2 allocations = 2 free

free(P->tableau);
free(P);
}
//=====FONCTION GETCASE=====//
int GetCase (plateau *P, int i, int j)
{

assert( i >=0 && i<=((P->n)-1)  && j>=0 && j<=((P->n)-1));

int *la_case; // la case ciblé = on souhaite atteindre
int *case_1; // la case initiale
int deplacement = 0;
int taille_tableau = P->n;

case_1 = P->tableau;
deplacement = (taille_tableau * i)+j; // (taille de la ligne * nombre de ligne à avancer) + nombre de case/colonne à avancer
la_case = case_1 + deplacement; // case initiale + le déplacement global
return *la_case;
}
//=====FONCTION SETCASE=====//
void setCase(plateau *P, int i, int j,int val)
{

assert(i >=0 && i<=((P->n)-1)  && j>=0 && j<=((P->n)-1));
assert( val>=-1 && val<=1);


int *la_case;
int *case_1;
int deplacement = 0;
int taille_tableau = P->n;

case_1 = P->tableau;
deplacement = (taille_tableau * i)+j; // (taille de la ligne * nombre de ligne à avancer) + nombre de case à avancer
la_case = case_1 + deplacement;
*la_case = val;
}

//===== 1er AFFICHAGE =====//
void affichage_v1(plateau *P){
	int i,j;
	for(i=0;i < P->n; i++)
		{
		for(j=0;j< P->n*P->n;j++)
			printf("%d",GetCase(P,i,j));

		printf("\n");
		}

}

//=====PROGRAMME PRINCIPAL=====//
 int main (){
	int *la_case;
	plateau *P;
	P = creerPlateau (3);

//====== test affichage valeur de la case =====//

printf("Taille tableau = %d\n",P->n);
la_case = P->tableau;
*la_case = 0;
la_case = la_case + 4;
*la_case = 1 ;
printf("1 case = %d\n", GetCase(P,0,0));
printf("2 case = %d\n", GetCase(P,1,1));

detruirePlateau(P);
//=====fin du programme =====//
	return 0;
}

