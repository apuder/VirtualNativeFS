
#ifndef __VNFS_H__
#define __VNFS_H__

#include <sys/stat.h>

typedef int VNFS_FILE;

extern int VNFS_EOF;

struct vnfs_stat {
    int st_size;
};

#define FILE VNFS_FILE
#define EOF VNFS_EOF
#define fopen vnfs_fopen
#define fclose vnfs_fclose
#define ftell vnfs_ftell
#define fseek vnfs_fseek
#define rewind vnfs_rewind
#define fflush vnfs_fflush
#define fwrite vnfs_fwrite
#define fread vnfs_fread
#define fprintf vnfs_fprintf
#define ftruncate vnfs_ftruncate
#define fgetc vnfs_fgetc
#define fputc vnfs_fputc
#define getc fgetc
#define putc fputc
#define fputs vnfs_fputs
#define fgets vnfs_fgets
#define fscanf vnfs_fscanf
#define stat vnfs_stat

FILE *vnfs_fopen(const char *filename, const char *mode);
int vnfs_fclose(FILE *stream);
long vnfs_ftell(FILE *stream);
int vnfs_fseek(FILE *stream, long offset, int whence);
void vnfs_rewind(FILE *stream);
int vnfs_fflush(FILE *stream);
size_t vnfs_fwrite(const void *ptr, size_t size, size_t nitems,
                   FILE *stream);
size_t vnfs_fread(void *ptr, size_t size, size_t nitems,
                  FILE *stream);
int vnfs_fprintf(FILE *stream, const char *format, ...);
int vnfs_ftruncate(int fildes, off_t length);
int vnfs_fgetc(FILE *stream);
int vnfs_fputc(int c, FILE *stream);
int vnfs_fputs(const char *s, FILE *stream);
char *vnfs_fgets(char *s, int n, FILE *stream);
int vnfs_fscanf(FILE *stream, const char *format, ... );
int vnfs_stat(const char *path, struct stat *buf);

#endif
