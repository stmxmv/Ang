// -*- Ang -*-
//===--------------------------- `target name` ---------------------------------===//
//
// Error.hpp
// include/Basic
// Created by Molybdenum on 3/7/23.
//===----------------------------------------------------------------------===//

#ifndef ANG_ERROR_HPP
#define ANG_ERROR_HPP

#include <string>
#include <utility>

namespace AN {


class Error {
    uint64_t _code;
    std::string _description;
public:

    Error() noexcept : _code() {}

    Error(uint64_t code, std::string description) noexcept : _code(code), _description(std::move(description)) {}

    uint64_t getCode() const noexcept {
        return _code;
    }
    const std::string &getDescription() const noexcept {
        return _description;
    }
};

}

#endif//ANG_ERROR_HPP
