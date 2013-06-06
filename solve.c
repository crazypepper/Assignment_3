/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description: Assignment3
 *                                                                             *
 * Written by AnpengChen for COMP9021                                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define KNIGHT 1
#define AT_LEAST 2
#define AT_MOST 4
#define EXACTLY 6

void swap_end_sentence(int,char **);
void print_all(int,char **);
void remove_all_momma(int,char **);
int get_number_of_occur_Sirs(int,char **);
void get_location_occur_of_Sirs(int [],int,char **);
int get_nb_of_Sirs(int [],int,char **);
void get_location_of_Sirs(int [],int [],int,char **);
void sort_Sirs(int [],int,char **);
void remove_all_quotes(int ,char **);
int saying_what(int ,int ,char **);
int whos_mentioned(int ,int ,char **,int [],int ,int );
bool is_possible(int,int,int,int,int);



int main(int argc,char **argv) {
    int nb_of_sentence=0;
    int nb_of_occur_Sirs=0;
    int nb_of_solutions=0;
    int current_i=0;
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
    int sentence_content[nb_of_sentence][5];
    for(int j=0;j<nb_of_sentence;j++)
        for(int k=0;k<5;k++)
            sentence_content[j][k]=0;
    for(int i=0;i<nb_of_sentence;i++){
        int is_quote=0;
        if(i==0)
            sentence_content[0][0]=1;
        else
            sentence_content[i][0]=end_of_sentence[i-1]+1;
        sentence_content[i][1]=end_of_sentence[i];
        for(int j=sentence_content[i][0];j<=sentence_content[i][1];j++){
            for(int k=0;k<strlen(argv[j]);k++){
                if(argv[j][k]=='"' && !is_quote){
                    sentence_content[i][2]=j;
                    is_quote=1;
                }
                else if(argv[j][k]=='"' && is_quote)
                    sentence_content[i][3]=j;
            }
        }
        if(sentence_content[i][2] && sentence_content[i][3]){
            for(int j=sentence_content[i][0];j<sentence_content[i][2];j++)
                if(!strcmp(argv[j],"Sir")){
                    sentence_content[i][4]=j+1;
                    break;
                }
            for(int j=sentence_content[i][3]+1;j<sentence_content[i][1];j++)
                if(!strcmp(argv[j],"Sir")){
                    sentence_content[i][4]=j+1;
                    break;
                }
            for(int j=0;j<nb_of_Sirs;j++){
                if(!strcmp(argv[location_of_Sirs[j]],argv[sentence_content[i][4]]))
                    sentence_content[i][4]=location_of_Sirs[j];
            }
            
        }
        
        printf("Sentence %d: %d,%d,%d,%d,%d\n",i+1,sentence_content[i][0],sentence_content[i][1],sentence_content[i][2],sentence_content[i][3],sentence_content[i][4]); 
    }
    remove_all_quotes(argc,argv);
    int sentence_info[nb_of_sentence][nb_of_Sirs+2];
    for(int i=0;i<nb_of_sentence;i++){
        int temp;
        for(int j=0;j<nb_of_Sirs+2;j++){
            sentence_info[i][j]=0;
        }
        if(sentence_content[i][4]){
            for(int j=0;j<nb_of_Sirs;j++)
                if(sentence_content[i][4]==location_of_Sirs[j])
                    sentence_info[i][0]=j;
            sentence_info[i][1]=saying_what(sentence_content[i][2],sentence_content[i][3],argv);
            temp=whos_mentioned(sentence_content[i][2],sentence_content[i][3],argv,location_of_Sirs,nb_of_Sirs,sentence_content[i][4]);
            for(int j=0;j<nb_of_Sirs;j++){
                if(temp & (1<<j))
                    sentence_info[i][j+2]=1;
            }
        }
        printf("sentence %d info:",i+1);
        for(int j=0;j<nb_of_Sirs+2;j++)
            printf("%d ",sentence_info[i][j]);
        printf("\n");
    }
    for(int i=0;i<pow(2,nb_of_Sirs);i++){
        bool is_solution=true;
        for(int j=0;j<nb_of_sentence;j++){
            int mentioned=0;
            if(!sentence_content[j][4])
                continue;
            if(!is_solution)
                continue;
            for(int k=0;k<nb_of_Sirs;k++){
                if(sentence_info[j][k+2])
                    mentioned|=(1<<(nb_of_Sirs-k-1));
            }
            is_solution=is_possible(sentence_info[j][1],i,mentioned,sentence_info[j][0],nb_of_Sirs);
        }
        if(is_solution){
            nb_of_solutions++;
            current_i=i;
            printf("%d\n",current_i);
        }   
    }
    print_all(argc,argv);
    printf("The Sirs are:");
    for(int i=0;i<nb_of_Sirs;i++)
        printf(" %s",argv[location_of_Sirs[i]]);
    printf("\n");
    if(!nb_of_solutions)
        printf("There is no solution.");
    else if(nb_of_solutions==1){
        printf("There is a unique solution:\n");
        for(int i=0;i<nb_of_Sirs;i++){
            if(current_i&(1<<(nb_of_Sirs-i-1)))
                printf("Sir %s is a Knight.\n",argv[location_of_Sirs[i]]);
            else
                printf("Sir %s is a Knave.\n",argv[location_of_Sirs[i]]);
        }
    }
    else
        printf("There are %d solutions.",nb_of_solutions);
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
        if(argv[i][strlen(argv[i])-1]==',' || argv[i][strlen(argv[i])-1]==':')
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

void remove_all_quotes(int argc,char **argv){
    for(int i=1;i<argc;i++){
        if(argv[i][0]=='"')
            memmove(argv[i],argv[i]+1,strlen(argv[i]));
        if(argv[i][strlen(argv[i])-1]=='"')
            argv[i][strlen(argv[i])-1]='\0';
    }
}

int saying_what(int quote_start,int quote_end,char **argv){
    int type=0;
    for(int i=quote_start;i<=quote_end;i++){
        if(!strcmp(argv[i],"Knight") || !strcmp(argv[i],"Knights"))
            type+=KNIGHT;
    }
    for(int i=quote_start;i<=quote_end;i++){
        if((!strcmp(argv[i],"At") || !strcmp(argv[i],"at")) && !strcmp(argv[i+1],"least"))
            return type+AT_LEAST;

        if((!strcmp(argv[i],"At") || !strcmp(argv[i],"at")) && !strcmp(argv[i+1],"most"))
            return type+AT_MOST;

        if(!strcmp(argv[i],"Exactly") || !strcmp(argv[i],"exactly"))
            return type+EXACTLY;

        if(!strcmp(argv[i],"All") || !strcmp(argv[i],"all"))
            return type;

        if(!strcmp(argv[i],"I") && !strcmp(argv[i+1],"am"))
            return type;

        if(!strcmp(argv[i],"Sir") && !strcmp(argv[i+2],"is"))
            return type;

        if(!strcmp(argv[i],"or"))
            return type+AT_LEAST;

        if(!strcmp(argv[i],"and"))
            return type;
    }
}

int whos_mentioned(int quote_start,int quote_end,char **argv,int location_of_Sirs[],int nb_of_Sirs,int talker){
    int sirs_mentioned[nb_of_Sirs];
    int mentioned=0;
    for(int i=0;i<nb_of_Sirs;i++)
        sirs_mentioned[i]=0;
    for(int i=quote_start;i<=quote_end;i++){
        for(int j=0;j<nb_of_Sirs;j++){
            if(!strcmp(argv[i],argv[location_of_Sirs[j]]))
                sirs_mentioned[j]=1;
        }
        if(!strcmp(argv[i],"I"))
            for(int j=0;j<nb_of_Sirs;j++)
                if(!strcmp(argv[talker],argv[location_of_Sirs[j]]))
                    sirs_mentioned[j]=1;
        if(!strcmp(argv[i],"us"))
            for(int j=0;j<nb_of_Sirs;j++)
                sirs_mentioned[j]=1;
    }
    for(int i=0;i<nb_of_Sirs;i++){
        if(sirs_mentioned[i])
            mentioned|=1<<i;
    }
    return mentioned;    
}

bool is_possible(int type,int possible,int mentioned,int speaker,int nb_of_Sirs){
    bool speaker_is_knight=false;
    int temp;
    if(possible==(possible|(1<<(nb_of_Sirs-speaker-1))))
        speaker_is_knight=true;
    switch(type){
        //Some Sir(s) is/are knave
        case 0:
            if(speaker_is_knight){
                if(mentioned==(mentioned & (mentioned^possible)))
                    return true;
            }
            else
                if(mentioned!=(mentioned&(mentioned^possible)))
                    return true;
            return false;
            //some Sirs is/are knight
        case 1:
            if(speaker_is_knight){
                if(mentioned==(mentioned&possible))
                    return true;
            }
            else
                if(mentioned!=(mentioned&possible))
                    return true;
            return false;
            //at least one is knave
        case 2:
            if(speaker_is_knight){
                if(mentioned!=(mentioned&possible))
                    return true;
            }
            else
                if(mentioned==(mentioned&possible))
                    return true;
            return false;
            //at least one is knight
        case 3:
            if(speaker_is_knight){
                if(mentioned!=(mentioned&(mentioned^possible)))
                    return true;
            }
            else if(mentioned==(mentioned&(mentioned^possible)))
                return true;
            return false;
        case 4://at most one is knave
            if(speaker_is_knight){
                if((temp=mentioned&(mentioned^possible))==0)
                    return true;
                for(int i=0;i<nb_of_Sirs;i++)
                    if((1<<i)==temp)
                        return true;
                return false;
            }
            else{
                temp=mentioned&(mentioned^possible);
                for(int i=0;i<nb_of_Sirs;i++)
                    if((1<<i)==temp || temp==0)
                        return false;
                return true;
            }
        case 5://at most one is knight
            if(speaker_is_knight){
                if((temp=mentioned&possible)==0)
                    return true;
                for(int i=0;i<nb_of_Sirs;i++)
                    if((1<<i)==temp)
                        return true;
                return false;
            }
            else{
                temp=mentioned&possible;
                for(int i=0;i<nb_of_Sirs;i++)
                    if((1<<i)==temp || temp==0)
                        return false;
                return true;
            }
        case 6://Exactly one is knave
            if(speaker_is_knight){
                temp=mentioned&(mentioned^possible);
                for(int i=0;i<nb_of_Sirs;i++)
                    if((1<<i)==temp)
                        return true;
                return false;
            }
            else{
                temp=mentioned&(mentioned^possible);
                for(int i=0;i<nb_of_Sirs;i++)
                    if((1<<i)==temp)
                        return false;
                return true;
            }
        case 7://Exactly one is knight
            if(speaker_is_knight){
                temp=mentioned&possible;
                for(int i=0;i<nb_of_Sirs;i++)
                    if((1<<i)==temp)
                        return true;
                return false;
            }
            else{
                temp=mentioned&possible;
                for(int i=0;i<nb_of_Sirs;i++)
                    if((1<<i)==temp)
                        return false;
                return true;
            }
    }
}


void print_all(int argc,char **argv){
    for(int i=1;i<=argc;i++){
        printf("%d%s ",i,argv[i]);
    }
    printf("\n");
}
