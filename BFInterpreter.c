/* Brainfuck Interpreter in C
 * By Sabbir Mollah
 */

#include<stdio.h>
#include<stdlib.h>
#include <string.h>

typedef struct NEST{
    long codePosition;      //File pointer
    int apt;                //Array Pointer, points to the point where nesting starts
    struct NEST *next;
}NEST;



int main(int argc, char *argv[]) {
    //Memory allocation for the interpreter
    char arr[3000];
    int arrPoint = 0, nestLayer = 0;
    memset(arr,'\0',3000);
    
    //check command line arguments number
    if(argc != 2){
        printf("Usage: BFInterpreter <fileName>");
        return 1;
    }

    char *fileName = argv[1];
    FILE *fp;
    char code;
    fp = fopen(fileName,"r");
    if(fp==NULL){
        printf("No file found. Please check whether the location is valid.");
        return 1;
    }

    //Structs to maintain nested loops
    NEST *root, *curr, *temp;
    root = ( NEST *) malloc( sizeof(NEST) );
    root->next = NULL;
    curr = root;

    while(1){
        code = fgetc(fp);
        if(code == EOF){
            break;
        }

        //Move to next cur
        fseek(fp, -1L, SEEK_CUR);

        int tempInput;
        switch(code){
            case '+':
                arr[arrPoint] = arr[arrPoint] + 1;
                break;
            case '-':
                arr[arrPoint] = arr[arrPoint] - 1;
                break;
            case '>':
                arrPoint++;
                break;
            case '<':
                arrPoint--;
                break;
            case '.':
                printf("%c",arr[arrPoint]);
                break;
            case ',':
                printf("BF Scan: ");
                scanf(" %d",&tempInput);
                arr[arrPoint] = tempInput ;
                break;
            case '[':

                nestLayer++;
                curr->next = ( NEST *) malloc( sizeof(NEST) );
                curr = curr->next;
                curr->next = NULL;
                curr->apt = arrPoint;
                curr->codePosition = ftell(fp);
                //printf("Code position is: %d\n",curr->codePosition);

                break;
            case ']':
                if(arr[curr->apt]==0){
                    //Deleting last node:
                    curr=root;
                    int i;
                    for(i=1; i<nestLayer; i++){
                        curr = curr->next;
                    }
                    free(curr->next);
                    curr->next = NULL;

                    nestLayer--;
                }
                else{
                    fseek(fp, curr->codePosition, SEEK_SET);
                }
                break;
        }
        fseek(fp, 1L, SEEK_CUR);
    }

    free(fp);
    free(root);
	return 0;
}
