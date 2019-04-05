#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#define PORT 9090

void *nambah(void *k) {
	int *barang;
	barang=(void *)k;
	*barang+=1;
}

void *cetak(void *c) {
	int *barang;
	barang=(void *)c;
	printf("STock barang = %d\n", *barang); 
	sleep(5);
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

	key_t key = 1234;
        int *barang;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        barang = shmat(shmid, NULL, 0);

	char *tambah="Tambah";

        //shmdt(value);
        //shmctl(shmid, IPC_RMID, NULL);

    valread = read( new_socket , buffer, 1024);
    //printf("%s\n",buffer );
   //printf("Hello message sent\n");

//-----------------------Thread-----------------------

    pthread_t thread1, thread2;//inisialisasi awal
    int  iret1, iret2;

	  if(strcmp(buffer, tambah)==0) {
	    	iret1 = pthread_create( &thread1, NULL, nambah, (void*) barang); //membuat thread pertama
	    	if(iret1) //jika eror
	    	{
			fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
			exit(EXIT_FAILURE);
	    	}
	    	pthread_join( thread1, NULL);
	    }

while(1) {
	    iret2 = pthread_create( &thread2, NULL, cetak, (void*) barang);//membuat thread kedua
	    if(iret2)//jika gagal
	    {
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
		exit(EXIT_FAILURE);
	    }
	    pthread_join( thread2, NULL); 
	}

exit(EXIT_SUCCESS);
   return 0;
}
