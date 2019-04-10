#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int faktorial[50];
int j=1;
pthread_t tid[50];

void *func_faktorial(void *args){
	int i, hasil=1;
	for(i=1; i<=faktorial[j]; i++){
		hasil = hasil*i;
	}
	printf("%d! = %d\n", faktorial[j], hasil);
	j++;
}

int main(int argc, void *argv[]){
	int i=1, j, k;

	while(argv[i] != NULL){
		faktorial[i] = atoi(argv[i]);
		i++;
	}

	for(j=1; j<argc; j++){
		for(k=j+1; k<argc; k++){
			int angka1 = faktorial[j];
			int angka2 = faktorial[k];
			if(angka1 > angka2){
				char *temp = argv[k];
				int temp2 = faktorial[k];
				faktorial[k] = faktorial[j];
				faktorial[j] = temp2;
				argv[k] = argv[j];
				argv[j] = temp;
			}
		}
	}

	for(i=1; i<argc; i++){
		pthread_create(&(tid[i-1]), NULL, &func_faktorial, (void*) argv[i]);
	}

	for(i=1; i<argc; i++){
		pthread_join(tid[i-1], NULL);
	}

	return 0;
}
