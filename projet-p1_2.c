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

//=====2eme AFFICHAGE=====//
void affichage_v2(plateau *P){
	assert(P != NULL && "il ny a pas de plateau a afficher");
	int i,j,k,val;
	for(i=0;i < P->n; i++){

		for(j=0;j< P->n;j++)
			printf("* * * * ");
		printf("*\n");
		printf("* ");
		
		for(j=0;j< P->n;j++){
			for(k=0;k< 6;k++) {
				printf(" ");
			}
			printf("* ");
		}
		printf("\n");

			
		for(j=0;j< P->n;j++){
			val=GetCase(P,i,j);
				if(val<0)
					printf("*   B   ");
				else if(val>0)
					printf("*   N   ");
				else
					printf("*       ");	
		}			
		printf("*\n");
		printf("* ");
		for(j=0;j< P->n;j++){
			for(k=0;k< 6;k++) {
				printf(" ");
			}
			printf("* ");
		}
		printf("\n");
	}

	for(j=0;j< P->n;j++)
		printf("* * * * ");
	printf("*\n");

}

//====== fonction IndiceValide =====//
int indiceValide(plateau *P , int indice)
{	
	int indice_v;
	if( indice > 0 && indice <= P->n-1)		/* test entre 0 & n-1, si valide = dans le tableau(carré) donc pas besoin de faire test avec colonne*/
		indice_v = 1;
	else
		indice_v = 0;

	return indice_v;						/* retourne 1 ou 0*/				
}
// ===== CASEVALIDE ======//
int caseValide (plateau *P, int indiceLigne,int indiceColonne)
{
int indice_v;

	if( indiceValide(P,indiceLigne)==1 && indiceValide(P,indiceColonne)==1 )/* on utlise la fonction IndiceValide = indique à la fois si c'est bien entre la ligne et la colonne*/
		indice_v = 1;
	else
		indice_v = 0;

	return indice_v;
}
//====== CASEVIDE ======//

int caseVide (plateau *P, int i, int j)
{
	 if (GetCase(P,i,j)==0)		/*GetCase permet d'attendre une case et possède les mêmes paramètres que caseVide. Si GetCase = 0, alors case = vide*/
		return 1;
	else 	
		return 0;

}
//===== insertionPionPossible =====//
int insertionPionPossible (plateau *P, int ligne, int col, int di, int dj,int pion)		/* vérifie juste si on peut insérer un pion*/
{
 	int retour = 1;														/* retourne 1 si les tests suivants = vrais*/

	if (ligne == 0 && col <= P->n-1 && di == 1 && dj == 0);				/* test pr 1ère ligne du tableau = bord supérieur*/
	else if (ligne== P->n-1 && col <= P->n-1 && di == -1 && dj == 0);	/*test bord inférieur*/
	else if (ligne<= P->n-1 &&  col == 0 && di == 0 && dj == 1);		/*test coté gauche*/
	else if (ligne<= P->n-1 &&  col == P->n-1 && di == 0 && dj == -1);	/*test coté droit*/
	else 																
		retour = 0;														/* si test = faux, retour vaut 0*/

return retour;

}

//=====insertion pion ======//
void insertionPion (plateau *P, int ligne, int col, int di, int dj, int pion)		/*effecue l'insertion*/
{
	int i, insert_v = insertionPionPossible(P, ligne , col, di, dj, pion); /* vérifie avec la conftion ci-dessus si l'isertion est possible */
	int n = P->n;
	if (insert_v == 1)
	{
		printf ("insertion possible\n");
		if( di == 0 && dj == 1)
		{
			for(i=n-1;i>=1;i--)
			{
			setCase(P,ligne, i, GetCase(P,ligne,i-1));	/* insertion pion = modif° valeur de la case, donc util° setCase, avec en paramètre GetCase car le déplacement fait que la case 															i va contenir la valeur de la case i-1.  Ici la ligne est statique et la col dynamique. Ici c'est tant que i>=1 car à i=0 = 															insertion du pion*/

			}
			setCase(P,ligne,0,pion);					/*change la couleur de la case où a lieu l'insertion*/
		}
		else if( di == 1 && dj == 0)
		{
			for(i=n-1; i>=1; i--;)
			{
			setCase(P,i,col, GetCase(P,i-1,col));
			}
			setCase(P,0, col, pion);
		}
		else if( di == -1 && dj == 0)
		{
			for(i=0; i<n-1; i++;)				/*att° on part de la ligne 0 vers n-1,dc i++*/
			{
			setCase(P,i,col, GetCase(P,i+1,col));
			}
			setCase(P,n-1, col, pion);
		}
		else if( di == 0 && dj == -1)
		{
			for(i=0;i<n-1;i++;)
			{
			setCase(P,ligne, i, GetCase(P,ligne,i+1));

			}
			setCase(P,ligne,n-1,pion);
		}

	}
	else
		printf("insertion impossible\n");		/* si test verifie que insertion est impssble*/


}
//===== gagne =====//
int gagne(plateau *P)		/* parcours cas pour connaître valeur + création tableau pour retenir score des joueurs*/
{
int J[3] = {0,0,0}; 
int ligne, col;
	for(ligne=0;ligne<P->n;ligne;)
	{
		col=0;
		while(GetCase(P,ligne,col) = GetCase(P,ligne,col+1) && col<P->n-1)
		{
		col++;
		}
		if(col=P->n) { J[GetCase(P,ligne,col-1)+1]++;)

	}
	for(col=0;col<P->n;col++;)
	{
		ligne=0;
		while(GetCase(P,ligne,col) = GetCase(P,ligne+1,col) && ligne<P->n-1)
		{
		ligne++;
		}
		if(ligne=P->n) { J[GetCase(P,ligne-1,col)+1]++;)

	}

	if (J[0] == 0 && J[2] == 0)
		return 0;

	if (J[0] == J[2])
		return 2;

	if ( J[0] > J[2])
		return -1;
	else 
		return 1;





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
affichage_v2(P);
printf("\n\n");

initPlateau(P); // on remet le plateau à 0 pour tester "SetCase"

//===== test de la fonction SetCase =====//
setCase(P,0,0,1);
setCase(P,2,2,-1);
affichage_v2(P);// on affiche le plateau, normalement la case en haut a gauche vaut 1 donc affichera "N" et la case en bas a droite affichera "B"

//====== test fonction indice valide ======//
printf("indice = %d\n", indiceValide(P,15));
printf("indice = %d\n", indiceValide(P,2));
printf("indice = %d\n", indiceValide(P,8));

//===== test caseValide ====//
printf("indice-case = %d\n", caseValide(P,0,1));
printf("indice-case = %d\n", caseValide(P,1,2));
printf("indice-case = %d\n", caseValide(P,15,18));

//==== test casevide ======//
printf("case = %d\n", caseVide(P,0,0));
printf("case = %d\n", caseVide(P,1,1));
printf("case = %d\n", caseVide(P,2,2));

//====== insertionPionPossible ======//
printf("insert = %d\n",insertionPionPossible(P,0,1,1,0,1));
printf("insert = %d\n",insertionPionPossible(P,1,0,0,1,0));
printf("insert = %d\n",insertionPionPossible(P,P->n-1,1,-1,0,1));
printf("insert = %d\n",insertionPionPossible(P,1,P->n-1,0,-1,0));
printf("insert = %d\n",insertionPionPossible(P,1,P->n-1,0,1,1));

//===== insertion pion =====//
insertionPion(P,1,0,0,1,1);
affichage_v2(P);
insertionPion(P,0,1,1,0,-1);
affichage_v2(P);
insertionPion(P,P->n-1,1,-1,0,1);
affichage_v2(P);
insertionPion(P,1,P->n-1,0,-1,-1);
affichage_v2(P);


detruirePlateau(P);
affichage_v2(P); //affichera un message d'erreur si le plateau n'existe pas (et c'est ce que l'on veut).
//=====fin du programme =====//
	return 0;
}

