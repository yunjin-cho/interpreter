/// Yunjin Cho
/// The MappedFile class represents an open file from mmap.

#ifndef INT_MAPPED_FILE_H
#define INT_MAPPED_FILE_H

#include <cerrno>
#include <system_error>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

class MappedFile {
    const char *mapping = 0;
    size_t size = 0;
    int fd = -1;

public:
    MappedFile(const char* name);

    ~MappedFile();

    operator const char*() const {
        return mapping;
    }
};

#endif // INT_MAPPED_FILE_H
