package org.puder.virtualnativefs;public class VirtualNativeFS {    static {        System.loadLibrary("vnfs");        init();    }    public static void touch() {        // Do nothing. Ensures class is loaded and initialized    }    native private static void init();    native public static String sayHello();    private static int fopen(String filename, String mode) {        return 0;    }    private static int fclose(int file) {        return 0;    }    private static long ftell(int stream) {        return 0;    }    private static int fseek(int stream, long offset, int whence) {        return 0;    }    private static void rewind(int stream) {    }    private static int fflush(int stream) {        return 0;    }    private static long fwrite(byte[] ptr, long size, long nitems, int stream) {        return 0;    }    private static long fread(byte[] ptr, long size, long nitems, int stream) {        return 0;    }    private static int fprintf(int stream, String str) {        return 0;    }    private static int truncate(String path, int length) {        return 0;    }    private static int fgetc(int stream) {        return 0;    }    private static int fputc(int c, int stream) {        return 0;    }    private static int fputs(String s, int stream) {        return 0;    }    private static String fgets(String s, int n, int stream) {        return null;    }    private static int fscanf(int stream, String format) {        return 0;    }    private static int stat(String path, long XXXX) {        return 0;    }}