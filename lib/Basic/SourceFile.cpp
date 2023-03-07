// -*- Ang -*-
//===--------------------------- `target name` ---------------------------------===//
//
// SourceFile.cpp
// lib/Basic
// Created by Molybdenum on 3/7/23.
//===----------------------------------------------------------------------===//

#include "Basic/SourceFile.hpp"

#ifdef _WIN32

#else

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include <iostream>

#endif

namespace AN {

struct SourceFile::Impl {
#ifdef _WIN32

#else
    int fd;
    size_t len;
#endif
};


SourceFile::SourceFile() : impl(new Impl()) {}


bool SourceFile::init(std::string_view path, Error *error) {
    // map file into memory
#ifdef _WIN32


#else
    int fd = open(path.data(), O_RDONLY);
    if (fd == -1) {
        if (error) {
            *error = Error(1, "open failed");
        }
        return false;
    }

    impl->fd = fd;

    // mmap
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        if (error) {
            *error = Error(2, "call fstat failed");
        }
        return false;
    }

    impl->len = sb.st_size;

    _buffer = (char *) mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (_buffer == MAP_FAILED) {
        if (error) {
            *error = Error(3, "map file failed");
        }
    }

#endif

    return true;
}

SourceFile::~SourceFile() {
    if (impl) {

        if (_buffer) {
            if (munmap((void *)_buffer, impl->len) == -1) {
                std::cerr << "Fail to unmap file at buffer" << (void *)_buffer << '\n';
            }
        }

        close(impl->fd);

        delete impl;
        impl = nullptr;
    }
}

}