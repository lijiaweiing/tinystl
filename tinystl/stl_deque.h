#ifndef STL_DEQUE_H
#define STL_DEQUE_H
#include"alloc.h"
#include<stl_uninitialized.h>
#include<initializer_list>
#include<utility>
#include <stdexcept>
#include<iostream>
#include"stl_construct.h"
#include"stl_algobase.h"
namespace tinystl {
inline size_t __deque_buf_size(size_t n,size_t sz)
{
    return (n!=0)?n:(sz < 512 ? size_t(512/sz):size_t(1));
}
//deque 的迭代器
template<class T,class Ref,class Ptr,size_t BufSiz>
struct __deque_iterator
{
  typedef __deque_iterator<T,T&,T*,BufSiz> iterator;
  typedef __deque_iterator<T,const T&,const T*,BufSiz> const_iterator;
  typedef random_access_iterator_tag iterator_category;
  static size_t buffer_size(){return __deque_buf_size(BufSiz,sizeof(T));}
  typedef T value_type;
  typedef Ptr pointer;
  typedef Ref reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T** map_pointer;
  typedef __deque_iterator self;
  T* cur;
  T *first;
  T* last;
  map_pointer node;
  void set_node(map_pointer new_node)
  {
    node = new_node;
    first = *node;
    last = *node+difference_type(buffer_size());
  }
  reference operator*()const {return *cur;}
  pointer operator->()const {return &(operator*());}
  difference_type operator-(const self& t)const{
    return difference_type(buffer_size())*(node - t.node-1)+(cur-first)+(t.last-t.cur);
  }
  self& operator++()
  {
    ++cur;
    if(cur == last)
      {
        set_node(node+1);
        cur = first;
      }
    return *this;
  }
  self & operator ++(int)
  {
    self tmp = *this;
    ++*this;
    return tmp;
  }
  self& operator--()
  {
    if(cur == first)
      {
        set_node(node-1);
        cur = last;
      }
    --cur;
    return *this;
  }
  self& operator--(int)
  {
    self tmp = *this;
    --*this;
    return tmp;
  }
  self& operator+=(difference_type n)
  {
    difference_type offset = n + (cur -  first);
    if(offset >=0 && offset < difference_type(buffer_size()))
      cur+=n;
    else {
       difference_type node_offset = offset > 0?offset / (difference_type(buffer_size())) :
                                                -difference_type((-offset-1)/difference_type(buffer_size())) - 1;
       set_node(node+ node_offset);
       cur = first + (offset - node_offset* difference_type(buffer_size()));
      }
    return *this;
  }
  self operator +(difference_type n)const
  {
    self tmp = *this;
    return tmp+=n;
  }
  self & operator-=(difference_type n)
  {

    return *this+=-n;

  }
  self & operator-(difference_type n)const
  {
    self tmp = *this;
    return tmp-=n;
  }
  reference operator[](difference_type n)const
  {
    return *(*this + n);
  }
  bool operator==(const self& x)const
  {
    return cur == x.cur;
  }
  bool operator != (const self& x)const
  {
    return !(cur == x.cur);
  }
  bool operator<(const self& x)const
  {
    return (node == x.node)?(cur < x.cur):(node < x.node);
  }

};


//deque
//参数一代表数据类型，参数二代表空间配置器类型，缺省使用 tinystl 的 alloc，参数三代表缓冲区大小，缺省使用 512 bytes
template<class T,typename Alloc = alloc,size_t Bufsize = 0>
class deque
{
public:

  typedef T value_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
public:
  typedef __deque_iterator<T,T&,T*,Bufsize> iterator;
  typedef __deque_iterator<const T,const T&,const T*,Bufsize> const_itertor;
  typedef pointer* map_pointer;

protected:
  iterator start;
  iterator finish;
  map_pointer map;
  size_type map_size;
  typedef allocator<value_type,Alloc> data_allocator;
  typedef allocator<pointer,Alloc> map_allocator;
  //构建deque 的结构
  void create_map_and_node(size_type num_elements)
  {
    size_type num_nodes = num_elements/buffer_size() + 1;
    map_size = max(initial_map_size , num_nodes+2);
    map = map_allocator::allocate(map_size);
    map_pointer nstart = map+(map_size - num_nodes)/2;
    map_pointer nfinish = nstart + num_nodes - 1;
    map_pointer cur;
    for(cur = nstart;cur <= nfinish;++cur)
      {
        *cur = allocate_node();
      }
    start.set_node(nstart);
    finish.set_node(nfinish);
    start.cur = start.first;
    finish.cur = finish.first+num_elements%buffer_size();


  }

  void fill_initialize(size_type n,const value_type &value)
  {
    //把deque 结构安排好
    create_map_and_node(n);
    for(auto cur = start.node;cur < finish.node;++cur)
      {
        tinystl::uninitialized_fill(*cur,*cur+buffer_size(),value);
      }
    tinystl::uninitialized_fill(finish.first,finish.cur,value);
  }
  template<class Intertor> void range_initiate(Intertor first,Intertor last);
  void clear();
public:
   size_type buffer_size() { return __deque_buf_size(Bufsize, sizeof(T)); }
    iterator begin()const {return start;}
    iterator end() const {return finish;}
    const_itertor  cbegin() const noexcept{return start;}
    const_itertor  cend()const noexcept{return finish;}
    reference operator[](size_type n){return start[n];}
    reference front(){return *start;}
    reference back()
    {
      iterator tmp = finish;
      --tmp;
      return *tmp;
    }
    size_type size()const noexcept
    {
      return finish - start;;
    }

    bool empty()const noexcept{ return start== finish;}
    size_type max_size(){return size_type(-1);}
    deque():start(),finish(),map(0),map_size(0){fill_initialize(0,value_type());}
    deque(int n,const value_type& value):start(),finish(),map(0),map_size(0){fill_initialize(n,value);}
    deque(const deque& deq);
    deque& operator =(const deque& deq);
    template<typename Inter> deque(Inter first,Inter last);
    ~deque();
    reference at(size_type n);
    const_reference at(size_type n)const;
    void push_back(const value_type &);
    void push_front(const value_type &);
    size_type get_map_size(){return finish.node - start.node;}
    iterator erase(iterator pos);
    iterator erase(iterator first,iterator last);
    void pop_front();
    void pop_back();
    iterator insert(iterator position,const T& x);

private:
    iterator insert_aux(iterator position,const T& x);
    void pop_front_aux();
    void pop_back_aux();
    size_type initial_map_size = 8;
    void push_back_aux(const value_type &);
    void push_front_aux(const value_type &);
    void reserve_map_at_back(size_type nodes_to_add =1);
    void reserve_map_at_front(size_type nodes_to_add =1);
    void reallocate_map(size_type nodes_to_add,bool add_at_front);
    value_type* allocate_node()
    {
      T* s = data_allocator::allocate(buffer_size());
      return s;
    }


};




//复制构造函数
template<class T,class Alloc,size_t Bufsize>
inline deque<T,Alloc,Bufsize>::deque(const deque& deq)
{
  create_map_and_node(deq.size());
  tinystl::uninitialized_copy(deq.begin(),deq.end(),start);
}
//复制赋值运算符
template<class T,class Alloc,size_t Bufsize>
inline deque<T,Alloc,Bufsize>& deque<T,Alloc,Bufsize>::operator=(const deque& deq)
{
  if(&deq != this)
    {
     create_map_and_node(deq.size());
     tinystl::uninitialized_copy(deq.begin(),deq.end(),start);

    }
  return *this;
}
//清空deque

template<class T,class Alloc,size_t Bufsize>
inline void deque<T,Alloc,Bufsize>::
clear()
{
  //始终保留一个缓冲区 这是deque的初始状态
  for(map_pointer cur = start.node + 1;cur < finish.node;++cur)
  {
      destory(*cur,*cur + buffer_size());
      data_allocator::deallocate(*cur,buffer_size());
  }
  if(start.node != finish.node)
    {
      destory(start.cur,start.last);
      destory(finish.first,finish.cur);
      data_allocator::deallocate(finish.first,__deque_iterator<T,T&,T*,Bufsize>::buffer_size());

    }
  else {
      destory(finish.first,finish.cur);
    }
  finish = start;


}
template<class T,class Alloc,size_t Bufsize>
inline deque<T,Alloc,Bufsize>::
~deque()
{
  if(map)
    {
      clear();
      data_allocator::deallocate(start.cur,__deque_iterator<T,T&,T*,Bufsize>::buffer_size());
      map_allocator::deallocate(map,map_size);
      map = nullptr;
    }
}

template<class T,class Alloc,size_t Bufsize>
 inline  typename deque<T,Alloc,Bufsize>::reference deque<T,Alloc,Bufsize>::
at(size_type n)
{
  try
  {
    return (*this)[n];
  }
  catch(std::out_of_range &exc)
  {
    std::cerr << exc.what() ;

  }

}
//
template<class T,class Alloc,size_t Bufsize>
void deque<T,Alloc,Bufsize>::reallocate_map(size_type nodes_to_add, bool add_at_front)
{
  size_type old_num_nodes = finish.node - start.node +1;
  size_type new_num_nodes = old_num_nodes + nodes_to_add;
  map_pointer new_nstart;
  if(map_size > 2* new_num_nodes)
    {
      new_nstart = map+(map_size - new_num_nodes)/2 + (add_at_front ? nodes_to_add : 0);
      if(new_nstart < start.node)
        {
         copy(start.node , finish.node + 1,new_nstart);
        }else {
         copy_backward(start.node,finish.node + 1,new_nstart + old_num_nodes);
        }

    }else {
      size_type new_map_size = map_size + max(map_size,nodes_to_add) + 2;
      map_pointer new_map = map_allocator::allocate(new_map_size);
      new_nstart = new_map + (new_map_size - new_num_nodes)/2 + (add_at_front ? add_at_front :0);
      copy(start.node,finish.node , new_nstart);
      map_allocator::deallocate(map,map_size);
      map = new_map;
      map_size = new_map_size;


    }
  start.set_node(new_nstart);
  finish.set_node(new_nstart + old_num_nodes -1);
}
template<class T,class Alloc,size_t Bufsize>
void deque<T,Alloc,Bufsize>::reserve_map_at_back(size_type nodes_to_add )
{
  if(nodes_to_add + 1 >map_size - (finish.node - map))
    {
      reallocate_map(nodes_to_add,false);
    }
}
template<class T,class Alloc,size_t Bufsize>
void deque<T,Alloc,Bufsize>::reserve_map_at_front(size_type nodes_to_add)
{
  if(nodes_to_add > size_type(start.node-map))
    {
      reallocate_map(nodes_to_add,true);
    }
}

template<class T,class Alloc,size_t Bufsize>
 void deque<T,Alloc,Bufsize>::
 push_back(const value_type & t)
 {
   if(finish.cur != finish.last -1)
     {
       construct(finish.cur,t);
       finish.cur++;
     }
   else {
       push_back_aux(t);
     }
 }
template<class T,class Alloc,size_t Bufsize>
 void deque<T,Alloc,Bufsize>::
 push_back_aux(const value_type &t)
 {
    value_type copy_ex= t;
    reserve_map_at_back();
    *(finish.node + 1) = allocate_node();
    construct(finish.cur,copy_ex);
    finish.set_node(finish.node + 1);
    finish.cur = finish.first;
 }
template<class T,class Alloc,size_t Bufsize>
void deque<T,Alloc,Bufsize>::push_front_aux(const value_type& s)
{
  value_type t_copy = s;
  reserve_map_at_front();
  *(start.node -1) = allocate_node();

  try
    {
      start.set_node(start.node - 1);
      start.cur = start.last - 1;
      construct(start.cur, t_copy);
    }
    catch (...)
    {
      ++start;
      data_allocator::deallocate(*(start.node - 1));
    }
}
//pop_back() 从尾端将元素清除掉
template<class T,class Alloc,size_t Bufsize>
void deque<T,Alloc,Bufsize>::pop_back_aux()
{
  data_allocator::deallocate(finish.first);
  finish.set_node(finish.node - 1);
  finish.cur = finish.last - 1;
  destory(finish.cur);
}
template<class T,class Alloc,size_t Bufsize>
void deque<T,Alloc,Bufsize>::pop_back()
{
  if(start.cur != start.first)
    {
     --finish.cur;
      destory(finish.cur);
    }else {
      pop_front_aux();
    }
}
// pop_front() 从首段将元素清除掉
template<class T,class Alloc,size_t Bufsize>
void deque<T,Alloc,Bufsize>::pop_front_aux()
{
  destory(start.cur);
  data_allocator::deallocate(start.first);
  start.set_node(start.node+1);
  start.cur = start.first;
}
template<class T,class Alloc,size_t Bufsize>
void deque<T,Alloc,Bufsize>::pop_front()
{
  if(start.cur != start.last -1)
    {
      destory(start.cur);
      ++start.cur;
    }else {
     pop_front_aux();
    }
}

template<class T,class Alloc,size_t Bufsize>
void deque<T,Alloc,Bufsize>::push_front(const value_type& s)
{
  if(start.cur != start.first)
    {
      construct(start.cur-1 , s);
      --start.cur;
    }
  else {
     push_front_aux(s);
    }
}
template<class T,class Alloc,size_t Bufsize>
typename deque<T,Alloc,Bufsize>::iterator
deque<T,Alloc,Bufsize>::erase(iterator pos)
{
  iterator next = pos;
  ++next;
  difference_type index = pos - start;
  if(index < (size() >> 1))
    {
     copy_backward(start,pos,next);
     pop_front();
    }else {
     copy(next,finish,pos);
     pop_back();
    }
  return start + index;
}
template<class T,class Alloc,size_t Bufsize>
typename deque<T,Alloc,Bufsize>::iterator
deque<T,Alloc,Bufsize>::erase(iterator first,iterator last)
{
  if(first == start && finish == last)
    {
      clear();
      return finish;
    }else {
     difference_type n = last - first;//清除区间的长度
     difference_type element_before = first - start; //清除区域前方的长度
     if(element_before < (size() - n)/2) // 如果前方元素较少
       {
        copy_backward(start,first,last);
        iterator new_start = start + n;
        destory(start , new_start);//将多余元素进行析构
        for(auto i = start.node; i < new_start.node ; ++i)
          {
            data_allocator::deallocate(*i,buffer_size());
          }
        start = new_start;
       }else {
         copy(last,finish,first);
         iterator new_finish = finish - n;
         destory(new_finish, finish);
         for(map_pointer cur = new_finish.node +1;cur <= finish.node + 1;++cur)
           {
             data_allocator::deallocate(*cur,buffer_size());

           }
         finish = new_finish;

       }
     return start + element_before;
    }

}
template<class T,class Alloc,size_t  Bufsize>
typename deque<T,Alloc,Bufsize>::iterator
deque<T,Alloc,Bufsize>::insert_aux(iterator position,const T&x)
{
  difference_type n = position - start;
  value_type x_copy = x;
  if(n < difference_type(size()/2))
    {
     push_front(front());
     iterator front1 = start;
     ++front1;
     iterator front2 = front1;
     ++front2;
     position = start + n;
     iterator pos1 = position;
     ++pos1;
     copy(front2,pos1,front1);

    }else {
      push_back(back());
      iterator back1 = finish;
      --back1;
      iterator back2 = back1;
      --back2;
      position = start + n;
      copy_backward(position,back2,back1);


    }
  *position = x_copy;
  return position;
}
template<class T,class Alloc,size_t Bufsize>
typename deque<T,Alloc,Bufsize>::iterator
deque<T,Alloc,Bufsize>::insert(iterator position ,const T&x)
{
 if(position.cur == start.cur)
   {
     push_front(x);
     return start;
   }else if (position.cur == finish.cur) {
    push_back(x);
    iterator tmp = finish;
    --tmp;
    return tmp;
   }
 else {
    return insert_aux(position ,x);
   }
}
}
#endif
