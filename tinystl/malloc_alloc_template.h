#ifndef MALLOC_ALLOC_TEMPLATE_H
#define MALLOC_ALLOC_TEMPLATE_H
#include<new>
#include<iostream>
#include<cstdlib>
namespace tinystl {

class __malloc_alloc_template
{
private:
    static void * oom_malloc(size_t);
    static void * oom_realloc(void*,size_t);
    static void (*_malloc_alloc_oom_handler)();
public:
    static void* allocte(size_t n)
    {
        void *restult = malloc(n);
        if(0 == restult)restult = oom_malloc(n);
        return restult;
    }
    static void deallocate(void* p,size_t /**/)
    {
        free(p);
    }
    static void *reallocate(void *p,size_t ,size_t new_sz)
    {
        void *result = realloc(p,new_sz);
        if(0 == result )result = oom_realloc(p,new_sz);
        return result;
    }
    static void (*set_malloc_handler(void (*f)()))()
    {
        void (*old)() = _malloc_alloc_oom_handler;
        _malloc_alloc_oom_handler = f;
        return (old);
    }
};
void(* __malloc_alloc_template::_malloc_alloc_oom_handler)();
void * __malloc_alloc_template::oom_malloc(size_t n)
{
    void (*my_malloc_hander)();
    void *result;
    for(;;)
    {
        my_malloc_hander = _malloc_alloc_oom_handler;
        if(0 == my_malloc_hander) {std::cerr<<"out of the memoty";}
        (*my_malloc_hander)();
        result = malloc(n);
        if(result) return result;
    }
}
void *__malloc_alloc_template::oom_realloc(void *p, size_t n)
{
    void (*my_malloc_hander)();
    void *result;
    for(;;)
    {
        my_malloc_hander = _malloc_alloc_oom_handler;
        if(0 == my_malloc_hander) {std::cerr<<"out of memory";}
        (*my_malloc_hander)();
        result = realloc(p,n);
        if(result) return (result);
    }
}
typedef __malloc_alloc_template alloc;
}
#endif // MALLOC_ALLOC_TEMPLATE_H
