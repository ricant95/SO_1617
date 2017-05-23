#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){

	char c;
	ssize_t n;
	while((n=read(0,&c,1))>0){
		if(c=='\n') {write(1,":",1); write(1,argv[1],strlen(argv[1])); write(1,&c,1);}
		else write(1,&c,1);
	}
}
