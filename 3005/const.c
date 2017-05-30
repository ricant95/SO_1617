#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*int main(int argc, char *argv[]){

	char c;
	ssize_t n;
	while((n=read(0,&c,1))>0){
		if(c=='\n') {write(1,":",1); write(1,argv[1],strlen(argv[1])); write(1,&c,1);}
		else write(1,&c,1);
	}
}*/

int main(int argc, char *argv[]){
    char c;
    char str[2048];
    char string[2048];
    char num[1024];
    ssize_t n;
    while((n=read(0,&c,1))>0){
        sprintf(string, "%c", c);
        if(c=='\n') {
            strcat(str,":");
            strcat(str,argv[1]);
            strcat(str,string);
        }
        else strcat(str,string);
    }
    
    write(1,str,strlen(str)+1);
}
