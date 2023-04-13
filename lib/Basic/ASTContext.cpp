//
// Created by aojoie on 3/2/2023.
//

#include "Basic/ASTContext.hpp"

namespace AN {

ASTContext::~ASTContext() {
    for (ASTAllocated *mem : allocations) {
        delete mem;
    }
}

void *ASTContext::allocate(size_t size) {
//    size = (size + (size_t)alignment - 1) & ~((size_t)alignment - 1);
    void *mem = malloc(size);
    allocations.push_back((ASTAllocated *)mem);
    return mem;
}

}