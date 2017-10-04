/* Brainfuck Interpreter
 * By Sabbir Mollah
 * Last change: 10/05/2017
 */

#include<stdio.h>
#include<stdlib.h>

typedef struct{
    long codePosition;      //File pointer
    int apt;        //Array Pointer
    struct NEST *next;
}NEST;

void displayArr(char ch[], int n){      //Function helps to debug
    int i;
    for(i=0; i<n; i++){
        printf("%d ",ch[i]);
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    char arr[3000];
    int arrPoint = 0, nestLayer = 0;
    memset(arr,'\0',3000);
    ///displayArr(arr,10);         ///msg

    char *fileName = argv[1];
    FILE *fp;
    char code;
    fp = fopen(fileName,"r");
    if(fp==NULL){
        printf("No file found. Please check whether the location is valid.");
        return 1;
    }

    NEST *root, *curr, *temp;
    root = ( NEST *) malloc( sizeof(NEST) );
    root->next = NULL;
    curr = root;

    while(1){
        code = fgetc(fp);
        if(code == EOF){
            break;
        }

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
