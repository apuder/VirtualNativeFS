#include <jni.h>
#include <string>

#include "vnfs.h"

int vnfs_errno = 0;
int VNFS_EOF = -1;

static JavaVM *jvm;
static jclass clazzVNFS;
static jfieldID errnoFieldID;
static jmethodID fopenMethodId;
static jmethodID fcloseMethodId;
static jmethodID removeMethodId;
static jmethodID fwriteMethodId;
static jmethodID freadMethodId;
static jmethodID fgetcMethodId;
static jmethodID fputcMethodId;
static jmethodID fgetsMethodId;
static jmethodID fputsMethodId;
static jmethodID ftellMethodId;
static jmethodID fseekMethodId;
static jmethodID truncateMethodId;


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
    fgetcMethodId = env->GetStaticMethodID(type, "fgetc", "(J)I");
    fputcMethodId = env->GetStaticMethodID(type, "fputc", "(IJ)I");
    fgetsMethodId = env->GetStaticMethodID(type, "fgets", "(IJ)Ljava/lang/String;");
    fputsMethodId = env->GetStaticMethodID(type, "fputs", "(Ljava/lang/String;J)I");
    ftellMethodId = env->GetStaticMethodID(type, "ftell", "(J)J");
    fseekMethodId = env->GetStaticMethodID(type, "fseek", "(JJI)I");
    truncateMethodId = env->GetStaticMethodID(type, "truncate", "(Ljava/lang/String;I)I");
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
    JNIEnv* env = getEnv();
    return env->CallStaticLongMethod(clazzVNFS, ftellMethodId, (jlong) stream);
}

int vnfs_fseek(FILE *stream, long offset, int whence) {
    JNIEnv* env = getEnv();
    return env->CallStaticIntMethod(clazzVNFS, fseekMethodId, (jlong) stream, (jlong) offset, whence);
}

void vnfs_rewind(FILE *stream) {
    vnfs_fseek(stream, 0, SEEK_SET);
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
    va_list argp;
    char buf[160];

    va_start(argp, format);
    vsprintf(buf, format, argp);
    fputs(buf, stream);
    va_end(argp);
    return 0;
}

int vnfs_truncate(const char* path, off_t length) {
    JNIEnv* env = getEnv();
    jstring jpath = env->NewStringUTF(path);
    jint err = env->CallStaticIntMethod(clazzVNFS, truncateMethodId, jpath, length);
    env->DeleteLocalRef(jpath);
    update_errno();
    return err;
}

int vnfs_fgetc(FILE *stream) {
    JNIEnv* env = getEnv();
    return env->CallStaticIntMethod(clazzVNFS, fgetcMethodId, (jlong) stream);
}

int vnfs_fputc(int c, FILE *stream) {
    JNIEnv* env = getEnv();
    return env->CallStaticIntMethod(clazzVNFS, fputcMethodId, c, (jlong) stream);
}

int vnfs_fputs(const char *s, FILE *stream) {
    JNIEnv* env = getEnv();
    jstring js = env->NewStringUTF(s);
    jint err = env->CallStaticIntMethod(clazzVNFS, fputsMethodId, js, (jlong) stream);
    env->DeleteLocalRef(js);
    update_errno();
    return err;
}

char *vnfs_fgets(char *s, int n, FILE *stream) {
    JNIEnv* env = getEnv();
    jstring ptr = (jstring) env->CallStaticObjectMethod(clazzVNFS, fgetsMethodId, n, (jlong) stream);
    if (ptr == NULL) {
        return NULL;
    }
    const char* str = env->GetStringUTFChars(ptr, NULL);
    strcpy(s, str);
    env->ReleaseStringUTFChars(ptr, str);
    return s;
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
