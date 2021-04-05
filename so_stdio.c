#include "so_stdio.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

SO_FILE *so_fopen(const char *pathname, const char *mode) {
	SO_FILE *file = (SO_FILE *) malloc (sizeof(SO_FILE));
	if (file == NULL) return NULL;

	int flags = 0;
	if 		(strcmp(mode, "r")  == 0) flags |= O_RDONLY;
	else if (strcmp(mode, "r+") == 0) flags |= O_RDWR;
	else if (strcmp(mode, "w") 	== 0) flags |= O_CREAT | O_WRONLY | O_TRUNC;
	else if (strcmp(mode, "w+") == 0) flags |= O_CREAT | O_RDWR | O_TRUNC;
	else if (strcmp(mode, "a") 	== 0) flags |= O_CREAT | O_WRONLY;
	else if (strcmp(mode, "a+") == 0) flags |= O_CREAT | O_RDWR;
	else { free(file); return NULL; }

	file->fd = open(pathname, flags, 0644);
	if (file-> fd < 0) { free(file); return NULL; }
	if (strstr(mode, "a") != NULL) lseek(file->fd, 0, SEEK_END);

	return file;
}

int so_fclose(SO_FILE *stream) {
	int ret = 0;
	ret = close(stream->fd);
	free(stream);
	return ret;
}

int so_fileno(SO_FILE *stream) {
	return 0;
}


int so_fflush(SO_FILE *stream) {
	return 0;
}

int so_fseek(SO_FILE *stream, long offset, int whence) {
	return 0;
}

long so_ftell(SO_FILE *stream) {
	return 0;
}

size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream) {
	return 0;
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream) {
	return 0;
}

int so_fgetc(SO_FILE *stream) {
	return 0; 
}

int so_fputc(int c, SO_FILE *stream) {
	return 0;
}

int so_feof(SO_FILE *stream) {
	return 0;
}

int so_ferror(SO_FILE *stream) {
	return 0;
}

SO_FILE *so_popen(const char *command, const char *type) {
	return NULL;
}

int so_pclose(SO_FILE *stream) {
	return 0;
}
