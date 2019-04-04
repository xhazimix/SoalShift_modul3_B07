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
