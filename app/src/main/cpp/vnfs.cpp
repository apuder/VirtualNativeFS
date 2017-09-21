#include <jni.h>
#include <string>

#include "vnfs.h"

int VNFS_EOF = 0xdeadbeef;

FILE *vnfs_fopen(const char *filename, const char *mode) {
    return NULL;
}

int vnfs_fclose(FILE *stream) {
    return 0;
}

long vnfs_ftell(FILE *stream) {
    return 0;
}

int vnfs_fseek(FILE *stream, long offset, int whence) {
    return 0;
}

void vnfs_rewind(FILE *stream) {

}

int vnfs_fflush(FILE *stream) {
    return 0;
}

size_t vnfs_fwrite(const void *ptr, size_t size, size_t nitems,
              FILE *stream) {
    return 0;
}

size_t vnfs_fread(void *ptr, size_t size, size_t nitems,
             FILE *stream) {
    return 0;
}

int vnfs_fprintf(FILE *stream, const char *format, ...) {
    return 0;
}

int vnfs_ftruncate(int fildes, off_t length) {
    return 0;
}

int vnfs_fgetc(FILE *stream) {
    return 0;
}

int vnfs_fputc(int c, FILE *stream) {
    return 0;
}

int vnfs_fputs(const char *s, FILE *stream) {
    return 0;
}

char *vnfs_fgets(char *s, int n, FILE *stream) {
    return 0;
}

int vnfs_fscanf(FILE *stream, const char *format, ... ) {
    return 0;
}

int vnfs_stat(const char *path, struct stat *buf) {
    return 0;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFS_sayHello(JNIEnv *env, jclass type) {
    std::string hello = "Hello from NDK";
    return env->NewStringUTF(hello.c_str());
}
