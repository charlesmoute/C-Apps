
NOM    	: MOUTE NYOKON
PRENOM 	: Charles Emmanuel
Matricule   : 03x202 
Filiére	: Informatique
Niveau	: 5

		----		Travail pratique sur l'UE intitulé Traitements de sons et de l'image		---

Le but de ce TP est d"effectuer des traitements d'images avec les algorithmes de convolution, de k-means et de Otsu.


		---		Services Exigés

1 - Systéme d'exploitation : une distribution linux.
2 - Librairie curses.h
3 - Le compilateur gcc et le programme make.
4 - Le visualiseur d'image kview (vous pourrez le modifier directement dans le code de la fonction visualize(char*filename) )
5 - Des fichiers images au format pgm (car le programme tourne sur la structure de ces derniers)


		---		 Instructions pour tester le programme		---

1- Pour recompiler vous devez taper d'abord make clean et ensuite make all

2- Pour tester l'application tapez make test. Afin d'éviter une erreur du genre 
   " vous n'avez pas la permission pour execlp " nous vous conseillons de copier 
   d'abord les fichiers sur votre distribution linux.

3- Pour effacer les fichiers générés lors de la compilation vous devez taper make clean.


		---		Présentation sommaire de l'application		---

L'interface de l'application a été fait à partir de la librairie curses, et nous avons considéré que 
la configuration de curses impose directement ncurses. Si ce n'est pas le cas de votre systéme, une erreur
pourrait survenir et pour la corriger vous devrez modifier les option SOURCE et CFLAGS du fichier Makefile ainsi
qu'il suit : 
	SOURCE=.:/usr/include/ncurses
	CFLAGS= -lncurses


Principalement vous aurez quatres menus. A tout moment vous pouvez taper q pour sortir d'un menu et l'option de
visualisation vous permet d'afficher la derniére image à laquelle vous avez eût accés.


- Le premier menu : Menu principale

 	1. Choisir une image.
	2. Quitter.

   Ce menu est le menu lorsqu'aucune image n'a été choisie.

- Le menu principale étendu : donne la liste des opérations possibles sur l'image courante.
  L'option visualiser à ce niveau vous permet d'afficher l'image originelle.

	1. Choisir une image.
	2. Visualiser image.
	3. Proprietes image.
	4. Histogramme image.
	5. Traitements sur image.
	6. Quitter

- Le menu des traitements

	1. Convolution.
	2. Segmentation.
	3. Precedent

- Le menu des opérations proposés sur la convolution :  vous ne pouvez visualiser l'image qu'une fois un 
  traitement appliqué. Auquel quel cas aucune image ne s'affiche.

	1. Filre moyenne-passe-bas.
	2. Filtre de Sobel 1.
	3. Filtre de Sobel 2.
	4. Filtre de Prewitt 1.
	5. Filtre de Prewitt 2.
	6. Filtre Quelconque.
	7. Visualiser image.
	8. Precedent.

- Le menu des opérations proposés sur la segmentation.

	1. K-means.
	2. Otsu.
	3. Visualiser image.
	4. Precedent.