# Penjelasan Soal Shift Modul 3

### Nomer 1
* Diminta untuk membuat program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan
Contoh:
    ./faktorial 5 3 4
    
    3! = 6
    
    4! = 24
    
    5! = 120

###### Jawab
Source Code: [soal1.c](https://github.com/xhazimix/SoalShift_modul3_B07/blob/master/soal1/soal1.c)

* Menyimpan input dari command line dengan memanfaatkan argc dan argv
* Menggunakan strtol untuk mengubah value string menjadi int
  ```
  int main(int argc, char *argv[]){
	   int i;
	   for(i=0; i<argc-1; i++){
		    int a = strtol(argv[i+1], NULL, 10);
     }
  }
  ```
  
* Fungsi faktorial yang digunakan adalah rekursi
   ```
   int func_faktorial(int data){
	    if(data == 0) return 1;
	    else return (data * func_faktorial(data-1));
   }
   ```

* Menggunakan priority queue ascending agar lebih mudah dalam pengeluaran output
* Terdapat tiga fungsi yang digunakan
  ```
  void display_pq();
  void check();
  void insert_pq();
  ```

### Nomer 2
* Diminta membuat dua server [server-pembeli](https://github.com/xhazimix/SoalShift_modul3_B07/blob/master/soal2/server-pembeli.c) dan [server-penjual](https://github.com/xhazimix/SoalShift_modul3_B07/blob/master/soal2/server-penjual.c)
* 1 server terhubung satu client : membedakan portnya port 8080 dan port 9090
* Mempunyai penyimpanan yang sama : menggunakan shared memory
  ```
  key_t key = 1234;
  int *value;

  int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
  value = shmat(shmid, NULL, 0);
  ```
* Client penjual hanya bisa menambah dengan mengirimkan string "Tambah"
  1. Membuat input di client
     ```
     char *nambah;
     scanf("%s", nambah);
     ```
  2. Membuat fungsi tambah di server
     ```
     void *nambah(void *k) {
        int *barang;
        barang=(void *)k;
        *barang+=1;
     }
     ```
* Client pembeli hanya bisa mengurangi dengan mengirimkan string "Beli"
  1. Membuat input di client
     ```
     char *kurang;
     scanf("%s", kurang);
     ```
  2. Membuat fungsi kurang di server
     ```
     void *kurang(void *k) {
        int *barang;
        barang=(void *)k;
        if(*barang>0) {
	   *barang-=1;
        }
     }
     ```
* Mengirimkan info ke client apakah transaksi berhasil atau tidak
  ```
  char *gagal="Transaksi Gagal";
  char *berhasil="Transaksi Berhasil";
  
  if(*barang<=0) {
    	send(new_socket , gagal , strlen(gagal) , 0 );
  }

  else  {
   	send(new_socket , berhasil , strlen(berhasil) , 0 );
  }
  ```
* Server penjual akan mencetak stok setiap 5 detik
  ```
  void *cetak(void *c) {
	int *barang;
	barang=(void *)c;
	printf("STock barang = %d\n", *barang); 
	sleep(5);
  }
  ```
* Menggunakan thread, socket dan shared memory : semua codingan saya menggunakan template yang disediakan di modul 3

### Nomer 3
Source Code : [soal3.c] (https://github.com/xhazimix/SoalShift_modul3_B07/blob/master/soal3/soal3.c)
* Membuat kedua karakter (Agmal dan Iraj) menjadi dua fungsi void
   ```
   void agmal(void *ptr){
      ...
   }
   
   void iraj(void *ptr){
      ...
   }
   ```
* Kemudian, kedua fungsi ini akan dijalankan bersama-sama menggunakan thread
   ```
   pthread_t tid[3];
   pthread_create(&tid[0], NULL, iraj, NULL);
   pthread_create(&tid[1], NULL, agmal, NULL);
   ...
   pthread_join(tid[0], NULL);
   pthread_join(tid[1], NULL);
   ```
* Semua status yang dibutuhkan diinisialisasi pada awal code. Count digunakan untuk menghitung, flag digunakan untuk penanda syarat agar program diberhentikan terpenuhi, dan agmal_wakeup serta iraj_sleep digunakan untuk penanda user memberikan command ke fungsi agmal atau iraj
   ```
   int WakeUp_Status = 0;
   int Spirit_Status = 100;
   int All_Status = 0;
   int agmal_wakeup = 0;
   int iraj_sleep = 0;
   int agmal_count = 0;
   int iraj_count = 0;
   int flag = 0;
   ```

* "All Status", yaitu menampilkan status kedua sahabat
   ```
   if(strcmp(input, "All Status") == 0){
	All_Status = 1;
	printf("Agmal WakeUp_status = %d\n", WakeUp_Status);
	printf("Iraj Spirit_status = %d\n\n", Spirit_Status);
   }
   ```
   
* “Agmal Ayo Bangun” menambah WakeUp_Status Agmal sebesar 15 point dan jika Spirit_Status Iraj <= 0, program akan berhenti.
   ```
   if(iraj_sleep && agmal_count < 3){
         iraj_sleep = 0;
         Spirit_Status -= 20;
         iraj_count++;
         if(Spirit_Status <= 0){
             printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
             exit(1); //program berhenti
         }
   }
   ```
   
* “Iraj Ayo Tidur” mengurangi Spirit_Status Iraj sebanyak 20 point dan jika WakeUp_Status Agmal >= 100, program akan berhenti.
   ```
   if(agmal_wakeup && iraj_count < 3){
         agmal_wakeup = 0;
         WakeUp_Status += 15;
         agmal_count++;
         if(WakeUp_Status >= 100){
             printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
             exit(1); //program berhenti
         }
   }
   ```

* Jika Fitur “Agmal Ayo Bangun” dijalankan sebanyak 3 kali, maka secara otomatis fitur “Iraj Ayo Tidur” tidak bisa dijalankan selama 10 detik menggunakan sleep(10)
   ```
   if(agmal_count == 3){
         printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
         sleep(10);
         iraj_sleep = 0;
         agmal_count = 0;
         printf("Fitur Iraj Ayo Tidur telah aktif\n");
   }
   ```

* Jika Fitur  “Iraj Ayo Tidur” dijalankan sebanyak 3 kali, maka Fitur “Agmal Ayo Bangun” Tidak bisa dijalankan selama 10 detik menggunakan sleep(10);
   ```
   if(iraj_count == 3){
         printf("Agmal Ayo Bangun disabled 10 s\n");
         sleep(10);
         agmal_wakeup = 0;
         iraj_count = 0;
         printf("Fitur Agmal Ayo Bangun telah aktif\n");
   }
   ```

### Nomer 4

* Membuat fungsi untuk menyimpan ps-aux minimal 10
  ```
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
  ```
  Keterangan :
  1. head -11 digunakan untuk membatasi jumlah progam yang dimasukkan di dalam file, karena yang diminta maximal 10 -> maka mengambil batas -11, kalau mengambil batas -10 nanti yang masuk di progam 9
  2. variabel iter nanti digunakan untuk mengecek eror
  
* Membuat fungsi untuk mengkompress
  ```
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
  ```
  Keterangan :
  1. Diminta untuk menghapus langsung file setelah di zip, oleh karena itu saya menggunakan -qmj, secara otomatis akan menghapus file lama
  2. Karena diminta untuk menaruh file kompresnya pada file tertentu maka format pengisian zipnya (nama file zip dengan direktorinya) dan (nama file yang akan di kompress dengan direktorinya)
  
* Membuat fungsi untuk mengextract file
  ```
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
  ```
  Keterangan :
  1. Menggunakan -qd karena hasil extractnya diminta untuk ditaruh pada file tertentu
  2. Format pengisiannya (tempat file extractnya) dan (tempat file kompresnya beserta direktorinya)
  
* Karena takut terjadi kesalahan saat proses pembuatan file, mengkompres, atau mengextract, maka saya membuat kondisi untuk menampilkan eror yang terjadi jika terdapat kesalahan
  ```
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
    ```
    Keterangan :
    1. while pertama digunakan untuk menjalankan fungsi creating_file dan mengecek apa ada kesalahan pada proses pembuatan file
    2. while kedua digunakan untuk menjalankan fungsi compress_file dan mengcek apa ada kesalahan pada proses kompress file
    3. whike ketiga digunakan untuk menjalankan fungsi extract_file dan mengecek apa ada kesalahan pada proses extract file
    
* Karena diminta untuk menjalankan secara bersamaan, maka digunakan pthread_join
  ```
  pthread_join(tid[0],NULL);
  pthread_join(tid[1],NULL);
  pthread_join(tid[2],NULL);
  pthread_join(tid[3],NULL);
  pthread_join(tid[4],NULL);
  pthread_join(tid[5],NULL);
  ```
