#ifndef STL_ALGOBASE_H
#define STL_ALGOBASE_H
#include"__type_traits.h"
#include"stl_iterator.h"
#include<cstring>
#include"__type_traits.h"
#include<cstddef>
#include<utility>
namespace tinystl {

//fill()
 template<typename ForwardIterator,typename T>void fill(ForwardIterator first,ForwardIterator last,const T& x)
{
    auto curr = first;
    for(;curr != last;++curr)
    {
        *curr = x;
    }
}
//fill_n()
template<typename ForwardIterator,class Size,typename T>ForwardIterator fill_n(ForwardIterator first,Size n,const T&x)
{
    auto curr = first;
    for(;n>0;--n,++curr)
    {
        *curr = x;
    }
    return curr;
}


template <class IIter, class OIter>
OIter
__copy(IIter first, IIter last, OIter result, input_iterator_tag)
{
  for (; first != last; ++first, ++result)
  {
    *result = *first;
  }
  return result;
}

// __copy 的 random_access_iterator_tag 版本
template <class RAIter, class OIter>
OIter
__copy(RAIter first, RAIter last, OIter result, random_access_iterator_tag)
{
  for (auto n = last - first; n > 0; --n, ++first, ++result)
  {
    *result = *first;
  }
  return result;
}

// __copy_t : 指针所指对象具备 trivial assignment operator
template <class T>
T* __copy_t(const T* first, const T* last, T* result, __true_type)
{
  memmove(result, first, sizeof(T) * (last - first));
  return result + (last - first);
}

// __copy_t : 指针所指对象具备 non-trivial assignment operator
template <class T>
T* __copy_t(const T* first, const T* last, T* result, __false_type)
{
  return __copy(first, last, result, random_access_iterator_tag());
}

// 根据类型性质分派不同函数
// __copy_dispatch 的泛化版本
template <class IIter, class OIter>
struct __copy_dispatch
{
  OIter operator()(IIter first, IIter last, OIter result)
  {
    return __copy(first, last, result, iterator_category(first));
  }
};

// __copy_dispatch 的偏特化版本，两个参数都是 T* 形式
template <class T>
struct __copy_dispatch<T*, T*>
{
  T* operator()(T* first, T* last, T* result)
  {
    typedef typename __type_traits<T>::has_trivial_assignment_operator Trivial;
    return __copy_t(first, last, result, Trivial());
  }
};

// __copy_dispatch的偏特化版本，第一个参数是 const T* 形式，第二个参数是 T* 形式
template <class T>
struct __copy_dispatch<const T*, T*>
{
  T* operator()(const T* first, const T* last, T* result)
  {
    typedef typename __type_traits<T>::has_trivial_assignment_operator Trivial;
    return __copy_t(first, last, result, Trivial());
  }
};

template <class IIter, class OIter>
OIter copy(IIter first, IIter last, OIter result)
{
  return __copy_dispatch<IIter, OIter>()(first, last, result);
}

// char* 的特化版本
char* copy(const char* first, const char* last, char* result)
{
  memmove(result, first, last - first);
  return result + (last - first);
}

// wchar_t* 的特化版本
wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
  memmove(result, first, sizeof(wchar_t) * (last - first));
  return result + (last - first);
}
//copy_backward
//
template<class Iter1,class Iter2>
Iter2 copy_backward_aux(Iter1 first,Iter1 last,Iter2 rsult,tinystl::bidirectional_iterator_tag)
{
  while (first != last) {
     *--rsult = *--last;
    }
  return *rsult;
}
template<class Iter1,class Iter2>
Iter2 copy_backward_aux(Iter1 first, Iter1 last, Iter2 rsult, tinystl::random_access_iterator_tag)
{
  for(auto cur = last-1;cur == first;--cur)
    {
      *--rsult = *cur;
    }
  return rsult;
}

template<class Inter1,class Inter2>
Inter2 copy_backward(Inter1 first,Inter1 last,Inter2 result)
{
  return copy_backward_aux(first,last,result,iterator_category(first));
}
//max
/*版本1 */
template<class T>const T& max(const T& a,const T& b)
{
  return a>b?a:b;
}
/*版本2 用仿函数进行判断*/
template<class T,class compare>
const T& max(const T& a,const T&b,compare comp)
{
  return comp(a,b)?a:b;
}
//min
//版本1
template<class T>
const T& min(const T&a,const T&b)
{
  return a<b?a:b;
}
//版本2 加入仿函数
template<class T,class compare>
const T& min(const T&a,const T&b,compare comp)
{
  return comp(b,a)?b:a;
}
//equal
template<class InputIterator1 ,class InputInterator2>
inline bool equal(InputIterator1 first,InputIterator1 last,InputInterator2 result)
{
  for(;first != last ;++first ,++result )
    {
      if(*first != *result)
        {
          return false;
        }
    }
  return true;
}
//equal 指定仿函数
template<class InputInterator1 ,class InputINterator2,class BinaryPre>
inline bool equal(InputInterator1 first,InputInterator1 last,InputINterator2 result,BinaryPre binary)
{
  for(;first != last;++first,++result)
    {
      if(!binary(*first,*result))
        {
          return false;
        }
      return true;
    }
}
//iter_swap 将迭代器所指的对象对调
template<class InputInterator1,class InputInterator2>
inline void iter_swap(InputInterator1 a,InputInterator2 b)
{
  typename iterator_traits<InputInterator1>::value_type tmp = *a;
  *a = *b;
  *b = tmp;
}
//以字典排列方式 进行比较
template<class InputIterator1 ,class InputIterator2>
inline bool lexicographical_compare(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2
                                    ,InputIterator2 last2)
{
  for(;first1 != last1 && first2 != last2;++first1 , ++first2)
    {
      if(*first1 < *first2)
        return true;
      if(*first2 < *first1)
        return false;
    }
  return first1 == last1 && first2 != last2;
}
template<class InputIterator1,class InputIterator2,class Compare>
bool lexicographical_compare(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2
                             ,InputIterator2 last2,Compare comp)
{
  for(;first1 != last1 && first2 != last2;++first1 , ++first2)
    {
      if(comp(*first1,*first2))
        return true;
      if(comp(*first2,*first1))
        return false;
    }
  return first1 == last1 && first2 != last2;
}
template<typename T>void swap(T& a, T& b)
  {
      T tmp(std::move(a)); // move a to tmp
      a = std::move(b);    // move b to a
      b = std::move(tmp);  // move tmp to b
}
}
#endif // STL_ALGOBASE_H
