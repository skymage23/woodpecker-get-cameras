#ifndef MEMORY_HH
#define MEMORY_HH

#include <err_handling.hh>

#define PROTECTED_CALLOC(V,T,C,S) \
    (V) = static_cast<T*>(calloc(static_cast<size_t>(C),(S))); \
    if((V) == nullptr){ \
        out_of_memory(); \
    }


#endif