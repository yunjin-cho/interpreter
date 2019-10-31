/// Yunjin Cho

#include "interpreter/MappedFile.h"

MappedFile::MappedFile(const char* name) {
    fd = open(name, O_RDONLY);
    if (fd == -1)
        throw std::system_error(errno, std::system_category());
    struct stat sbuf;
    fstat(fd, &sbuf);
    size = sbuf.st_size;
    mapping = (const char*) mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapping == MAP_FAILED)
        throw std::system_error(errno, std::system_category());
}

MappedFile::~MappedFile() {
    munmap((void*)mapping, size);
}


