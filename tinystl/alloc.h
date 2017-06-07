#ifndef ALLOC_H
#define ALLOC_H
#include"malloc_alloc_template.h"
namespace tinystl {

template<class T,class ALLOC = alloc>class allocator
{
public:
    static T* allocate(size_t n){return (0== n)?0:(T*)ALLOC::allocte(n*sizeof(T));}
    static T* allocate(){return (T*)ALLOC::allocte(sizeof(T));}
    static void deallocate(T *p,size_t n = 0)
    {
        ALLOC::deallocate(p,n);
    }

};
}

#endif // ALLOC_H
