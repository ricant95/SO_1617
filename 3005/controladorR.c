#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <errno.h>
#include <wordexp.h>
#include "separaStrings.h"

// Para compilar: gcc -Wall -o controlador controlador.c separaStrings.c
struct bd
{
    int num;    //numero do nodo
    int connect[100];   //ligacoes
    int n_connect; //numero de connections
    char** command; //comando que faz
    int pip[2]; //pipe sem nome ligado ao processo pai
    int pid; //pid quando é feito o fork()
};
struct bd nodes[100];

struct fdes
{
    int num;    //numero do nodo
    int filD;   //descritor
    int leitura;  //flags para dizer se vai ler do nodo num ou escrever para o nodo num
    int escrita;
};
struct fdes fdesc[100];

int n_nodes = 0; 


void connectRec(int nodo, struct bd nodes[100], int n_nodes){
    printf("%d\n",nodo);
    char ready[100] = "ready";
    for(int k = 0; k<nodes[nodo].n_connect; k++){
        for(int l = 0; l<n_nodes; l++)
            if(nodes[nodo].connect[k] == nodes[l].num){
                write(nodes[l].pip[1],ready,strlen(ready)+1);
                connectRec(nodes[l].num, nodes, n_nodes);
            }
    }
}

ssize_t readln(int fildes, char *buf, size_t nbytes){

    //tamanho do buffer, verificar
    ssize_t count = 0;
    while(nbytes>0){
        if(read(fildes,buf,1)<=0) break;
        ++count;
        if(*buf=='\n') break;
        if(count>nbytes) break;
        buf++;
    }
    return count;
}

void do_child(int fd, char *comando, char **args){

    pid_t pid = fork();
    if(pid == -1){
            perror("fork error");
            exit(-1);
    }
    else if(pid == 0){
            dup2(fd,1);
            close(fd);
            if(execvp(comando, args) == -1) perror("erro a fazer o exec");
            _exit(-1);
    }
    else{
            wait(NULL);
    }

}

int main(int argc, char*argv[]){

/*struct bd
{
    int num;    //numero do nodo
    int connect[100];   //ligacoes
    int n_connect; //numero de connections
    char** command; //comando que faz
    int pip[2]; //pipe sem nome ligado ao processo pai
    int pid; //pid quando é feito o fork()
};
struct bd nodes[100];

struct fdes
{
    int num;    //numero do nodo
    int filD;   //descritor
    int leitura;  //flags para dizer se vai ler do nodo num ou escrever para o nodo num
    int escrita;
};
struct fdes fdesc[100];
*/
int n_fdesc = 0;

int n_nodes = 0;    //numero de nodos
    
char buffer[2048];
char bufferP[2048];
char resultado[2048];   //que recebe de um processo irmao, pelo fifo com nome
char** comando;
char** comandoP;
//int pip[100][2];

int pides = 0;  //para a condicao em baixo
int saved_stdout;
int n_pips = 0;
int nmb;

while(1){
    saved_stdout = dup(1);
    int n = read(0,buffer,2048);
    if(n<0){ perror("Erro a ler do terminal"); break;}
    if(n==0){ perror("Termina a leitura"); break;}

    buffer[n-1] = '\0';
    comando = separa(buffer);   // comando é um array de strings de que vai ler
                                // contaPal é o número de palavras que contém o comando

    //falta ver sobre o tamanho do array

    //int nmb = atoi(comando[1]); //numero do nodo

    // insere na estrutura o nodo

    /*for(int i=0;i<n_nodes;i++) { if(nmb == nodes[i].num) pides = 1; }
    if(pides == 0){
              nodes[n_nodes].num = nmb;
              nodes[n_nodes].connect = malloc(100 * sizeof(int));
              nodes[n_nodes].command = comando;
              if(pipe(pip[n_pips]) == -1) perror("Erro a criar o pip.");
              n_nodes++;
    }*/

    if(strcmp(comando[0],"node") == 0){
    nmb = atoi(comando[1]);
    for(int i=0;i<n_nodes;i++) { if(nmb == nodes[i].num) pides = 1; }
    if(pides == 0){
              nodes[n_nodes].num = nmb;
              nodes[n_nodes].command = comando+2;
              nodes[n_nodes].n_connect = 0;
              if(pipe(nodes[n_nodes].pip) == -1) perror("Erro a criar o pip.");
    }
    if(pides == 0){
    pid_t pid = fork();
    if(pid == -1){
            perror("fork error");
            exit(-1);
    }
    else if(pid == 0){
            close(nodes[n_nodes].pip[1]);
            dup2(nodes[n_nodes].pip[0],0);
            close(nodes[n_nodes].pip[0]);
            //fazer matriz para guardar o numero do nodo e o descritor de ficheiro associado
            //int fdL[100];   //descritores de fifo de leitura
            //int fdE[100];   //descritores de fifo de escrita
            //int contL;  //contador de descs de leitura
            //int contE;  //contador de descs de escrita
            while(1){
            bufferP[0] = '\0';
            int n = read(0,bufferP,2048);
            if(n<0){ perror("Erro a ler do pip");  _exit(-1);}
            else if(n==0){ perror("Termina a leitura do pip do filho");  _exit(-1);}    
            bufferP[n-1] = '\0';
            comandoP = separa(bufferP);
            char str[100] = "node";
            if(strcmp(comandoP[0], "dar") == 0){
                strcat(str,comandoP[1]);
                strcat(str,"node");
                strcat(str,comandoP[2]);
                int fd = open(str, O_WRONLY);
                if(fd == -1) perror("Erro ao abrir o fifo");
                else{
                    fdesc[n_fdesc].filD = fd;
                    //printf("fd dar: %d nodo: %d\n",fd, nodes[n_nodes].num);
                    fdesc[n_fdesc].escrita = 1;
                    fdesc[n_fdesc++].num = atoi(comandoP[2]);
                }
            }
            else if(strcmp(comandoP[0],"recebe") == 0){
                strcat(str,comandoP[2]);
                strcat(str,"node");
                strcat(str,comandoP[1]);
                int fd = open(str, O_RDONLY);
                if(fd == -1) perror("Erro ao abrir o fifo");
                else{
                    fdesc[n_fdesc].filD = fd;
                    //printf("fd recebe: %d nodo: %d\n",fd, nodes[n_nodes].num);
                    fdesc[n_fdesc].leitura = 1;
                    fdesc[n_fdesc++].num = atoi(comandoP[2]);
                }
            }
           /* else if(strcmp(comandoP[0],"disconecta") == 0){
                int da = atoi(comandoP[1]);
                int recebe = atoi(comandoP[2]);
                for(int i=0;i<n_fdesc;i++){
                    if(fdesc[i].num == recebe && fdesc[i].leitura == 1){
                        fdesc[i].leitura = 0;
                        close(fdesc[i].filD);
                        //remover elemento do array de structs
                        //fork() exec() para remover o fifo
                        //n_fdesc--;
                        break;
                    }
                    else if(fdesc[i].num == da && fdesc[i].escrever == 1){
                        fdesc[i].escreve = 0;
                        close(fdesc[i].filD);
                        //remover elemento do array de structs
                        //fork() exec() para remover o fifo
                        //n_fdesc--;
                        break;
                    }

                }
                //fechar o descritor de ficheiro associado a ligacao
                //eliminar o numero do fd do array
                //decrementar o contL/E dependendo da circunstancia
                //decrementar o n_connect e retirar do 
            }*/

        else if(strcmp(comandoP[0],"ready") == 0){

            //printf("a ouvir %d\n",nodes[n_nodes].num);
            //printf("quem sou: %d\n",nodes[n_nodes].num);
            //fd_set rfds;
            //struct timeval tv;
            //FD_ZERO(&rfds);
            //tv.tv_usec = 0;
            //int retval;
            int pidsAux[100];
            int n_pidsAux = 0;
            int nfdesc = 0;
            for(int i=0;i<n_fdesc;i++) 
                if(fdesc[i].leitura == 1){
                        pid_t pid = fork();
                        if(pid == -1){
                                perror("fork error");
                                exit(-1);
                        }
                        else if(pid == 0){
                                //dup2(fd,1);
                                //close(fd);
                                //printf("%d estou a receber\n",nodes[n_nodes].num);
                                int n = readln(fdesc[i].filD, resultado, 2048);
                                if(n<0){ perror("Erro a ler do pip do comeca");  _exit(-1);}
                                else if(n==0){ perror("Termina a leitura do pip do filho");  _exit(-1);}
                                //resultado[n] = '\0';
                                printf("%d e %d -> %s",n,nodes[n_nodes].num,resultado);
                                int pipeAux[2];

                                for(int j=0;j<n_fdesc;j++){
                                    if(fdesc[j].escrita == 1){
                                    pipe(pipeAux);
                                    pid_t pid = fork();
                                    if(pid == -1){
                                        perror("fork error");
                                        exit(-1);
                                    }
                                    else if(pid == 0){
                                        close(pipeAux[1]);
                                        dup2(pipeAux[0],0);
                                        close(pipeAux[0]);

                                        dup2(fdesc[j].filD,1);
                                        close(fdesc[j].filD);

                                        if(execvp(nodes[n_nodes].command[0], nodes[n_nodes].command) == -1) perror("erro a fazer o exec");
                                        _exit(-1);
                                }
                                else{
                                    close(pipeAux[0]);
                                    write(pipeAux[1],resultado,n);
                                    close(pipeAux[1]);
                                    wait(NULL);
                                }
                    //printf("fd comeca: %d\n",fdesc[i].filD);
                    //printf("->%s\n",nodes[n_nodes].command[0]);
                    //dup2(fdescrip,0);
                    //printf("%s\n",nodes[n_nodes].command[0]);
                    //do_child(fdesc[i].filD, nodes[n_nodes].command[0], nodes[n_nodes].command);
                    //printf("1ola\n");
                                }
                            }
                                //printf("%d - %s\n",nodes[n_nodes].num,resultado);
                                //printf(" «%d recebi\n",nodes[n_nodes].num);
                            _exit(-1);
                        }
                        else{
                                pidsAux[n_pidsAux++] = pid;
                        }
                    //int n = read(fdesc[i].filD, resultado, 2048);
                    //printf("sou o nodo num: %d\n",nodes[n_nodes].num);
                    //resultado[n] = '\0';    //para impedir que seja imprimido carater extra do fim
                    //https://stackoverflow.com/questions/25219874/extra-characters-are-printed-when-file-contents-are-read-into-a-array-in-c
                    //printf("%s\n",resultado);
                }

            for(int i=0;i<n_pidsAux;i++) waitpid(pidsAux[i], NULL, 0);

            printf("%d - terminei\n",nodes[n_nodes].num);

            //for(int i=0;i<n_fdesc;i++) if(fdesc[i].leitura == 1){ FD_SET(fdesc[i].filD, &rfds); nfdesc++; }
            //fazer também para o caso do write para ver se há disponível para escrever
            /*retval = select(nfdesc, &rfds, NULL, NULL, NULL);
            if (retval == -1)
               perror("select()");
            else if (retval)
               printf("Data is available now.\n");
                for(int i=0;i<n_fdesc;i++)
                    if(fdesc[i].leitura == 1){
                    if(FD_ISSET(fdesc[i].filD, &rfds)){
                        printf("ola\n");
                        int n = read(fdesc[i].filD,resultado,2048);
                        if(n<0){ perror("Erro a ler do names fifo");  _exit(-1);}
                        else if(n==0){ perror("Termina a leitura do named fifo");  _exit(-1);}
                        printf("%s\n",resultado);
                    }
                    }
            else
               printf("No data within five seconds.\n");*/

        }

        else if(strcmp(comandoP[0],"comeca") == 0){
            char injetar[100] = "inject";
            char numb[100];
            strcat(injetar," ");
            sprintf(numb, "%d", nodes[n_nodes].num);
            strcat(injetar,numb);
            int fdescrip = open(injetar,O_RDONLY);
            //int fdescrip = open("inject",O_RDONLY);
            int n = read(fdescrip,bufferP,2048);
            if(n<0){ perror("Erro a ler do pip do comeca");  _exit(-1);}
            else if(n==0){ perror("Termina a leitura do pip do filho");  _exit(-1);}
            int pipeAux[2];

            //descartar saida
            //if(n_fdesc == 0) 

            for(int i=0;i<n_fdesc;i++){
                if(fdesc[i].escrita == 1){
                        pipe(pipeAux);
                        pid_t pid = fork();
                        if(pid == -1){
                                perror("fork error");
                                exit(-1);
                        }
                        else if(pid == 0){
                                close(pipeAux[1]);
                                dup2(pipeAux[0],0);
                                close(pipeAux[0]);

                                dup2(fdesc[i].filD,1);
                                close(fdesc[i].filD);

                                if(execvp(nodes[n_nodes].command[0], nodes[n_nodes].command) == -1) perror("erro a fazer o exec");
                                _exit(-1);
                        }
                        else{
                                close(pipeAux[0]);
                                write(pipeAux[1],bufferP,n);
                                close(pipeAux[1]);
                                wait(NULL);
                        }
                    //printf("fd comeca: %d\n",fdesc[i].filD);
                    //printf("->%s\n",nodes[n_nodes].command[0]);
                    //dup2(fdescrip,0);
                    //printf("%s\n",nodes[n_nodes].command[0]);
                    //do_child(fdesc[i].filD, nodes[n_nodes].command[0], nodes[n_nodes].command);
                    //printf("1ola\n");
                }
            }

            close(fdescrip);
        }

            //usar o comandoP para abrir o fifo, abrir de leitura se for receber, se for dar abrir de escrita
            //separado em palavras, fazer atoi, abrir os pipes com isso, os argumentos vao depender do que for(escrita, leitura)
            //depois fazer disconnect, verificar se ao abrir muitos fifos vai dar problemas entretanto com ficar parado ou assim
            //disconnect é eliminar o fifo e depois remover da lista dos conectados
            //do_child(comandoP[2],comandoP+2);
            }
        _exit(-1);
    }

    else{
        //close(pip[n_pips][0]);
        //nmb = atoi(comando[1]);
        //for(int i=0;i<n_nodes;i++) { if(nmb == nodes[i].num) pides = 1; }
        /*if(pides == 0){
              nodes[n_nodes].num = nmb;
              nodes[n_nodes].connect = malloc(100 * sizeof(int));
              nodes[n_nodes].command = comando;
              if(pipe(pip[n_pips]) == -1) perror("Erro a criar o pip.");
              n_nodes++;
        }*/
        //dup2(pip[n_pips][1],1); //A retirar para colocar quando faz inject apenas
        n_pips++;
        nodes[n_nodes].pid = pid;
        n_nodes++;

    }

    } else if(pides == 1){
        //close(pip[nmb][0]);
        nmb = atoi(comando[1]);
        dup2(nodes[n_nodes].pip[1],1);
    }
    //close(pip[n_pips][0]);
    //dup2(pip[n_pips][1],1);
    //write(1,buffer,n);
    pides = 0;
    dup2(saved_stdout,1);
    close(saved_stdout);
    //n_nodes++;
    //n_pips++;
}

else if(strcmp(comando[0],"connect") == 0){

    int nmb = atoi(comando[1]); //numero do nodo source
    //int nmb2 = atoi(comando[2]); //numero do nodo target
    char str[100] = "node"; //nome do fifo a criar
    char numb[100];
    char numb2[100];
    char msg[100] = "dar ";
    char msg2[100] = "recebe ";

    if(contaPal(buffer)-2 > n_nodes){ printf("erro no num do nodo\n"); _exit(1); }

    for(int i=2; i<contaPal(buffer); i++){ 
        int nmb2 = atoi(comando[i]);
        for(int i=0; i<n_nodes; i++){ 
            str[0] = '\0'; 
            strcat(str,"node"); msg[0] = '\0'; 
            msg2[0] = '\0'; strcat(msg,"dar "); 
            strcat(msg2,"recebe "); 
            if(nodes[i].num == nmb){
                sprintf(numb, "%d", nmb);
                sprintf(numb2, "%d", nmb2);
                strcat(str,numb);
                strcat(str,"node");
                strcat(str,numb2);
                if(mkfifo(str,0622)==-1) perror("Já existe o fifo");
                //tratar de ligar os fifos aos processos
                else{ nodes[i].connect[nodes[i].n_connect++] = nmb2;
                strcat(msg,numb);
                strcat(msg," ");
                strcat(msg,numb2);
                //printf("%s\n",msg);
                //dup2(nodes[i].pip[1],1);
                write(nodes[i].pip[1],msg,strlen(msg)+1);
                for(int j=0;j<n_nodes;j++){
                        if(nodes[j].num == nmb2){
                                strcat(msg2,numb2);
                                strcat(msg2," ");
                                strcat(msg2,numb);
                                write(nodes[j].pip[1],msg2,strlen(msg2)+1);
                        }
                }
                }
            } 
        }
    }
    dup2(saved_stdout,1);
    close(saved_stdout);
                                            //nodes[nmb].connect[nodes[nmb].n_connect] = nmb2; printf("%d\n",nodes[nmb].connect[nodes[nmb].n_connect]); nodes[nmb].n_connect++; }
                               //printf("%d %d\n", nmb, nmb2); 
                               /* ver se ja esta repetido na lista dos connects, se sim adicionar e incrementar*/ 

}

else if(strcmp(comando[0],"disconnect") == 0){
    int nmb = atoi(comando[1]);
    int nmb2 = atoi(comando[2]);
    char msg[100] = "disconecta ";
    char numb[100];

    for(int i=0;i<n_nodes;i++){
        if(nodes[i].num == nmb){
            for(int j=0;j<nodes[i].n_connect;j++){
                if(nodes[i].connect[j] == nmb2){
                    sprintf(numb, "%d", nmb2);
                    strcat(msg,numb);
                    write(nodes[i].pip[1],msg,strlen(msg)+1); 
                    //ver como ligar o connect ao fifo que ligou - pelo nome do fifo?                  
                }
            }
        }
    }

dup2(saved_stdout,1);
close(saved_stdout);

}

else if(strcmp(comando[0],"inject") == 0){
    char ready[100] = "ready";
    char comeca[100] = "comeca";
    char injetar[100] = "inject"; //porque varios processos a ler do mesmo named pipe
                                  // leva a que apenas 1 receba a mensagem direito
                                  // por ser FIFO(out)
    strcat(injetar," ");
    strcat(injetar,comando[1]);
    int comecaInicio = -1;
    if(mkfifo(injetar,0622)==-1) perror("Já existe o fifo");
    //int fdescrip = open("inject",O_WRONLY);
    //funcao recursiva que vai ativar apenas aqueles que sao necessarios para fazer o inject sem problemas
    //para nap haver esperas infinitas por resultado que nao vai receber
    //funcRec(ready,atoi(comando[1])) -> ativa com quem estiver ligado
    // depois ciclo pelos ligados e fazer chamada da funcao com esses em vez para ativar os "filhos" desses
    for(int j=0;j<n_nodes;j++){
    if(nodes[j].num != atoi(comando[1])){
    //    for(int k = 0; k<nodes[j].n_connect; k++){
    //       for(int l = 0; l<n_nodes; l++)
    //            if(nodes[j].connect[k] == nodes[l].num){ 
    //                printf("%d\n",nodes[l].num); 
                    write(nodes[j].pip[1],ready,strlen(ready)+1);
                    connectRec(nodes[j].num, nodes, n_nodes);
    }
        //}
    //}
    else { write(nodes[j].pip[1],comeca,strlen(comeca)+1); comecaInicio = j; }
    }
    if(comecaInicio != -1){
        int fdescrip = open(injetar,O_WRONLY);

        do_child(fdescrip, comando[2], comando+2);
        close(fdescrip);
        //close(fdescrip);
        /*pid_t pid2 = fork();
        if(pid2 == -1){
            perror("fork error");
            exit(-1);
        }
        else if(pid2 == 0){
            int fdescrip = open("inject",O_WRONLY);
            dup2(fdescrip,1);
            //close(fdescrip);
            execvp(comando, ); 
            _exit(-1);
        }
        else{
            wait(NULL);
        }*/

    } else printf("Nao existe o nodo com esse número.\n");

        //AQUI 18/05 ATUALIZAÇAO
        //primeiro envia mensagem aos outros processos sem ser o próprio para os preparar para receber mensagem (select com os vários pipes)
        //por o tal preparado de maneira diferente -> ouvir do unnamed pipe(read 2x do unnamed, uma como protocolo, outra o resultado) 
        //o resultado do pipe e depois fazer fork exec a enviar para os vários fifos a resposta(ex do face e da aula, ciclo, etc)
        //os outros ouvem a primeira resposta que der e criam um processo para enviar para os outros e assim sucessivamente ..
        // o que nao tiver para onde mandar é descartada a saida(verificar se nao tem para onde enviar com condição, etc)
        //outra condiçao para caso encontre o tal, espera que o ciclo termine e depois faz fork e exec com o output a ser enviado para o pipe
}
    

    //enviar para todos a dizer para se prepararem para receber -> ler dos fifos




}

//for(int i = 0;i<n_pips;i++) { printf("%d\n", i); close(pip[i][1]); }

for(int i = 0;i<n_nodes;i++){ close(nodes[i].pip[1]); kill(nodes[n_nodes].pid,SIGKILL); }
//for(int i=0;i<n_nodes;i++){ printf("%d\n",i); wait(NULL); }

/*for(int i=0;i<n_nodes;i++){ printf("i = %d, Num = %d , connects = %d\n",i,nodes[i].num, nodes[i].n_connect);
                            for(int j=0;j<nodes[i].n_connect;j++){ printf("%d\n",nodes[i].connect[j]); }
                        }*/

//sleep(2);
_exit(0);

}
