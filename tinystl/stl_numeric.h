#ifndef STL_NUMERIC_H
#define STL_NUMERIC_H
#include"stl_iterator.h"
#include"stl_function.h"
namespace tinystl {
  template<class InputInterator ,class T>
  T accumulate(InputInterator first,InputInterator last,T a)
  {
    for(;first != last;++first)
      {
        a = a + *first;
      }
    return a;
  }
  template<class InputInterator,class T,class BinaryOperation>
  T  accumulate(InputInterator first,InputInterator last,T a,BinaryOperation binary_op)
  {
    for(;first != last;++first)
      {
        a = binary_op(a,first);
      }
    return a;
  }
  //用来计算相邻元素的差额
  template<class InputIterator,class OutputIterator>
  OutputIterator adjacent_different(InputIterator first,InputIterator last,OutputIterator result)
  {
    if(first == last)return result;
    *result = *first;
    iterator_traits<InputIterator>::value_type  value= *first;
    while (++first != last) {
        T *tmp = *first;
        *++result = tmp - value;
        value = first;
      }
    return ++result;
  }
  //计算[first1,last1)和[first2,first2 + (last1 - first1) 的内积
  //版本1
  template<class InputIterator,class OutputIterator,class T>
  T inner_products(InputIterator first1,InputIterator last1,OutputIterator first2,T init)
  {
    for(;first1 != last1;++first1,++first2)
      {
        init = init + (*first1 * *first2);
      }
    return init;
  }
  //版本2 用仿函数替代 operator+ operator*
  template<class InputIterator,class OutputIterator,class T,class
           BinaryOperation1,class BinaryOperation2>
  T inner_products(InputIterator first1,InputIterator last1,OutputIterator result,T init,
                   BinaryOperation1 binary_1,BinaryOperation2 binary_2)
  {
    for(;first1 !=last1;++first1,++result)
      {
        init = binary_1(init,binary_2(*first1,*result));
      }
    return init;
  }

  template<class T,class Integer ,class Monoidoperation>
  inline T power(T x, Integer n,Monoidoperation op)
  {
    if(n==0)
      {
        return identity_element(op);
      }else {
        while ((n & 1) == 0)) {
           n >>=1;
           x = op(x,x);
          }
        T result = x;
        n >>=1;
        while (n != 0) {
           x = op(x,x);
           if((n & 1)!=0)
             result = op(result,x);
           n>>=1;
          }
        return result;
      }

  }


  template<class T,class Integer>
  inline T power(T x,Integer n)
  {
    return power
  }
}

#endif // STL_NUMERIC_H
