#include <stdio.h>
#include <stdlib.h>

int faktorial[10];
int front, rear;

void display_pq();
void check();
void insert_pq();

int main(int argc, char *argv[]){
	front = rear = -1;
	int i;
	for(i=0; i<argc-1; i++){
		int a = strtol(argv[i+1], NULL, 10);
//		printf("a = %d\n", a);
		insert_pq(a);
	}
	display_pq(argc);
	return 0;
}

int func_faktorial(int data){
	if(data == 0) return 1;
	else return (data * func_faktorial(data-1));
}

void display_pq(int j){
	int i;
	for(i=0; i < j-1; i++){
		printf("%d! = %d\n", faktorial[i], func_faktorial(faktorial[i]));
	}
}

void check(int data){
	int i, j;
	for(i=0; i <= rear; i++){
		if(data <= faktorial[i]){
			for(j=rear+1; j > i; j--){
				faktorial[j] = faktorial [j-1];
			}
			faktorial[i] = data;
			return;
		}
	}
	faktorial[i] = data;
//	display_pq();
}

void insert_pq(int data){
	if((front == -1) && (rear == -1)){
		front++; rear++;
		faktorial[rear] = data;
//		printf("faktorial = %d\n", faktorial[rear]);
		return;
	}
	else	check(data);
	rear++;
}

