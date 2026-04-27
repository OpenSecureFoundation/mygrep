#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define max_line 1024

int main(int argc, char *argv[])
{
    FILE *file;
    char line[max_line];
    if(argc < 3){
        fprintf(stderr,"insufficient number of arguments, write : %s <motif> <file>",argv[0]);
        return EXIT_FAILURE;
    }
    int i;
    int flag_n = 0;
    int flag_l = 0;
    int flag_s = 0;
    for(i=1;i<argc-2;i++){
        if(argv[i][0]=='-'){
            if(strcmp(argv[i],"-n")==0){
                flag_n=1;
            }
            else if(strcmp(argv[i],"-l")==0){
                flag_l=1;
            }
            else if(strcmp(argv[i],"-s")==0){
                flag_s=1;
            }
            else{
                printf("flag not recognized");
            }
        }
    }
    file = fopen(argv[argc-1],"r");
    if(file==NULL){
        if(flag_s==1){
            return EXIT_FAILURE;
        }
        else{
            perror("error in opening file");
            return EXIT_FAILURE;
        }
    }
    int n=0;
    while(fgets(line,sizeof(line),file)!=NULL){
        n=n+1;
        if(strstr(line,argv[argc-2])!=NULL){
            if(argc==3){
                printf("%s",line);
            }
            if(argc>3){
                if(flag_l==1){
                    printf("%s\n",argv[argc-1]);
                    fclose(file);
                    return EXIT_SUCCESS;
                }
                if(flag_n==1){
                    printf("%d: %s\n",n,line);
                }
                else{
                    printf("%s\n",line);
                }
            }
        }
    }
    fclose(file);
    if(flag_l==1){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}