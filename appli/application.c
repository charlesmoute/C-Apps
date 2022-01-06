// Charles MOUTE --> 03X202
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include "algorithmes.h"


			/*Variables Globales d'interface */


//int kwidth=5; //largeur ou nombre de colonne du filtre que l'on applique a info.image
//int kheight=5; //hauteur ou nombre de ligne que l'on applique a info.image
int normalized=1; //booleen signalant si le resultat d'une operation sur une image doit etre normalisee ou pas



//kernel_* represente un filtre que l'on appliquera a info.image
int kernel [5][5] = {{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,24,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1}};
int kernel_moyenne_passe_bas[3][3] = {{1,1,1},{1,1,1},{1,1,1}}; // filtre moyenne-passe-bas.
int kernel_sobel_1 [3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};//sobel filter.
int kernel_sobel_2 [3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}}; // sobel filter 2.
int kernel_prewitt_1 [3][3] = {{-1,-1,-1},{0,0,0},{1,1,1}}; // filtre de Prewitt 1.
int kernel_prewitt_2 [3][3] = {{-1,0,1},{-1,0,1},{-1,0,1}}; // filtre de Prewitt 2

//int kernel[3][3] = {{1,2,1},{2,4,2},{1,2,1}};
//int kernel[5][5] = {{1,4,6,6,1},{4,16,24,16,4},{6,24,36,24,6},{4,16,24,16,4},{1,4,6,6,1}};
//int kernel[5][5] = {{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,24,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1}};


char file_current [MAX_STRING] ="\0"; //variable représentant le fichier courant
int isFirst = 1 ;
//Menu principale lorsque l'on entre dans le programme pour la première fois.
char* main_menu [] = {
			"1. Choisir une image.",
			"2. Quitter.",
			 0
		     };

//Représente le menu principal lorsqu'une image a déjà était choisie.
char* extend_menu [] = {
				"1. Choisir une image.",
				"2. Visualiser image.",
				"3. Proprietes image.",
				"4. Histogramme image.",
				"5. Traitements sur image.",
				"6. Quitter",
				 0
		       };


//Donne la liste des opérations applicables sur une image.
char* traitement_submenu [] = {
			        "1. Convolution.",
			        "2. Segmentation.",
				"3. Precedent",
			         0
			      };


//Représente les différents filtres applicables pour une convolution.
char* convolution_submenu [] = {
				 "1. Filre moyenne-passe-bas.",
				 "2. Filtre de Sobel 1.",
				 "3. Filtre de Sobel 2.",
				 "4. Filtre de Prewitt 1.",
				 "5. Filtre de Prewitt 2.",
				 "6. Filtre Quelconque.",
				 "7. Visualiser image.",
				 "8. Precedent.",
				  0	
			      };


// Représente les différents options pour une segmentation.
char* segmentation_submenu [] = { 
                                   "1. K-means.",
				   "2. Otsu.",
				   "3. Visualiser image.",
				   "4. Precedent.",
				    0
			        };


/* Permet de mettre en surbrillance le choix d'un utilisateur en attendant sa validation.
   Les paramétres, une liste d'option, l'option en mettre en surbrillance dans la liste des
   colonnes, la ligne et colonne de début où commencer à afficher les options à l'écran.
*/

/* Demande  une saisie qu'elle lit à la position courante du curseur. Elle supprime tout saut 
   de ligne présent à la fin de la ligne*/
void getString(char* chaine) {

  int length ;
  wgetnstr(stdscr,chaine,MAX_STRING);
  length = strlen(chaine);
  if(length >0 && chaine[length-1]== '\n') chaine[length-1]='\0';

}

/* Demande confirmation à l'utilisateur pour une action */
int showMessageConfirm() {

   int confirm = 0 ;
   char answer ;

   mvprintw(QUESTION_ROW,5,"Etes-vous sur ? ");
   clrtoeol();
   refresh();
   cbreak();
   answer = getch();
   if(answer=='o' || answer== 'y' || answer=='O' || answer=='Y') confirm = 1 ;
   nocbreak();

   if(!confirm){
	mvprintw(QUESTION_ROW,1,"     Annulation");
	clrtoeol();
	refresh();
	sleep(1);
   }
   return confirm ;
}

/* Nettoie l'écran et affiche l'objectif de notre application sous forme de titre. */
void clear_screen() {
	clear();
	//attron(A_UNDERLINE);
	mvprintw(2,20,"Application de Traitements d'Image");
	//attroff(A_UNDERLINE);
	if(file_current[0])
	  mvprintw(PROMPT_ROW-2,0,"Le fichier actif est : %s\n",file_current);
	refresh();
}

/* Permet d'afficher le menu avec un element en subrillance */
void display_menu(char* options[],int surbrill_item,int row_begin,int columm_begin){

	int row_current = 0 ;
	char** option;
	char* txt;
	option = options ;
	while(*option){
		if(row_current==surbrill_item) mvaddch(row_begin+row_current,columm_begin-3,ACS_RARROW);
		else  mvaddch(row_begin+row_current,columm_begin-3,' ');		
		txt = options[row_current];		
		mvprintw(row_begin+row_current,columm_begin,"%s",txt);
		row_current++;
		option++;
	}
	mvprintw(row_begin+row_current+5,columm_begin,"Déplacez la surbrillance puis appuyer sur entrée ");
	refresh();
}


/* Permet d'obtenir le choix effectue par l'utilisateur à partir d'un ensemble d'options.*/
int select_item(char* prompt,char* options[]){

	static int row_selected = 0 ;
	int row_max = 0;
	int row_begin = MESSAGE_ROW, col_begin = 10 ;
	char** option;
	int selected;
	int touche = 0 ;
	
	option = options ;

	while(*option)	{
	   row_max++;
	   option++;
	}
	
	if (row_selected >= row_max) row_selected = 0;
	clear_screen();
	mvprintw(row_begin-2,col_begin,prompt);
	
	keypad(stdscr,TRUE);
	cbreak();
	noecho();
	
	touche = 0 ;
	
	while( touche!='q' && touche!=KEY_ENTER && touche!='\n') {
	
	  if(touche==KEY_UP){		
		if(row_selected==0) row_selected = row_max -1 ;
		else row_selected--;
	  }
	
	  if(touche==KEY_DOWN){		
		if(row_selected==(row_max-1)) row_selected = 0 ;
		else row_selected++;
	  }
	
	  selected = *options[row_selected];
	  display_menu(options,row_selected,row_begin,col_begin);
	  touche = getch();
	}

	keypad(stdscr,FALSE);
	nocbreak();
	echo();
	
	if(touche =='q') selected = 'q';
	return (selected);	
}	

void chooseFile(){

	int row_begin = MESSAGE_ROW,col_begin = 10 ;
	clear_screen();
	mvprintw(row_begin-2,1,"{ Path : Home - Select a file }");	
	mvprintw(row_begin,col_begin,"Donnez le chemin d'un fichier image : ");
	getString(file_current);
	free_memory();
	if(file_current[0]){ isFirst = 0; init_variables(file_current); }
	else  isFirst = 1 ;
}


/* Menu principal dans le cas du traitement d'une segmentation */
//1-4
void main_segmentation() {

	int select ;	
	do{
	  select =  select_item ("{ Path : Home - Traitements- Segmentation }",segmentation_submenu);
	  switch(select) {
		case 'q' : break;//mvprintw(ERROR_ROW-4,25,"Quitter");break;
		case '4' : select ='q'; break;//mvprintw(ERROR_ROW-4,5,segmentation_submenu[3]);break;
		case '1' : clear_screen();
			   mvprintw(MESSAGE_ROW,5,"Donnez le nombre de cluster : ");
			   int k = getch();
			   kmeans(k);
			   rewriteImage(getOperationResult());
			   mvprintw(MESSAGE_ROW,5,"%s effectuee avec %d clusters",segmentation_submenu[0]+2,k);
			   getch();
			   break;
			   //mvprintw(ERROR_ROW-4,25,segmentation_submenu[0]);break;
		case '2':  clear_screen();
			   Otsu();
			   rewriteImage(getOperationResult());
			   mvprintw(MESSAGE_ROW,5,"%s effectuee avec un seuil de %d",segmentation_submenu[1]+2,getOtsuSeuil());
			   getch();
			   break; 
			
			   //mvprintw(ERROR_ROW-4,25,segmentation_submenu[1]);break;
		case '3' : clear_screen();
			   visualize(getNameFile());
			   getch();
			   break;
			   //mvprintw(ERROR_ROW-4,25,segmentation_submenu[2]);break;
	  }	
	  refresh();
	}while(select!='q');
}


/* Menu principal dans le cas du traitement d'une convolution */
void main_convolution() {

	int select ;	
	do{
	  select =  select_item ("{ Path : Home - Traitements }",convolution_submenu);
	  switch(select) {
		case 'q' : break;//mvprintw(ERROR_ROW-4,25,"Quitter");break;
		case '8' : select ='q'; break;//mvprintw(ERROR_ROW-4,5,convolution_submenu[7]);break;
		case '1' : clear_screen();
			   initNameFile("_Filre_Moyenne_Passe_Bas");
			   convolve((int*)kernel_moyenne_passe_bas,3,3,normalized);
			   rewriteImage(getOperationResult());
			   mvprintw(MESSAGE_ROW,5,"%s effectuee",convolution_submenu[0]+2);
			   getch();
			   break;
			   //mvprintw(ERROR_ROW-4,25,convolution_submenu[0]);break;
		case '2' : clear_screen();
			   initNameFile("_Filre_Sobel_1");
			   convolve((int*)kernel_sobel_1,3,3,normalized);
			   rewriteImage(getOperationResult());
			   mvprintw(MESSAGE_ROW,5,"%s effectuee",convolution_submenu[1]+2);
			   getch();
			   break;
			   //mvprintw(ERROR_ROW-4,25,convolution_submenu[1]);break;
		case '3' : clear_screen();
			   initNameFile("_Filre_Sobel_2");
			   convolve((int*)kernel_sobel_2,3,3,normalized);
			   rewriteImage(getOperationResult());
			   mvprintw(MESSAGE_ROW,5,"%s effectuee",convolution_submenu[2]+2);
			   getch();
			   break;
			    //mvprintw(ERROR_ROW-4,25,convolution_submenu[2]);break;
		case '4' : clear_screen();
			   initNameFile("_Filre_Prewitt_1");
			   convolve((int*)kernel_prewitt_1,3,3,normalized);
			   rewriteImage(getOperationResult());
			   mvprintw(MESSAGE_ROW,5,"%s effectuee",convolution_submenu[3]+2);
			   getch();
			   break;
			   //mvprintw(ERROR_ROW-4,25,convolution_submenu[3]);break;
		case '5' : clear_screen();
			   initNameFile("_Filre_Prewitt_2");
			   convolve((int*)kernel_prewitt_2,3,3,normalized);
			   rewriteImage(getOperationResult());
			   mvprintw(MESSAGE_ROW,5,"%s effectuee",convolution_submenu[4]+2);
			   getch();
			   break;
			   //mvprintw(ERROR_ROW-4,25,convolution_submenu[4]);break;
		case '6' : clear_screen();
			   initNameFile("_Filre_Quelconque");
			   convolve((int*)kernel,5,5,normalized);
			   rewriteImage(getOperationResult());
			   mvprintw(MESSAGE_ROW,5,"%s effectuee",convolution_submenu[5]+2);
			   getch();
			   break;
			    //mvprintw(ERROR_ROW-4,25,convolution_submenu[5]);break;
		case '7' : clear_screen();
			   visualize(getNameFile());
			   getch();
			   break;
			   //mvprintw(ERROR_ROW-4,25,convolution_submenu[6]);break;
	  }	
	  refresh();
	}while(select!='q');
}


/* Menu principal dans le cas du sous-menu traitement */
void main_traitement() {

	int select ;	
	do{
	  select =  select_item ("{ Path : Home - Traitements }",traitement_submenu);
	  switch(select) {
		case 'q' : break;//mvprintw(ERROR_ROW-4,25,"Quitter");break;
		case '3' : select ='q'; break; //mvprintw(ERROR_ROW-4,5,traitement_submenu[2]);break;
		case '1' : main_convolution();  break;//mvprintw(ERROR_ROW-4,25,traitement_submenu[0]);
		case '2' : main_segmentation(); break;//mvprintw(ERROR_ROW-4,25,traitement_submenu[1]);
	  }	
	  refresh();
	}while(select!='q');
	initNameFile(NULL);
}

int main() {

	int select;
	initscr();	
	start_color();
	do{
	
	   select =  select_item ("{ Path : Home }",file_current[0]?extend_menu:main_menu);
	   switch(select) {
		case 'q' : //mvprintw(ERROR_ROW-3,5,"Quitter");
			   break;
		case '6' : //mvprintw(ERROR_ROW-3,5,extend_menu[5]);
			   select = 'q';
			   break;
		case '2' :
			   if(isFirst) select = 'q';
			   else{
			    clear_screen();
			    visualize(getImageName());
			    getch();
			   }
			   break ;		
		case '1' : chooseFile();
			   //mvprintw(ERROR_ROW-3,5,main_menu[0]);
			   break ;
		case '3' : clear_screen();
			   displayImageProperties();
			   getch();
			   //mvprintw(ERROR_ROW-3,5,extend_menu[2]); 
			   break ;
		case '4' : //mvprintw(ERROR_ROW-3,5,extend_menu[3]);
			   clear_screen();
			   displayImageHistogramme();
			   getch();
			   break ;
		case '5' : main_traitement();
			   break ;//ivprintw(ERROR_ROW-3,5,extend_menu[4]);
	   }
	    refresh();
	}while(select!='q');
	endwin();
	free_memory();
	exit(EXIT_SUCCESS);
}
