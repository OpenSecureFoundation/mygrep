# mygrep
Redévelopper la commande grep à partir de zéro en utilisant le langage C.

Pour cela, nous devrons travailler sur une distribution Linux. Notre choix s'est porte sur la distro debian
Pour l'obtenir, nous avons telecharge l'image iso du systeme. L'image est telechargeable et disponible via le lien https://cdimage.debian.org/debian-cd/current/amd64/iso-cd/debian-13.4.0-amd64-netinst.iso . 

## Fonctionnalités actuellement disponibles 
- Recherche d'un motif;
- Rercherche dans plusieurs fichiers
- Affichage du numéro de ligne
- Rcherche insensible à la casse
- Affcihage du nombre d'occurrences
- Recherche depuis la sortie d'une autre commande


## Comment utiliser mygrep ?

1. Cloner le repository
- copier le lien du repo (disponible au niveau du bouton vert Clone)
- dans votre terminal git, taper git clone lien_du_projet_copie_sur_github

2. Récupérer les changements (s'il y en a eu)
- taper la commande git pull origin

3. Installer la commande dans la variable d'environnement $PATH
- exécuter le fichier principal ainsi que les fichiers secondaires tsala.c, teteya.c, etc. avec la commande git -o mygrep main.c
- copier le fichier exécutable dans le dossier /usr/local/bin ou /bin ou /usr/bin

4. Félicitations ! Vous pouvez maintenant lancer la commande mygrep en respectant le format
mygrep [options] "motif" fichier


## Options déjà implémentées

Sur les 49 options de la commande grep, nous n'avons pu qu'implémenter 36 parmi lesquelles : 
-i: recherche sans prise e =n compte de la casse ; 
-v: recherche inversée ;
-r: recherche récursive
-c: affcihage du nombre de lignes qui contiennent le fichier ;
-n: affichage des lignes numérotées qui contiennent le motif ;
-A, -B, -C: affichage des lignes de contexte
-P: gestion des perl-regex
-F: gestion des chaînes fixes

## Lacunes de notre commande
- Avec notre commande, toutes les options de la commande originelle (grep) ne sont pas disponibles ;
- Les options peuvent être combinées, mais pas sur toutes les formes c'est-à-dire mygrep -i -c "motif" test.txt fonctionne
mais la forme mygrep -ic "motif" test.txt ne fonctionne pas encore.
- Le code n'est pas optimisé, donc il y a des risques de fuites de mémoire 

## Auteurs:
ALOBWEDE Lesnar Ahone 
EKANJE VANESSA EWOSE 
TETEYA TOLEQUE Etienne Dieu beni 
TSALA Yannick Bertrand
NINKAM YOUBISSI Noémie Amanda 
