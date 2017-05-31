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
	char str[2048];
	char str2[2048];
	char string[2048];

	//buff -> a string a ser escrita para o output.
	//colLi -> Primeiro argumento de coluna.
	//colLj -> Segundo argumento de coluna.
	//col -> Numero da coluna.

	while(read(0,&c,1)>0){

		//Adquire o numero correspondente ao primeiro argumento.
		//numLi -> buffer que guarda esse numero
		if((col == colLi) && (c != ':') && (c != '\n')){
			numLi[i] = c;
			i++;
			sprintf(string,"%c",c);
			strcat(str,string);
		}

		//Adquire o numero correspondente ao segundo argumento.
		//numLj -> buffer que guarda esse numero
		else if((col == colLj) && (c != ':') && (c != '\n')){
			numLj[j] = c;
			j++;
			sprintf(string,"%c",c);
			strcat(str,string);
		}

		else if(c == ':') { col++;
			sprintf(string,"%c",c);
			strcat(str,string); }

		else if(c == '\n'){
			
			sprintf(string,"%c",c);
			strcat(str,string);
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
				strcat(str2,str);
			}
			}

			else if(strcmp(argv[2],">") == 0){
			if(numsLi > numsLj){
				strcat(str2,str);
			}
			}

			else if(strcmp(argv[2],">=") == 0){
			if(numsLi >= numsLj){
				strcat(str2,str);
			}
			}

			else if(strcmp(argv[2],"<=") == 0){
			if(numsLi <= numsLj){
				strcat(str2,str);
			}
			}

			else if(strcmp(argv[2],"!=") == 0){
			if(numsLi != numsLj){
				strcat(str2,str);
			 }
			}

			//Limpa o buffer para guardar uma nova string.
			//Passa o valor de l para 0 porque limpa o buffer.
			//buff[0] = '\0';
			str[0]= '\0';
			numLi[0] = '\0';
			numLj[0] = '\0';
			i=0; j=0; l = 0;
		}

			
	else {
			sprintf(string,"%c",c);
			strcat(str,string); }

	}

	//Para o caso em que não termina em \n e depois EOF.
	// Copia do código em cima.
	if(col != 1){
			//buff[l++] = '\n';
			sprintf(string,"%c",'\n');
			strcat(str,string);
			numLi[i] = '\0';
			numLj[j] = '\0';
			numsLi= atoi(numLi);
			numsLj = atoi(numLj);
			if(strcmp(argv[2],"<") == 0){
			if(numsLi < numsLj){
				strcat(str2,str);
			}
			} 
			else if(strcmp(argv[2],">") == 0){
			if(numsLi > numsLj){
				strcat(str2,str);
			}
			}
			else if(strcmp(argv[2],">=") == 0){
			if(numsLi >= numsLj){
				strcat(str2,str);
			}
			}
			else if(strcmp(argv[2],"<=") == 0){
			if(numsLi <= numsLj){
				strcat(str2,str);
			}
			}
			else if(strcmp(argv[2],"!=") == 0){
			if(numsLi != numsLj){
				strcat(str2,str);
			}
			}
		}
	write(1,str2,strlen(str2)+1);
	_exit(0);

}