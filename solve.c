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
int get_number_of_occur_Sirs(int,char **);
void get_location_occur_of_Sirs(int [],int,char **);
int get_nb_of_Sirs(int [],int,char **);
void get_location_of_Sirs(int [],int [],int,char **);
void sort_Sirs(int [],int,char **);


int main(int argc,char **argv) {
    int nb_of_sentence=0;
    int nb_of_occur_Sirs=0;
    int sentence_content[5]={0};
    swap_end_sentence(argc,argv);
    remove_all_mommas(argc,argv);
    //Get how many shentence in the text
    for(int i=1;i<argc;i++){
        if(argv[i][strlen(argv[i])-1]=='.' ||
            argv[i][strlen(argv[i])-1]=='?' ||
            argv[i][strlen(argv[i])-1]=='!')
            ++nb_of_sentence;
    }
    int end_of_sentence[nb_of_sentence];
    int flag=0;
    //Record the location of every end of sentence
    for(int i=1;i<argc;i++){
        if(argv[i][strlen(argv[i])-1]=='.' ||
            argv[i][strlen(argv[i])-1]=='?' ||
           argv[i][strlen(argv[i])-1]=='!'){
            end_of_sentence[flag++]=i;
            argv[i][strlen(argv[i])-1]='\0';
        }
        
    }
    nb_of_occur_Sirs=get_number_of_occur_Sirs(argc,argv);
    int location_occur_of_Sirs[nb_of_occur_Sirs];
    get_location_occur_of_Sirs(location_occur_of_Sirs,argc,argv);
    int nb_of_Sirs=get_nb_of_Sirs(location_occur_of_Sirs,nb_of_occur_Sirs,argv);
    int location_of_Sirs[nb_of_Sirs];
    get_location_of_Sirs(location_occur_of_Sirs,location_of_Sirs,nb_of_occur_Sirs,argv);
    sort_Sirs(location_of_Sirs,nb_of_Sirs,argv);
    for(int i=0;i<nb_of_sentence;i++){
        int is_quote=0;
        for(int j=0;j<5;j++)
            sentence_content[j]=0;
        if(i==0)
            sentence_content[0]=1;
        else
            sentence_content[0]=end_of_sentence[i-1]+1;
        sentence_content[1]=end_of_sentence[i];
        for(int j=sentence_content[0];j<=sentence_content[1];j++){
            for(int k=0;k<strlen(argv[j]);k++){
                if(argv[j][k]=='"' && !is_quote){
                    sentence_content[2]=j;
                    is_quote=1;
                }
                else if(argv[j][k]=='"' && is_quote)
                    sentence_content[3]=j;
            }
        }
        if(sentence_content[2] && sentence_content[3]){
            for(int j=sentence_content[0];j<sentence_content[2];j++)
                if(!strcmp(argv[j],"Sir")){
                    sentence_content[4]=j+1;
                    break;
                }
            for(int j=sentence_content[3]+1;j<sentence_content[1];j++)
                if(!strcmp(argv[j],"Sir")){
                    sentence_content[4]=j+1;
                    break;
                }
            printf("Sentence %d: %d,%d,%d,%d,%d\n",i+1,sentence_content[0],sentence_content[1],sentence_content[2],sentence_content[3],sentence_content[4]);
        }
          
    }
    print_all(argc,argv);
    return EXIT_SUCCESS;
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

int get_number_of_occur_Sirs(int argc,char **argv){
    int nb_of_occur_Sirs=0;
    for(int i=1;i<argc;i++){
        int temp=1;
        if(strcmp(argv[i],"Sir")==0)
            nb_of_occur_Sirs++;
        if(strcmp(argv[i],"Sirs")==0){
            nb_of_occur_Sirs++;
            while(strcmp(argv[i+temp],"and")){
                temp++;
                nb_of_occur_Sirs++;
            }
            
        }
        
    }
    return nb_of_occur_Sirs;
}

void get_location_occur_of_Sirs(int location_of_occur_Sirs[],int argc,char **argv){
    int temp=0;
    for(int i=1;i<argc;i++){
        int temp2=1;
        if(strcmp(argv[i],"Sir")==0)
            location_of_occur_Sirs[temp++]=i+1;
        if(strcmp(argv[i],"Sirs")==0){
            while(strcmp(argv[i+temp2],"and")){
                location_of_occur_Sirs[temp++]=i+temp2;
                temp2++;
            }
            if(strcmp(argv[i+temp2],"and")==0)
                location_of_occur_Sirs[temp++]=i+temp2+1;
        }   
    }   
}

int get_nb_of_Sirs(int location_of_occur_Sirs[],int length,char **argv){
    int nb_of_Sirs=1;
    for(int i=1;i<length;i++){
        int flag=0;
        for(int j=i-1;j>=0;j--){
            if(!strcmp(argv[location_of_occur_Sirs[i]],argv[location_of_occur_Sirs[j]]))
                flag=1;
        }
        if(!flag)
            nb_of_Sirs++;
    }
    return nb_of_Sirs;
}

void get_location_of_Sirs(int location_of_occur_Sirs[],int location_of_Sirs[],int length,char **argv){
    int temp=1;
    location_of_Sirs[0]=location_of_occur_Sirs[0];
    for(int i=1;i<length;i++){
        int flag=0;
        for(int j=i-1;j>=0;j--){
            if(!strcmp(argv[location_of_occur_Sirs[i]],argv[location_of_occur_Sirs[j]]))
                flag=1;
        }
        if(!flag)
            location_of_Sirs[temp++]=location_of_occur_Sirs[i];
    }
}

void sort_Sirs(int location_of_Sirs[],int length, char **argv){
    int temp;
    for(int i=length-1;i>0;i--)
        for(int j=0;j<i;j++){
            int first=location_of_Sirs[j];
            int second=location_of_Sirs[j+1];
            if(strcmp(argv[location_of_Sirs[j]],argv[location_of_Sirs[j+1]])>0){
                temp=location_of_Sirs[j];
                location_of_Sirs[j]=location_of_Sirs[j+1];
                location_of_Sirs[j+1]=temp;
            }
        }
}



void print_all(int argc,char **argv){
    for(int i=1;i<=argc;i++){
        printf("%d%s ",i,argv[i]);
    }
    printf("\n");
    
    
}
