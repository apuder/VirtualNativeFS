package org.puder.virtualnativefs;

public interface VirtualNativeFSAPI {
    int getErrno();
    long fopen(String filename, String mode);
    int remove(String path);
    int fclose(long file);
    long ftell(long stream);
    int fseek(long stream, long offset, int whence);
    void rewind(long stream);
    int fflush(long stream);
    long fwrite(byte[] ptr, long size, long nitems, long stream);
    long fread(byte[] ptr, long size, long nitems, long stream);
    int fprintf(long stream, String str);
    int truncate(String path, int length);
    int fgetc(long stream);
    int fputc(int c, long stream);
    int fputs(String s, long stream);
    String fgets(String s, int n, long stream);
    int fscanf(long stream, String format);
    int stat(String path, long XXXX);
}
