//
// Created by aojoie on 3/2/2023.
//

#include "Grammar/ASTContext.hpp"

namespace AN::grammar {

ASTContext::~ASTContext() {
    for (void *mem : allocations) {
        free(mem);
    }
}

void *ASTContext::allocate(size_t size, unsigned alignment) {
    size = (size + (size_t)alignment - 1) & ~((size_t)alignment - 1);
    void *mem = malloc(size);
    allocations.push_back(mem);
    return mem;
}

}