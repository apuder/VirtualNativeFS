package org.puder.virtualnativefs;

public class VirtualNativeFS {

    static {
        System.loadLibrary("vnfs");
        init();
    }

    private static VirtualNativeFSAPI vnfsImpl;

    public static void init(VirtualNativeFSAPI impl) {
        vnfsImpl = impl;
    }

    native private static void init();
    native public static String sayHello();

    private static int errno;

    private static long fopen(String filename, String mode) {
        long f = vnfsImpl.fopen(filename, mode);
        errno = vnfsImpl.getErrno();
        return f;
    }

    private static int fclose(long file) {
        return vnfsImpl.fclose(file);
    }

    private static int remove(String path) {
        int err = vnfsImpl.remove(path);
        errno = vnfsImpl.getErrno();
        return err;
    }

    private static long ftell(long stream) {
        return vnfsImpl.ftell(stream);
    }

    private static int fseek(long stream, long offset, int whence) {
        return vnfsImpl.fseek(stream, offset, whence);
    }

    private static int fflush(long stream) {
        return vnfsImpl.fflush(stream);
    }

    private static long fwrite(byte[] ptr, long size, long nitems, long stream) {
        return vnfsImpl.fwrite(ptr, size, nitems, stream);
    }

    private static long fread(byte[] ptr, long size, long nitems, long stream) {
        return vnfsImpl.fread(ptr, size, nitems, stream);
    }

    private static int fprintf(long stream, String str) {
        return 0;
    }

    private static int truncate(String path, int length) {
        return vnfsImpl.truncate(path, length);
    }

    private static int fgetc(long stream) {
        return vnfsImpl.fgetc(stream);
    }

    private static int fputc(int c, long stream) {
        return vnfsImpl.fputc(c, stream);
    }

    private static int fputs(String s, long stream) {
        return vnfsImpl.fputs(s, stream);
    }

    private static String fgets(int n, long stream) {
        return vnfsImpl.fgets(n, stream);
    }

    private static int fscanf(long stream, String format) {
        return vnfsImpl.fscanf(stream, format);
    }

    private static int stat(String path, long XXXX) {
        return vnfsImpl.stat(path, XXXX);
    }
}
