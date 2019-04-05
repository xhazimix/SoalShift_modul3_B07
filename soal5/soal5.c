#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>

char name[50];
int hunger_stat = 200;
int hygiene_stat = 100;
int health_stat = 300;
int health_monster = 100;
int bath_cool = 0;
int food = 5;
int stat = 0;
int flag = 0;
int alrdy_battle = 0;

char getch(){
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
	if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}

void *hunger(void *ptr){
	while(stat != 1){
		sleep(10);
		hunger_stat -= 5;
		if(hunger_stat <= 0){
			printf("Your pet died because of hunger!");
			flag = 1;
		}
	}
}

void *hygiene(void *ptr){
	while(stat != 1){
		sleep(10);
		hygiene_stat -= 10;
		if(hygiene_stat <= 0){
			printf("Your pet died because of dirty!");
			flag = 1;
		}
	}
}

void *bath(void *ptr){
	while(1){
		if(bath_cool != 0){
			hygiene_stat += 30;
			while(bath_cool != 0){
				sleep(1);
				bath_cool -= 1;
			}
		}
	}
}

void *regen(void *ptr){
	while(stat != 1){
		if(stat == 0){
			health_stat += 5;
			if(health_stat >= 300) {health_stat = 300;}
			sleep(10);
		}
	}
}

void *standby(void *ptr){
	char input;
	while(1){
		if(stat == 0){
			input = getch();
			if(input == '1'){
				if(food != 0){
					food--;
					hunger_stat += 15;
					if(hunger_stat > 200) {hunger_stat = 200;}
					printf("Eating ...\n");
					sleep(1);
				}
				else{
					printf("You have no food!\n");
					sleep(1);
				}
			}
			else if(input == '2'){
				if(hygiene_stat > 100) {hygiene_stat = 100;}
				printf("Bathing ...\n");
				sleep(1);
				if(bath_cool == 0) bath_cool = 20;
			}
			else if(input == '3') {stat = 1;}
			else if(input == '4') {stat = 2;}
			else {flag = 1;}
		}
	}
}

void *battle(void *ptr){
	char input;
	while(1){
		if(stat == 1){
			input = getch();
			if(input == '1'){
				printf("Attack!!\n"); sleep(1);
				health_monster -= 20;
				printf("Getting Hit!!\n"); sleep(1);
				health_stat -= 20;
				if(health_monster == 0){
					printf("Monster defeated!!\n"); sleep(1);
					health_monster = 100;
					stat = 0;
				}
				alrdy_battle = 1;
			}
			else if(input == '2'){
				printf("Escaping ...\n");
				health_monster = 100;
				stat = 0;
				alrdy_battle = 1;
			}
		}
	}

	if(alrdy_battle == 1){
	pthread_t tid2[10];
	pthread_create(&tid2[0], NULL, hunger, NULL);
  	pthread_create(&tid2[1], NULL, hygiene, NULL);
  	pthread_create(&tid2[2], NULL, bath, NULL);
  	pthread_create(&tid2[3], NULL, regen, NULL);
  	pthread_create(&tid2[4], NULL, standby, NULL);
  	pthread_create(&tid2[5], NULL, battle, NULL);

	pthread_join(tid2[0], NULL);
	pthread_join(tid2[1], NULL);
	pthread_join(tid2[2], NULL);
	pthread_join(tid2[3], NULL);
	pthread_join(tid2[4], NULL);
	pthread_join(tid2[5], NULL);

	alrdy_battle = 0;
	}
}

void *shop(void *ptr){
	char input;
	while(1){
		if(stat == 2){
			printf("not yet done!!\n"); sleep(1);
			stat = 0;
		}
	}
}

int main(){
	printf("Give your pet a name: ");
	scanf("%s", name);
	
	pthread_t tid[10];
	pthread_create(&tid[0], NULL, hunger, NULL);
  	pthread_create(&tid[1], NULL, hygiene, NULL);
  	pthread_create(&tid[2], NULL, bath, NULL);
  	pthread_create(&tid[3], NULL, regen, NULL);
  	pthread_create(&tid[4], NULL, standby, NULL);
  	pthread_create(&tid[5], NULL, battle, NULL);

  	while(flag == 0){
  		if(stat == 0){
  			printf("Standby\n");
			printf("-- %s's Status --\n", name);
  			printf("Health : %d\n", health_stat);
  			printf("Hunger : %d\n", hunger_stat);
  			printf("Hygiene : %d\n", hygiene_stat);
  			printf("Food : %d\n", food);
  			if (bath_cool == 0) {printf("Bath is available\n");}
  			else {printf("Bath will be available after %ds\n", bath_cool);}
  			printf("Choices: \n");
  			printf("1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n");
		}
		else if(stat == 1){
			printf("Monster Battle!\n");
			printf("-- %s's Status --\n", name);
			printf("Pet Health : %d\n", health_stat);
			printf("Monster Health : %d\n", health_monster);
			printf("Choices : \n");
			printf("1. Attack\n2. Run\n\n");
		}
		else if(stat == 2){
			printf("Shopping for Food!");
			printf("Choices: \n");
			printf("1. Buy\n2.Exit\n");
		}
		sleep(1);
		system("clear");
	}
	
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);
	pthread_join(tid[3], NULL);
	pthread_join(tid[4], NULL);
	pthread_join(tid[5], NULL);
	
	exit(0);
	return 0;
}
