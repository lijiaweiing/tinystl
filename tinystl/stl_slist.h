#ifndef STL_SLIST_H
#define STL_SLIST_H
#include<cstddef>
#include"alloc.h"
#include"stl_iterator.h"
#include"stl_construct.h"
namespace tinystl {
  struct _slist_node_base
  {
    _slist_node_base *next;
  };
  template<class T>
  struct _slist_node:public _slist_node_base
  {
    T data;
  };
  //全局函数已知某一个点,插入一个新节点
  inline _slist_node_base * _slist_make_link(_slist_node_base *prev_node,_slist_node_base * new_node)
  {
    new_node->next = prev_node->next;
    prev_node->next = new_node;
    return new_node;
  }
  //计算元素个数
  inline size_t _slist_size(_slist_node_base * node)
  {
    size_t result = 0;
    for(;node != 0;node = node->next)
      {
        ++result;
      }
    return result;
  }
  //单向链表 只有operator++ 没有operator--
  struct _slist_iterator_base
  {
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef forward_iterator_tag iterator_category;
    _slist_node_base *node;
    _slist_iterator_base(_slist_node_base *x):node(x){}
    void incr(){node = node->next;}
    bool operator==(const _slist_iterator_base & x)
    {
      return node == x.node;
    }
    bool operator != (const _slist_iterator_base &x)
    {
      return node != x.node;
    }

  };
  template<class T,class Ref,class Ptr>
  struct _slist_iterator:public _slist_iterator_base
  {
    typedef _slist_iterator<T,T&,T*> iterator;
    typedef _slist_iterator<T,const T&,const T*> const_iterator;
    typedef _slist_iterator<T,Ref,Ptr> self;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef _slist_node<T>  list_node;
    _slist_iterator(list_node *x):_slist_iterator_base(x){}
    _slist_iterator():_slist_iterator_base(0){}
    _slist_iterator(const _slist_iterator & x):_slist_iterator_base(x.node){}
    reference operator*()const{return ((list_node*)node)->data;}
    pointer operator ->()const{return &(operator *());}
    self& operator++()
    {
      incr();
      return *this;
    }
    self operator++(int)
    {
      self tmp = *this;
      incr();
      return tmp;
    }

  };
  template<class T,class Alloc = tinystl::alloc>
  class slist
  {
  public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const pointer const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef _slist_iterator<T,T&,T*> iterator;
    typedef _slist_iterator<T,const T&,const T*>const_iterator;
  private:
    typedef _slist_node<T> list_node;
    typedef _slist_node_base list_node_base;
    typedef _slist_iterator_base iterator_base;
    typedef allocator<list_node,Alloc>  list_allocator;
    static list_node* create_node(const value_type &x)
    {
      list_node * node = list_allocator::allocate();
      construct(&node->data,x);
      node->next = 0;
         return node;
    }
    static void  destory_node(list_node * x)
    {
      destory(&x->data);
      list_allocator::deallocate(x);
    }
    list_node_base head;
    void clear()
    {
      auto i = head.next;
      for(;i != 0;i= i->next)
        {
          destory_node((list_node*)i);
        }
    }

  public:
    slist(){head.next = 0;}
    ~slist(){clear();}
    iterator begin()const{return iterator((list_node*)head.next);}
    iterator end()const{return iterator(0);}
    size_type size(){return _slist_size(head.next);}
    bool empty()const
    {
      return head.next == 0;
    }
    void swap(slist &l)
    {
      std::swap(l.head.next,head.next);
    }
    reference front()
    {
      return ((list_node*)head.next)->data;
    }
    void push_front(const value_type &x)
    {
      _slist_make_link(&head,create_node(x));

    }
    void pop_front()
    {
      list_node* node = (list_node*) head.next;
      head.next = node->next;
      destory_node(node);
    }

  void insert(iterator s,const T& x)
  {
    _slist_make_link(s.node,create_node(x));
  }

  };

}
#endif // STL_SLIST_H
