#ifndef STL_ALGO_H
#define STL_ALGO_H
#include"stl_iterator.h"
namespace tinystl {
  template<class InputInterator,class T>
  typename tinystl::iterator_traits<InputInterator>::difference_type
  count(InputInterator first,InputInterator last,const T& value)
  {
    typename iterator_traits<InputInterator>::difference_type n= 0;
    for(;first != last;++first)
      {
        if(*first == value)
          {
            ++n;
          }
      }
    return n;
  }
  template<class InputInterator,class prediate>
  typename tinystl::iterator_traits<InputInterator>::difference_type
  count_if(InputInterator first,InputInterator last,prediate pre)
  {
    typename iterator_traits<InputInterator>::difference_type n = 0;
    for(;first != last;++first)
      {
        if(pre(*first))
          {
            ++n;
          }
      }
    return n;
  }
  template<class InputInterator,class T>
  InputInterator find(InputInterator first,InputInterator last,const T& value)
  {
    while (first != last && *first != value) {
        ++first;
      }
    return first;
  }

}
#endif // STL_ALGO_H
