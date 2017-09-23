#include <jni.h>
#include <string>

#include "vnfs.h"

int vnfs_errno = 0;
int VNFS_EOF = 0xdeadbeef;

static JavaVM *jvm;
static jclass clazzVNFS;
static jfieldID errnoFieldID;
static jmethodID fopenMethodId;
static jmethodID fcloseMethodId;
static jmethodID removeMethodId;
static jmethodID fwriteMethodId;
static jmethodID freadMethodId;


static JNIEnv* getEnv() {
    JNIEnv *env;
    jvm->AttachCurrentThread(&env, NULL);
    return env;
}

static void update_errno() {
    vnfs_errno = getEnv()->GetStaticIntField(clazzVNFS, errnoFieldID);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFS_init(JNIEnv *env, jclass type) {
    clazzVNFS = (jclass) env->NewGlobalRef(type);
    int status = env->GetJavaVM(&jvm);
    errnoFieldID = env->GetStaticFieldID(type, "errno", "I");
    fopenMethodId = env->GetStaticMethodID(type, "fopen", "(Ljava/lang/String;Ljava/lang/String;)J");
    fcloseMethodId = env->GetStaticMethodID(type, "fclose", "(J)I");
    removeMethodId = env->GetStaticMethodID(type, "remove", "(Ljava/lang/String;)I");
    fwriteMethodId = env->GetStaticMethodID(type, "fwrite", "([BJJJ)J");
    freadMethodId = env->GetStaticMethodID(type, "fread", "([BJJJ)J");
}

FILE *vnfs_fopen(const char *filename, const char *mode) {
    JNIEnv* env = getEnv();
    jstring jfilename = env->NewStringUTF(filename);
    jstring jmode = env->NewStringUTF(mode);
    jlong file = env->CallStaticLongMethod(clazzVNFS, fopenMethodId, jfilename, jmode);
    env->DeleteLocalRef(jfilename);
    env->DeleteLocalRef(jmode);
    update_errno();
    return (FILE*) file;
}

int vnfs_fclose(FILE *stream) {
    JNIEnv* env = getEnv();
    jint err = env->CallStaticIntMethod(clazzVNFS, fcloseMethodId, (jlong) stream);
    return err;
}

int remove(const char* path) {
    JNIEnv* env = getEnv();
    jstring jpath = env->NewStringUTF(path);
    jint err = env->CallStaticIntMethod(clazzVNFS, removeMethodId, jpath);
    env->DeleteLocalRef(jpath);
    update_errno();
    return err;
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
    JNIEnv* env = getEnv();
    long n = size * nitems;
    jbyteArray data = env->NewByteArray(n);
    env->SetByteArrayRegion(data, 0, n, (jbyte*) ptr);
    jlong len = env->CallStaticLongMethod(clazzVNFS, fwriteMethodId, data, size, nitems, (jlong) stream);
    env->DeleteLocalRef(data);
    return len;
}

size_t vnfs_fread(void *ptr, size_t size, size_t nitems,
             FILE *stream) {
    JNIEnv* env = getEnv();
    long n = size * nitems;
    jbyteArray data = env->NewByteArray(n);
    jlong len = env->CallStaticLongMethod(clazzVNFS, freadMethodId, data, size, nitems, (jlong) stream);
    env->GetByteArrayRegion(data, 0, n, (jbyte*) ptr);
    env->DeleteLocalRef(data);
    return len;
}

int vnfs_fprintf(FILE *stream, const char *format, ...) {
    return 0;
}

int vnfs_truncate(const char* path, off_t length) {
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
