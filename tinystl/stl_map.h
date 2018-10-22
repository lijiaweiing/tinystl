#ifndef STL_MAP_H
#define STL_MAP_H
#include<stl_pair.h>
#include<stl_function.h>
#include<alloc.h>
#include<stl_rb_tree.h>
namespace tinystl {
 template<class Key,class T,class compare = tinystl::less<Key>,class ALLOC = alloc>
  class map
  {
    typedef Key key_type;
    typedef T data_type;
    typedef T mapped_type;
    typedef pair<const Key,T> value_type;
  };
}
#endif // STL_MAP_H
