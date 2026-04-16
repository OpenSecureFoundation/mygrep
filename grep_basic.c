#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define max_ligne 1024


int main (int argc, char *argv[])
{
    FILE *f;
    char ligne[max_ligne];
    printf("Nombre d'arguments: %d\n", argc);
    if (argc < 3)
    {
        fprintf(stderr, "Nombre d'arguments insuffisant, ecrire : %s <motif> <fichier>", argv[0]);
        return EXIT_FAILURE;
    }
    

    //
    if (argc==3) 
    {
        char *motif=argv[1];
        char *filename=argv[2];

        f=fopen(filename, "r");

        if (f==NULL)
        {
            perror("Erreur lors de l'ouverture");
            return EXIT_FAILURE;
        }

        while (!feof(f))
        {
            while (fgets(ligne, sizeof(ligne), f)!= NULL){
                if (strstr(ligne, motif)!=NULL)
                    printf("%s", ligne);
            }
        }

    }
    return EXIT_SUCCESS;

}