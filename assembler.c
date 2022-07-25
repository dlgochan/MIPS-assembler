#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*******************************************************
 * Function Declaration
 *
 *******************************************************/
char *change_file_ext(char *str);
void process(FILE* input);

FILE* result;
/*******************************************************
 * Function: main
 *
 * Parameters:
 *  int
 *      argc: the number of argument
 *  char*
 *      argv[]: array of a sting argument
 *
 * Return:
 *  return success exit value
 *
 * Info:
 *  The typical main function in C language.
 *  It reads system arguments from terminal (or commands)
 *  and parse an assembly file(*.s).
 *  Then, it converts a certain instruction into
 *  object code which is basically binary code.
 *
 *******************************************************/
int
main(int argc, char *argv[])
{
    FILE *input, *output;
    char *filename;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <*.s>\n", argv[0]);
        fprintf(stderr, "Example: %s sample_input/example?.s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    filename = strdup(argv[1]); // strdup() is not a standard C library but fairy used a lot.
    if(change_file_ext(filename) == NULL) {
        fprintf(stderr, "'%s' file is not an assembly file.\n", filename);
        exit(EXIT_FAILURE);
    }

    output = fopen(filename, "w");
    if (output == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    // result = output;
    // process(input);

    fclose(input);
    fclose(output);
    exit(EXIT_SUCCESS);
}


/*******************************************************
 * Function: change_file_ext
 *
 * Parameters:
 *  char
 *      *str: a raw filename (without path)
 *
 * Return:
 *  return NULL if a file is not an assembly file.
 *  return empty string
 *
 * Info:
 *  This function reads filename and converst it into
 *  object extention name, *.o
 *
 *******************************************************/
char
*change_file_ext(char *str)
{
    char *dot = strrchr(str, '.');

    if (!dot || dot == str || (strcmp(dot, ".s") != 0)) {
        return NULL;
    }

    str[strlen(str) - 1] = 'o';
    return "";
}
//
//
//      !@#$#^%^%&*^*&*^%^$%#%$^&&(^*%$&^%@#$)
//
//
// int pc = 0;

// typedef struct{
//     char name[10];
//     long long value;
// }Data;

// typedef struct{
//     int op;
//     int rs;
//     int rt;
//     int rd;
//     int shamt;
//     int func;
// }R;

// typedef struct{
//     int op;
//     int rs;
//     int rd;
//     int imm;
// }I;

// typedef struct{
//     int op;
//     char label[10];
//     long long addr;
// }J;

// typedef struct{
//     int op;
//     int rs;
//     int rd;
//     char label[10];
//     int bAddr;
// }Branch;

// typedef struct{
//     char name[10];
//     int address;
// }Label;

// typedef union{
//     R r;
//     I i;
//     J j;
//     Branch b;
// }Format;

// typedef struct{
//     char cmd[10];
//     Format format;
// }Ins;

// Data data[10]; Ins ins[50];

// void process(FILE* input){
//     //parsing
//     char* tmp = (char*)(sizeof(char)*30);
//     fgets(tmp, 30, input);
//     do{
//         printf("%s",tmp);

//         fgets(tmp, 30,input);
//     }while(tmp);

//     free(tmp);
// }


















