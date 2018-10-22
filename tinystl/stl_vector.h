#ifndef STL_VECTOR_H
#define STL_VECTOR_H
#include"alloc.h"
#include<cstddef>
#include<utility>
#include"stl_uninitialized.h"
#include"stl_construct.h"
#include<initializer_list>
//一棵红黑树满足以下定理
//1 每个结点不是红色就是黑色
//2 根节点是黑色
//3 如果一个结点是红色，那么它的子节点是黑色的
//4 黑高都是相同的
/*
namespace tinystl {

template<class T,class Alloc = alloc>class vector
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type* iterator;
    typedef value_type& reference;
    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;
protected:

    typedef allocator<value_type,Alloc> data_allocator;
    iterator first;
    iterator finish;
    iterator end_of_storage;
public:

    void pop_back()
    {
        --finish;
        destory(finish);
    }
    //右值引用
    vector(vector&& vec) noexcept:first(vec.first),finish(vec.finish),end_of_storage(vec.end_of_storage)
    {
        vec.finish = vec.finish = vec.end_of_storage = NULL;
    }
    vector& operator =(vector && vec) noexcept
    {
        if(&vec != this)
        {
            free();
            first = vec.first;
            finish = vec.finish;
            end_of_storage = vec.end_of_storage;
            vec.first = vec.finish = vec.end_of_storage = NULL;

        }
        return *this;
    }

    ~vector(){free();}
    vector& operator =(const vector& vec)
    {
        auto newdata = copy_and_alloc(vec.begin(),vec.end());
        free();
        first = newdata.first;
        finish = end_of_storage = newdata.second;
        return *this;
    }

    vector(const vector& vec)
    {
        auto newdata = copy_and_alloc(vec.begin(),vec.end());
        first = newdata.first;
        finish = end_of_storage = newdata.second;
    }

    vector(std::initializer_list<T> a)
    {
        auto newdata = copy_and_alloc(a.begin(),a.end());
        first = newdata.first;
        finish = end_of_storage = newdata.second;
    }

   template<typename U>   vector(T* a,T* b)
    {
      auto newdata=   copy_and_alloc(a,b);
      first = newdata.first;
      finish = end_of_storage = newdata.second;
    }
    void swap(vector &vec)
    {
      tinystl::swap(first,vec.first);
       tinystl::swap(finish,vec.finish);
       tinystl::swap(end_of_storage,vec.end_of_storage);

    }
    void push_back(T &&x)
    {
        chk_n_alloc();
        construct(finish++,std::move(x));
    }

    iterator begin()const{return first;}
    iterator end()const {return finish;}
    size_type size()const{return size_type(finish - first);}
    bool empty()const {return begin() == end();}
    reference operator[](size_type n){return *(begin() +n);}
    vector():first(0),finish(0),end_of_storage(0){}
    size_type capacity()const{return size_type(end_of_storage - first);}
   explicit vector(int n,const T& x){fill_initiate(n,x);}
    void push_back(const T& x)
    {

        chk_n_alloc();
        construct(finish++,x);
    }

private:
    void reallocate()
    {
        auto newcaptitiy = size()*2;
        iterator newdata = data_allocator::allocate(newcaptitiy);
        iterator dest = newdata;
        iterator elem= first;
        for(size_t i = 0;i != size();++i)
        {
            construct(dest++,std::move(*elem++));
        }
        free();
        first = newdata;
        finish = dest;
        end_of_storage = newdata + newcaptitiy;
    }
    void free()
    {
        if(first)
        {
            for(auto p = finish ; p!=first; )
            {
                destory(--p);
            }
            data_allocator::deallocate(first,size_type(end_of_storage - first));
        }
    }

    void chk_n_alloc()
    {
        if(size() == capacity())
        {
            reallocate();
        }
    }
    void fill_initiate(size_type n,const T& x)
    {
        first = allocate_and_fill(n,x);
         finish = first + n;
         end_of_storage = finish;
    }

    iterator allocate_and_fill(size_type n,const T& x)
    {
        iterator result = data_allocator::allocate(n);
        uninitialized_fill_n(result,n,x);
        return result;
    }
    template<typename U> std::pair<iterator,iterator> copy_and_alloc(U a,U b)
    {
        auto i = data_allocator::allocate(b - a);
        return{i , uninitialzed_copy(a,b,i)};
    }


};

}*/
namespace tinystl {
  typedef bool __rb_tree_color_type;
  const __rb_tree_color_type __rb_tree_red = false;  //设置红节点为false
  const __rb_tree_color_type __rb_tree_black = true;  //设置黑节点为true
  //红黑树的初始节点
  struct __rb_tree_node_base{
    typedef __rb_tree_node_base* base_ptr;
    typedef  __rb_tree_color_type color_type;
    color_type color;
    base_ptr parent;
    base_ptr left;
    base_ptr right;

    static base_ptr minimum(base_ptr x){
      while (x->left != nullptr) {
          x = x->left;
        }
      return  x;
    }
    static base_ptr maximum(base_ptr x){
      while (x->right != nullptr) {
          x = x->right;
        }
      return  x;
    }
  };
  template<typename Value> struct __rb_tree_node:public __rb_tree_node_base{
    typedef  __rb_tree_node<Value>* link_type;
     Value vlaue_field;
  };
  //对map表构建迭代器
  struct __rb_tree_iterator_base{
    typedef  __rb_tree_node_base::base_ptr base_ptr;
    typedef bidirectional_iterator_tag iterator_category;
    base_ptr node;
    //结点自增
    void  increment(){
      //
      if (node->right != nullptr){
          node = node->right;
          while (node->left != nullptr) {
              node = node->right;
            }
        }else {
            auto y = node->parent;
            while (node == y->right) {
                node = y;
                y = y->parent;
              }
            if (node->right != y){
                node = y;
              }
        }
    }
    void decrement(){
      if (node->color == __rb_tree_red && node->parent->parent){
          node = node->right;
        }else if(node ->left != nullptr){
          base_ptr y = node->left;

        }
    }

  };

}
#endif // STL_VECTOR_H
