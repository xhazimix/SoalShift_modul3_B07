#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int *shop_food_stock;
pthread_t tid[2];

static struct termios old, new;
/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
	tcgetattr(0, &old); /* grab old terminal i/o settings */
	new = old; /* make new settings same as old settings */
	new.c_lflag &= ~ICANON; /* disable buffered i/o */
	if (echo) {
		new.c_lflag |= ECHO; /* set echo mode */
	} else {
		new.c_lflag &= ~ECHO; /* set no echo mode */
	}
	tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
	tcsetattr(0, TCSANOW, &old);
}
char getch_(int echo) 
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
	return getch_(0);
}

void *shared_memory(void *pointer){
	key_t key=1234;
	int shmid=shmget(key, sizeof(int), IPC_CREAT | 0666);
	shop_food_stock=shmat(shmid, NULL, 0);\
}

void *menu(void *menu){
	while(1){
		system("clear");
		printf("\n-----SHOP----\n");
		printf("Shop Stock = %d\n", *shop_food_stock); 
		printf("1. Restock\n");
		printf("2. Exit\n");
		sleep(1);
	}
}

int main(){
	char choose;
	printf("\nSHOP\n");
	pthread_create(&(tid[0]),NULL,shared_memory,NULL);
	pthread_create(&(tid[1]),NULL,menu,NULL);

	while(1){
		choose=getch();
		if(choose=='1'){
			*shop_food_stock+=1;
			printf("\nRestock Proses...\n"); sleep(1);
		}
		else if(choose=='2'){
			system("clear");
			printf("\nSEE YOU NEXT TIME!\n\n");
			exit(EXIT_FAILURE);
		}
	}

}
