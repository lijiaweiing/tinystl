#ifndef SHARED_PTR_H
#define SHARED_PTR_H
#include<functional>
template<typename T>

class shared_ptr{
private:
    unsigned * use_count;
    T *m_project ;
      std::function<void(T*)> defalut_del= [](T*p) {delete p; p = nullptr; };
public:
    void reset();
    T * get() const noexcept;
    shared_ptr();
    explicit shared_ptr(T*);
    shared_ptr(const shared_ptr & );
    unsigned use_counts() const noexcept;
    T& operator*()const;
    T* operator->()const;
    ~shared_ptr();
    //默认一个function作为删除器

};
template <typename T>T& shared_ptr<T>::operator*()const{
    return  *m_project;
}
template <typename T>T* shared_ptr<T>::operator->()const{
    return  &this->operator*();
}
//template<typename T> std::function<void(T*)> shared_ptr<T>::defalut_del = [](T*p){delete p;p = nullptr;};
template<typename T>
shared_ptr<T>::shared_ptr():use_count(new unsigned(1)) ,m_project(nullptr) {}
template<typename T>
shared_ptr<T>::shared_ptr(T *p):use_count(new unsigned(1)) , m_project(p){}
template<typename T,typename... Args>
shared_ptr<T> make_shared(Args &&...args ){
    return shared_ptr<T>(new T(std::forward<Args>(args)...));
}
template <typename T>T *
shared_ptr<T>::get() const noexcept{
    return  m_project;
}
template <typename T>
unsigned shared_ptr<T>::use_counts()const noexcept{
    return  *use_count;
}
template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr &rhs):m_project(rhs.m_project),defalut_del(rhs.defalut_del){
    ++(*use_count);
}
template <typename T>
shared_ptr<T>::~shared_ptr(){
    if (--(*use_count) == 0){
        defalut_del(m_project);
    }
}
#endif // SHARED_PTR_H
