#ifndef STL_UNINITIALIZED_H
#define STL_UNINITIALIZED_H
#include<cstdlib>
#include"stl_algobase.h"
#include<stl_iterator.h>
#include"stl_construct.h"
#include<__type_traits.h>
namespace tinystl {

//迭代器first 指向欲初始化空间的初始处
//n代表 初始化的空间大小
//x 代表初值
template<typename Iter,class Size ,class T,class T1>inline Iter _uninitialized_fill_n(Iter first,Size size,const T& x,T1*)
{
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    return _uninitialized_fill_n_aux(first,size,x,is_POD());
}
template<typename Iter,class Size,class T>inline Iter _uninitialized_fill_n_aux(Iter first,Size size,const T& x,__true_type)
{
    return fill_n(first,size,x);
}
template<typename Iter,class Size,class T>inline Iter _uninitialized_fill_n_aux(Iter first,Size size,const T&x,__false_type)
{
    Iter result = first;
    for(;size > 0;--size,++result)
    {
       construct(&*result,x);
    }
    return result;
}
template<typename Iter,class Size ,class T>inline Iter uninitialized_fill_n(Iter first,Size size,const T& x)
{
    return _uninitialized_fill_n(first,size,x,value_type(first));
}

//
//迭代器 first 指向输入端的起始位置
//迭代器 last 指向输入端的结束位置
//迭代器 result 指向输出端的起始位置
  template <class IIter, class FIter>
  FIter __uninitialized_copy(IIter first, IIter last, FIter result, __true_type)
  {
    return copy(first, last, result);
  }

  template <class IIter, class FIter>
  FIter __uninitialized_copy(IIter first, IIter last, FIter result, __false_type)
  {
    auto cur = result;
    for (; first != last; ++first, ++cur)
    {
      construct(&*cur, *first);
    }
    return cur;
  }

  template <class IIter, class FIter>
  FIter uninitialized_copy(IIter first, IIter last, FIter result)
  {
    return __uninitialized_copy(first, last, result,
                                       typename __type_traits<typename
                                       iterator_traits<IIter>::value_type
                                       >::is_POD_type());
  }

//
//迭代器 first 指向输出端的起始位置
//迭代器 last 指向输出端的结束处
//x 表示初值
  template <class FIter, class T>
  void __uninitialized_fill(FIter first, FIter last, const T& x, __true_type)
  {
    fill(first, last, x);
  }

  template <class FIter, class T>
  void __uninitialized_fill(FIter first, FIter last, const T& x, __false_type)
  {
    auto cur = first;
    for (; cur != last; ++cur)
    {
      construct(&*cur, x);
    }
  }

  template <class FIter, class T>
  void  uninitialized_fill(FIter first, FIter last, const T& x)
  {
    __uninitialized_fill(first, last, x, typename __type_traits<
                                typename iterator_traits<FIter>::
                                value_type>::is_POD_type());
  }

}
#endif // STL_UNINITIALIZED_H
