#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define max_line 1024

void flag_Z_function(char* filename);
int flag_l_function(int flag_Z, char* filename, FILE* file);
void flag_n_function(int n, char* line);
int flag_q_function(FILE* file);
int flag_F_function(char* line, FILE* file);
int flag_I_function(FILE* file);
int flag_d_function(int argc, char* argv[]);
void flag_Z_function(char* filename) {
    char nul = '\0'; printf("%s%c", filename, nul);
}
int flag_l_function(int flag_Z, char* filename, FILE* file) {
    if (flag_Z == 1) {
        flag_Z_function(filename);
    }
    else {
        printf("%s\n", filename);
    }
    fclose(file);
    return EXIT_SUCCESS;
}
void flag_n_function(int n, char* line) {
    printf("%d: %s\n", n, line);
}

int flag_q_function(FILE* file) {
    fclose(file);
    return EXIT_SUCCESS;
}
int flag_F_function(char* line, FILE* file) {
    printf("%s\n", line);
    fclose(file);
    return EXIT_SUCCESS;
}
int flag_I_function(FILE* file) {
    fclose(file);
    return EXIT_FAILURE;
}
int flag_d_function(int argc, char* argv[]) {
    int d; int i;
    for (i = 1; i < argc - 2; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            d = i + 1;
        }
        else {
            i = i + 1;
        }
    }

    if ((strcmp(argv[d + 1], "read") == 0) || (strcmp(argv[d + 1], "skip") == 0)) {

        printf("error");
        return EXIT_FAILURE;
    }
    if ((strcmp(argv[d + 1], "recurse") == 0)) {
        /* call flag -r */
    }
    else {
        printf("invalid action");
    }
    return EXIT_SUCCESS;
}
int main(int argc, char* argv[])
{
    FILE* file;
    char line[max_line];
    if (argc < 3) {
        fprintf(stderr,
            "insufficient number of arguments, write : %s <motif> <file>",
            argv[0]);
        return EXIT_FAILURE;
    }
    int i;
    int flag_n = 0;
    int flag_l = 0;
    int flag_s = 0;
    int flag_F = 0;
    int flag_q = 0;
    int flag_Z = 0;
    int flag_d = 0;
    int flag_I = 0;

    for (i = 1; i < argc - 2; i++) {

        if (argv[i][0] == '-') {

            if (strcmp(argv[i], "-n") == 0) {
                flag_n = 1;
            }
            else if (strcmp(argv[i], "-l") == 0) {
                flag_l = 1;
            }
            else if (strcmp(argv[i], "-s") == 0) {
                flag_s = 1;
            }
            else if (strcmp(argv[i], "-F") == 0) {
                flag_F = 1;
            }
            else if (strcmp(argv[i], "-q") == 0) {
                flag_q = 1;
            }
            else if (strcmp(argv[i], "-Z") == 0) {
                flag_Z = 1;
            }
            else if (strcmp(argv[i], "-d") == 0) {
                flag_d = 1;
            }
            else if (strcmp(argv[i], "-I") == 0) {
                flag_I = 1;
            }
            else {
                printf("flag not recognized");
            }
        }
    }

    file = fopen(argv[argc - 1], "r");

    if (file == NULL) {

        if (flag_s == 1) {
            return EXIT_FAILURE;
        }
        else {
            perror("error in opening file");
            return EXIT_FAILURE;
        }
    }

    int I;

    while ((I = fgetc(file)) != EOF) {

        if (I == '\0') {

            if (flag_I == 1) {
                return flag_I_function(file);
            }
            else {
                printf("match found");
                fclose(file);
                return EXIT_SUCCESS;
            }
        }
    }

    if (flag_d == 1) {
        flag_d_function(argc, argv);
    }

    rewind(file);

    int n = 0;

    while (fgets(line, sizeof(line), file) != NULL) {

        n = n + 1;

        if (strstr(line, argv[argc - 2]) != NULL) {

            if (argc == 3) {
                printf("%s", line);
            }

            if (argc > 3) {

                if (flag_l == 1) {
                    return flag_l_function(
                        flag_Z,
                        argv[argc - 1],
                        file
                    );
                }

                if (flag_n == 1) {
                    flag_n_function(n, line);
                }

                if (flag_q == 1) {
                    return flag_q_function(file);
                }

                if (flag_F == 1) {
                    return flag_F_function(line, file);
                }

                else {
                    printf("%s\n", line);
                }
            }
        }
    }

    fclose(file);

    if (flag_l == 1) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
