// -*- Ang -*-
//===--------------------------- `target name` ---------------------------------===//
//
// SourceFile.hpp
// include/Basic
// Created by Molybdenum on 3/7/23.
//===----------------------------------------------------------------------===//

#ifndef ANG_SOURCEFILE_HPP
#define ANG_SOURCEFILE_HPP

#include "Basic/Error.hpp"
#include <string_view>

namespace AN {

class SourceFile {

    const char *_buffer{};

    struct Impl;
    Impl *impl;

public:

    SourceFile();

    bool init(std::string_view path, Error *error = nullptr);

    ~SourceFile();

    const char *getBuffer() {
        return _buffer;
    }


};


}

#endif//ANG_SOURCEFILE_HPP
