Nume: Badita Rares Octavian
Grupa: 333CB

# Tema 2 Sisteme de Operare

Organizare
-

### Structura de date folosita

Pentru gestionarea fisierelor folosesc structura de data ```_so_file``` ce contie campurile :
``` c
struct _so_file {
	int fd;				// File Descriptor
	ssize_t curr;			// Currsor position in file

	char* buffer;			// Buffer for buffered IO
	ssize_t offset;			// First unwritten position in buffer
	ssize_t buf_size;		// Max size of buffer

	int flags;
};
```

Pentru campu de flags am definit 5 flaguri care sunt setate in diferite conditii:
1.	```WRITTEN``` setat daca in buffer au fost scrisi bytes, dar acestia nu au fost scrisi mai departe in fisie;
1.	```REACH_EOF``` setat daca s-a ajuns la finalul fisierului cu cititul din buffer;
1.	```LAST_BUF``` setat daca bufferul curent a citit pana a ajuns la finalul fisierului;
1.	```WRITE_ERR``` setat daca exista vreo eroare la scrierea in fisier (daca functia ```write()``` intoarce -1);
1.	```READ_ERR``` setat daca exista vreai eroare la citirea din fisier (daca functia ```read()``` intoarce -1).

### Amanunte mici legate de implementare

Functia ```so_fflush()``` scrie in fisier doar daca flagul de WRITTEN este setat in stream.

La momentul inchiderii fisierului cu  ```so_fclose()``` sau la schimbarea pozitiei cursorului cu ```so_fseek()```,
pentru a nu se pirder caractere scrise in buffer si nu in fisier, se apleaza ```so_fflush()```.

Pozitia cursorului din functiile ```so_fwrite()``` si ```so_fread()``` este mutata dupa ce toate elementele au fost scrise/citite,
nu dupa fiecare apel al lui write/read.

Pentru a nu depasii End Of File, functiile ```so_fread()``` si ```so_fgetc()``` se uita la flagul de REACH_EOF inainte sa faca vreo actiune.

Functia ```so_ferror()``` returneaza SO_EOF doar daca unul din flagurile WRITE_ERR sau READ_ERR este setat.

Implementare
-

* Tema este realizata partial. Pe masina locala si pe vm checker am 78 de puncte. Nu este implementata partea de procese.
* Nu este implementata functionalitatea de Windows a temei.
* Tema am facut-o pe Windows integral, folosind WSL pentru a rula checkerul si Makefileul.
* Makefile-ul contine reguli de build, clean, si zip (pentru crearea arhivei);


Resurse Utile
-

* Au fost foarte utile [paginile de manual](https://man7.org/linux/man-pages/) de linux ale functiilor.


[Github](https://github.com/WhyNotRaresh/Tema2SO)
-
