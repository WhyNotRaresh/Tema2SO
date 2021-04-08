#include "so_stdio.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

SO_FILE *so_fopen(const char *pathname, const char *mode)
{
	SO_FILE *file = (SO_FILE *) malloc(sizeof(SO_FILE));
	int flags = 0;

	if (file == NULL)
		return NULL;

	if (strcmp(mode, "r") == 0)
		flags |= O_RDONLY;
	else if (strcmp(mode, "r+") == 0)
		flags |= O_RDWR;
	else if (strcmp(mode, "w") == 0)
		flags |= O_CREAT | O_WRONLY | O_TRUNC;
	else if (strcmp(mode, "w+") == 0)
		flags |= O_CREAT | O_RDWR | O_TRUNC;
	else if (strcmp(mode, "a") == 0)
		flags |= O_CREAT | O_WRONLY | O_APPEND;
	else if (strcmp(mode, "a+") == 0)
		flags |= O_CREAT | O_RDWR | O_APPEND;
	else {
		free(file);
		return NULL;
	}

	file->fd = open(pathname, flags, 0644);
	if (file->fd < 0) {
		free(file);
		return NULL;
	}
	file->curr = lseek(file->fd, 0, SEEK_CUR);

	file->buffer = calloc(DEFAULT_BUF_SIZE, sizeof(char));
	if (file->buffer == NULL) {
		free(file);
		return NULL;
	}
	file->buf_size = DEFAULT_BUF_SIZE;
	file->offset = 0;

	file->flags = 0;

	return file;
}

int so_fclose(SO_FILE *stream)
{
	int ret = so_fflush(stream);

	if (ret != SO_EOF)
		ret = close(stream->fd);
	free(stream->buffer);
	free(stream);
	return ret;
}

int buf_read(SO_FILE *stream)
{
	memset(stream->buffer, 0, DEFAULT_BUF_SIZE);

	int ret = read(stream->fd, stream->buffer, DEFAULT_BUF_SIZE);

	if (ret == -1) {
		stream->flags |= READ_ERR;
		return SO_EOF;
	}

	if (ret < DEFAULT_BUF_SIZE) {
		stream->buf_size = ret;
		stream->flags |= LAST_BUF;
	}

	return ret;
}

int buf_write(SO_FILE *stream)
{
	// Fulushing contents
	so_fflush(stream) == SO_EOF;

	int ret = stream->offset;

	stream->offset = 0;
	memset(stream->buffer, 0, stream->buf_size);

	return ret;
}

int so_fflush(SO_FILE *stream)
{
	if ((stream->flags & WRITTEN) != 0) {
		stream->flags &= ~WRITTEN;

		// Writing stream-offset bytes into file
		size_t ret = 0;

		while (ret != stream->offset) {
			size_t bytes = write(stream->fd, stream->buffer + ret, stream->offset - ret);

			ret += bytes;
			if (bytes == -1) {
				stream->flags |= WRITE_ERR;
				return SO_EOF;
			}
		}
	}
	return 0;
}

int so_fileno(SO_FILE *stream)
{
	return (stream == NULL) ? -1 : stream->fd;
}

int so_fseek(SO_FILE *stream, long offset, int whence)
{
	if (so_fflush(stream) == SO_EOF)
		return SO_EOF;

	memset(stream->buffer, 0, stream->buf_size);
	stream->offset = 0;

	stream->flags &= ~REACH_EOF;
	stream->flags &= ~LAST_BUF;

	lseek(stream->fd, offset, whence);
	stream->curr = lseek(stream->fd, 0, SEEK_CUR);
	return 0;
}

long so_ftell(SO_FILE *stream)
{
	return stream->curr;
}

size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	if ((stream->flags & REACH_EOF) != 0)
		return 0;

	size_t ret = 0;

	do {
		// Reading into Buffer
		if (stream->offset == 0) {
			if (buf_read(stream) == SO_EOF)
				break;
		}

		// Number of bytes to copy
		size_t bytes_no = ((nmemb - ret) * size < stream->buf_size - stream->offset) ?
			(nmemb - ret) * size :
			stream->buf_size - stream->offset;

		// Writing at pointer
		memcpy(ptr + ret * size, stream->buffer + stream->offset, bytes_no);
		ret += bytes_no / size;

		// Last possible read sequance
		if ((stream->flags & LAST_BUF) != 0 && bytes_no == stream->buf_size - stream->offset)
			stream->flags |= REACH_EOF;

		// Updating offset
		stream->offset = (stream->offset + bytes_no) % stream->buf_size;
	} while (ret < nmemb);

	stream->curr += ret;

	return ret;
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	size_t ret = 0;

	do {
		// Flushing buffer
		if (stream->offset == DEFAULT_BUF_SIZE) {
			if (buf_write(stream) == SO_EOF)
				break;
		}

		// Number of bytes to write
		size_t bytes_no = ((nmemb - ret) * size < stream->buf_size - stream->offset) ?
			(nmemb - ret) * size :
			stream->buf_size - stream->offset;

		// Writing into buffer
		memcpy(stream->buffer + stream->offset, ptr + ret * size, bytes_no);
		ret += bytes_no / size;

		// Updating offset
		stream->offset += bytes_no;

		// Updating flags
		stream->flags |= WRITTEN;
	} while (ret < nmemb);

	stream->curr += ret;

	return ret;
}

int so_fgetc(SO_FILE *stream)
{
	if ((stream->flags & REACH_EOF) != 0)
		return SO_EOF;
	if (stream->offset == stream->buf_size && (stream->flags & LAST_BUF) != 0) {
		stream->flags |= REACH_EOF;
		return SO_EOF;
	}

	stream->offset %= stream->buf_size;

	// Reading into Buffer
	if (stream->offset == 0) {
		if (buf_read(stream) == SO_EOF)
			return SO_EOF;
	}

	unsigned char c = stream->buffer[stream->offset];

	stream->offset++;
	stream->curr++;

	return (int) c;
}

int so_fputc(int c, SO_FILE *stream)
{
	// Writing to file
	if (stream->offset == stream->buf_size) {
		if (buf_write(stream) == SO_EOF)
			return SO_EOF;
	}

	stream->buffer[stream->offset] = c;
	stream->offset++;
	stream->curr++;
	stream->flags |= WRITTEN;

	return (int) c;
}

int so_feof(SO_FILE *stream)
{
	return stream->flags & REACH_EOF;
}

int so_ferror(SO_FILE *stream)
{
	return ((stream->flags & (WRITE_ERR | READ_ERR)) != 0) ?
		SO_EOF : 0;
}

SO_FILE *so_popen(const char *command, const char *type)
{
	return NULL;
}

int so_pclose(SO_FILE *stream)
{
	return 0;
}
