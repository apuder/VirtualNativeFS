
#ifndef __VNFS_H__
#define __VNFS_H__

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
#define fgetc vnfs_getc
#define fputc vnfs_putc
#define getc fgetc
#define putc fputc
#define fputs vnfs_fputs
#define fgets vnfs_fgets
#define fscanf vnfs_fscanf
#define stat vnfs_stat

#endif
