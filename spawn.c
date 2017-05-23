#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>


int count = 0;
int valor;

int sys(char *comando){
    const char space[6] = " \t\n";
    char *com;
    char* comandos[10];
    int i=0;
    
    if(fork()==0){
        com=strtok(comando,space);
        
        while(com != NULL) {
            comandos[i]=strdup(com);
            i++;
            com = strtok(NULL,space);
            
        }
        comandos[i]=NULL;
        execvp(comandos[0],comandos);
        _exit(0);
    }

    int status;
    wait(&status);
    if(WIFEXITED(status)){
    count++;
    printf("%d\n",WEXITSTATUS(status));
    valor = WEXITSTATUS(status);
   }
}


char *strcat1(char s1[], char s2[]){
	int i, len=strlen(s1), j;
    s1[len]=' ';
	for(i=len+1,j=0;s2[j]!='\0';i++,j++){
		s1[i]=s2[j];
	}
	s1[i]='\0';
	return s1;
}


int main(int argc, char** argv){
    int i;
    ssize_t n;
    char command[1000];
    for(i=1;i<argc;i++){
    strcat1(command,argv[i]);
    }
    sys(command);

    char buf;
    char string[100];
    sprintf(string, "%d", valor);
    while((n=read(0,&buf,1))>0){
		if(buf=='\n') {write(1,":",1); write(1,string,1); write(1,&buf,1);}
		else write(1,&buf,1);
	}

    //write(1,":",1); write(1,string,1); write(1,"\n",1);

    
    return 0;
    
}


