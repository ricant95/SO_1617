#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

	//verificar se dá com floats ou com negativos..

	int col=1, i=0, j=0, l=0,
	colLi = atoi(argv[1]), colLj = atoi(argv[3]),
	numsLi, numsLj;
	char c, numLi[100], numLj[100], numC[100], buff[100];

	//buff -> a string a ser escrita para o output.
	//colLi -> Primeiro argumento de coluna.
	//colLi -> Segundo argumento de coluna.
	//col -> Numero da coluna.

	while(read(0,&c,1)>0){

		//Adquire o numero correspondente ao primeiro argumento.
		//numLi -> buffer que guarda esse numero
		if((col == colLi) && (c != ':') && (c != '\n')){
			numLi[i] = c;
			i++;
			buff[l++] = c;
		}

		//Adquire o numero correspondente ao segundo argumento.
		//numLj -> buffer que guarda esse numero
		else if((col == colLj) && (c != ':') && (c != '\n')){
			numLj[j] = c;
			j++;
			buff[l++] = c;
		}

		else if(c == ':') { col++; buff[l++] = c; }

		else if(c == '\n'){
			buff[l++] = '\n';
			col = 1;
			//Termina os buffers dos numeros com \0
			numLi[i] = '\0';
			numLj[j] = '\0';
			//Obter os numeros correspondentes as colunas em int.
			numsLi= atoi(numLi);
			numsLj = atoi(numLj);

			//Testa os varios casos possiveis dos operadores.
			if(strcmp(argv[2],"<") == 0){
			if(numsLi < numsLj){
				buff[l] = '\0';
				write(1,buff,strlen(buff));
			}
			}

			else if(strcmp(argv[2],">") == 0){
			if(numsLi > numsLj){
				buff[l] = '\0';
				write(1,buff,strlen(buff));
			}
			}

			else if(strcmp(argv[2],">=") == 0){
			if(numsLi >= numsLj){
				buff[l] = '\0';
				write(1,buff,strlen(buff));
			}
			}

			else if(strcmp(argv[2],"<=") == 0){
			if(numsLi <= numsLj){
				buff[l] = '\0';
				write(1,buff,strlen(buff));
			}
			}

			else if(strcmp(argv[2],"!=") == 0){
			if(numsLi != numsLj){
				buff[l] = '\0';
				write(1,buff,strlen(buff));
			}
			}

			//Limpa o buffer para guardar uma nova string.
			//Passa o valor de l para 0 porque limpa o buffer.
			buff[0] = '\0';
			numLi[0] = '\0';
			numLj[0] = '\0';
			i=0; j=0; l = 0;
		}

			
	else buff[l++] = c;

	}

	//Para o caso em que não termina em \n e depois EOF.
	// Copia do código em cima.
	if(col != 1){
			buff[l++] = '\n';
			numLi[i] = '\0';
			numLj[j] = '\0';
			numsLi= atoi(numLi);
			numsLj = atoi(numLj);
			if(strcmp(argv[2],"<") == 0){
			if(numsLi < numsLj){
				buff[l] = '\0';
				write(1,buff,strlen(buff));
			}
			} 
			else if(strcmp(argv[2],">") == 0){
			if(numsLi > numsLj){
				buff[l] = '\0';
				write(1,buff,strlen(buff));
			}
			}
			else if(strcmp(argv[2],">=") == 0){
			if(numsLi >= numsLj){
				buff[l] = '\0';
				write(1,buff,strlen(buff));
			}
			}
			else if(strcmp(argv[2],"<=") == 0){
			if(numsLi <= numsLj){
				buff[l] = '\0';
				write(1,buff,strlen(buff));
			}
			}
			else if(strcmp(argv[2],"!=") == 0){
			if(numsLi != numsLj){
				buff[l] = '\0';
				write(1,buff,strlen(buff));
			}
			}
		}
	_exit(0);

}