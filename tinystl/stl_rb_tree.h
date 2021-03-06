﻿#ifndef STL_RB_TREE_H
#define STL_RB_TREE_H
#include"stl_pair.h"
#include<functional>
#include"stl_iterator.h"
#include"alloc.h"
#include"stl_construct.h"
#include"stl_algobase.h"
#include<iostream>
namespace tinystl {
  typedef bool __rb_tree_color_type;
  const __rb_tree_color_type _rb_tree_red = false;
  const __rb_tree_color_type _rb_tree_black = true;
  //
  struct _rb_tree_node_base
  {
    typedef __rb_tree_color_type color_type;//红色为0
    typedef _rb_tree_node_base * base_ptr;//黑色为1
    color_type color;
    base_ptr parent;
    base_ptr left;
    base_ptr right;
    static base_ptr minimum(base_ptr a)
    {
      while (a->left != nullptr) {
         a = a->left;
        }
      return a;
    }
    static base_ptr maximum(base_ptr a)
    {

      while (a->right != nullptr) {
          a = a->right;
        }
      return a;
    }

  };
  template<class T>
  struct _rb_tree_node:public _rb_tree_node_base
  {
    typedef _rb_tree_node<T>* link_type;
    T value_file;
  };
  struct _rb_tree_iterator_base
  {
    typedef bidirectional_iterator_tag iterator_category;
    typedef _rb_tree_node_base::base_ptr base_ptr;
    typedef ptrdiff_t difference_type;
    base_ptr node;
    //实现operator++
    void increment()
    {
      if(node->right != nullptr) //case1 如果有右节点
        {
          node = node->right;
          while (node->right != nullptr) {
              node = node->right;

            }
        }else { //case2 如果没有右节点
         base_ptr y = node->parent;
         while (node == y->right) {
             node = y;
             y = y->parent;
           }
         if(node->right != y)
           {
             node = y;
           }
        }
    }
    //实现operator--
    void decrement()
    {

      if(node->color == _rb_tree_red && node->parent->parent == node)//
        {
          node = node->right;
        }
      else if(node->left != nullptr)
        {
          base_ptr y = node->left;
          while (y->right != nullptr) {
              y = y->right;

            }
          node = y;
        }
      else {
         base_ptr y = node->parent;
         while (node == y->left) {
             node =y;
             y = y->parent;

           }
         node =y;
        }

    }
  };
  template<class T,class Ref,class Ptr>
  struct _rb_tree_iterator:public _rb_tree_iterator_base
  {
    typedef T value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef _rb_tree_iterator<T,T&,T*> iterator;
    typedef _rb_tree_iterator<T,const T&,const T*> const_iterator;
    typedef _rb_tree_iterator<T,Ref,Ptr> self;
    typedef _rb_tree_node<T>* link_type;

    _rb_tree_iterator(){}
    _rb_tree_iterator(link_type x){node = x;}
    _rb_tree_iterator(const iterator &it){node = it.node;}

    //operator*
    reference operator*()const{return link_type(node)->value_file;}
    //operator->
    pointer operator->()const{return &(operator *());}
    self& operator++()
    {
      increment();
      return *this;
    }
    self operator++(int)
    {
      self tmp = *this;
      increment();
      return tmp;
    }
    self& operator--()
    {
      decrement();
      return *this;
    }
    self operator --(int)
    {
      self tmp = *this;
      decrement();
      return tmp;
    }
    bool operator==(const _rb_tree_iterator &x)
    {
      return node == x.node;
    }
    bool operator!=(const _rb_tree_iterator &x)
    {
      return node != x.node;
    }
  };
  //定义3个全局函数
  //左旋
  inline void _rb_rotate_left(_rb_tree_node_base * x,_rb_tree_node_base*& root)
  {
    _rb_tree_node_base* y = x->right;
    x->right = y->left;
    if(y->left != nullptr)
      {
        y->left->parent = x;
      }
    y->parent = x->parent;
    if(x == root)
      root = y;
    else if (x == x->parent->left) {
        x->parent->left = y;
      }
    else {
        x->parent->right = y;
      }
    y->left = x;
    x->parent = y;
  }
  //右旋
  inline void _rb_rotate_right(_rb_tree_node_base * x,_rb_tree_node_base *& root)
   {
    _rb_tree_node_base * y = x->left;
    x->left = y->right;
    if(y->right != nullptr)
      {
       y->right->parent = x;
      }
    y->parent = x->parent;
    if(x == root)
       root = y;
    else if (x == x->parent->right) {
         x->parent->right = y;
       }
    else {
       x->parent->left = y;
      }
    y->right = x;
    x->parent = y;
  }
  //插入数据后进行调整
  inline void _rb_tree_insert_fixup(_rb_tree_node_base* x,_rb_tree_node_base*& root)
  {
    x->color = _rb_tree_red;

    while (x->parent->color==_rb_tree_red&& x != root) {
        if(x->parent == x->parent->parent->left)
          {
            //case 1 如果父节点为红色叔节点也为红色
            //那么 将父节点和叔节点 换为黑色 ,爷节点换为红色
           _rb_tree_node_base *y = x->parent->parent->right;
           if(y->color== _rb_tree_red&& y)
             {
               x->parent->color = _rb_tree_black;
              y->color = _rb_tree_black;
               x->parent->parent->color = _rb_tree_red;
               x = x->parent->parent;
             }
           //case 2 叔节点为黑色 且x为一个右孩子
           //进行一次左旋
           else   {
          if(x == x->parent->right)
            {   x = x->parent;
               _rb_rotate_left(x,root);
             }
          //case 3 叔节点为黑色 且x为左孩子
          //改变父节点,祖父节点颜色 进行右旋
          x->parent->color = _rb_tree_black;
          x->parent->parent->color = _rb_tree_red;
          _rb_rotate_right(x->parent->parent , root);
             }
          }else {//case 4,5,6
              _rb_tree_node_base *y = x->parent->parent->left;
              if(y->color == _rb_tree_red && y)
                {
                  y->color = _rb_tree_black;
                  x->parent->color = _rb_tree_black;
                  x->parent->parent->color = _rb_tree_red;
                  x = x->parent->parent;
                }
              else {
                 if(x == x->parent->left)
                   {
                     x = x->parent;
                     _rb_rotate_right(x,root);
                   }
                 x->parent->color = _rb_tree_black;
                 x->parent->parent->color = _rb_tree_red;
                 _rb_rotate_left(x->parent->parent,root);
                }
          }
      }

    root->color = _rb_tree_red;
  }
  inline void rb_Transplation(_rb_tree_node_base * u ,_rb_tree_node_base *v ,_rb_tree_node_base *& root){
    if(u->parent == nullptr){
        root = u;
      }else if(u == v->parent->left){
        u->parent->left = v;
      }else {
        u->parent->right = v;
      }
    v->parent = u->parent;
  }
//删除后数据调整
inline void rb_delete_fixup(_rb_tree_node_base *x,_rb_tree_node_base *& root)
{
  while(x != root && x->color != _rb_tree_red){
      if (x->parent->left == x){
          auto w = x->parent->right;
          //情况1 x 的兄弟结点是红色
          if (w->color == _rb_tree_red ){
              w->color = _rb_tree_black;
              x->parent->color = _rb_tree_red;
              _rb_rotate_left(x->parent , root);
              w = x->parent->right;
            }
          //  情况2 x 的兄弟结点w是黑色 w的两个子结点是黑色
          if (w->left->color == _rb_tree_black && w->right->color == _rb_tree_black ){
              w->color = _rb_tree_red;
              x = x->parent;
            }else {
               // 情况3 x的兄弟结点w是黑色 w的左孩子是红色，w的右孩子是黑色
              if (w->right->color == _rb_tree_black){
                  w->left->color = _rb_tree_black;
                  w->color = _rb_tree_red;
                  _rb_rotate_right(w, root);
              }
              //情况4 x 的兄弟结点w是黑色，且w的右孩子是红色
              w->color = _rb_tree_red;
              w->right->color = _rb_tree_black;
              x->parent->color = _rb_tree_black;
              _rb_rotate_left(x->parent , root);
              x = root;
              break;
            }
    }else {
         auto w = x->parent->left;
         //情况1 x 的兄弟结点w是红色
         if(w->color == _rb_tree_red){
              w->color = _rb_tree_black;
              x->parent->color = _rb_tree_red;
              _rb_rotate_right(x->parent , root);
           }
         //情况2 x的兄弟结点w是黑色  w的两个子结点是黑色
         if (w->right->color == _rb_tree_black && w->left->color == _rb_tree_black){
             w->color = _rb_tree_red;
             x = x->parent;
           }else {
              //情况3 x的兄弟结点w是黑色的
             if(w->left->color == _rb_tree_black){
                 w->right->color = _rb_tree_red;
                 w->color = _rb_tree_red;
                 _rb_rotate_left(w, root);
               }
             w->color = _rb_tree_red;
             w->left->color = _rb_tree_black;
             _rb_rotate_right(x->parent , root);
             x = root;
             break;
           }


    }

    }
  x->color = _rb_tree_red;
}

  template<class Key,class Value,class KeyOfvale,class Compare,class ALloc= alloc>
  class rb_tree
  {
  protected:
    typedef void* void_pointer;
    typedef _rb_tree_node_base *base_ptr;
    typedef _rb_tree_node<Value> rb_tree_node;
    typedef allocator<rb_tree_node,ALloc> rb_tree_node_allocator;
    typedef __rb_tree_color_type color_type;
  public:
    typedef Key key_value;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef rb_tree_node* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
 protected:
    link_type get_node(){return rb_tree_node_allocator::allocate();}
    void put_node(link_type p){rb_tree_node_allocator::deallocate(p);}
    link_type create_node(const value_type& x)
    {
      auto i = get_node();
      try
      {
        construct(&i->value_file,x);
      }catch(...)
      {
        put_node(i);
      }
      return i;
    }
    link_type clone_node(link_type x)
    {
      link_type tmp = create_node(x->value_file);
      tmp->color = x->color;
      tmp->left = 0;
      tmp->right = 0;
      return tmp;
    }
    void destory_node(link_type x)
    {
      destory(&x->value_file);
      put_node(x);
    }
  protected:
    size_type node_count;
    link_type header;
    Compare key_compare;

    link_type& root()const{return const_cast<link_type&>(header->parent);}
    link_type& leftmost()const{ return const_cast<link_type&>(header->left);}
    link_type& rightmost()const{return const_cast<link_type&>(header->right);}
    //获取节点成员
    static link_type& left(link_type x){return const_cast<link_type&>(x->left);}
    static link_type&right(link_type x){return const_cast<link_type&>(x->right);}
    static link_type& parent(link_type x){return const_cast<link_type&>(x->parent);}
    static reference value(link_type x){return  (static_cast<link_type&>(x))->value_file;}
    static const Key& key(link_type x){return KeyOfvale()(value(x));}
    static color_type& color(link_type x){return  static_cast<color_type&>(x->color) ;}

    static link_type& left(base_ptr x){return  static_cast<link_type&>(x->left) ;}
    static link_type&right(base_ptr x){return static_cast<link_type&>(x->right) ;}
    static link_type& parent(base_ptr x){return  static_cast<link_type&>(x->parent) ;}
    static reference value(base_ptr x){return (static_cast<link_type>(x))->value_file;}
    static const Key& key(base_ptr x){return KeyOfvale() (value(link_type( x )) );}
    static color_type& color(base_ptr x){return static_cast<color_type&>(x->color);}

    static link_type minimum(link_type x)
    {
      return static_cast<link_type>(_rb_tree_node_base::minimum(x)) ;
    }
    static link_type maximum(link_type x)
    {
      return static_cast<link_type>(_rb_tree_node_base::maximum(x)) ;
    }
  public:
    typedef _rb_tree_iterator<Value,reference,pointer> iterator;
    typedef  _rb_tree_iterator<Value , const_reference , const_pointer> const_iterator;
  private:

    void clear();
    link_type __copy(link_type x,link_type y);
    void rb_transpalnt(link_type u,link_type v){
        if(parent(u) == nullptr){
            root() = v;
        }else if (u == (u->parent)->left) {
            u->parent->left =  v;
        }else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }
    //有问题缺少更改leftmost rightmost 的问题
    link_type _erase(link_type z){
      link_type y = z;
       __rb_tree_color_type y_origin_color= y->color;
      link_type x = nullptr;
      if(left(z) == nullptr){
            x = right(z);
            rb_transpalnt(z , z->right);
        }else if (right(z) == nullptr) {
            x = left(z);
            rb_transpalnt(z, z->left);
        }else {
          y = minimum(right(z));
         y_origin_color = y->color;
          x = right(y);
          if (parent(y) == z){
              parent(x) = y;
          }else {
              right(y) = right(z);

          }

          rb_transpalnt(z, y);
          y->left = z->left;
          y->left->parent = y;
          y->color = z->color;

        }
      if (y == x){
          if(leftmost() == z){
              if(right(x) == nullptr){
                leftmost() = parent(x);
              }else {
                  leftmost() = minimum(x);
              }
          }
          if (rightmost() == z){
              if(left(x) == nullptr){
                  rightmost() = parent(x);
              }else {
                   rightmost() = maximum(x);
              }
          }
      }
      if (y_origin_color == _rb_tree_black){
          rb_delete_fixup(x, root());
      }
        return x;
    }
    void init()//对header 初始化
    {
      header = get_node();
      color(header) = _rb_tree_red;
      root() = 0;
      leftmost() = header;
      rightmost() = header;
    }
    void _rb_transplant(link_type x,link_type y)
    {
      if(x->parent == header)
        {
          root() = y;
        }
      else if (x == x->parent->left) {
          x->parent->right = y;
        }else {
          x->parent->right = y;
        }
       y->parent = x->parent;
    }
    
  public:
    friend bool operator==(const rb_tree&,const rb_tree&);
    friend bool operator !=(const rb_tree &,const rb_tree&);
    friend void swap(const rb_tree &,const rb_tree &);
    rb_tree(const Compare& comp = Compare()):node_count(0),key_compare(comp){init();}
    rb_tree(const rb_tree & );
    rb_tree &operator =(const rb_tree & x);
    iterator __insert(base_ptr x__,base_ptr y__,const value_type &v)
    {

      link_type x = static_cast<link_type>(x__) ;
      link_type y = static_cast<link_type>(y__) ;
      link_type z;
      if(y == header || x!= 0||key_compare(KeyOfvale()(v),key(y)))
        {
          z = create_node(v);
          left(y) = z;
          if(y == header)
            {
              root() = z;
              rightmost() = z;
            }
          else if (y == leftmost()) {

              leftmost() = z;
            }


        }else {
          z = create_node(v);
          right(y) = z;
          if(y == rightmost())
              rightmost() = z;
        }
      parent(z) = y;
      left(z) = 0;
      right(z) = 0;
      _rb_tree_insert_fixup(z,header->parent);

      ++node_count;
      return iterator(z);

    }
  public:
    Compare key_comp(){return key_compare;}
    iterator begin()const{return leftmost();}
    iterator end()const{return header;}
    bool empty()const{return  node_count == 0;}
    size_type size()const{return node_count;}
    size_type max_size()const{return size_type(-1);}
  public:

    iterator  insert_equal(const value_type& v)
    {
      link_type y = header;
      link_type x = root();
      while (x != 0) {
          y = x;
          x = key_compare(KeyOfvale()(v),key(x))?left(x):right(x);

        }
          return __insert(x,y,v);

    }
    pair<iterator , bool> insert_unique(const Value &);
    iterator find(const Value & );
    iterator upper_bound(const key_value & k){
        link_type y = header;
        link_type x = root();
        while (x != nullptr) {
             y = x;
             x = key_compare(KeyOfvale()(k) , key(x))?left(x):right(x);
        }
        return  iterator(x);
    }
    iterator lower_bound(const key_value & k){
          link_type y = header;
          link_type x = root();
          while (x != nullptr) {
               y = x;
               x = key_compare(KeyOfvale()(k) , key(x))?right(x) : left(x);
          }
          return  iterator(x);
    }

    iterator erase(const iterator & x){
       auto k =  _erase(x);
        destory_node(x);
        --node_count;
        return  iterator(k);
    }
    pair<iterator , iterator> equal_range(const Key& __k);
    pair<const_iterator , const_iterator> equal_range(const Key & __k)const{
        return pair<const_iterator, const_iterator>(lower_bound(__k) , upper_bound(__k));
    }
    size_type erase(const Key &);
    iterator erase(const iterator first,const iterator last);
    size_type count(const Key &k );

  };
  template<class Key,class Value,class KeyOfvale,class Compare,class ALloc>
   typename rb_tree<Key, Value,KeyOfvale,Compare, ALloc>::size_type
  rb_tree<Key,Value,KeyOfvale,Compare,ALloc>::erase(const Key & k){
        pair<iterator ,iterator>  p = equal_range(k);
        size_type n = 0;
        n = distance(p.first , p.second);
        erase(p.first , p.second);
        return  n;
  }
  template<class Key,class Value,class KeyOfvale,class Compare,class ALloc>
  typename  rb_tree< Key, Value,KeyOfvale,Compare, ALloc>::iterator
  rb_tree<Key,Value,KeyOfvale,Compare,ALloc>::erase(const iterator first , const iterator last){
      if(first == begin() && last == end() ){
          clear();
      }else {
          while (first != last) {
              erase(first++);
          }
      }
  }


  template<class Key,class Value,class KeyOfvale,class Compare,class ALloc>
  tinystl::pair<typename rb_tree< Key, Value,KeyOfvale,Compare, ALloc>::iterator, typename rb_tree< Key, Value,KeyOfvale,Compare, ALloc>::iterator>
  rb_tree<Key,Value,KeyOfvale,Compare,ALloc>::equal_range(const Key & k){
      return pair<iterator , iterator>(lower_bound(k) , upper_bound(k));
  }
  template<class Key,class Value,class KeyOfvale,class Compare,class ALloc>
  typename rb_tree<Key, Value,KeyOfvale,Compare, ALloc>::size_type
  rb_tree<Key,Value,KeyOfvale,Compare,ALloc>::count(const Key & k){
      pair<const_iterator ,const_iterator > k__ = equal_range(k);
      size_type __n = 0;
      __n = distance(k__.first, k__.last);
      return  __n;
  }
  template<class Key,class Value,class KeyOfvale,class Compare,class ALloc>
  tinystl::pair<typename rb_tree< Key, Value,KeyOfvale,Compare, ALloc>::iterator,bool>
  rb_tree<Key,Value,KeyOfvale,Compare,ALloc>::insert_unique(const Value & v)
  {
    link_type y = header;
    link_type x =root();
    bool comp = true;
    while (x != nullptr) {
         y = x;
         comp = key_compare(KeyOfvale()(v),key(x));
         x = comp ? left(x):right(x);
      }
    iterator j = iterator(y);
    if(comp)
      {
        if( j == begin())
            return pair<iterator , bool>(__insert(x,y,v),true);
        else {
            --j;
          }

      }
    if(key_compare(key(j.node),KeyOfvale()(v)))
      {
         return pair<iterator , bool>(__insert(x,y,v),true);
      }
     return pair<iterator , bool>(j,false);
  }
template<class Key,class Value,class KeyOfvale,class Compare,class ALloc>
typename rb_tree<Key,Value,KeyOfvale,Compare,ALloc>::iterator
rb_tree<Key,Value,KeyOfvale,Compare,ALloc>::find(const Value &v)
{
  link_type y = header;
  link_type x = root();
  while(x != 0)
    {
      if(! key_compare(key(x),v))
        y = x,x = left(x);
      else {
          x = right(x);
        }
    }
  iterator j = iterator(y);
  return (j==end()||key_compare(v,key(j.node)))?end():j;
}
template<class Key,class Value,class KeyOfvale,class Compare,class ALloc>
bool operator ==(const rb_tree<Key,Value,KeyOfvale,Compare,ALloc>& a,const rb_tree<Key,Value,KeyOfvale,Compare,ALloc> &b)
{
  return a.size() == b.size() && tinystl::equal(a.begin(),a.end(),b.begin());
}
template<class Key,class Value,class KeyOfvale,class Compare,class ALloc>
bool operator !=(const rb_tree<Key,Value,KeyOfvale,Compare,ALloc>& a,const rb_tree<Key,Value,KeyOfvale,Compare,ALloc> &b)
{
  return !(a == b);
}

}


#endif // STL_RB_TREE_H
