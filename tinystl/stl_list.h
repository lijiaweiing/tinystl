#ifndef STL_LIST_H
#define STL_LIST_H
#include<utility>
#include"stl_iterator.h"
#include"__type_traits.h"
#include"alloc.h"
#include<stl_uninitialized.h>
#include<initializer_list>
#include"stl_construct.h"
namespace tinystl {
//构建list 的节点
template<class T>
struct __list_node
{
    typedef void * void_pointer;
    void_pointer prev;
    void_pointer next;
    T data;

};
//构建list 的迭代器
template<class T,class ref,class ptr>
struct __list_iterator
{
    typedef __list_iterator<T,T&,T*> iterator;
    typedef __list_iterator<T,ref,ptr> self;
    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptr pointer;
    typedef ref reference;
    typedef __list_node<T> * link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    link_type node;
    __list_iterator(link_type x):node(x){}
    __list_iterator(){}
    __list_iterator(const __list_iterator &l):node(l.node){}

    bool operator==(const __list_iterator &x){return node == x.node;}
    bool operator != (const __list_iterator &x){return node != x.node;}

    reference operator*()const{return (*node).data;}
    pointer operator->()const{return &((*node).data);}

    self &operator++()
    {
        node = (link_type)(node->next);
        return *this;
    }
    self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }
    self &operator--()
    {
        node = (link_type)(node->prev);
        return *this;
    }
    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }

};
//构建list
template<typename T,class ALLOC = alloc>
class list
{
public:
    typedef __list_node<T> list_node;
    typedef allocator<list_node,ALLOC> list_node_allocator;


public:
    typedef __list_iterator<T,T&,T*> iterator;
    typedef list_node *link_type;
private:
    link_type node;


protected:
    std::pair<iterator ,iterator> alloc_and_copy(iterator a,iterator b)
    {
        auto p = list_node_allocator::allocate(distance( b , a));
        return {p,uninitialzed_copy(a,b,p)};
    }

    link_type get_node(){return list_node_allocator::allocate();}
    void put_node(link_type p){list_node_allocator::deallocate(p,0);}
    link_type create_node(const T & x)
    {
        link_type p = get_node();
        construct(&(p->data),x);
        return p;
    }
    void destory_node(link_type p)
    {
        destory(&p->data);
        put_node(p);
    }
    void empty_list()
    {
        node = get_node();
        node->next = node;
        node->prev = node;
    }
public:
    list(){empty_list();}
    iterator begin()const{return (link_type)(node->next);}
    iterator end()const{return node;}
    //函数insert()
    iterator insert(iterator tmp,const T & s)
    {
        link_type p = create_node(s);
        p->next = tmp.node;
        p->prev = tmp.node->prev;
       (link_type(tmp.node->prev))->next = p;
        tmp.node->prev = p;

    return p;
    }
    iterator insert(iterator tmp, T &&s)
    {

    }

    void push_back(const T& x){insert(end(),x);}
    void push_back(T && x){insert(end(),std::move(x));}
    list(const list & x)
    {
        empty_list();
        for(auto i = x.begin();i != x.end();++i)
        {
            this->push_back(*i);
        }

    }
    iterator erase(iterator position)
    {
       link_type next_node =  position.node->next;
       link_type prev_node = position.node->prev;
       next_node->prev = prev_node;
       prev_node->next = next_node;
       destory_node(position);
       return iterator(next_node);
    }


    list &operator =(const list &rhs)
    {
        if(&rhs == this)
        {

                for(auto i = begin();i != end();++i)
                {
                    erase(i);
                }

            for(auto i = rhs.begin(); i != rhs.end() ;++i)
            {
                empty_list();
                for(auto i = rhs.begin();i != rhs.end();++i)
                {
                    this->push_back(*i);
                }
            }
        }
        return *this;
    }
    template<typename Iter>list(Iter a,Iter b)
    {
        empty_list();
        for(auto i= a;i != b;++i)
        {
           push_back(*i);
        }
    }

    list(const std::initializer_list<T> &ill)
    {
        empty_list();
        for(auto i = ill.begin();i != ill.end() ; ++i)
        {
            push_back(*i);
        }
    }
    void push_front(const T& x){insert(begin(),x);}
    void pop_front(){erase(begin());}
    void pop_back(){erase(end() - 1);}
    ~list()
    {
        link_type cur  = (link_type)node->next;
        while (cur != node) {
            link_type tmp = cur;
            cur = (link_type)cur->next;
           destory_node(tmp);
        }
        node->next = node;
        node->prev = node;
    }



};


}
#endif // STL_LIST_H
