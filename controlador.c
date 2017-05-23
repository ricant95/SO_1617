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

void do_child(char *comando, char **args){

    pid_t pid = fork();
    if(pid == -1){
            perror("fork error");
            exit(-1);
    }
    else if(pid == 0){
            execvp(comando, args); 
            _exit(-1);
    }
    else{
            wait(NULL);
    }

}

int main(int argc, char*argv[]){

struct bd
{
    int num;    //numero do nodo
    int connect[100];   //ligacoes
    int n_connect;
    char** command; //comando que faz
    int pip[2]; //pipe sem nome ligado ao processo pai
};
struct bd nodes[100];

int n_nodes = 0;    //numero de nodos
    
char buffer[2048];
char bufferP[2048];
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
              nodes[n_nodes].command = comando;
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
            int fdL[100];   //descritores de fifo de leitura
            int fdR[100];   //descritores de fifo de escrita
            int contL;  //contador de descs de leitura
            int contR;  //contador de descs de escrita
            while(1){
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
                    fdL[contL++] = fd;
                }
            }
            else if(strcmp(comandoP[0],"recebe") == 0){
                strcat(str,comandoP[2]);
                strcat(str,"node");
                strcat(str,comandoP[1]);
                int fd = open(str, O_RDONLY);
                if(fd == -1) perror("Erro ao abrir o fifo");
                else{
                    fdL[contR++] = fd;
                }
            }
            else if(strcmp(comandoP[0],"disconecta") == 0){

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
    for(int j=0;j<n_nodes;j++){
    if(nodes[j].num != atoi(comando[1])){
        //AQUI 18/05 ATUALIZAÇAO
        //primeiro envia mensagem aos outros processos sem ser o próprio para os preparar para receber mensagem (select com os vários pipes)
        //por o tal preparado de maneira diferente -> ouvir do unnamed pipe(read 2x do unnamed, uma como protocolo, outra o resultado) 
        //o resultado do pipe e depois fazer fork exec a enviar para os vários fifos a resposta(ex do face e da aula, ciclo, etc)
        //os outros ouvem a primeira resposta que der e criam um processo para enviar para os outros e assim sucessivamente ..
        // o que nao tiver para onde mandar é descartada a saida(verificar se nao tem para onde enviar com condição, etc)
        //outra condiçao para caso encontre o tal, espera que o ciclo termine e depois faz fork e exec com o output a ser enviado para o pipe
    }
    }

    //enviar para todos a dizer para se prepararem para receber -> ler dos fifos

}


}

//for(int i = 0;i<n_pips;i++) { printf("%d\n", i); close(pip[i][1]); }
for(int i = 0;i<n_nodes;i++) close(nodes[i].pip[1]);
for(int i=0;i<n_nodes;i++){ printf("%d\n",i); wait(NULL); }

/*for(int i=0;i<n_nodes;i++){ printf("i = %d, Num = %d , connects = %d\n",i,nodes[i].num, nodes[i].n_connect);
                            for(int j=0;j<nodes[i].n_connect;j++){ printf("%d\n",nodes[i].connect[j]); }
                        }*/

//sleep(2);
_exit(0);

}
