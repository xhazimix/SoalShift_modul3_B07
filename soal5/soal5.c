#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>

pthread_t tid[8];
int hp=300;
char monster_name[50];
int hunger_status=200;
int hygiene_status=100;
int enemy_hp=100;
int food_stock=100;
int *shop_food_stock;
int layer=1;
int battle_mode=0;
int bath_status=0;
int time_hunger=10;
int time_hygiene=30;
int time_regen=10;
int time_bath=20;

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

void *standby(void *menu){
	while(1){
		while(layer!=1){
		}
		system("clear");
		printf("\n-----%s's Profile-----\n\n", monster_name);
		printf("Health status : %d\n", hp);
		printf("Hunger : %d\n", hunger_status);
		printf("Hygiene : %d\n", hygiene_status);
		printf("Food left : %d\n", food_stock);

		if(bath_status!=1){
			printf("Bath will ready in : %d\n", time_bath);
		}
		else {
			printf("Bath is ready\n");
		}

		printf("1. Eat\n");
		printf("2. Bath\n");
		printf("3. Battle\n");
		printf("4. Shop\n");
		printf("5. Exit\n");
		sleep(1);
	}
}

void *bath_ready(void *pointer){
	while(1){
		while(bath_status==1){
		}
		sleep(1);
		if(time_bath==1){
			time_bath=20;
			bath_status=1;
		}
		else{
			time_bath--;
		}
	}
}

void *shop(void *menu){
	while(1){
		while(layer!=3){
		}
		system("clear");
		printf("\nSHOP MODE\n\n");
		printf("Shop food stock : %d\n", *shop_food_stock);
		printf("Your food stock : %d\n", food_stock);
		printf("Choices\n");
		printf("1. Buy\n");
		printf("2. Back\n");
		sleep(1);
	}
}

void *shared_memory(void *pointer){
	key_t key=1234;
	int shmid=shmget(key, sizeof(int), IPC_CREAT | 0666);
	shop_food_stock=shmat(shmid, NULL, 0);
	*shop_food_stock=2;
}

void *battle(void *menu){
	while(1){
		while(layer!=2){
		}
		system("clear");
		printf("\nBATTLE MODE\n\n");
		printf("Monster's Health : %d\n", hp);
		printf("Enemy's Health : %d\n", enemy_hp);
		printf("Choices\n");
		printf("1. Attack\n");
		printf("2. Run\n");
		sleep(1);
	}
}

void *regen_hp(void *pointer){
	while(1){
		while(layer==2){
		}
		sleep(1);
		if(time_regen==1){
			time_regen=10;
			hp+=5;
		}
		else{
			time_regen--;
		}
		
		if(hp<=0){
			printf("Monster anda mati karena dicabut nyawanya oleh malaikat maut\n");
			exit(EXIT_FAILURE);
		}
	}
}

void *decrement_hunger(void *pointer){
	while(1){
		while(layer==2){
		}
		sleep(1);
		if(time_hunger==1){
			time_hunger=10;
			hunger_status-=5;
		}
		else{
			time_hunger--;
		}
		if(hunger_status<=0){
			printf("Monster mati karena kamu pelit makanan\n");
			exit(EXIT_FAILURE);
		}
	}
}

void *decrement_hygiene(void *pointer){
	while(1){
		while(layer==2){
		}		
		sleep(1);
		if(time_hygiene==1){
			time_hygiene=30;
			hygiene_status-=10;
		}
		else{
			time_hygiene--;
		}
		if(hygiene_status<=0){
			printf("Monster mati karena anda dan peliharaan anda jarang mandi\n");
			exit(EXIT_FAILURE);
		}
	}
}

int main(){
	char choose;
	printf("Masukkan nama monster : ");
	scanf ("%49[^\n]",monster_name);
	pthread_create(&(tid[0]),NULL,standby,NULL);
	pthread_create(&(tid[1]),NULL,bath_ready,NULL);
	pthread_create(&(tid[2]),NULL,shop,NULL);
	pthread_create(&(tid[3]),NULL,shared_memory,NULL);
	pthread_create(&(tid[4]),NULL,battle,NULL);
	pthread_create(&(tid[5]),NULL,regen_hp,NULL);
	pthread_create(&(tid[6]),NULL,decrement_hunger,NULL);
	pthread_create(&(tid[7]),NULL,decrement_hygiene,NULL);

	while(1){
		choose=getch();
		if(choose=='1'){
			if(food_stock>0){
				printf("\nFeeding your monster\n");
				hunger_status+=15;
				food_stock--;
				if(hunger_status>200){
					hunger_status=200;
				}
			}
		}
		else if(choose=='2'){
			if(bath_status==1){
				printf("\nCleaning your monster\n"); sleep(1);
				bath_status=0;
				hygiene_status+=30;
				if(hygiene_status>100){
					hygiene_status=100;
				}
			}
			else if(bath_status==0){
				printf("\nBath not yet ready\n"); sleep(1);
			}
		}
		else if(choose=='3'){
			enemy_hp=100;
			while(1){
				layer=2;
				char choose_battle;
				choose_battle=getch();
				if(choose_battle=='1'){
					enemy_hp-=20;
					hp-=20;
					if(enemy_hp<=0){
						printf("\n You WIN\n\n");
						break;
					}
					else if(hp<=0){
						printf("\n You LOSE NOOB\n\n");
						exit(EXIT_FAILURE);
					}
				}
				if(choose_battle=='2'){
					layer=1;
					break;
				}
			}
		}
		else if(choose=='4'){
			while(1){
				layer=3;
				char choose_shop;
				choose_shop=getch();
				if(choose_shop=='1'){
					if(*shop_food_stock>0){
						food_stock+=1;
						*shop_food_stock-=1;
					}
					else{
						printf("\nSorry, Items SOLD OUT\n");
					}
				}
				if(choose_shop=='2'){
					layer=1;
					break;
				}
			}
		}
		else if(choose=='5'){
			system("clear");
			printf("\nSEE YOU NEXT TIME!\n\n");
			exit(EXIT_FAILURE);
		}
	}
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);
	pthread_join(tid[3],NULL);
	pthread_join(tid[4],NULL);
	pthread_join(tid[5],NULL);
	pthread_join(tid[6],NULL);
	pthread_join(tid[7],NULL);

	return 0;
}
