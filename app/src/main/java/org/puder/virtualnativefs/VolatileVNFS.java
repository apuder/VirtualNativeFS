package org.puder.virtualnativefs;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class VolatileVNFS implements VirtualNativeFSAPI {

    class FileDescriptor {
        long inode;
        String name;
        String mode;
        int pos;
        List<Byte> content;
    }

    class File {
        long inode;
        String name;
        List<Byte> content;
    }

    private int errno;

    private Map<String, Long> directory;
    private Map<Long, File> files;
    private Map<Long, FileDescriptor> fileDescriptors;
    private long nextAvailableInode;
    private long nextAvailableFileHandle;

    public VolatileVNFS() {
        errno = 0;
        directory = new HashMap<>();
        files = new HashMap<>();
        fileDescriptors = new HashMap<>();
        nextAvailableInode = 0;
        nextAvailableFileHandle = 1;
    }

    @Override
    public int getErrno() {
        return errno;
    }

    @Override
    public long fopen(String filename, String mode) {
        boolean writeMode = mode.contains("w");
        boolean appendMode = mode.contains("a");
        if (!directory.containsKey(filename) && !writeMode) {
            errno = 1;
            return 0;
        }
        errno = 0;
        if (!directory.containsKey(filename)) {
            long inode = nextAvailableInode++;
            directory.put(filename, inode);
            File file = new File();
            file.inode = inode;
            file.name = filename;
            file.content = new ArrayList<>();
            files.put(inode, file);
        }
        long inode = directory.get(filename);
        File file = files.get(inode);
        int filePosition = 0;
        if (appendMode) {
            filePosition = file.content.size();
        } else if (writeMode) {
            file.content.clear();
        }
        FileDescriptor fd = new FileDescriptor();
        fd.inode = inode;
        fd.mode = mode;
        fd.name = filename;
        fd.pos = filePosition;
        fd.content = file.content;
        long fileHandle = nextAvailableFileHandle++;
        fileDescriptors.put(fileHandle, fd);
        return fileHandle;
    }

    @Override
    public int fclose(long file) {
        fileDescriptors.remove(file);
        return 0;
    }

    @Override
    public int remove(String path) {
        if (!directory.containsKey(path)) {
            errno = 1;
            return -1;
        }
        long inode = directory.get(path);
        directory.remove(path);
        files.remove(inode);
        // TODO: remove file handles that use that file?
        errno = 0;
        return 0;
    }

    @Override
    public long ftell(long stream) {
        FileDescriptor fd = fileDescriptors.get(stream);
        return fd.pos;
    }

    @Override
    public int fseek(long stream, long offset, int whence) {
        FileDescriptor fd = fileDescriptors.get(stream);
        long pos = 0;
        switch(whence) {
            case 0:
                pos = offset;
                break;
            case 1:
                pos = fd.pos + offset;
                break;
            case 2:
                pos = fd.content.size() + offset;
                break;
        }
        if (pos < 0) {
            pos = 0;
        }
        if (pos > fd.content.size()) {
            pos = fd.content.size();
        }
        fd.pos = (int) pos;
        return 0;
    }

    @Override
    public int fflush(long stream) {
        return 0;
    }

    @Override
    public long fwrite(byte[] ptr, long size, long nitems, long stream) {
        long n = size * nitems;
        FileDescriptor fd = fileDescriptors.get(stream);
        for (int i = 0; i < n; i++) {
            if (fd.pos == fd.content.size()) {
                fd.content.add(fd.pos++, ptr[i]);
            } else {
                fd.content.set(fd.pos++, ptr[i]);
            }
        }
        return n;
    }

    @Override
    public long fread(byte[] ptr, long size, long nitems, long stream) {
        long n = size * nitems;
        FileDescriptor fd = fileDescriptors.get(stream);
        for (int i = 0; i < n; i++) {
            if (fd.pos == fd.content.size()) {
                return i;
            }
            ptr[i] = fd.content.get(fd.pos++);
        }
        return n;
    }

    @Override
    public int fprintf(long stream, String str) {
        return 0;
    }

    @Override
    public int truncate(String path, int length) {
        if (!directory.containsKey(path)) {
            errno = 1;
            return -1;
        }
        long inode = directory.get(path);
        File file = files.get(inode);
        int pos = file.content.size();
        if (pos >= length) {
            while (pos != length) {
                pos--;
                file.content.remove(pos);
            }
        } else {
            while (pos != length) {
                pos++;
                file.content.add((byte) 0);
            }
        }
        errno = 0;
        return 0;
    }

    @Override
    public int fgetc(long stream) {
        FileDescriptor fd = fileDescriptors.get(stream);
        if (fd.pos == fd.content.size()) {
            return -1;
        }
        return fd.content.get(fd.pos++);
    }

    @Override
    public int fputc(int c, long stream) {
        FileDescriptor fd = fileDescriptors.get(stream);
        if (fd.pos == fd.content.size()) {
            fd.content.add(fd.pos++, (byte) c);
        } else {
            fd.content.set(fd.pos++, (byte) c);
        }
        return c;
    }

    @Override
    public int fputs(String s, long stream) {
        for (int i = 0; i < s.length(); i++) {
            fputc(s.charAt(i), stream);
        }
        return 0;
    }

    @Override
    public String fgets(int n, long stream) {
        FileDescriptor fd = fileDescriptors.get(stream);
        if (fd.pos == fd.content.size()) {
            return null;
        }
        StringBuilder s = new StringBuilder();
        for (int i = 0; i < n - 1; i++) {
            if (fd.pos == fd.content.size()) {
                break;
            }
            byte ch = fd.content.get(fd.pos++);
            s.append((char) ch);
            if (ch == '\n') {
                break;
            }
        }
        return s.toString();
    }

    @Override
    public int fscanf(long stream, String format) {
        return 0;
    }

    @Override
    public int stat(String path, long XXXX) {
        return 0;
    }
}
