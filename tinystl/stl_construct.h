#ifndef STL_CONSTRUCT_H
#define STL_CONSTRUCT_H
#include"__type_traits.h"
#include"stl_iterator.h"
#include<new>
namespace tinystl
{
template<typename T1,class T2>inline void construct(T1* p,const T2& value)
{
    new(p)T1(value);
}
template<typename T>inline void destory(T *p)
{
    p->~T();
}
//接受两个迭代器

template<typename Iter,class T>inline void _destory(Iter first,Iter last,T*)
{
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    _destory_aux(first,last,trivial_destructor());

}
template<typename Iter>inline void _destory_aux(Iter ,Iter ,__true_type){}

template<typename Iter>inline void _destory_aux(Iter first,Iter last,__false_type)
{
    for(;first !=  last;++first)
    {
        destory(&*first);
    }
}
inline void destory(char* ,char *){}
inline void destory(wchar_t*,wchar_t*){}
template<typename Iter>inline void destory(Iter a,Iter b)
{
    _destory(a,b,value_type(a));
}
}

#endif // STL_CONSTRUCT_H
