// -*- Ang -*-
//===--------------------------- `target name` ---------------------------------===//
//
// SourceFile.hpp
// include/Basic
// Created by Molybdenum on 3/7/23.
//===----------------------------------------------------------------------===//

#ifndef ANG_SOURCEFILE_HPP
#define ANG_SOURCEFILE_HPP

#include "Basic/typedef.h"
#include "Basic/Error.hpp"
#include <string_view>


namespace AN {

/// \brief SourceFile manages the mapping of the file into memory
class SourceFile : private NonCopyable {

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
