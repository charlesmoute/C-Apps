// Charles MOUTE --> 03X202
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <curses.h>


#define taille 256
#define diese '#'
#define p5  "P5"
#define p6  "P6"


		/*	Types  	*/


			/*Constantes pour interfaces */

#define KEY_ESCAPE     27         /*  Représente le bouton echape  */
#define MAX_STRING     1024	  /*  Longueur maximale d'une réponse */
//#define MAX_NAMEFILE   80       /*  Longueur max d'un chemin menant à un fichier image */
#define MESSAGE_ROW    6          /*  Ligne affichant des messages divers */
#define ERROR_ROW      22	  /*  Ligne signalant les messages  */
#define QUESTION_ROW   20	  /*  Ligne pour les questions   */
#define PROMPT_ROW     18	  /*  LIGNE de prompt	*/


// Type definissant les proprietes d'une image.
typedef struct image_properties {
	int width  ;    // largeur de l'image.
	int height ;    // hauteur de l'image.
	char* format;   // Format de  l'image.
	int valuePixel; // valuer maximale des pixels de l'image.
	char* nameFile; // nom du fichier image.
	unsigned char* image; // contenu de l'image.
} properties ;


// Type definissant la structure d'un element dans l'algorithme de k-means.
typedef struct item_kmeans {
	int numCluster ; // Represente le numero de cluster au quel l'elt est affecte.
	int distance ; // Represente la distance de l'elt au centre du cluster auquel il appartient.
} item ;

// Type definissant la structure d'un cluster dans l'algorithme de k-means.
// Le centre d'un cluster est la moyenne des valuers des pixels qu'il contient.
typedef struct cluster_kmeans {
	int centroide; //Represente le centre du cluster.	
	int size; //Represente le nombre d'elt contenu dans le cluster.
} cluster ;

		/*	Variables  	*/

properties info ;  // proprietes d'une image de nom info.nameFile.
int* histogramme ; // est l'histogramme de info.nameFile. de taille valuePixel.
int* image; // image est la representation sous forme matricielle de info.image.
int* result;// result est l'image resultante de l'application d'une operation sur info.image.


int otsuSeuil ; //represente le seuil donnee par l'algorithme de otsu.

int K ; // Represente le nombre de cluster que k-means doit definr.
int variance_limit = 100; //Represente la limite en dessous de laquelle k-means ne doit effectue de traitement.
int min_modif;//Represente le min de modification de clusters en dessus du quel on les considere instables.
cluster* clusters; // Represente les k clusters que k-means devra definir.
item* items; //Representent les pixels de l'image regroupés en cluster.


char* nameFile ; //Nom donne a l'image resultante des operations subies par info.image.


				/*	Operations	*/


// Initialise toutes les variables par des valeurs par defaut.
void default_init() {
	
	int i,j;

	/*pour info*/
	info.width = -1 ;
	info.height = -1 ;
	info.format = NULL ;
	info.valuePixel = -1 ;
	info.nameFile = NULL ;
	info.image = NULL ;
        //info.image = (unsigned char*)malloc(taille*taille);

	/*pour histogramme de l'image */
	histogramme = NULL ;

	// pour le nom de fichier
	nameFile = NULL ;
	
 	//pour la representation matricielle de info.image
	image = NULL ;

	//pour le resultat matricielle d'une operation subie par info.image
        result = NULL ;

	//pour seuil de otsu
	otsuSeuil = -1 ;

	//Pour l'algorithme de k-means
	K = -1 ;
	clusters = NULL ;
	items = NULL ;

}


// Libere les espaces memoires alloues
void free_memory() {
	
	if (items!=NULL)	 { free(items); items = NULL; }	
	if (image!=NULL)         { free(image); image = NULL; }
	if (result!=NULL)        { free(result); result = NULL; }
	if (clusters!=NULL)	 { free(clusters); clusters =  NULL; }	
	if (nameFile!=NULL)      { free(nameFile); nameFile = NULL; }	
	if (info.format!=NULL)   { free(info.format); info.format = NULL; }	
	if (info.image!=NULL)    { free(info.image);  info.image = NULL ; }
	if (histogramme!=NULL)   { free(histogramme); histogramme = NULL ; }
	if (info.nameFile!=NULL) { free(info.nameFile); info.nameFile = NULL; }
	
}

// Retourne la largeur de l'image courrante.
int getImageWidth() { return info.width ; }

//Retourne la hauteur de l'image courante.
int getImageHeight() { return info.height ; }

//Retourne la valeur maximale des pixels de l'image.
int getImageMaxValuePixel() { return info.valuePixel; }

//Retourne le format de l'image courante.
char* getImageFormat() { return info.format ; }

//Retourne le nom de l'image courante.
char* getImageName() { return info.nameFile ; }

//Retourne le contenu de l'image courante.
unsigned char* getImageContent() { return info.image ; }

//Retourne le nom de l'image modifiee.
char* getNameFile() {return nameFile; }

//Retourne le resultat effectue sur l'image.
int* getOperationResult() { return result; }

//Retourne le seuil de Ostu.
int getOtsuSeuil() { return otsuSeuil; }

//Obient les proprietes du fichier passee en parametre
void getPropertiesOf(char* file) {

	FILE *f;
        char *row =(char*)malloc(taille),*param=(char*)malloc(taille);

	f = fopen(file,"r");

	if(f) {

	    // Recherche format de l'image
	   fgets(row,taille,f);
	   info.format = (char*) malloc((sizeof(char))*(strlen(row))) ;
	   strcpy(info.format,row);
	   info.format[(strlen(row))-1] = '\0';
			
	   if (strcmp(p5,info.format)==0 || strcmp(p6,info.format)==0 ) {

		if(!feof(f)){
			
			fgets(row,taille,f);

			//Effacement des eventuels commentaires dans le fichier
			while(row[0]==diese&&!feof(f)){
				fgets(row,taille,f);
			}
			
			// Recherche des dimensions de l'image
			if(row[0]!=diese){
			   row[strlen(row)]='\0';
			   info.width = atoi(strtok(row," "));
			   info.height = atoi(strtok(NULL," "));
				
			   // Recherche de la valeur maximale des pixels
			   if(!feof(f)){
			        fgets(row,taille,f);
				row[strlen(row)]='\0';
				info.valuePixel = atoi(strtok(row," "));

				info.nameFile =  (char*)malloc( (sizeof(char))* (strlen(file)) );	
				strcpy(info.nameFile,file);
			
				if(!feof(f)){
				   info.image = (unsigned char*)malloc(info.width*info.height);
				   fread(info.image,sizeof(unsigned char),info.width*info.height,f);
				}
			   }
			}
		}
	   } else{
	     mvprintw(ERROR_ROW,5,"Le fichier %s n'est pas au format P5 ou P6",file);
	     free_memory();
	   }
	   fclose(f);
    	}else {
	   mvprintw(ERROR_ROW,5,"Impossible d'ouvrir %s",file);
	   free_memory();

        }
	free(row);
	free(param);
}


//Initialise le nom que l'on donnera a l'image resultante d'une operation sur info.image

void initNameFile(char* nameUser){

	//En cas de PB enlever les commentaires et les mettre ou ils doivent se trouvent

	char* file,*nomFichier,*extension,str[]="_modified",point[]=".",*name,ext[]=".pgm";
	
	if(nameFile!=NULL) { free(nameFile); nameFile = NULL; }
	
	if(info.nameFile!=NULL) {

	   name = (char*)malloc(strlen(info.nameFile)+1);
	   sprintf(name,"%s\0",info.nameFile);

	   file = (strrchr(name,'/')+1);
 
           if(file==NULL) file = (strrchr(name,'\\')+1);
	
	   if(file==NULL) file = name ;

	   //extension = strrchr(file,'.')+1;
	   nomFichier = strtok(file,".");
	
	  if(nameUser!=NULL){	

		char ext_name [strlen(nameUser)+1];
		sprintf(ext_name,"%s",nameUser);

		nameFile=(char*)malloc(strlen(nomFichier)+strlen(ext_name)+strlen(ext)+1);
	   	sprintf(nameFile,"%s%s%s",nomFichier,ext_name,ext);
		
		//nameFile=(char*)malloc(strlen(nomFichier)+strlen(ext_name)+strlen(point)+strlen(extension)+1);
	   	//sprintf(nameFile,"%s%s%s%s",nomFichier,ext_name,point,extension);
		
	   }else{
		
		nameFile = (char*)malloc(strlen(nomFichier)+strlen(str)+strlen(ext)+1);
	   	sprintf(nameFile,"%s%s%s",nomFichier,str,ext);

		//nameFile = (char*)malloc(strlen(nomFichier)+strlen(str)+strlen(point)+strlen(extension)+1);
	   	//sprintf(nameFile,"%s%s%s%s",nomFichier,str,point,extension);
	  }
	  free(name);
	}
}

//Obtient l'histogramme de l'image courante
void getImageHistogramme() {

	if(info.image!=NULL){
		int i ,length ;
		int max = taille;
		//int max = info.valuePixel+1;
		if(max!=-1){
			histogramme =(int*)malloc((sizeof(int))*max);
			for(i=0;i<max;i++) histogramme[i] = 0;
			length = strlen(info.image);
			for(i=0;i<length;i++) histogramme[info.image[i]]++;
		}

	}

}

// Creer la forme matricielle de l'image courante
void createImageMatrix() {

	int i,j,k=0 ;

	if(info.image!=NULL) {

	   image = (int*)malloc((sizeof(int))*(info.height*info.width));
	   int* matrix = image ;

	   for(i=0;i<info.height;i++) for(j=0;j<info.width;j++) {*matrix = 0 ; matrix++;}

	   matrix = image ;
	   for(i=0;i<info.height;i++) {
	      for(j=0;j<info.width;j++) {
		 *matrix = (int) info.image[k];
		  matrix++;
		  k++;
	      }
	   }
	
	}

}


//Obtient d'une representation matricielle le contenu d'une image.
unsigned char* getImageOf(const int* matrix) {

	if(info.image!=NULL){
		unsigned char* _image = (unsigned char*)malloc(info.height*info.width);
		int i,j,k=0,val;
		for(i=0;i<info.height;i++){
			for(j=0;j<info.width;j++){
	
				val = (unsigned char) (*matrix);
				if(val>info.valuePixel) val = info.valuePixel ;
				if(val<0) val = 0 ;
	
				_image[k] = val ;
				k++;
				matrix++;
			}		
		}
		return _image ;
	}else return NULL ;

}

// Obtient de la matrice courante, le contenue de l'image originelle
unsigned char* getImageOfCurrent() {
	return getImageOf(image) ;
}


/* Obtient de la matrice resultante d'une operation sur l'image originelle.
   Le contenue de l'image originelle modifiee par l'application de l'operation.
*/

unsigned char* getImageOfResult() {
	return getImageOf(result) ;
}


//Initialise toutes les variables en regroupant les fonctioons adequates
void init_variables(char* file){
	
	default_init();
	getPropertiesOf(file);
	initNameFile(NULL);
	getImageHistogramme();	
	createImageMatrix();	
}


// Affiche a l'ecran les proprietes de l'image courante, celle stockee dans info.image
void displayImageProperties(){

    mvprintw(MESSAGE_ROW,5,"Image :%s",info.nameFile);
    mvprintw(MESSAGE_ROW+2,5,"-----     Proprietes de l'image     -----");
    mvprintw(MESSAGE_ROW+4,5,"    Format de l'image  :%s",info.format);
    mvprintw(MESSAGE_ROW+5,5,"    Largeur de l'image :%d",info.width);
    mvprintw(MESSAGE_ROW+6,5,"    Hauteur de l'image :%d",info.height);
    mvprintw(MESSAGE_ROW+7,5,"    Valeur Max des pixels de l'image :%d",info.valuePixel);

}


void displayContent(const unsigned char * _image){

    int i,size ;
	
    if(_image==NULL) 
	mvprintw(MESSAGE_ROW+2,5,"    Contenue de l'image : [ Empty Image ] ");
    else {
	mvprintw(MESSAGE_ROW+2,5,"    Contenue de l'image :   ");
	size = strlen(_image);
	mvprintw(MESSAGE_ROW+3,5,"[");
	for(i=0;i<size;i++){
		if(i==(size-1 )) printw("%d",_image[i]);
		else printw("%d, ",_image[i]);
	}
	printw("]");
    }
		
}

//Affiche a l'ecran le contenu de l'image :
void displayImageContent() {
    mvprintw(MESSAGE_ROW,5,"-----     Image originelle     -----");
    displayContent(info.image);
}


// Affiche a l'ecran l'histogramme de l'image courante, celle stockee dans info.image
void displayImageHistogramme(){
   int j,length ;
   mvprintw(MESSAGE_ROW,5,"-----     Histogramme de %s     -----",info.nameFile);
   if(histogramme==NULL) printw("   [ Empty Histogramme-Aucune Image de selectionnee ]");
   else {
	length = info.valuePixel+1;
	printw("[");
	for(j=0;j<length;j++){
	    if(j==(length-1)) printw("%d",histogramme[j]);
	    else printw("%d, ",histogramme[j]);
	}
	printw("]");
   }
}



// Affiche a l'ecran le contenu de la representation matricielle, _image, du fichier file.
void displayMatrix(char* file, int* _image){
	int i,j;	
	mvprintw(MESSAGE_ROW+1,5,"    Matrice de %s  :   ",file);
	if(_image==NULL) printw("[ Empty Matrix ]");
	else {
		int* matrix = _image ;
		printw("[");
		for(i=0;i<info.height;i++){
			printw("[");
			for(j=0;j<info.width;j++){
				if(j==(info.width-1)) printw("%d",*matrix);
				else printw("%d,",*matrix);
				matrix++;
			}
			if(i==(info.height-1)) printf("]");
			else printw("],\n");		
		}
		printw("]");
	}
}


/* Affiche a l'ecran le contenu de la matrice de l'image courante. Celle contenu dans info.image */
void displayImageMatrix() {
	mvprintw(MESSAGE_ROW,5,"-----     Image originelle     -----");
	displayMatrix(info.nameFile,image);
}


/* Affiche a l'ecran le contenu de la matrice resultante d'une operation sur l'image courante.*/
void displayResult() {	
	mvprintw(MESSAGE_ROW,5,"-----     Resulat Operation sur image originelle     -----");
	displayMatrix(nameFile,result);
}


/* Cree une image de nom nameFile, de largeur info.width, de hauteur info.height et
   de valeur maximale de pixel info.valuePixel a partir du contenu de la representation matricielle d'une
   image, nomee matrix.

*/
void rewriteImage(int* matrix){
	
    FILE *f;
    if(nameFile!=NULL && matrix!=NULL) {
	f = fopen(nameFile,"w");
	if(!f) mvprintw(ERROR_ROW,5,"Erreur::Impossible de creer le fichier %s",nameFile);	
	else {
		
		//fputs("P5\n",f);	
		fprintf(f,"%s\n",info.format);
		fputs("#File modified by Moute Charles. Matricule 0x202\n",f);
		fprintf(f,"%d %d\n",info.width,info.height);
		fprintf(f,"%d\n",info.valuePixel);
		fwrite(getImageOf(matrix),sizeof(unsigned char),info.width*info.height,f);
		fclose(f);
	}
    }else mvprintw(ERROR_ROW,5,"Erreur::L'image ayant subie les modifications n'a pas pu etre creee.");
}


			/*	Traitement avec la convolution	*/


/* Retourne une valeur de pixel de la matrice image de largeur witdh et de hauteur height. Tout en 
   s'assurant que les coordonnees du pixel soient bien dans l'image. */

int getValuePixel(int x,int y, int* _image,int width,int height){
	if(x<=0) x = 0;
	if(x>=width) x= width-1;
	if(y<=0) y = 0;
	if(y>=height) y= height-1;
	if(_image!=NULL) return (*(_image+(x+y*width)));
	else return 0.0 ;
}


/* Effectue une convolution sur l'image image de largeur witdh et de hauteur height, avec le filtre kernel
   de largeur kw et de hauteur kh tout s'assurant si le resultat,result, de la convolution est normalise
   , normalized!=0 , ou pas , normalized==0
*/
void convolve(int* _kernel,int kw ,int kh,int _normalized){
	
	
	if (image==NULL)   
	   mvprintw(ERROR_ROW,5,"Erreur::La forme matricielle de l'image n'est pas initialisee.");
	if (_kernel==NULL) mvprintw(ERROR_ROW,5,"Erreur::Le noyau de Convolution n'est pas initialise.");
	
        if(image!=NULL && _kernel!=NULL){
	
	  if(result!=NULL) { free(result); result=NULL; }

	  if( (info.width<1) || (info.height<1) )
	   mvprintw(ERROR_ROW,5,"La dimension %d x %d representant celle de l'image est incorrecte",info.width,info.height);
	  else if( ( (kw%2)==0)|| ( (kh%2) ==0) ) 
	   mvprintw(ERROR_ROW+1,5," La dimension du noyau de convolution %dx%d ne doit avoir que des nombres impaires",kw,kh);
	  else{
	     int x,y,u,v,uc,vc,i,j;
	     int limitX,limitY ; 
	     float scale = 1;
	     int *k= _kernel,sum=0;
		
	     result = (int*)malloc( (sizeof(int))*(info.width*info.height) );
	     for(i=0;i<info.height;i++) for(j=0;j<info.width;j++) *(result+i+j) = 0 ;

	     if(_normalized!=0){
		for(i=0;i<kw;i++) for(j=0;j<kh;j++){ sum+=(*k); k++; }
		if(sum!=0) scale/=sum ;
		sum = 0 ;
	     }

              uc = kw/2;
	      vc = kh/2;
	      limitX = info.width - uc ;
	      limitY = info.height - vc ;
	
             for(y=0;y<info.height;y++){
		for(x=0;x<info.width;x++){
		    k = _kernel ;
		    sum = 0;
		    int edgePixel =( (y<vc) || (y>=limitY) || (x<uc) || (x>=limitX) ) ;
		    for(v=-vc;v<=vc;v++){
			int offset = x+(y+v)*info.width;
			for(u=-uc;u<=uc;u++){
			    if(edgePixel==1)  sum+=getValuePixel(x+u,y+v,image,info.width,info.height) * (*k);
			    else sum+= (*(image+offset+u))*(*k);
			    k++;
			}
		    }
		    if(sum<0) sum = 0 ;
		    if(sum>255) sum =255 ;
		    *(result+(x+y*info.width)) = (sum*scale) ; //(float) (sum*scale) ;
		}	
	     }
	  }

	}
}


//Permet de visualiser sur kview l'image passee en parametre.
void visualize(char* fileImage) {

	pid_t pid ;
	
	if(fileImage!=NULL){		

		char file[strlen(fileImage)+1] ;
		sprintf(file,"%s",fileImage);
		pid = fork();
		if(pid==-1) 	
		   mvprintw(ERROR_ROW,5,"Erreur:: L'image %s ne peut etre visualisee. Faite le manuellement.",file);
		else if(pid==0){
			mvprintw(MESSAGE_ROW,5,"Visualisation de %s",file);
			//recouvre le code executable herite du pere par  celui de la commande kview nameFile
			execlp("kview","kview",file,NULL);
		}else{
			wait();	
			mvprintw(MESSAGE_ROW+1,5,"Arret de la Visualisation");
		}
		
	}
	
}

			/*	Segmentation  avec l'Algorithme d'Otsu	*/


// Calcule la somme du sous-partie de l'histogramme
double computePartOfHistogramme(int indexBegin,int indexEnd){

   double soe = 0 ;
   int i ;
   if(histogramme!=NULL && indexBegin>=0 && indexEnd<=info.valuePixel && indexBegin<=indexEnd){
   	for(i=indexBegin;i<=indexEnd;i++) soe+= (double)histogramme[i];
   }
   return soe ;

}


// Calcule une partie de la variance d"une sous  partie  de l'histogramme
double computePartOfVariance(int indexBegin,int indexEnd,double moyenne){

   double soe = 0 ;
   int i ;
   if(histogramme!=NULL && indexBegin>=0 && indexEnd<=info.valuePixel && indexBegin<=indexEnd){
   	for(i=indexBegin;i<=indexEnd;i++) soe+= pow( ((double) histogramme[i]) - moyenne,2.0);
   }
   return soe ;

}


// Calcule du seuil par l'algorithme d'Otsu
void OtsuAlgorithm() {

	int T ;
	double part1,part2,u1,u2,var1,var2,p1,p2,aux,var,dim = info.height*info.width ;	
	otsuSeuil = -1 ;
	if(image!=NULL) {

		int valeur = (info.valuePixel > 255)? 255:info.valuePixel ;

		for(T=1;T<=valeur;T++){
		
		   part1 = computePartOfHistogramme(0,T-1);
		   part2 = computePartOfHistogramme(T,255);
		
		   u1 = part1/T ;
		   u2 = part2/(256-T);
	
		   var1 =  computePartOfVariance(0,T-1,u1);
		   var2 =  computePartOfVariance(T,255,u2);
		
		   p1 = part1/dim ;
		   p2 = part2/dim;
		
		   aux = (p1*var1)+(p2*var2);

		   if(T==1) { var = aux; otsuSeuil = T; }

		   if(aux<var) { var = aux; otsuSeuil = T; }

		}
	}
}


// Modifie l'image selon le seuil trouvé par l'algoritme d'otsu.
void applyOtsuOnImage() {

	if(result!=NULL) { free(result); result=NULL; }

	if(otsuSeuil!=-1 && image!=NULL) {
	   int i,j;
	   result = (int*)malloc( (sizeof(int))*(info.height*info.width) );
	   for(i=0;i<info.height;i++){
		for(j=0;j<info.width;j++){
		   if( (* (image+j+i*info.width) ) < otsuSeuil ) *(result+j+i*info.width) = 0 ;
		   else *(result+j+i*info.width) = 255 ;
		}
	   }
	}

}


// L'algorithme d'Ostu de facon atomique
void Otsu() {

	char ext_name [] = "_Otsu";
	initNameFile(ext_name);
	OtsuAlgorithm();
	applyOtsuOnImage();
}



/*			Segmentation avec k-means	*/


// Attribue un niveau de gris pseudo-aléatoire au centre du cluster i. 0 < i < k+1 
int ramdomGrayLevel(int i) {

	int couleur = 0 ;
	int valeur ;	
	
	if(K>0){
		
		if(i>0 && i<=K){
		
			valeur = 256/K ;
			if(valeur==0) couleur = K%256;
			else {
				if(i==1) couleur=0;
				else { couleur = valeur*i ; if(couleur>255) couleur = 255; }
			}
	
		}else mvprintw(ERROR_ROW,5,"Erreur : Le numero de l'element n'est pas correcte");
		
	}else mvprintw(ERROR_ROW,5,"Erreur : Le nombre de cluster n'est pas correcte");
	
	return couleur ;


}


//Initialise les clusters existants en leur attribuant de facon pseudo-aleatoire des elts.
void initItems() {

	int i,j,length,count,_cluster,add;
	if(items!=NULL) { free(items); items= NULL; }

	if(image!=NULL && clusters!=NULL && K >0) {
		
		length = info.height*info.width;
		count =  length/K;
		_cluster = 0 ;
		add = 0 ;

	 	items = (item*) malloc((sizeof(item))*length);
		for(i=0;i<info.height;i++){
		   for(j=0;j<info.width;j++){
			item* elt = (items+j+i*info.width);			
			elt->numCluster = _cluster;
			elt->distance = -1;
			clusters[_cluster].size++;
			add++;
			if(_cluster<(K-1) && add>count) { add = 0; _cluster++;}
		   }
		}
	}
}


//calcule le centre des differents clusters
void computeCentroideOfClusters() {

  	int i,j;
	double dim,count ;	
	
	if(K>0 && clusters!=NULL&&items!=NULL){

	 
	  for(i=0;i<K;i++) (clusters+i)->centroide = 0 ;
		
	  for(i=0;i<info.height;i++){
	    for(j=0;j<info.width;j++){	
		item elt = *(items+j+i*info.width) ;
		cluster* c = clusters+elt.numCluster;
		c->centroide += *(image+j+i*info.width);
	    }
	  }
	
	  for(i=0;i<K;i++) {
	   cluster* c = clusters+i;
	   if(c->size!=0) c->centroide /= c->size ;
	  }
	}
}


/* Affectation des donnees au  cluster dont le  centre est le plus proche.
   On appele redistributeItems si le nombre de permutation est >= min_modif
   avec min_modif = (K/2)+(K%2).
*/

int distributeItems() {
	
	int i,j,k,count_modif=0,old;
	double aux ;
		
	if(K>0 && items!=NULL && clusters!=NULL){	
		
	  int b[K];// Ensemble de booleen signalant si un cluster a subi une modification ou pas.
	
	
	  for(i=0;i<K;i++) b[i] = 0 ;

	  for(i=0;i<info.height;i++) {
	    for(j=0;j<info.width;j++){
		item* elt = (items+j+i*info.width);
		int pixel = *(image+j+i*info.width);

		if(elt->distance == -1)  //premier iteration
		 elt->distance = abs( clusters[elt->numCluster].centroide - pixel ) ;
	
		old = elt->numCluster;
		for(k=0;k<K;k++){
			if(k==old) continue;
			cluster c = clusters[k];
			aux = abs(c.centroide - pixel);
			if(aux<elt->distance){
			   elt->distance = aux ;
			   elt->numCluster = k ;
			}
		}

		if(old!=elt->numCluster) { 
		  cluster *c = clusters+old ;
		  c->size-=1; 
		  c = clusters+elt->numCluster;
		  c->size+=1; 
		  b[old]=1;
		  b[elt->numCluster]=1; 
		}
	    }
	  }

	  for(i=0;i<K;i++) count_modif+=b[i];

	}
	return count_modif;
}

//Initialise l'algorithme des k-means pour son premier calcule.
void initKmeans(){	

	int i;

	if(clusters!=NULL) { free(clusters); clusters!=NULL; }

	if(K>0){
	  clusters = (cluster*)malloc(sizeof(cluster)*K);
	  for(i=0;i<K;i++) { 
		cluster* c = clusters+i ;
		c->size=0; 
		c->centroide = ramdomGrayLevel(i+1); 
	}
	  initItems();
	}
}


// Determine les differents cluster
void kmeansAlgorithm() {	
	if(K>0 && image!=NULL){		
		min_modif = (K/2)+(K%2);
		int count_modif ;
		initKmeans();
		do{
		   computeCentroideOfClusters();
		   count_modif = distributeItems();
		}while(count_modif>=min_modif);
		
	}
}

//Applique les niveaux de gris a chaque cluster.
void applyKmeansOnImage() {
	
	int i,j;	
	if(result!=NULL) { free(result); result=NULL; }
	
	if(K>0 && items!=NULL && clusters!=NULL) {	
	 result = (int*) malloc( sizeof(int)*info.height*info.width);
	 for(i=0;i<info.height;i++){
	   for(j=0;j<info.width;j++){
		*(result+j+i*info.width) = clusters[(*(items+j+i*info.width)).numCluster].centroide;
	   }
	 }
	
	}

}

// Algorithme k-means  atomique.
void kmeans(int k){

	char ext_name [] = "_Kmeans";
	initNameFile(ext_name);
	K = k ;
	kmeansAlgorithm();	
	applyKmeansOnImage();
}
