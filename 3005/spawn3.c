#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>



//int count = 0;
int valor;
int pipefd[2];


int sys(char *comando){
    const char space[6] = " \t\n";
    char *com;
    char* comandos[10];
    int i=0;
    int fd2 = open("error.txt",O_WRONLY|O_CREAT|O_TRUNC,0644);
   pipe(pipefd);
    if(fork()==0){
        //int stdin_fd = open("/dev/null",O_RDONLY);
       // if(stdin_fd == -1) perror("Erro a abrir o ficheiro");
        com=strtok(comando,space);
        
        while(com != NULL) {
            comandos[i]=strdup(com);
            i++;
            com = strtok(NULL,space);
            
        }
        comandos[i]=NULL;
        dup2(pipefd[1],1);
        dup2(fd2,2);
        close(pipefd[1]);
        close(fd2);
        execvp(comandos[0],comandos);
        _exit(-1);
    }

    int status;
    wait(&status);
    if(WIFEXITED(status)){
    //count++;
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
    int cifrao[100];
    char str[1024];
    char arg[100];
    int count = 1;

    int c=0;
    int pos;
    
    for(i=1; i<argc; i++){
        if(argv[i][0] == '$'){
            cifrao[c]=atoi(&argv[i][1]);
            c++;
            pos=i;
        }

    }
    char buf;
    char string[100];
    char string2[100];
    i=0;

    while((n=read(0,&buf,1))>0){
        sprintf(string2,"%c",buf);
        if(buf=='\n') { arg[i]='\0';
            for(int j=1;j<argc;j++){
                if(j==pos)
                     strcat1(command,arg);
                else strcat1(command,argv[j]);
        }
        sys(command);
        sprintf(string, "%d", valor);
        strcat(str,":"); strcat(str,string); strcat(str,string2);
        command[0] = '\0';
        count = 1;
        arg[0] = '\0';
        i=0;
        }

    else if(buf==':') { count++; strcat(str,":"); }
    
    else if((count == cifrao[0]) && (buf!= ':') && (buf != '\n')) {
        strcat(str,string2); arg[i] = buf; i++; }

    else strcat(str,string2);
    }

    //if((buf!='\n') && n==0){
    if(count!=1) {
        arg[i]='\0';
        for(int j=1;j<argc;j++){
                if(j==pos)
                     strcat1(command,arg);
                else strcat1(command,argv[j]);
        }
        sys(command);
        sprintf(string, "%d", valor);
        strcat(str,":"); strcat(str,string);
        command[0] = '\0';
        count = 1;
        arg[0] = '\0';
        i=0;
    }

    write(1,str,strlen(str)+1);
    return 0;
    
}


