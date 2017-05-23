#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

	//operadores a considerar -> avg, max, min, sum.

	//verificar se dá com floats ou com negativos..

	int col=1,lin=1, min=0, max=0, avg=0, sum=0, i=0, j=0,
	colT=atoi(argv[1]),linT=atoi(argv[3]);
	char c, num[100], numC[100];

	int nums[100];

	while(read(0,&c,1)>0){
		if((col == colT) && (c != ':') && (c != '\n')){
			num[i] = c;
			i++;
		}
		if(c == ':') col++;
		else if(c == '\n'){ 
			col = 1;
			num[i] = '\0';
			write(1,":",1);
			nums[lin] = atoi(num); 
			if(lin == 1) {  }

			else if(lin > linT){
			for(int k = lin-1; k>=lin-linT; k--){ sum += nums[k];
		 		if(k == lin-1){ min = nums[k]; max = nums[k];}
				if(min > nums[k]) min = nums[k];
				if(max < nums[k]) max = nums[k];
			}
			avg = sum/linT;
			}

			else{
			for(int k = lin-1; k>0; k--){  sum += nums[k]; 
				if(k == lin-1){ min = nums[k]; max = nums[k];}
				if(min > nums[k]) min = nums[k];
				if(max < nums[k]) max = nums[k];
			}

			avg = sum/(lin-1);

			}
			if(strcmp(argv[2],"avg") == 0){
			sprintf(numC, "%d", avg);
			write(1,numC,strlen(numC));
			}

			else if(strcmp(argv[2],"min") == 0){
			sprintf(numC, "%d", min);
			}

			else if(strcmp(argv[2],"max") == 0){
			sprintf(numC, "%d", max);
			}

			else if(strcmp(argv[2],"sum") == 0){
			sprintf(numC, "%d", sum);
			}

			sum = 0;
			avg = 0;
			lin++;
			i=0;
			}
			write(1,&c,1);
	}
	if(col!=1){
			col = 1;
			num[i] = '\0';
			write(1,":",1);
			nums[lin] = atoi(num); 
			if(lin == 1) {  }

			else if(lin > linT){
			for(int k = lin-1; k>=lin-linT; k--){ sum += nums[k];
		 		if(k == lin-1){ min = nums[k]; max = nums[k];}
				if(min > nums[k]) min = nums[k];
				if(max < nums[k]) max = nums[k];
			}
			avg = sum/linT;
			}

			else{
			for(int k = lin-1; k>0; k--){  sum += nums[k]; 
				if(k == lin-1){ min = nums[k]; max = nums[k];}
				if(min > nums[k]) min = nums[k];
				if(max < nums[k]) max = nums[k];
			}

			avg = sum/(lin-1);

			}
			if(strcmp(argv[2],"avg") == 0){
			sprintf(numC, "%d", avg);
			write(1,numC,strlen(numC));
			}

			else if(strcmp(argv[2],"min") == 0){
			sprintf(numC, "%d", min);
			}

			else if(strcmp(argv[2],"max") == 0){
			sprintf(numC, "%d", max);
			}

			else if(strcmp(argv[2],"sum") == 0){
			sprintf(numC, "%d", sum);
			}

			sum = 0;
			avg = 0;
			lin++;
			i=0;
			write(1,"\n",1);

	}
	_exit(0);

}