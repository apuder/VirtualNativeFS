#include <jni.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#define BEGIN_TEST() int check_counter = 0
#define END_TEST() return 0

#define CHECK(cond) check_counter++; if (!(cond)) return check_counter

#define CHECK_FILE_EXISTS(fn) {\
    FILE* f = fopen(fn, "r");\
    CHECK(f != NULL);\
    fclose(f);\
  }

#define CHECK_FILE_NOT_EXIST(fn) {\
    FILE* f = fopen(fn, "r");\
    CHECK(f == NULL);\
    CHECK(errno == ENOENT);\
  }

#define CHECK_CONTENT(fn, content) {\
    FILE* f = fopen(fn, "r");\
    int n = strlen(content);\
    char buf[n + 1];\
    size_t size = fread(buf, 1, n + 1, f);\
    CHECK(size == n);\
    buf[n] = '\0';\
    CHECK((strcmp(content, buf) == 0) && (strlen(buf) == n));\
    fclose(f);\
  }

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testFileNotExists(JNIEnv *env, jclass type) {
    BEGIN_TEST();
    CHECK_FILE_NOT_EXIST("/sdcard/does_not_exist");
    END_TEST();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testRemoveFail(JNIEnv *env, jclass type) {
    BEGIN_TEST();
    CHECK(remove("/sdcard/does_not_exist") != 0);
    CHECK(errno == ENOENT);
    END_TEST();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testCreateAndDeleteFile(JNIEnv *env,
                                                                               jclass type) {
    BEGIN_TEST();
    FILE* f = fopen("/sdcard/test_file", "w");
    CHECK(f != NULL);
    CHECK(fclose(f) == 0);
    CHECK_FILE_EXISTS("/sdcard/test_file");
    CHECK(remove("/sdcard/test_file") == 0);
    CHECK_FILE_NOT_EXIST("/sdcard/test_file");
    END_TEST();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testBasicReadWrite(JNIEnv *env, jclass type) {
    BEGIN_TEST();
    const char* content1 = "Hello World!";
    FILE* f = fopen("/sdcard/test_file", "w");
    CHECK(f != NULL);
    size_t size = fwrite(content1, 1, strlen(content1), f);
    CHECK(size == strlen(content1));
    fclose(f);
    CHECK_CONTENT("/sdcard/test_file", content1);

    // Overwrite same file with different content
    const char* content2 = "VNFS is awesome!";
    f = fopen("/sdcard/test_file", "wb");
    CHECK(f != NULL);
    size = fwrite(content2, 1, strlen(content2), f);
    CHECK(size == strlen(content2));
    fclose(f);
    CHECK_CONTENT("/sdcard/test_file", content2);

    // Remove file
    CHECK(remove("/sdcard/test_file") == 0);
    CHECK_FILE_NOT_EXIST("/sdcard/test_file");

    END_TEST();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testAppendToFile(JNIEnv *env, jclass type) {
    BEGIN_TEST();
    const char* content1 = "Hello World!";
    FILE* f = fopen("/sdcard/test_file", "w");
    fwrite(content1, 1, strlen(content1), f);
    fclose(f);

    const char* content2 = " VNFS is awesome!";
    f = fopen("/sdcard/test_file", "a");
    CHECK(f != NULL);
    size_t size = fwrite(content2, 1, strlen(content2), f);
    CHECK(size == strlen(content2));
    fclose(f);
    CHECK_CONTENT("/sdcard/test_file", "Hello World! VNFS is awesome!");

    // Remove file
    CHECK(remove("/sdcard/test_file") == 0);
    CHECK_FILE_NOT_EXIST("/sdcard/test_file");

    END_TEST();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testTell(JNIEnv *env, jclass type) {
    BEGIN_TEST();
    const char* content1 = "Hello World!";
    FILE* f = fopen("/sdcard/test_file", "w");
    CHECK(ftell(f) == 0);
    fwrite(content1, 1, strlen(content1), f);
    CHECK(ftell(f) == strlen(content1));
    fclose(f);

    const char* content2 = " VNFS is awesome!";
    f = fopen("/sdcard/test_file", "a");
    CHECK(f != NULL);
    CHECK(ftell(f) == strlen(content1));
    fwrite(content2, 1, strlen(content2), f);
    CHECK(ftell(f) == strlen(content1) + strlen(content2));
    fclose(f);
    CHECK_CONTENT("/sdcard/test_file", "Hello World! VNFS is awesome!");

    // Remove file
    CHECK(remove("/sdcard/test_file") == 0);
    CHECK_FILE_NOT_EXIST("/sdcard/test_file");

    END_TEST();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testGetc(JNIEnv *env, jclass type) {
    BEGIN_TEST();
    const char* content1 = "0123";
    FILE* f = fopen("/sdcard/test_file", "w");
    fwrite(content1, 1, strlen(content1), f);
    fclose(f);
    f = fopen("/sdcard/test_file", "r");
    CHECK(fgetc(f) == '0');
    CHECK(fgetc(f) == '1');
    CHECK(getc(f) == '2');
    CHECK(getc(f) == '3');
    CHECK(getc(f) == EOF);
    fclose(f);
    CHECK(remove("/sdcard/test_file") == 0);
    END_TEST();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testPutc(JNIEnv *env, jclass type) {
    BEGIN_TEST();
    const char* content1 = "Hello World!";
    FILE* f = fopen("/sdcard/test_file", "w");
    fwrite(content1, 1, strlen(content1), f);
    CHECK(fputc('0', f) == '0');
    CHECK(fputc('1', f) == '1');
    CHECK(putc('2', f) == '2');
    CHECK(putc('3', f) == '3');
    fclose(f);
    CHECK_CONTENT("/sdcard/test_file", "Hello World!0123");

    // Remove file
    CHECK(remove("/sdcard/test_file") == 0);
    CHECK_FILE_NOT_EXIST("/sdcard/test_file");

    END_TEST();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testSeekInFile(JNIEnv *env, jclass type) {
    BEGIN_TEST();
    const char* content = "0123456789abcdefghijklmnopqrstuvwxyz";
    FILE* f = fopen("/sdcard/test_file", "w+");
    fwrite(content, 1, strlen(content), f);
    CHECK(fseek(f, 5, SEEK_SET) == 0);
    CHECK(fgetc(f) == content[5]);
    CHECK(fseek(f, 3, SEEK_CUR) == 0);
    CHECK(fgetc(f) == content[5 + 3 + 1]);
    CHECK(fseek(f, -5, SEEK_END) == 0);
    CHECK(fgetc(f) == content[strlen(content) - 5]);
    CHECK(fseek(f, strlen(content) + 10, SEEK_SET) == 0);
    CHECK(fgetc(f) == EOF);
    fclose(f);
    // Remove file
    CHECK(remove("/sdcard/test_file") == 0);
    END_TEST();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testUpdateFile(JNIEnv *env, jclass type) {
    BEGIN_TEST();
    const char* content = "0123456789abcdefghijklmnopqrstuvwxyz";
    const char* newContent = "01234XY789Zbcdefghijklmnopqrstuvwxyz";
    FILE* f = fopen("/sdcard/test_file", "w+");
    fwrite(content, 1, strlen(content), f);
    CHECK(fseek(f, 5, SEEK_SET) == 0);
    CHECK(fputc('X', f) == 'X');
    CHECK(fputc('Y', f) == 'Y');
    CHECK(fseek(f, 3, SEEK_CUR) == 0);
    CHECK(fputc('Z', f) == 'Z');
    fclose(f);
    CHECK_CONTENT("/sdcard/test_file", newContent);

    // Remove file
    CHECK(remove("/sdcard/test_file") == 0);
    END_TEST();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testFlush(JNIEnv *env, jclass type) {
    BEGIN_TEST();
    const char* content = "0123456789abcdefghijklmnopqrstuvwxyz";
    FILE* f = fopen("/sdcard/test_file", "w+");
    fwrite(content, 1, strlen(content), f);
    CHECK(fflush(f) == 0);
    fclose(f);
    // Remove file
    CHECK(remove("/sdcard/test_file") == 0);
    END_TEST();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testTruncate(JNIEnv *env, jclass type) {
    BEGIN_TEST();
    const char* content = "0123456789abcdefghijklmnopqrstuvwxyz";
    FILE* f = fopen("/sdcard/test_file", "w+");
    fwrite(content, 1, strlen(content), f);
    fclose(f);
    CHECK(truncate("/sdcard/test_file", 12) == 0);
    CHECK_CONTENT("/sdcard/test_file", "0123456789ab");
    CHECK(truncate("/sdcard/test_file", 15) == 0);
    f = fopen("/sdcard/test_file", "r");
    char buf[20];
    size_t size = fread(buf, 1, 20, f);
    CHECK(size == 15);
    for (int i = 0; i < 15; i++) {
        char ch = buf[i];
        if (i < 12) {
            CHECK(ch == content[i]);
        } else {
            CHECK(ch == '\0');
        }
    }
    // Remove file
    CHECK(remove("/sdcard/test_file") == 0);
    END_TEST();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testRewind(JNIEnv *env, jclass type) {
    BEGIN_TEST();
    const char* content = "0123456789abcdefghijklmnopqrstuvwxyz";
    FILE* f = fopen("/sdcard/test_file", "w+");
    fwrite(content, 1, strlen(content), f);
    rewind(f);
    CHECK(fgetc(f) == content[0]);
    CHECK(fgetc(f) == content[1]);
    fclose(f);
    // Remove file
    CHECK(remove("/sdcard/test_file") == 0);
    END_TEST();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testPrintf(JNIEnv *env, jclass type) {
    BEGIN_TEST();
    const char* content = "42,X\n'ABC',-42";
    FILE* f = fopen("/sdcard/test_file", "w+");
    fprintf(f, "%d,%c\n", 42, 'X');
    fprintf(f, "'%s',%d", "ABC", -42);
    fclose(f);
    CHECK_CONTENT("/sdcard/test_file", content);
    // Remove file
    CHECK(remove("/sdcard/test_file") == 0);
    END_TEST();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testScanf(JNIEnv *env, jclass type) {
    BEGIN_TEST();
    const char* content = "25 Hamster 56789 0123 56a72";
    FILE* f = fopen("/sdcard/test_file", "w+");
    fwrite(content, 1, strlen(content), f);
    rewind(f);
    int i, n; float x; char name[50];
    n = fscanf(f, "%d%s", &i, name);
    CHECK(n == 2);
    CHECK(i == 25);
    CHECK(strcmp(name, "Hamster") == 0);
    (void) fscanf(f, "%2d%f%*d %[0123456789]", &i, &x, name);
    CHECK(i == 56);
    CHECK(x == 789);
    CHECK(strcmp(name, "56") == 0);
    CHECK(fgetc(f) == 'a');
    fclose(f);
    // Remove file
    CHECK(remove("/sdcard/test_file") == 0);
    END_TEST();
}

extern "C"
JNIEXPORT jint JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFSTest_testStat(JNIEnv *env, jclass type) {
    BEGIN_TEST();
    struct stat st;
    CHECK(stat("/sdcard/test_file", &st) == -1);
    const char* content = "0123456789abcdefghijklmnopqrstuvwxyz";
    FILE* f = fopen("/sdcard/test_file", "w+");
    fwrite(content, 1, strlen(content), f);
    fclose(f);
    CHECK(stat("/sdcard/test_file", &st) == 0);
    CHECK(st.st_size == strlen(content));
    // Remove file
    CHECK(remove("/sdcard/test_file") == 0);
    END_TEST();
}
