//
// Created by aojoie on 3/2/2023.
//

#ifndef ANG_ASTCONTEXT_HPP
#define ANG_ASTCONTEXT_HPP

#include <vector>
#include <cassert>

namespace AN {

class ASTAllocated;

/// \brief context to manage AST object life-circle
class ASTContext {
    std::vector<ASTAllocated *> allocations;
public:

    ~ASTContext();

    void *allocate(size_t size);

};

#ifdef _MSC_VER
#pragma warning(disable : 4291) // ignore no matching delete operator warning
#endif


/// ASTAllocated object lifetime is the lifetime of ASTContext
class ASTAllocated {
public:
    // Make vanilla new/delete illegal.
    void *operator new(size_t Bytes) noexcept = delete;
    void operator delete(void *data) noexcept {}

    // Only allow allocation using the allocator in ASTContext
    // or by doing a placement new.
    void *operator new(size_t bytes, ASTContext &ctx) {
        return ctx.allocate(bytes);
    }

    void *operator new(size_t Bytes, void *Mem) noexcept {
        assert(Mem && "placement new into failed allocation");
        return Mem;
    }

    virtual ~ASTAllocated() = default;
};

}

#endif//ANG_ASTCONTEXT_HPP
