Nume: Badita Rares Octavian
Grupa: 333CB

# Tema 2 Sisteme de Operare

Organizare
-

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




Implementare
-

* Tema este realizata partial. Pe masina locala si pe vm checker am 78 de puncte. Nu este implementata partea de procese.
* Nu este implementata functionalitatea de Windows a temei.
* Tema am facut-o pe Windows integral, folosind WSL pentru a rula checkerul si Makefileul.


Resurse Utile
-

* Au fost foarte utile [paginile de manual](https://man7.org/linux/man-pages/) de linux ale functiilor.


[Github](https://github.com/WhyNotRaresh/Tema2SO)
-
