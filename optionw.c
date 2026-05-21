#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h> //pour gerer les types de variables
#define max_ligne 1024

//fonction aui permet de savoir si un caractere fait partie d'un mot
//qui retoure 1 si c'est une lettre, un chiffre ou un underscore et 0 sinon
int est_char_mot(char c){
    return isalnum((unsigned char)c) || c =='_';
}

//fonction qui vereifie si un motif est un mot entier dans une ligne
int est_mot_entier(char *ligne, char *motif){
    int gauche, droite;
    char *pos= ligne;
    int longueur=strlen(motif); //on declare un pointeur aui permettra de parcourir la ligne parce au'on ne doit pas modifier la ligne
    while (strstr(ligne, motif)!=NULL){
        pos=strstr(ligne, motif);
        gauche=(pos==ligne) || (est_char_mot(pos[-1])==0); //au debut de la ligne ou le caractere qui vient avnt le mot n'appartient pas un mot surleauel on se trouve actuellement
        droite=(est_char_mot(pos[longueur])==0); //si apres i.e. a droite du mot sur leauel on se trouve, il y a un delimiteur (espace, aller a la ligne)
        if (gauche && droite){
            return 1;
        }
        else {return 0;}
    }
}

int main (int argc, char *argv[])
{
    FILE *f;
    char ligne[max_ligne];
    if (argc < 3)
    {
        fprintf(stderr, "Nombre d'arguments insuffisant, ecrire : %s [OPTION]... MOTIFS [FICHIER]...\n Try \'grep --help\' for more information. ", argv[0]);
        return EXIT_FAILURE;
    }
    
    //if we have mygrep pattern file
    if (argc==3) 
    {
        char *motif=argv[1];
        char *filename=argv[2];
        f=fopen(filename, "r");

        if (f==NULL){
            perror("Erreur lors de l'ouverture");
            return EXIT_FAILURE;
        }
        while (!feof(f)){
            while (fgets(ligne, sizeof(ligne), f)!= NULL){
                if (strstr(ligne, motif)!=NULL){
                    if (est_mot_entier(ligne, motif)==1){
                        printf("%s", ligne);
                    }
                }
            }
        }
        fclose(f);
    }
    return EXIT_SUCCESS;

}