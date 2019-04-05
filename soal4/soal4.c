#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdlib.h>

pthread_t tid[5];
int iter;

void* creating_file(void *arg){
    
    pthread_t id=pthread_self();
    
    iter=0;
    if(pthread_equal(id,tid[0])){
        system("ps aux | head -11 > /home/drajad/Documents/FolderProses1/SimpanProses1.txt");   
    }
    
    else if(pthread_equal(id,tid[1])){
        system("ps aux | head -11 > /home/drajad/Documents/FolderProses2/SimpanProses2.txt");
    }
    
    iter=1;
    return NULL;
}

void* compress_file(void *arg){
    
    while(iter!=1){
    }
    
    pthread_t id=pthread_self();
    if(pthread_equal(id,tid[2])){
        system("zip -qmj /home/drajad/Documents/FolderProses1/KompresProses1.zip /home/drajad/Documents/FolderProses1/SimpanProses1.txt");
    }
    
    else if(pthread_equal(id,tid[3])){
        system("zip -qmj /home/drajad/Documents/FolderProses2/KompresProses2.zip /home/drajad/Documents/FolderProses2/SimpanProses2.txt");
    }
    
    iter=2;
    return NULL;
}

void* extract_file(void *arg){
    
    while(iter!=2){
    }
    
    sleep(15);
    pthread_t id=pthread_self();
    if(pthread_equal(id,tid[4])){
        system("unzip -qd /home/drajad/Documents/FolderProses1 /home/drajad/Documents/FolderProses1/KompresProses1.zip");
    }
    
    else if(pthread_equal(id,tid[5])){
        system("unzip -qd /home/drajad/Documents/FolderProses2 /home/drajad/Documents/FolderProses2/KompresProses2.zip");   
    }
    
    return NULL;
}

int main(void)
{
    int i=0, cek;
    while(i<2){
        cek=pthread_create(&(tid[i]),NULL,&creating_file,NULL);
        if(cek!=0){
            printf("\n can't create thread : [%s]",strerror(cek));
        }
        i++;
    }
    
    while(i<4){
        cek=pthread_create(&(tid[i]),NULL,&compress_file,NULL);
        if(cek!=0){
            printf("\n can't create thread : [%s]",strerror(cek));
        }
        i++;
    }
    printf("\n Menunggu 15 detik untuk mengekstrak kembali\n        Sabar Coi \n\n");
    
    while(i<6){
        cek=pthread_create(&(tid[i]),NULL,&extract_file,NULL);
        if(cek!=0){
            printf("\n can't create thread : [%s]",strerror(cek));
        }
        i++;
    }
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    pthread_join(tid[2],NULL);
    pthread_join(tid[3],NULL);
    pthread_join(tid[4],NULL);
    pthread_join(tid[5],NULL);
    return 0;
}
