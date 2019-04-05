# Penjelasan Soal Shift Modul 3

### Nomer 1
Diminta untuk membuat program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan
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
