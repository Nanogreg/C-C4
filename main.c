/*--- Gregory Cuyle ---*/
/* 2010 ISAT Charleroi */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <myconio.h>

#define TRUE 1
#define FALSE 0
#define GAUCHE 75
#define DROITE 77
#define BAS 80
#define ENTER 13
#define J1 1
#define FALSE 0
#define BIP printf("\a");

/*--- Prototypes ---*/
void show_game (void);
void init_mat (int *mat, int lig, int col);
int find_hole (int mat[][7], int ligne, int colonne, int nocol);
unsigned succes (int mat[][7], int ligne,int colonne, int ligcour, int colcour, int joueur);
void display_winner (int gagne,char joueur[10]);
void displayDG(char *chaine, int x1, int x2, int timing, int color);
void displayGD(char *chaine, int x1, int x2, int timing, int color);
unsigned f1tof12(void);
void CenterMessage (char chaine[],int debut, int fin,int ligne,int colortxt,int color_avplan);
//void verifWin(char nom[10],char* winners);

/*--- Le main ---*/
int main(void)
{
    short joueur,couleur,x,y,continuer,i,j;
    int cond,ligne,nocol,mat[6][7], ScWinners[20];
    unsigned gagne,touche;
    char joueur1[10], joueur2[10], winners[20][10];

    /*---Masquage du curseur---*/
    CONSOLE_CURSOR_INFO cci;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hConsole, &cci);
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cci);

    do
    {
        /*--- Initialisation ---*/
        joueur=J1;
        gagne=FALSE;
        for(i=0;i<20;i++)
        {
            strcpy(winners[i],"test\0");
            ScWinners[i]=-1;
        }

        /*--- Affichage de l ecran de jeu ---*/
        Sleep(1000);
        show_game();

        /*--- Init matrice a -1 ---*/
        init_mat(&mat[0][0],6,7);

        /*--- Commencement du jeu ---*/
        /* Affiche les joueurs */
        gotoxy(5,10);textcolor(YELLOW);  printf("Jaune : ");scanf("%s",joueur1);
        gotoxy(5,14);textcolor(LIGHTRED);printf("Rouge : ");scanf("%s",joueur2);
        MYclrwin(1,10,27,14);
        gotoxy(10,10);textcolor(YELLOW);  printf("%s",joueur1);
        gotoxy(10,14);textcolor(LIGHTRED);  printf("%s",joueur2);
        /*verifWin(joueur1,*winners);
        verifWin(joueur2,*winners);*/

        /* Affiche la barre du fond */
        //textbackground(RED);MYclrwin(1,25,80,25);
        textcolor(WHITE);
        gotoxy(2,1);printf("<F1> : Rejouer");
        gotoxy(65,1);printf("<F12> : Quitter");

        do
        {
            /* Affiche le joueur actuel */
            textcolor(LIGHTGRAY);textbackground(BLACK);
            MYclrwin(1,11,27,11);MYclrwin(1,15,27,15);
            if(joueur==J1){gotoxy(10,11);cond=strlen(joueur1);}
            else {gotoxy(10,15);cond=strlen(joueur2);}
            for(i=0;i<cond;i++) printf("-");

            do{
                x=12;
                nocol=3;
                if(joueur==J1) couleur=YELLOW;
                else couleur=LIGHTRED;
                gotoxy(1,5);textbackground(BLACK);clreol(); //Effacage de l'ancien point
                textcolor(couleur);gotoxy(30+x,5);printf("%c",15);
                do{
                    do{
                        touche=getch();
                        if(kbhit())touche=getch();
                        if(!touche)touche=getch();
                        else if(touche==ENTER)touche=BAS;
                    }while(touche!=DROITE && touche!=GAUCHE && touche!=BAS && touche !=134 && touche!=59);

                    switch(touche)
                    {
                        case DROITE : if(x<24){x+=4;nocol++;}break;
                        case GAUCHE : if(x>0){x-=4;nocol--;}break;
                    }
                    gotoxy(1,5);textbackground(BLACK);clreol(); //Effacage de l'ancien point
                    textcolor(couleur);gotoxy(30+x,5);printf("%c",15);
                }while(touche!=BAS && touche!=134 && touche!=59);

                if(touche!=134 && touche!=59)
                    /* On cherche le noligne */
                    ligne=find_hole(mat,6,7,nocol);
            }while((nocol<0 || nocol>6 || ligne==-1) && touche!=59 && touche!=134);

            if(touche!=134 && touche!=59)
            {
                mat[ligne][nocol]=couleur;

                /* On determine la position du point a l ecran en fonction de sa pos dans la matrice */
                switch(nocol)
                {
                    case 0 : x=30;break;
                    case 1 : x=34;break;
                    case 2 : x=38;break;
                    case 3 : x=42;break;
                    case 4 : x=46;break;
                    case 5 : x=50;break;
                    case 6 : x=54;
                }
                switch(ligne)
                {
                    case 0 : y=17;break;
                    case 1 : y=15;break;
                    case 2 : y=13;break;
                    case 3 : y=11;break;
                    case 4 : y=9;break;
                    case 5 : y=7;
                }

                for(i=5;i<y;i+=2)
                {
                    textcolor(couleur);gotoxy(x,i);printf("%c",15);
                    Sleep(100);
                    textcolor(BLACK);gotoxy(x,i);printf(" ");
                }
                textcolor(couleur);gotoxy(x,y);printf("%c",15);
                textbackground(BLACK);

                gagne=succes(mat,6,7,ligne,nocol,joueur);

                if(gagne==FALSE)joueur*=-1;

                continuer=FALSE;
                for(i=0;continuer==FALSE && i<6;i++)
                    for(j=0; continuer==FALSE && j<7;j++)
                        if(mat[i][j]==-1)continuer=TRUE;
                if(continuer==FALSE)gagne=2;
            }
        }while(gagne==FALSE && touche!=59 && touche!=134);

        if(touche!=134 && touche!=59)
        {
            if(joueur==J1) display_winner(gagne,joueur1);
            else display_winner(gagne,joueur2);

            do{touche=f1tof12();}while(touche!=12 && touche!=1);
        }
        if(touche==59)touche=1;
        else if(touche==134)touche=12;

    }while(touche==1);
	return 0;
}

/*--- Les fonctions ---*/
void show_game (void)
{
    short i,j,couleur;
    textbackground(BLACK);MYclrwin(1,1,80,25);
    //textbackground(LIGHTGRAY);MYclrwin(1,1,80,1);
    gotoxy(1,2);
    displayDG("P U I S S A N C E",2,79,15,WHITE);
    Sleep(1000);
    displayDG("4",25,79,15,LIGHTGREEN);
    textbackground(BLACK);textcolor(LIGHTGRAY);

    /* Lignes verticales */
    for(j=0;j<=28;j+=4)
        for(i=0;i<=11;i++)
        {
            gotoxy(28+j,6+i);printf("%c",179);
        }

    /* Ligne horizontales */
    for(j=0;j<14;j+=2)
        for (i=1;i<28;i++)
        {
            gotoxy(28+i,6+j);printf("%c",196);
        }

    /* Cotes */
        /* Bas */
        gotoxy(28,18);printf("%c",192);
        gotoxy(56,18);printf("%c",217);

        /* Haut */
        gotoxy(28,6);printf("%c",218);
        gotoxy(56,6);printf("%c",191);
    /* Millieu */
        for(j=0;j<10;j+=2)
            for(i=0;i<24;i+=4)
            {
                gotoxy(32+i,8+j);printf("%c",197);
            }
        for(i=0;i<24;i+=4)
        {
            gotoxy(32+i,6);printf("%c",194);
            gotoxy(32+i,18);printf("%c",193);
        }
    /* Test couleur */
    /*for(i=0,couleur=1;i<16;i++,couleur++)
    {
        textbackground(couleur);MYclrwin(60,6+i,62,7+i);
    }*/
}
void init_mat (int *mat, int lig, int col)
{
    int i;
    for(i=0;i<(lig*col);i++) *(mat+i)=-1;
}
int find_hole (int mat[][7], int ligne, int colonne, int nocol)
{
    short i;
    int noligne=-1;

    for(i=0;i<ligne && noligne==-1;i++)
        if(mat[i][nocol]==-1) noligne=i;

    return noligne;
}
unsigned succes (int mat[][7], int ligne,int colonne, int ligcour, int colcour, int joueur)
{
    unsigned gagne=0,puissance=0,couleur;
    int lig_par,col_par;

    if(joueur==J1)couleur=YELLOW;
    else couleur=LIGHTRED;

    /* Vertical */
    for(lig_par=ligcour;puissance!=4 && lig_par>=0;lig_par--)
        if(mat[lig_par][colcour]==couleur)puissance+=1;
        else puissance=0;

    if(puissance==4)gagne=1;

    /* Horizontal */
    for(col_par=0;gagne==FALSE && col_par<4;col_par++)
    {
        if(mat[ligcour][col_par]==couleur)
        {
            /* Si alligne de la meme couleur, gagne */
            if(mat[ligcour][col_par+1]==couleur && mat[ligcour][col_par+2]==couleur && mat[ligcour][col_par+3]==couleur)
            gagne=1;
        }
    }

    /* Diagonale vers bas */
    if(gagne==FALSE)
    {
        lig_par=ligcour;
        col_par=colcour;

        /* On cherche le point d attaque */
        while(lig_par>0 && col_par<6)
        {
            lig_par--;
            col_par++;
        }

        /* On verifie si 4 alligne */
        while(gagne==FALSE && lig_par<6 && col_par>0)
        {
            if( mat[lig_par][col_par]==couleur )
                if( mat[lig_par+1][col_par-1]==couleur && mat[lig_par+2][col_par-2]==couleur && mat[lig_par+3][col_par-3]==couleur )
                { gagne=1; }
                else { lig_par++; col_par--; }

            else { lig_par++; col_par--; }
        }
    }

    /* Diagonale vers haut */
    if(gagne==FALSE)
    {
        lig_par=ligcour;
        col_par=colcour;

        /* On cherche le point d attaque */
        while(lig_par>0 && col_par>0)
        {
            lig_par--;
            col_par--;
        }

        /* On verifie si 4 alligne */
        while(gagne==FALSE && lig_par<6 && col_par<7)
        {
            if( mat[lig_par][col_par]==couleur )
                if( mat[lig_par+1][col_par+1]==couleur && mat[lig_par+2][col_par+2]==couleur && mat[lig_par+3][col_par+3]==couleur )
                { gagne=1; }
                else { lig_par++; col_par++; }

            else { lig_par++; col_par++; }
        }
    }
    return gagne;
}
void display_winner (int gagne,char joueur[10])
{
    char message[30]={};
    strcat(message,"Le joueur \"");
    strcat(message,joueur);
    strcat(message,"\" gagne !\0");
    if(gagne==2)
    {
        BIP BIP BIP
        CenterMessage ("Match nul !",1, 80,22,WHITE,BLACK);
    }
    else
    {
        BIP
        gotoxy(32,22);
        CenterMessage (message,1, 80,22,WHITE,BLACK);
    }
}
void displayDG(char *chaine, int x1, int x2, int timing, int color)
{
  int i, j, cpt, arret, ln_chaine;

     textcolor(color);
     ln_chaine=strlen(chaine);
     arret=((x1+x2)/2)-(ln_chaine/2);
     i=x2;

     cpt=1;
     while(arret<=i)
     {
       gotoxy(i,wherey());
       if(cpt<ln_chaine)
       {
          for(j=0;j<cpt;j++) putchar(chaine[j]);
          cpt++;
       }
       else
       {
          MYclrwin(wherex(),wherey(),x2,wherey());
          gotoxy(i,wherey());
          printf("%s",chaine);
       }
       i--;
       Sleep(timing);
     }
}
void displayGD(char *chaine, int x1, int x2, int timing, int color)
{
  int i, j, cpt, arret, ln_chaine;
  char revchaine[81];

     strcpy(revchaine,chaine);
     strrev(revchaine);
     ln_chaine=strlen(chaine);

     textcolor(color);
     arret=((x1+x2)/2)+(strlen(chaine)/2);
     i=x1;
     cpt=1;
     while(arret>i)
     {
       if(cpt<ln_chaine)
       {
          for(j=0;j<cpt;j++)
          {
              gotoxy(x1+j,wherey()); putchar(revchaine[cpt-j-1]);
          }
          cpt++;
       }
       else
       {
          MYclrwin(x1,wherey(),x2,wherey());
          gotoxy(i-(ln_chaine-1),wherey());
          printf("%s",chaine);
       }
       i++;
       Sleep(timing);
     }
}

unsigned f1tof12(void)
{
    unsigned key;

     while(kbhit()) getch();
     key=getch();
     if(!key || key==224)
     {
      key=getch();
      if(key>=59 && key<=68) key-=58;
      else  if (key==133 || key==134) key-=122;
            else key=0;
     }
     else key=0;

     return(key);
}

void CenterMessage (char chaine[],int debut, int fin,int ligne,int colortxt,int color_avplan)
{
    int debutchaine,taille_chaine;
    textbackground(color_avplan);MYclrwin(debut,ligne,fin,ligne);
    textcolor(colortxt);

    taille_chaine=strlen(chaine);
    debutchaine=((fin-debut)/2)-(taille_chaine/2);

    gotoxy(debutchaine+debut,ligne);printf("%s",chaine);
}
/*
void verifWin(char nom[10],char* winners)
{
    short int i,trouve=FALSE,fin=FALSE;

    for(i=0;i<20 && !trouve && !fin;i++)
    {
        if (strcmp(&winners[i],"test")) fin=TRUE;
        else if ( strcmp(nom,&winners[i]) ) trouve=TRUE;
    }
    if (fin) strcpy(&winners[i-1],nom);
}*/
