#ifndef STL_SET_H
#define STL_SET_H
#include"alloc.h"
#include<cstddef>
#include<utility>
#include"stl_uninitialized.h"
#include"stl_construct.h"
#include<initializer_list>
#include<stl_function.h>
#include<stl_algo.h>
#include<stl_rb_tree.h>
#include<stl_function.h>
namespace tinystl {
template<class Key , class Compare = less<Key>,class Alloc =  alloc>
    class set{
        typedef Key key_type;
        typedef Key value_type;
        typedef  Compare key_compare;
        typedef Compare value_compare;
    private:
        typedef rb_tree<key_type , identity<value_type> , key_compare , Alloc>rep_type;
        rep_type t;
    public:
        typedef typename rep_type::const_pointer pointer;
        typedef typename rep_type::const_pointer const_pointer;
        typedef  typename rep_type::const_reference reference;
        typedef typename rep_type::const_iterator iterator;
        set():t(Compare()){}
        explicit set(const Compare& comp):t(comp){}
        template<class InputIterator>
        set(InputIterator first,InputIterator last ):t(Compare()){t.insert_unique(first , last);}
        template<class InputIterator>
        set(InputIterator first , InputIterator last , const Compare& comp):t(comp){t.insert_unique(first, last );}
        set(const set<Key , Compare , Alloc>&x):t(x.t){}




    };
}
#endif // STL_SET_H
