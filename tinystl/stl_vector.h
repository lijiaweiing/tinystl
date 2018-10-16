#ifndef STL_VECTOR_H
#define STL_VECTOR_H
#include"alloc.h"
#include<cstddef>
#include<utility>
#include"stl_uninitialized.h"
#include"stl_construct.h"
#include<initializer_list>
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

}
#endif // STL_VECTOR_H
