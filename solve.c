/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description: Assignment3
 *                                                                             *
 * Written by AnpengChen for COMP9021                                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void swap_end_sentence(int,char **);
void print_all(int,char **);
void remove_all_momma(int,char **);

int main(int argc,char **argv) {
    swap_end_sentence(argc,argv);
    remove_all_mommas(argc,argv);
    print_all(argc,argv);
    return EXIT_SUCCESS;
}

void remove_all_mommas(int argc,char **argv){
    for(int i=1;i<argc;i++){
        if(argv[i][strlen(argv[i])-1]==',')
            argv[i][strlen(argv[i])-1]='\0';
        else if(argv[i][strlen(argv[i])-1]=='"' && argv[i][strlen(argv[i])-2]==','){
            argv[i][strlen(argv[i])-2]='"';
            argv[i][strlen(argv[i])-1]='\0';
        }
    }
    
}

void swap_end_sentence(int argc,char **argv){
    for(int i=1;i<argc;i++){
        if(argv[i][strlen(argv[i])-1]=='"' && argv[i][strlen(argv[i])-2]=='.'){
            argv[i][strlen(argv[i])-1]='.';
            argv[i][strlen(argv[i])-2]='"';
        }
        else if(argv[i][strlen(argv[i])-1]=='"' && argv[i][strlen(argv[i])-2]=='!'){
            argv[i][strlen(argv[i])-1]='!';
            argv[i][strlen(argv[i])-2]='"';
        }
        else if(argv[i][strlen(argv[i])-1]=='"' && argv[i][strlen(argv[i])-2]=='?'){
            argv[i][strlen(argv[i])-1]='?';
            argv[i][strlen(argv[i])-2]='"';
        }
    }
    
}

void print_all(int argc,char **argv){
    for(int i=1;i<=argc;i++){
        printf("%s ",argv[i]);
    }
    printf("\n");
    
    
}
