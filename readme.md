Traitements d'images au format pgm avec les algorithmes de convolution, de k-means et de otsu

Url     : http://codes-sources.commentcamarche.net/source/53673-traitements-d-images-au-format-pgm-avec-les-algorithmes-de-convolution-de-k-means-et-de-otsu

Auteur  : lemout

Date    : 13/08/2013

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
<br/>
<br/> - un fichier algorithmes.h : qui regroupent en son sein les diff&eacu
te;rentes m&eacute;thodes de gestion de l'interface et les algorithmes de convol
ution, de k-means et autres;
<br/> - un fichier application.c repr&eacute;sent
ant l'application qui est lanc&eacute; &agrave; partir d'un makefile
<br/> - un Readme et un dossier images contenant quelques images pgm pour les tests
<br/>

<br/><a name='conclusion'></a><h2> Conclusion : </h2>

<br />---		Services Exig&eacute;s
<br />
<br />1 - Syst&eacute;me d'exploitation : une distribution linux.
<br />2 - Librairie curses.h
<br />3 - Le compilateur gcc et le programme make.
<br />4 - Le visualiseur d'image kview (vous pourrez le modifier directeme
nt dans le code de la fonction visualize(char*filename) )
<br />5 - Des fichiers images au format pgm (car le programme tourne sur la structure de ces derniers)
<br />
<br />		---		 Instructions pour tester le programme		---
<br />
<br/>1- Pour recompiler vous devez taper d'abord make clean et ensuite make all
<br />
<br />2- Pour tester l'application tapez make test. Afin d'&eacute;viter 
une erreur du genre 
<br />   &quot; vous n'avez pas la permission pour execlp&quot; nous vous conseillons de copier 
<br />   d'abord les fichiers sur votre distribution linux.
<br />
<br />3- Pour effacer les fichiers g&eacute;n&eacute;r&eacute;s lors de la compilation vous devez taper make clean.
<br />
<br /
>		---		Pr&eacute;sentation sommaire de l'application		---
<br />
<br />L'interface de l'application a &eacute;t&eacute; fait &agrave; partir de la librairie 
curses, et nous avons consid&eacute;r&eacute; que 
<br />la configuration de curses impose directement ncurses. Si ce n'est pas le cas de votre syst&eacute;me,
 une erreur pourrait survenir et pour la corriger vous devrez modifier les option SOURCE et CFLAGS du fichier Makefile ainsi qu'il suit : 
<br />
	SOURCE=.:/usr/include/ncurses
<br />	CFLAGS= -lncurses
<br />Principalement vous aurez quatres menus. A tout moment vous pouvez taper q pour sortir d'un menu
 et l'option de visualisation vous permet d'afficher la derni&eacute;re image &agrave; laquelle vous avez e&ucirc;t acc&eacute;s.
<br />
<br />- Le premier menu : Menu principale
<br />
<br /> 	1. Choisir une image.
<br />	2. Quitter.
<br />
<br />   Ce menu est le menu lorsqu'aucune image n'a &eacute;t&eacute; choisie.
<br />
<br />- Le menu principale &eacute;tendu : donne la liste des op&eacute;rations possibles sur l'image courante.
<br />  L'option visualiser &agrave; ce niveau vous permet d'afficher l'image originelle.
<br />
<br
 />	1. Choisir une image.
<br />	2. Visualiser image.
<br />	3. Proprietes ima
ge.
<br />	4. Histogramme image.
<br />	5. Traitements sur image.
<br />	6. Quitter
<br />
<br />- Le menu des traitements
<br />
<br />	1. Convolution.
<br />	2. Segmentation.
<br />	3. Precedent
<br />
<br />- Le menu des op&eacute;rations propos&eacute;s sur la convolution :  vous ne pouvez visualiser l'i
mage qu'une fois un traitement appliqu&eacute;. Auquel quel cas aucune image ne s'affiche.
<br />
<br />	1. Filtre moyenne-passe-bas.
<br />	2. Filtre de Sobel 1.
<br />	3. Filtre de Sobel 2.
<br />	4. Filtre de Prewitt 1.
<br />	5. Filtre de Prewitt 2.
<br />	6. Filtre Quelconque.
<br />	7. Visualiser image.
<br />	8. Precedent.
<br />
<br />- Le menu des op&eacute;rations propos&eacute;s sur la segmentation.
<br />
<br />	1. K-means.
<br />	2. Otsu.
<br />	3. Visualiser image.
<br />	4. Precedent.
