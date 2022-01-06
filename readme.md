Traitements d'images au format pgm avec les algorithmes de convolution, de k-means et de otsu---------------------------------------------------------------------------------------------
Url     : http://codes-sources.commentcamarche.net/source/53673-traitements-d-images-au-format-pgm-avec-les-algorithmes-de-convolution-de-k-means-et-de-otsuAuteur  : lemoutDate    : 13/08/2013
Licence :
=========

Ce document intitulé « Traitements d'images au format pgm avec les algorithmes de convolution, de k-means et de otsu » issu de CommentCaMarche
(codes-sources.commentcamarche.net) est mis à disposition sous les termes de
la licence Creative Commons. Vous pouvez copier, modifier des copies de cette
source, dans les conditions fixées par la licence, tant que cette note
apparaît clairement.

Description :
=============

La source ici pr&eacute;sent&eacute; est constitu&eacute;e, notamment de :
<br 
/>
<br /> - un fichier algorithmes.h : qui regroupent en son sein les diff&eacu
te;rentes m&eacute;thodes de gestion de l'interface et les algorithmes de convol
ution, de k-means et autres;
<br /> - un fichier application.c repr&eacute;sent
ant l'application qui est lanc&eacute; &agrave; partir d'un makefile
<br /> - u
n Readme et un dossier images contenant quelques images pgm pour les tests
<br 
/><a name='source-exemple'></a><h2> Source / Exemple : </h2>
<br /><pre class=
'code' data-mode='basic'>
--- Le fichier algorithmes.h ---
// Charles MOUTE --
&gt; 03X202
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;s
tring.h&gt;
#include &lt;unistd.h&gt;
#include &lt;math.h&gt;
#include &lt;cu
rses.h&gt;

#define taille 256
#define diese '#'
#define p5  &quot;P5&quot;

#define p6  &quot;P6&quot;

		/*	Types  	*/

			/*Constantes pour interface
s */

#define KEY_ESCAPE     27         /*  Représente le bouton echape  */
#
define MAX_STRING     1024	  /*  Longueur maximale d'une réponse */
//#define M
AX_NAMEFILE   80       /*  Longueur max d'un chemin menant à un fichier image */

#define MESSAGE_ROW    6          /*  Ligne affichant des messages divers */

#define ERROR_ROW      22	  /*  Ligne signalant les messages  */
#define QUESTI
ON_ROW   20	  /*  Ligne pour les questions   */
#define PROMPT_ROW     18	  /* 
 LIGNE de prompt	*/

// Type definissant les proprietes d'une image.
typedef 
struct image_properties {
	int width  ;    // largeur de l'image.
	int height 
;    // hauteur de l'image.
	char* format;   // Format de  l'image.
	int value
Pixel; // valuer maximale des pixels de l'image.
	char* nameFile; // nom du fic
hier image.
	unsigned char* image; // contenu de l'image.
} properties ;

//
 Type definissant la structure d'un element dans l'algorithme de k-means.
typed
ef struct item_kmeans {
	int numCluster ; // Represente le numero de cluster au
 quel l'elt est affecte.
	int distance ; // Represente la distance de l'elt au 
centre du cluster auquel il appartient.
} item ;

// Type definissant la stru
cture d'un cluster dans l'algorithme de k-means.
// Le centre d'un cluster est 
la moyenne des valuers des pixels qu'il contient.
typedef struct cluster_kmeans
 {
	int centroide; //Represente le centre du cluster.	
	int size; //Represente
 le nombre d'elt contenu dans le cluster.
} cluster ;

		/*	Variables  	*/


properties info ;  // proprietes d'une image de nom info.nameFile.
int* histog
ramme ; // est l'histogramme de info.nameFile. de taille valuePixel.
int* image
; // image est la representation sous forme matricielle de info.image.
int* res
ult;// result est l'image resultante de l'application d'une operation sur info.i
mage.

int otsuSeuil ; //represente le seuil donnee par l'algorithme de otsu.


int K ; // Represente le nombre de cluster que k-means doit definr.
int vari
ance_limit = 100; //Represente la limite en dessous de laquelle k-means ne doit 
effectue de traitement.
int min_modif;//Represente le min de modification de cl
usters en dessus du quel on les considere instables.
cluster* clusters; // Repr
esente les k clusters que k-means devra definir.
item* items; //Representent le
s pixels de l'image regroupés en cluster.

char* nameFile ; //Nom donne a l'im
age resultante des operations subies par info.image.

				/*	Operations	*/


// Initialise toutes les variables par des valeurs par defaut.
void default_ini
t() {
	
	int i,j;

	/*pour info*/
	info.width = -1 ;
	info.height = -1 ;

	info.format = NULL ;
	info.valuePixel = -1 ;
	info.nameFile = NULL ;
	info.i
mage = NULL ;
        //info.image = (unsigned char*)malloc(taille*taille);


	/*pour histogramme de l'image */
	histogramme = NULL ;

	// pour le nom de f
ichier
	nameFile = NULL ;
	
 	//pour la representation matricielle de info.im
age
	image = NULL ;

	//pour le resultat matricielle d'une operation subie pa
r info.image
        result = NULL ;

	//pour seuil de otsu
	otsuSeuil = -1 
;

	//Pour l'algorithme de k-means
	K = -1 ;
	clusters = NULL ;
	items = NU
LL ;

}

// Libere les espaces memoires alloues
void free_memory() {
	
	i
f (items!=NULL)	 { free(items); items = NULL; }	
	if (image!=NULL)         { fr
ee(image); image = NULL; }
	if (result!=NULL)        { free(result); result = N
ULL; }
	if (clusters!=NULL)	 { free(clusters); clusters =  NULL; }	
	if (nameF
ile!=NULL)      { free(nameFile); nameFile = NULL; }	
	if (info.format!=NULL)  
 { free(info.format); info.format = NULL; }	
	if (info.image!=NULL)    { free(i
nfo.image);  info.image = NULL ; }
	if (histogramme!=NULL)   { free(histogramme
); histogramme = NULL ; }
	if (info.nameFile!=NULL) { free(info.nameFile); info
.nameFile = NULL; }
	
}

// Retourne la largeur de l'image courrante.
int g
etImageWidth() { return info.width ; }

//Retourne la hauteur de l'image coura
nte.
int getImageHeight() { return info.height ; }

//Retourne la valeur maxi
male des pixels de l'image.
int getImageMaxValuePixel() { return info.valuePixe
l; }

//Retourne le format de l'image courante.
char* getImageFormat() { retu
rn info.format ; }

//Retourne le nom de l'image courante.
char* getImageName
() { return info.nameFile ; }

//Retourne le contenu de l'image courante.
uns
igned char* getImageContent() { return info.image ; }

//Retourne le nom de l'
image modifiee.
char* getNameFile() {return nameFile; }

//Retourne le result
at effectue sur l'image.
int* getOperationResult() { return result; }

//Reto
urne le seuil de Ostu.
int getOtsuSeuil() { return otsuSeuil; }

//Obient les
 proprietes du fichier passee en parametre
void getPropertiesOf(char* file) {


	FILE *f;
        char *row =(char*)malloc(taille),*param=(char*)malloc(taill
e);

	f = fopen(file,&quot;r&quot;);

	if(f) {

	    // Recherche format d
e l'image
	   fgets(row,taille,f);
	   info.format = (char*) malloc((sizeof(ch
ar))*(strlen(row))) ;
	   strcpy(info.format,row);
	   info.format[(strlen(row
))-1] = '\0';
			
	   if (strcmp(p5,info.format)==0 || strcmp(p6,info.format)=
=0 ) {

		if(!feof(f)){
			
			fgets(row,taille,f);

			//Effacement des e
ventuels commentaires dans le fichier
			while(row[0]==diese&amp;&amp;!feof(f))
{
				fgets(row,taille,f);
			}
			
			// Recherche des dimensions de l'imag
e
			if(row[0]!=diese){
			   row[strlen(row)]='\0';
			   info.width = atoi(
strtok(row,&quot; &quot;));
			   info.height = atoi(strtok(NULL,&quot; &quot;)
);
				
			   // Recherche de la valeur maximale des pixels
			   if(!feof(f)
){
			        fgets(row,taille,f);
				row[strlen(row)]='\0';
				info.valueP
ixel = atoi(strtok(row,&quot; &quot;));

				info.nameFile =  (char*)malloc( (
sizeof(char))* (strlen(file)) );	
				strcpy(info.nameFile,file);
			
				if(
!feof(f)){
				   info.image = (unsigned char*)malloc(info.width*info.height);

				   fread(info.image,sizeof(unsigned char),info.width*info.height,f);
				}

			   }
			}
		}
	   } else{
	     mvprintw(ERROR_ROW,5,&quot;Le fichier %
s n'est pas au format P5 ou P6&quot;,file);
	     free_memory();
	   }
	   fc
lose(f);
    	}else {
	   mvprintw(ERROR_ROW,5,&quot;Impossible d'ouvrir %s&qu
ot;,file);
	   free_memory();

        }
	free(row);
	free(param);
}

//
Initialise le nom que l'on donnera a l'image resultante d'une operation sur info
.image

void initNameFile(char* nameUser){

	//En cas de PB enlever les comm
entaires et les mettre ou ils doivent se trouvent

	char* file,*nomFichier,*ex
tension,str[]=&quot;_modified&quot;,point[]=&quot;.&quot;,*name,ext[]=&quot;.pgm
&quot;;
	
	if(nameFile!=NULL) { free(nameFile); nameFile = NULL; }
	
	if(inf
o.nameFile!=NULL) {

	   name = (char*)malloc(strlen(info.nameFile)+1);
	   s
printf(name,&quot;%s\0&quot;,info.nameFile);

	   file = (strrchr(name,'/')+1)
;
 
           if(file==NULL) file = (strrchr(name,'\\')+1);
	
	   if(file==
NULL) file = name ;

	   //extension = strrchr(file,'.')+1;
	   nomFichier = 
strtok(file,&quot;.&quot;);
	
	  if(nameUser!=NULL){	

		char ext_name [strl
en(nameUser)+1];
		sprintf(ext_name,&quot;%s&quot;,nameUser);

		nameFile=(ch
ar*)malloc(strlen(nomFichier)+strlen(ext_name)+strlen(ext)+1);
	   	sprintf(nam
eFile,&quot;%s%s%s&quot;,nomFichier,ext_name,ext);
		
		//nameFile=(char*)mall
oc(strlen(nomFichier)+strlen(ext_name)+strlen(point)+strlen(extension)+1);
	   
	//sprintf(nameFile,&quot;%s%s%s%s&quot;,nomFichier,ext_name,point,extension);

		
	   }else{
		
		nameFile = (char*)malloc(strlen(nomFichier)+strlen(str)+st
rlen(ext)+1);
	   	sprintf(nameFile,&quot;%s%s%s&quot;,nomFichier,str,ext);


		//nameFile = (char*)malloc(strlen(nomFichier)+strlen(str)+strlen(point)+strlen
(extension)+1);
	   	//sprintf(nameFile,&quot;%s%s%s%s&quot;,nomFichier,str,poi
nt,extension);
	  }
	  free(name);
	}
}

//Obtient l'histogramme de l'imag
e courante
void getImageHistogramme() {

	if(info.image!=NULL){
		int i ,len
gth ;
		int max = taille;
		//int max = info.valuePixel+1;
		if(max!=-1){
		
	histogramme =(int*)malloc((sizeof(int))*max);
			for(i=0;i&lt;max;i++) histogr
amme[i] = 0;
			length = strlen(info.image);
			for(i=0;i&lt;length;i++) histo
gramme[info.image[i]]++;
		}

	}

}

// Creer la forme matricielle de l'i
mage courante
void createImageMatrix() {

	int i,j,k=0 ;

	if(info.image!=N
ULL) {

	   image = (int*)malloc((sizeof(int))*(info.height*info.width));
	  
 int* matrix = image ;

	   for(i=0;i&lt;info.height;i++) for(j=0;j&lt;info.wi
dth;j++) {*matrix = 0 ; matrix++;}

	   matrix = image ;
	   for(i=0;i&lt;inf
o.height;i++) {
	      for(j=0;j&lt;info.width;j++) {

<ul>		 <li>matrix = (i
nt) info.image[k];</li></ul>
		  matrix++;
		  k++;
	      }
	   }
	
	}


}

//Obtient d'une representation matricielle le contenu d'une image.
unsign
ed char* getImageOf(const int* matrix) {

	if(info.image!=NULL){
		unsigned c
har* _image = (unsigned char*)malloc(info.height*info.width);
		int i,j,k=0,val
;
		for(i=0;i&lt;info.height;i++){
			for(j=0;j&lt;info.width;j++){
	
				va
l = (unsigned char) (*matrix);
				if(val&gt;info.valuePixel) val = info.valueP
ixel ;
				if(val&lt;0) val = 0 ;
	
				_image[k] = val ;
				k++;
				matr
ix++;
			}		
		}
		return _image ;
	}else return NULL ;

}

// Obtient d
e la matrice courante, le contenue de l'image originelle
unsigned char* getImag
eOfCurrent() {
	return getImageOf(image) ;
}

/* Obtient de la matrice resul
tante d'une operation sur l'image originelle.
   Le contenue de l'image origine
lle modifiee par l'application de l'operation.

<ul><li>/</li></ul>

unsigne
d char* getImageOfResult() {
	return getImageOf(result) ;
}

//Initialise to
utes les variables en regroupant les fonctioons adequates
void init_variables(c
har* file){
	
	default_init();
	getPropertiesOf(file);
	initNameFile(NULL);

	getImageHistogramme();	
	createImageMatrix();	
}

// Affiche a l'ecran les
 proprietes de l'image courante, celle stockee dans info.image
void displayImag
eProperties(){

    mvprintw(MESSAGE_ROW,5,&quot;Image :%s&quot;,info.nameFile
);
    mvprintw(MESSAGE_ROW+2,5,&quot;-----     Proprietes de l'image     -----
&quot;);
    mvprintw(MESSAGE_ROW+4,5,&quot;    Format de l'image  :%s&quot;,in
fo.format);
    mvprintw(MESSAGE_ROW+5,5,&quot;    Largeur de l'image :%d&quot;
,info.width);
    mvprintw(MESSAGE_ROW+6,5,&quot;    Hauteur de l'image :%d&quo
t;,info.height);
    mvprintw(MESSAGE_ROW+7,5,&quot;    Valeur Max des pixels d
e l'image :%d&quot;,info.valuePixel);

}

void displayContent(const unsigned
 char * _image){

    int i,size ;
	
    if(_image==NULL) 
	mvprintw(MESSAG
E_ROW+2,5,&quot;    Contenue de l'image : [ Empty Image ] &quot;);
    else {

	mvprintw(MESSAGE_ROW+2,5,&quot;    Contenue de l'image :   &quot;);
	size = st
rlen(_image);
	mvprintw(MESSAGE_ROW+3,5,&quot;[&quot;);
	for(i=0;i&lt;size;i++
){
		if(i==(size-1 )) printw(&quot;%d&quot;,_image[i]);
		else printw(&quot;%d
, &quot;,_image[i]);
	}
	printw(&quot;]&quot;);
    }
		
}

//Affiche a l
'ecran le contenu de l'image :
void displayImageContent() {
    mvprintw(MESSA
GE_ROW,5,&quot;-----     Image originelle     -----&quot;);
    displayContent(
info.image);
}

// Affiche a l'ecran l'histogramme de l'image courante, celle
 stockee dans info.image
void displayImageHistogramme(){
   int j,length ;
  
 mvprintw(MESSAGE_ROW,5,&quot;-----     Histogramme de %s     -----&quot;,info.n
ameFile);
   if(histogramme==NULL) printw(&quot;   [ Empty Histogramme-Aucune I
mage de selectionnee ]&quot;);
   else {
	length = info.valuePixel+1;
	printw
(&quot;[&quot;);
	for(j=0;j&lt;length;j++){
	    if(j==(length-1)) printw(&quo
t;%d&quot;,histogramme[j]);
	    else printw(&quot;%d, &quot;,histogramme[j]);

	}
	printw(&quot;]&quot;);
   }
}

// Affiche a l'ecran le contenu de la r
epresentation matricielle, _image, du fichier file.
void displayMatrix(char* fi
le, int* _image){
	int i,j;	
	mvprintw(MESSAGE_ROW+1,5,&quot;    Matrice de %s
  :   &quot;,file);
	if(_image==NULL) printw(&quot;[ Empty Matrix ]&quot;);
	e
lse {
		int* matrix = _image ;
		printw(&quot;[&quot;);
		for(i=0;i&lt;info.h
eight;i++){
			printw(&quot;[&quot;);
			for(j=0;j&lt;info.width;j++){
				if
(j==(info.width-1)) printw(&quot;%d&quot;,*matrix);
				else printw(&quot;%d,&q
uot;,*matrix);
				matrix++;
			}
			if(i==(info.height-1)) printf(&quot;]&qu
ot;);
			else printw(&quot;],\n&quot;);		
		}
		printw(&quot;]&quot;);
	}
}


/* Affiche a l'ecran le contenu de la matrice de l'image courante. Celle con
tenu dans info.image */
void displayImageMatrix() {
	mvprintw(MESSAGE_ROW,5,&q
uot;-----     Image originelle     -----&quot;);
	displayMatrix(info.nameFile,i
mage);
}

/* Affiche a l'ecran le contenu de la matrice resultante d'une oper
ation sur l'image courante.*/
void displayResult() {	
	mvprintw(MESSAGE_ROW,5,
&quot;-----     Resulat Operation sur image originelle     -----&quot;);
	displ
ayMatrix(nameFile,result);
}

/* Cree une image de nom nameFile, de largeur i
nfo.width, de hauteur info.height et
   de valeur maximale de pixel info.valueP
ixel a partir du contenu de la representation matricielle d'une
   image, nomee
 matrix.

<ul><li>/</li></ul>
void rewriteImage(int* matrix){
	
    FILE *f
;
    if(nameFile!=NULL &amp;&amp; matrix!=NULL) {
	f = fopen(nameFile,&quot;w
&quot;);
	if(!f) mvprintw(ERROR_ROW,5,&quot;Erreur::Impossible de creer le fich
ier %s&quot;,nameFile);	
	else {
		
		//fputs(&quot;P5\n&quot;,f);	
		fprint
f(f,&quot;%s\n&quot;,info.format);
		fputs(&quot;#File modified by Moute Charle
s. Matricule 0x202\n&quot;,f);
		fprintf(f,&quot;%d %d\n&quot;,info.width,info.
height);
		fprintf(f,&quot;%d\n&quot;,info.valuePixel);
		fwrite(getImageOf(ma
trix),sizeof(unsigned char),info.width*info.height,f);
		fclose(f);
	}
    }e
lse mvprintw(ERROR_ROW,5,&quot;Erreur::L'image ayant subie les modifications n'a
 pas pu etre creee.&quot;);
}

			/*	Traitement avec la convolution	*/

/* 
Retourne une valeur de pixel de la matrice image de largeur witdh et de hauteur 
height. Tout en 
   s'assurant que les coordonnees du pixel soient bien dans l'
image. */

int getValuePixel(int x,int y, int* _image,int width,int height){

	if(x&lt;=0) x = 0;
	if(x&gt;=width) x= width-1;
	if(y&lt;=0) y = 0;
	if(y&gt
;=height) y= height-1;
	if(_image!=NULL) return (*(_image+(x+y*width)));
	else
 return 0.0 ;
}

/* Effectue une convolution sur l'image image de largeur wit
dh et de hauteur height, avec le filtre kernel
   de largeur kw et de hauteur k
h tout s'assurant si le resultat,result, de la convolution est normalise
   , n
ormalized!=0 , ou pas , normalized==0

<ul><li>/</li></ul>
void convolve(int*
 _kernel,int kw ,int kh,int _normalized){
	
	
	if (image==NULL)   
	   mvpri
ntw(ERROR_ROW,5,&quot;Erreur::La forme matricielle de l'image n'est pas initiali
see.&quot;);
	if (_kernel==NULL) mvprintw(ERROR_ROW,5,&quot;Erreur::Le noyau de
 Convolution n'est pas initialise.&quot;);
	
        if(image!=NULL &amp;&amp;
 _kernel!=NULL){
	
	  if(result!=NULL) { free(result); result=NULL; }

	  if
( (info.width&lt;1) || (info.height&lt;1) )
	   mvprintw(ERROR_ROW,5,&quot;La d
imension %d x %d representant celle de l'image est incorrecte&quot;,info.width,i
nfo.height);
	  else if( ( (kw%2)==0)|| ( (kh%2) ==0) ) 
	   mvprintw(ERROR_RO
W+1,5,&quot; La dimension du noyau de convolution %dx%d ne doit avoir que des no
mbres impaires&quot;,kw,kh);
	  else{
	     int x,y,u,v,uc,vc,i,j;
	     int 
limitX,limitY ; 
	     float scale = 1;
	     int *k= _kernel,sum=0;
		
	   
  result = (int*)malloc( (sizeof(int))*(info.width*info.height) );
	     for(i=
0;i&lt;info.height;i++) for(j=0;j&lt;info.width;j++) *(result+i+j) = 0 ;

	   
  if(_normalized!=0){
		for(i=0;i&lt;kw;i++) for(j=0;j&lt;kh;j++){ sum+=(*k); k
++; }
		if(sum!=0) scale/=sum ;
		sum = 0 ;
	     }

              uc = kw/
2;
	      vc = kh/2;
	      limitX = info.width - uc ;
	      limitY = info.h
eight - vc ;
	
             for(y=0;y&lt;info.height;y++){
		for(x=0;x&lt;inf
o.width;x++){
		    k = _kernel ;
		    sum = 0;
		    int edgePixel =( (y&lt
;vc) || (y&gt;=limitY) || (x&lt;uc) || (x&gt;=limitX) ) ;
		    for(v=-vc;v&lt;
=vc;v++){
			int offset = x+(y+v)*info.width;
			for(u=-uc;u&lt;=uc;u++){
			
    if(edgePixel==1)  sum+=getValuePixel(x+u,y+v,image,info.width,info.height) *
 (*k);
			    else sum+= (*(image+offset+u))*(*k);
			    k++;
			}
		    }

		    if(sum&lt;0) sum = 0 ;
		    if(sum&gt;255) sum =255 ;

<ul>		    <li>
(result+(x+y*info.width)) = (sum*scale) ; //(float) (sum*scale) ;</li></ul>
		}
	
	     }
	  }

	}
}

//Permet de visualiser sur kview l'image passee en 
parametre.
void visualize(char* fileImage) {

	pid_t pid ;
	
	if(fileImage!
=NULL){		

		char file[strlen(fileImage)+1] ;
		sprintf(file,&quot;%s&quot;,f
ileImage);
		pid = fork();
		if(pid==-1) 	
		   mvprintw(ERROR_ROW,5,&quot;Er
reur:: L'image %s ne peut etre visualisee. Faite le manuellement.&quot;,file);

		else if(pid==0){
			mvprintw(MESSAGE_ROW,5,&quot;Visualisation de %s&quot;,fi
le);
			//recouvre le code executable herite du pere par  celui de la commande 
kview nameFile
			execlp(&quot;kview&quot;,&quot;kview&quot;,file,NULL);
		}el
se{
			wait();	
			mvprintw(MESSAGE_ROW+1,5,&quot;Arret de la Visualisation&qu
ot;);
		}
		
	}
	
}

			/*	Segmentation  avec l'Algorithme d'Otsu	*/

/
/ Calcule la somme du sous-partie de l'histogramme
double computePartOfHistogra
mme(int indexBegin,int indexEnd){

   double soe = 0 ;
   int i ;
   if(hist
ogramme!=NULL &amp;&amp; indexBegin&gt;=0 &amp;&amp; indexEnd&lt;=info.valuePixe
l &amp;&amp; indexBegin&lt;=indexEnd){
   	for(i=indexBegin;i&lt;=indexEnd;i++)
 soe+= (double)histogramme[i];
   }
   return soe ;

}

// Calcule une par
tie de la variance d&quot;une sous  partie  de l'histogramme
double computePart
OfVariance(int indexBegin,int indexEnd,double moyenne){

   double soe = 0 ;

   int i ;
   if(histogramme!=NULL &amp;&amp; indexBegin&gt;=0 &amp;&amp; index
End&lt;=info.valuePixel &amp;&amp; indexBegin&lt;=indexEnd){
   	for(i=indexBeg
in;i&lt;=indexEnd;i++) soe+= pow( ((double) histogramme[i]) - moyenne,2.0);
   
}
   return soe ;

}

// Calcule du seuil par l'algorithme d'Otsu
void Ots
uAlgorithm() {

	int T ;
	double part1,part2,u1,u2,var1,var2,p1,p2,aux,var,di
m = info.height*info.width ;	
	otsuSeuil = -1 ;
	if(image!=NULL) {

		int va
leur = (info.valuePixel &gt; 255)? 255:info.valuePixel ;

		for(T=1;T&lt;=vale
ur;T++){
		
		   part1 = computePartOfHistogramme(0,T-1);
		   part2 = comput
ePartOfHistogramme(T,255);
		
		   u1 = part1/T ;
		   u2 = part2/(256-T);
	

		   var1 =  computePartOfVariance(0,T-1,u1);
		   var2 =  computePartOfVaria
nce(T,255,u2);
		
		   p1 = part1/dim ;
		   p2 = part2/dim;
		
		   aux = 
(p1*var1)+(p2*var2);

		   if(T==1) { var = aux; otsuSeuil = T; }

		   if(a
ux&lt;var) { var = aux; otsuSeuil = T; }

		}
	}
}

// Modifie l'image sel
on le seuil trouvé par l'algoritme d'otsu.
void applyOtsuOnImage() {

	if(res
ult!=NULL) { free(result); result=NULL; }

	if(otsuSeuil!=-1 &amp;&amp; image!
=NULL) {
	   int i,j;
	   result = (int*)malloc( (sizeof(int))*(info.height*in
fo.width) );
	   for(i=0;i&lt;info.height;i++){
		for(j=0;j&lt;info.width;j++)
{
		   if( (* (image+j+i*info.width) ) &lt; otsuSeuil ) *(result+j+i*info.width
) = 0 ;
		   else *(result+j+i*info.width) = 255 ;
		}
	   }
	}

}

// L
'algorithme d'Ostu de facon atomique
void Otsu() {

	char ext_name [] = &quot
;_Otsu&quot;;
	initNameFile(ext_name);
	OtsuAlgorithm();
	applyOtsuOnImage();

}

/*			Segmentation avec k-means	*/

// Attribue un niveau de gris pseudo
-aléatoire au centre du cluster i. 0 &lt; i &lt; k+1 
int ramdomGrayLevel(int i
) {

	int couleur = 0 ;
	int valeur ;	
	
	if(K&gt;0){
		
		if(i&gt;0 &amp
;&amp; i&lt;=K){
		
			valeur = 256/K ;
			if(valeur==0) couleur = K%256;
		
	else {
				if(i==1) couleur=0;
				else { couleur = valeur*i ; if(couleur&gt;
255) couleur = 255; }
			}
	
		}else mvprintw(ERROR_ROW,5,&quot;Erreur : Le n
umero de l'element n'est pas correcte&quot;);
		
	}else mvprintw(ERROR_ROW,5,&
quot;Erreur : Le nombre de cluster n'est pas correcte&quot;);
	
	return couleu
r ;

}

//Initialise les clusters existants en leur attribuant de facon pseu
do-aleatoire des elts.
void initItems() {

	int i,j,length,count,_cluster,add
;
	if(items!=NULL) { free(items); items= NULL; }

	if(image!=NULL &amp;&amp; 
clusters!=NULL &amp;&amp; K &gt;0) {
		
		length = info.height*info.width;
		
count =  length/K;
		_cluster = 0 ;
		add = 0 ;

	 	items = (item*) malloc((
sizeof(item))*length);
		for(i=0;i&lt;info.height;i++){
		   for(j=0;j&lt;info
.width;j++){
			item* elt = (items+j+i*info.width);			
			elt-&gt;numCluster =
 _cluster;
			elt-&gt;distance = -1;
			clusters[_cluster].size++;
			add++;

			if(_cluster&lt;(K-1) &amp;&amp; add&gt;count) { add = 0; _cluster++;}
		   
}
		}
	}
}

//calcule le centre des differents clusters
void computeCentro
ideOfClusters() {

  	int i,j;
	double dim,count ;	
	
	if(K&gt;0 &amp;&amp;
 clusters!=NULL&amp;&amp;items!=NULL){

	 
	  for(i=0;i&lt;K;i++) (clusters+i
)-&gt;centroide = 0 ;
		
	  for(i=0;i&lt;info.height;i++){
	    for(j=0;j&lt;
info.width;j++){	
		item elt = *(items+j+i*info.width) ;
		cluster* c = cluste
rs+elt.numCluster;
		c-&gt;centroide += *(image+j+i*info.width);
	    }
	  }

	
	  for(i=0;i&lt;K;i++) {
	   cluster* c = clusters+i;
	   if(c-&gt;size!=0
) c-&gt;centroide /= c-&gt;size ;
	  }
	}
}

/* Affectation des donnees au 
 cluster dont le  centre est le plus proche.
   On appele redistributeItems si 
le nombre de permutation est &gt;= min_modif
   avec min_modif = (K/2)+(K%2).


<ul><li>/</li></ul>

int distributeItems() {
	
	int i,j,k,count_modif=0,ol
d;
	double aux ;
		
	if(K&gt;0 &amp;&amp; items!=NULL &amp;&amp; clusters!=NU
LL){	
		
	  int b[K];// Ensemble de booleen signalant si un cluster a subi une
 modification ou pas.
	
	
	  for(i=0;i&lt;K;i++) b[i] = 0 ;

	  for(i=0;i&l
t;info.height;i++) {
	    for(j=0;j&lt;info.width;j++){
		item* elt = (items+j
+i*info.width);
		int pixel = *(image+j+i*info.width);

		if(elt-&gt;distance
 == -1)  //premier iteration
		 elt-&gt;distance = abs( clusters[elt-&gt;numClu
ster].centroide - pixel ) ;
	
		old = elt-&gt;numCluster;
		for(k=0;k&lt;K;k+
+){
			if(k==old) continue;
			cluster c = clusters[k];
			aux = abs(c.centro
ide - pixel);
			if(aux&lt;elt-&gt;distance){
			   elt-&gt;distance = aux ;

			   elt-&gt;numCluster = k ;
			}
		}

		if(old!=elt-&gt;numCluster) { 
	
	  cluster *c = clusters+old ;
		  c-&gt;size-=1; 
		  c = clusters+elt-&gt;nu
mCluster;
		  c-&gt;size+=1; 
		  b[old]=1;
		  b[elt-&gt;numCluster]=1; 
		
}
	    }
	  }

	  for(i=0;i&lt;K;i++) count_modif+=b[i];

	}
	return coun
t_modif;
}

//Initialise l'algorithme des k-means pour son premier calcule.

void initKmeans(){	

	int i;

	if(clusters!=NULL) { free(clusters); clusters
!=NULL; }

	if(K&gt;0){
	  clusters = (cluster*)malloc(sizeof(cluster)*K);
	
  for(i=0;i&lt;K;i++) { 
		cluster* c = clusters+i ;
		c-&gt;size=0; 
		c-&gt
;centroide = ramdomGrayLevel(i+1); 
	}
	  initItems();
	}
}

// Determine 
les differents cluster
void kmeansAlgorithm() {	
	if(K&gt;0 &amp;&amp; image!=
NULL){		
		min_modif = (K/2)+(K%2);
		int count_modif ;
		initKmeans();
		do
{
		   computeCentroideOfClusters();
		   count_modif = distributeItems();
		
}while(count_modif&gt;=min_modif);
		
	}
}

//Applique les niveaux de gris 
a chaque cluster.
void applyKmeansOnImage() {
	
	int i,j;	
	if(result!=NULL)
 { free(result); result=NULL; }
	
	if(K&gt;0 &amp;&amp; items!=NULL &amp;&amp;
 clusters!=NULL) {	
	 result = (int*) malloc( sizeof(int)*info.height*info.widt
h);
	 for(i=0;i&lt;info.height;i++){
	   for(j=0;j&lt;info.width;j++){

<ul>
		<li>(result+j+i*info.width) = clusters[(*(items+j+i*info.width)).numCluster].c
entroide;</li></ul>
	   }
	 }
	
	}

}

// Algorithme k-means  atomique.

void kmeans(int k){

	char ext_name [] = &quot;_Kmeans&quot;;
	initNameFile(
ext_name);
	K = k ;
	kmeansAlgorithm();	
	applyKmeansOnImage();
}

--- Le 
fichier application.c ---

// Charles MOUTE --&gt; 03X202
#include &lt;unistd
.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;stdio.h&gt;
#include &lt;string
.h&gt;
#include &lt;curses.h&gt;
#include &quot;algorithmes.h&quot;

			/*Va
riables Globales d'interface */

//int kwidth=5; //largeur ou nombre de colonn
e du filtre que l'on applique a info.image
//int kheight=5; //hauteur ou nombre
 de ligne que l'on applique a info.image
int normalized=1; //booleen signalant 
si le resultat d'une operation sur une image doit etre normalisee ou pas

//ke
rnel_* represente un filtre que l'on appliquera a info.image
int kernel [5][5] 
= {{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,24,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1
,-1,-1}};
int kernel_moyenne_passe_bas[3][3] = {{1,1,1},{1,1,1},{1,1,1}}; // fi
ltre moyenne-passe-bas.
int kernel_sobel_1 [3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}
};//sobel filter.
int kernel_sobel_2 [3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}}; // 
sobel filter 2.
int kernel_prewitt_1 [3][3] = {{-1,-1,-1},{0,0,0},{1,1,1}}; // 
filtre de Prewitt 1.
int kernel_prewitt_2 [3][3] = {{-1,0,1},{-1,0,1},{-1,0,1}}
; // filtre de Prewitt 2

//int kernel[3][3] = {{1,2,1},{2,4,2},{1,2,1}};
//i
nt kernel[5][5] = {{1,4,6,6,1},{4,16,24,16,4},{6,24,36,24,6},{4,16,24,16,4},{1,4
,6,6,1}};
//int kernel[5][5] = {{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,24,-1,
-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1}};

char file_current [MAX_STRING] =&quot
;\0&quot;; //variable représentant le fichier courant
int isFirst = 1 ;
//Menu
 principale lorsque l'on entre dans le programme pour la première fois.
char* m
ain_menu [] = {
			&quot;1. Choisir une image.&quot;,
			&quot;2. Quitter.&quo
t;,
			 0
		     };

//Représente le menu principal lorsqu'une image a déjà 
était choisie.
char* extend_menu [] = {
				&quot;1. Choisir une image.&quot;,

				&quot;2. Visualiser image.&quot;,
				&quot;3. Proprietes image.&quot;,

				&quot;4. Histogramme image.&quot;,
				&quot;5. Traitements sur image.&quot
;,
				&quot;6. Quitter&quot;,
				 0
		       };

//Donne la liste des opé
rations applicables sur une image.
char* traitement_submenu [] = {
			        
&quot;1. Convolution.&quot;,
			        &quot;2. Segmentation.&quot;,
				&quo
t;3. Precedent&quot;,
			         0
			      };

//Représente les différents
 filtres applicables pour une convolution.
char* convolution_submenu [] = {
		
		 &quot;1. Filre moyenne-passe-bas.&quot;,
				 &quot;2. Filtre de Sobel 1.&qu
ot;,
				 &quot;3. Filtre de Sobel 2.&quot;,
				 &quot;4. Filtre de Prewitt 1
.&quot;,
				 &quot;5. Filtre de Prewitt 2.&quot;,
				 &quot;6. Filtre Quelco
nque.&quot;,
				 &quot;7. Visualiser image.&quot;,
				 &quot;8. Precedent.&q
uot;,
				  0	
			      };

// Représente les différents options pour une se
gmentation.
char* segmentation_submenu [] = { 
                               
    &quot;1. K-means.&quot;,
				   &quot;2. Otsu.&quot;,
				   &quot;3. Visu
aliser image.&quot;,
				   &quot;4. Precedent.&quot;,
				    0
			        }
;

/* Permet de mettre en surbrillance le choix d'un utilisateur en attendant 
sa validation.
   Les paramétres, une liste d'option, l'option en mettre en sur
brillance dans la liste des
   colonnes, la ligne et colonne de début où commen
cer à afficher les options à l'écran.

<ul><li>/</li></ul>

/* Demande  une 
saisie qu'elle lit à la position courante du curseur. Elle supprime tout saut 

   de ligne présent à la fin de la ligne*/
void getString(char* chaine) {

  
int length ;
  wgetnstr(stdscr,chaine,MAX_STRING);
  length = strlen(chaine);

  if(length &gt;0 &amp;&amp; chaine[length-1]== '\n') chaine[length-1]='\0';


}

/* Demande confirmation à l'utilisateur pour une action */
int showMessag
eConfirm() {

   int confirm = 0 ;
   char answer ;

   mvprintw(QUESTION_R
OW,5,&quot;Etes-vous sur ? &quot;);
   clrtoeol();
   refresh();
   cbreak();

   answer = getch();
   if(answer=='o' || answer== 'y' || answer=='O' || answ
er=='Y') confirm = 1 ;
   nocbreak();

   if(!confirm){
	mvprintw(QUESTION_R
OW,1,&quot;     Annulation&quot;);
	clrtoeol();
	refresh();
	sleep(1);
   }

   return confirm ;
}

/* Nettoie l'écran et affiche l'objectif de notre app
lication sous forme de titre. */
void clear_screen() {
	clear();
	//attron(A_
UNDERLINE);
	mvprintw(2,20,&quot;Application de Traitements d'Image&quot;);
	/
/attroff(A_UNDERLINE);
	if(file_current[0])
	  mvprintw(PROMPT_ROW-2,0,&quot;L
e fichier actif est : %s\n&quot;,file_current);
	refresh();
}

/* Permet d'a
fficher le menu avec un element en subrillance */
void display_menu(char* optio
ns[],int surbrill_item,int row_begin,int columm_begin){

	int row_current = 0 
;
	char** option;
	char* txt;
	option = options ;
	while(*option){
		if(row
_current==surbrill_item) mvaddch(row_begin+row_current,columm_begin-3,ACS_RARROW
);
		else  mvaddch(row_begin+row_current,columm_begin-3,' ');		
		txt = option
s[row_current];		
		mvprintw(row_begin+row_current,columm_begin,&quot;%s&quot;,
txt);
		row_current++;
		option++;
	}
	mvprintw(row_begin+row_current+5,colu
mm_begin,&quot;Déplacez la surbrillance puis appuyer sur entrée &quot;);
	refre
sh();
}

/* Permet d'obtenir le choix effectue par l'utilisateur à partir d'u
n ensemble d'options.*/
int select_item(char* prompt,char* options[]){

	stat
ic int row_selected = 0 ;
	int row_max = 0;
	int row_begin = MESSAGE_ROW, col_
begin = 10 ;
	char** option;
	int selected;
	int touche = 0 ;
	
	option = o
ptions ;

	while(*option)	{
	   row_max++;
	   option++;
	}
	
	if (row_se
lected &gt;= row_max) row_selected = 0;
	clear_screen();
	mvprintw(row_begin-2
,col_begin,prompt);
	
	keypad(stdscr,TRUE);
	cbreak();
	noecho();
	
	touch
e = 0 ;
	
	while( touche!='q' &amp;&amp; touche!=KEY_ENTER &amp;&amp; touche!=
'\n') {
	
	  if(touche==KEY_UP){		
		if(row_selected==0) row_selected = row_m
ax -1 ;
		else row_selected--;
	  }
	
	  if(touche==KEY_DOWN){		
		if(row_s
elected==(row_max-1)) row_selected = 0 ;
		else row_selected++;
	  }
	
	  se
lected = *options[row_selected];
	  display_menu(options,row_selected,row_begin
,col_begin);
	  touche = getch();
	}

	keypad(stdscr,FALSE);
	nocbreak();

	echo();
	
	if(touche =='q') selected = 'q';
	return (selected);	
}	

void
 chooseFile(){

	int row_begin = MESSAGE_ROW,col_begin = 10 ;
	clear_screen()
;
	mvprintw(row_begin-2,1,&quot;{ Path : Home - Select a file }&quot;);	
	mvpr
intw(row_begin,col_begin,&quot;Donnez le chemin d'un fichier image : &quot;);
	
getString(file_current);
	free_memory();
	if(file_current[0]){ isFirst = 0; in
it_variables(file_current); }
	else  isFirst = 1 ;
}

/* Menu principal dans
 le cas du traitement d'une segmentation */
//1-4
void main_segmentation() {


	int select ;	
	do{
	  select =  select_item (&quot;{ Path : Home - Traiteme
nts- Segmentation }&quot;,segmentation_submenu);
	  switch(select) {
		case 'q
' : break;//mvprintw(ERROR_ROW-4,25,&quot;Quitter&quot;);break;
		case '4' : se
lect ='q'; break;//mvprintw(ERROR_ROW-4,5,segmentation_submenu[3]);break;
		cas
e '1' : clear_screen();
			   mvprintw(MESSAGE_ROW,5,&quot;Donnez le nombre de 
cluster : &quot;);
			   int k = getch();
			   kmeans(k);
			   rewriteImage
(getOperationResult());
			   mvprintw(MESSAGE_ROW,5,&quot;%s effectuee avec %d
 clusters&quot;,segmentation_submenu[0]+2,k);
			   getch();
			   break;
			
   //mvprintw(ERROR_ROW-4,25,segmentation_submenu[0]);break;
		case '2':  clear
_screen();
			   Otsu();
			   rewriteImage(getOperationResult());
			   mvpr
intw(MESSAGE_ROW,5,&quot;%s effectuee avec un seuil de %d&quot;,segmentation_sub
menu[1]+2,getOtsuSeuil());
			   getch();
			   break; 
			
			   //mvprintw
(ERROR_ROW-4,25,segmentation_submenu[1]);break;
		case '3' : clear_screen();
	
		   visualize(getNameFile());
			   getch();
			   break;
			   //mvprintw(E
RROR_ROW-4,25,segmentation_submenu[2]);break;
	  }	
	  refresh();
	}while(sel
ect!='q');
}

/* Menu principal dans le cas du traitement d'une convolution *
/
void main_convolution() {

	int select ;	
	do{
	  select =  select_item (
&quot;{ Path : Home - Traitements }&quot;,convolution_submenu);
	  switch(selec
t) {
		case 'q' : break;//mvprintw(ERROR_ROW-4,25,&quot;Quitter&quot;);break;

		case '8' : select ='q'; break;//mvprintw(ERROR_ROW-4,5,convolution_submenu[7])
;break;
		case '1' : clear_screen();
			   initNameFile(&quot;_Filre_Moyenne_P
asse_Bas&quot;);
			   convolve((int*)kernel_moyenne_passe_bas,3,3,normalized);

			   rewriteImage(getOperationResult());
			   mvprintw(MESSAGE_ROW,5,&quot;
%s effectuee&quot;,convolution_submenu[0]+2);
			   getch();
			   break;
			
   //mvprintw(ERROR_ROW-4,25,convolution_submenu[0]);break;
		case '2' : clear_
screen();
			   initNameFile(&quot;_Filre_Sobel_1&quot;);
			   convolve((int*
)kernel_sobel_1,3,3,normalized);
			   rewriteImage(getOperationResult());
			
   mvprintw(MESSAGE_ROW,5,&quot;%s effectuee&quot;,convolution_submenu[1]+2);
	
		   getch();
			   break;
			   //mvprintw(ERROR_ROW-4,25,convolution_submenu
[1]);break;
		case '3' : clear_screen();
			   initNameFile(&quot;_Filre_Sobel
_2&quot;);
			   convolve((int*)kernel_sobel_2,3,3,normalized);
			   rewriteI
mage(getOperationResult());
			   mvprintw(MESSAGE_ROW,5,&quot;%s effectuee&quo
t;,convolution_submenu[2]+2);
			   getch();
			   break;
			    //mvprintw(E
RROR_ROW-4,25,convolution_submenu[2]);break;
		case '4' : clear_screen();
			 
  initNameFile(&quot;_Filre_Prewitt_1&quot;);
			   convolve((int*)kernel_prewi
tt_1,3,3,normalized);
			   rewriteImage(getOperationResult());
			   mvprintw
(MESSAGE_ROW,5,&quot;%s effectuee&quot;,convolution_submenu[3]+2);
			   getch(
);
			   break;
			   //mvprintw(ERROR_ROW-4,25,convolution_submenu[3]);break;

		case '5' : clear_screen();
			   initNameFile(&quot;_Filre_Prewitt_2&quot;)
;
			   convolve((int*)kernel_prewitt_2,3,3,normalized);
			   rewriteImage(ge
tOperationResult());
			   mvprintw(MESSAGE_ROW,5,&quot;%s effectuee&quot;,conv
olution_submenu[4]+2);
			   getch();
			   break;
			   //mvprintw(ERROR_ROW
-4,25,convolution_submenu[4]);break;
		case '6' : clear_screen();
			   initNa
meFile(&quot;_Filre_Quelconque&quot;);
			   convolve((int*)kernel,5,5,normaliz
ed);
			   rewriteImage(getOperationResult());
			   mvprintw(MESSAGE_ROW,5,&q
uot;%s effectuee&quot;,convolution_submenu[5]+2);
			   getch();
			   break;

			    //mvprintw(ERROR_ROW-4,25,convolution_submenu[5]);break;
		case '7' : c
lear_screen();
			   visualize(getNameFile());
			   getch();
			   break;
	
		   //mvprintw(ERROR_ROW-4,25,convolution_submenu[6]);break;
	  }	
	  refresh
();
	}while(select!='q');
}

/* Menu principal dans le cas du sous-menu trai
tement */
void main_traitement() {

	int select ;	
	do{
	  select =  select
_item (&quot;{ Path : Home - Traitements }&quot;,traitement_submenu);
	  switch
(select) {
		case 'q' : break;//mvprintw(ERROR_ROW-4,25,&quot;Quitter&quot;);br
eak;
		case '3' : select ='q'; break; //mvprintw(ERROR_ROW-4,5,traitement_subme
nu[2]);break;
		case '1' : main_convolution();  break;//mvprintw(ERROR_ROW-4,25
,traitement_submenu[0]);
		case '2' : main_segmentation(); break;//mvprintw(ERR
OR_ROW-4,25,traitement_submenu[1]);
	  }	
	  refresh();
	}while(select!='q');

	initNameFile(NULL);
}

int main() {

	int select;
	initscr();	
	start_
color();
	do{
	
	   select =  select_item (&quot;{ Path : Home }&quot;,file_c
urrent[0]?extend_menu:main_menu);
	   switch(select) {
		case 'q' : //mvprintw
(ERROR_ROW-3,5,&quot;Quitter&quot;);
			   break;
		case '6' : //mvprintw(ERRO
R_ROW-3,5,extend_menu[5]);
			   select = 'q';
			   break;
		case '2' :
			
   if(isFirst) select = 'q';
			   else{
			    clear_screen();
			    visual
ize(getImageName());
			    getch();
			   }
			   break ;		
		case '1' : ch
ooseFile();
			   //mvprintw(ERROR_ROW-3,5,main_menu[0]);
			   break ;
		cas
e '3' : clear_screen();
			   displayImageProperties();
			   getch();
			   
//mvprintw(ERROR_ROW-3,5,extend_menu[2]); 
			   break ;
		case '4' : //mvprin
tw(ERROR_ROW-3,5,extend_menu[3]);
			   clear_screen();
			   displayImageHist
ogramme();
			   getch();
			   break ;
		case '5' : main_traitement();
			 
  break ;//ivprintw(ERROR_ROW-3,5,extend_menu[4]);
	   }
	    refresh();
	}wh
ile(select!='q');
	endwin();
	free_memory();
	exit(EXIT_SUCCESS);
}

--- l
e fichier MakeFile ---

SOURCE=./
CC=gcc
CFLAGS= -lcurses
CMD=application


all: exe

exe:	$(CMD).c
	$(CC) -I $(SOURCE) -o $(CMD).exe $(CMD).c $(CFLAGS
)

test:	$(CMD).exe
	./$(CMD).exe

clean: 
	rm -rf $(CMD).exe
</pre>
<br
 /><a name='conclusion'></a><h2> Conclusion : </h2>
<br />---		Services Exig&e
acute;s
<br />
<br />1 - Syst&eacute;me d'exploitation : une distribution linu
x.
<br />2 - Librairie curses.h
<br />3 - Le compilateur gcc et le programme m
ake.
<br />4 - Le visualiseur d'image kview (vous pourrez le modifier directeme
nt dans le code de la fonction visualize(char*filename) )
<br />5 - Des fichier
s images au format pgm (car le programme tourne sur la structure de ces derniers
)
<br />
<br />		---		 Instructions pour tester le programme		---
<br />
<br
 />1- Pour recompiler vous devez taper d'abord make clean et ensuite make all
<
br />
<br />2- Pour tester l'application tapez make test. Afin d'&eacute;viter 
une erreur du genre 
<br />   &quot; vous n'avez pas la permission pour execlp 
&quot; nous vous conseillons de copier 
<br />   d'abord les fichiers sur votre
 distribution linux.
<br />
<br />3- Pour effacer les fichiers g&eacute;n&eacu
te;r&eacute;s lors de la compilation vous devez taper make clean.
<br />
<br /
>		---		Pr&eacute;sentation sommaire de l'application		---
<br />
<br />L'inte
rface de l'application a &eacute;t&eacute; fait &agrave; partir de la librairie 
curses, et nous avons consid&eacute;r&eacute; que 
<br />la configuration de cu
rses impose directement ncurses. Si ce n'est pas le cas de votre syst&eacute;me,
 une erreur
<br />pourrait survenir et pour la corriger vous devrez modifier le
s option SOURCE et CFLAGS du fichier Makefile ainsi
<br />qu'il suit : 
<br />
	SOURCE=.:/usr/include/ncurses
<br />	CFLAGS= -lncurses
<br />Principalement v
ous aurez quatres menus. A tout moment vous pouvez taper q pour sortir d'un menu
 et l'option de
<br />visualisation vous permet d'afficher la derni&eacute;re i
mage &agrave; laquelle vous avez e&ucirc;t acc&eacute;s.
<br />
<br />- Le pre
mier menu : Menu principale
<br />
<br /> 	1. Choisir une image.
<br />	2. Qu
itter.
<br />
<br />   Ce menu est le menu lorsqu'aucune image n'a &eacute;t&e
acute; choisie.
<br />
<br />- Le menu principale &eacute;tendu : donne la lis
te des op&eacute;rations possibles sur l'image courante.
<br />  L'option visua
liser &agrave; ce niveau vous permet d'afficher l'image originelle.
<br />
<br
 />	1. Choisir une image.
<br />	2. Visualiser image.
<br />	3. Proprietes ima
ge.
<br />	4. Histogramme image.
<br />	5. Traitements sur image.
<br />	6. Q
uitter
<br />
<br />- Le menu des traitements
<br />
<br />	1. Convolution.

<br />	2. Segmentation.
<br />	3. Precedent
<br />
<br />- Le menu des op&ea
cute;rations propos&eacute;s sur la convolution :  vous ne pouvez visualiser l'i
mage qu'une fois un 
<br />  traitement appliqu&eacute;. Auquel quel cas aucune
 image ne s'affiche.
<br />
<br />	1. Filre moyenne-passe-bas.
<br />	2. Filt
re de Sobel 1.
<br />	3. Filtre de Sobel 2.
<br />	4. Filtre de Prewitt 1.
<b
r />	5. Filtre de Prewitt 2.
<br />	6. Filtre Quelconque.
<br />	7. Visualiser
 image.
<br />	8. Precedent.
<br />
<br />- Le menu des op&eacute;rations pro
pos&eacute;s sur la segmentation.
<br />
<br />	1. K-means.
<br />	2. Otsu.

<br />	3. Visualiser image.
<br />	4. Precedent.
