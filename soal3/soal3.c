#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<stdbool.h>

int WakeUp_Status = 0;
int Spirit_Status = 100;
int All_Status = 0;
int agmal_wakeup = 0;
int iraj_sleep = 0;
int agmal_count = 0;
int iraj_count = 0;
int flag = 0;

void *agmal(void *ptr){

    while(WakeUp_Status < 100 && Spirit_Status > 0){        
        if(agmal_wakeup && iraj_count < 3){
            agmal_wakeup = 0;
            WakeUp_Status += 15;
            agmal_count++;
            if(WakeUp_Status >= 100){
                printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
                exit(1);
            }
        }
        
        if(iraj_count == 3){
            printf("Agmal Ayo Bangun disabled 10 s\n");
            sleep(10);
            agmal_wakeup = 0;
            iraj_count = 0;
            printf("Fitur Agmal Ayo Bangun telah aktif\n");
        }
    }
    flag = 1;
}

void *iraj(void *fitur){
	while(WakeUp_Status < 100 && Spirit_Status > 0){
        if(iraj_sleep && agmal_count < 3){
            iraj_sleep = 0;
            Spirit_Status -= 20;
            iraj_count++;
            if(Spirit_Status <= 0){
                printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
                exit(1);
            }
        }
        
        if(agmal_count == 3){
            printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
            sleep(10);
            iraj_sleep = 0;
            agmal_count = 0;
            printf("Fitur Iraj Ayo Tidur telah aktif\n");
        }
    }

    flag = 1;
}

int main(){
	pthread_t tid[3];
	pthread_create(&tid[0], NULL, iraj, NULL);
	pthread_create(&tid[1], NULL, agmal, NULL);
	
	while(flag == 0){
		char *input = malloc(100);
		while(1){
			scanf("%[^\n]%*c", input);
			if(strcmp(input, "All Status") == 0){
				All_Status = 1;
				printf("Agmal WakeUp_status = %d\n", WakeUp_Status);
				printf("Iraj Spirit_status = %d\n\n", Spirit_Status);
			}
			else if(strcmp(input, "Agmal Ayo Bangun") == 0) agmal_wakeup = 1;
			else if(strcmp(input, "Iraj Ayo Tidur") == 0) iraj_sleep = 1;
		}
	}

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	exit(0);
	return 0;
}
