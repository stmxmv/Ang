//
// Created by aojoie on 3/7/2023.
//

#ifndef ANG_TYPEDEF_H
#define ANG_TYPEDEF_H

#ifdef __cplusplus

#include <iostream>
#ifdef __cpp_lib_source_location
#include <source_location>
#endif

namespace AN {
struct NonCopyable {
protected:
    NonCopyable()                               = default;
    ~NonCopyable()                              = default;
    NonCopyable(NonCopyable &&)                 = default;
    NonCopyable &operator=(NonCopyable &&)      = default;
public:
    NonCopyable(const NonCopyable &)            = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;
};


/// \note https://cppinsights.io

#ifdef __clang__
template<typename, typename ...>
struct [[deprecated]] dump {};

#else
template<typename, typename ...>
struct dump;
#endif

template<typename ...T>
void runtime_dump() {
    std::cout <<
#ifdef __GNUC__
            std::string_view(__PRETTY_FUNCTION__).substr(5)
#elif defined(_MSC_VER)
            __FUNCSIG__
#else
#error "no runtime_dump() implementation for this compiler"
#endif
              << '\n';
}


template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
    return static_cast<typename std::underlying_type<E>::type>(e);
}

template<typename E>
struct enable_bitmask_operators : std::false_type {};

}

template<typename E>
auto operator|(E lhs,E rhs) -> typename std::enable_if<AN::enable_bitmask_operators<E>::value, E>::type {
    typedef typename std::underlying_type<E>::type underlying;
    return static_cast<E>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template<typename E>
auto operator&(E lhs,E rhs) -> typename std::enable_if<AN::enable_bitmask_operators<E>::value, E>::type {
    typedef typename std::underlying_type<E>::type underlying;
    return static_cast<E>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template<typename E>
auto operator^(E lhs,E rhs) -> typename std::enable_if<AN::enable_bitmask_operators<E>::value, E>::type {
    typedef typename std::underlying_type<E>::type underlying;
    return static_cast<E>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
}

template<typename E>
auto operator~(E e) -> typename std::enable_if<AN::enable_bitmask_operators<E>::value, E>::type {
    typedef typename std::underlying_type<E>::type underlying;
    return static_cast<E>(~static_cast<underlying>(e));
}

template<typename E>
auto operator&=(E &lhs,E rhs) -> typename std::enable_if<AN::enable_bitmask_operators<E>::value, E>::type {
    typedef typename std::underlying_type<E>::type underlying;
    lhs = lhs & rhs;
    return lhs;
}


template<typename E>
auto operator|=(E &lhs,E rhs) -> typename std::enable_if<AN::enable_bitmask_operators<E>::value, E>::type {
    typedef typename std::underlying_type<E>::type underlying;
    lhs = lhs | rhs;
    return lhs;
}


template<typename E>
auto operator^=(E &lhs,E rhs) -> typename std::enable_if<AN::enable_bitmask_operators<E>::value, E>::type {
    typedef typename std::underlying_type<E>::type underlying;
    lhs = lhs ^ rhs;
    return lhs;
}

#endif

#endif//ANG_TYPEDEF_H
