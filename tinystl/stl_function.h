#ifndef STL_FUNCTION_H
#define STL_FUNCTION_H
namespace tinystl {

//用来呈现一元函数的参数类别和返回值型别
template<class Arg,class Result>
struct unary_function
{
  typedef Arg first_argument_type;
  typedef Result result_type;
};

template<class Arg1,class Arg2,class Result>
struct binary_function
{
  typedef Arg1 first_arguments_type;
  typedef Arg2 second_arguments_type;
  typedef Result result_type;
};
//加法 plus
template<class T>
struct plus:public binary_function<T,T,T>
{
  T operator()(const T& x,const T& y)const {return x+y;}
};
//减法 minus
template<class T>
struct minus:public binary_function<T,T,T>
{
  T operator()(const T&x,const T &y)const{return x-y;}
};
//乘法 multiplies
template<class T>
struct multiplies:public binary_function<T,T,T>
{
  T operator()(const T& x,const T&y){return x*y;}
};
//除法 divides
template<class T>
struct divides:public binary_function<T,T,T>
{
  T operator()(const T&x,const T&y)const{return x/y;}
};
//模取 modulus
template<class T>
struct modulus:public binary_function<T,T,T>
{
  T operator()(const T&x,const T& y)const {return x%y;}
};
//否定 negate
template<class T>
struct negate:public unary_function<T,T>
{
  T operator()(const T&x)const {return -x;}
};
//等于 equal_to
template<class T>
struct equal_to:public binary_function<T,T,bool>
{
  bool operator()(const T&x,const T& y)const {return x==y;}
};
//不等于 not_equal_to
template<class T>
struct not_equal_to:public binary_function<T,T,bool>
{
  bool operator()(const T&x,const T&y){return x!=y;}
};
//大于 greater
template<class T>
struct greater:public binary_function<T,T,bool>
{
  bool operator()(const T&x,const T&y){return x>y;}
};
template<class T>
struct greater_equal:public binary_function<T,T,T>
{
  bool operator()(const T&x,const T&y)const{return x>=y;}
};
template<class T>
struct less:public binary_function<T,T,bool>
{
  bool operator()(const T&x,const T&y)const{return x < y;}
};
template<class T>
struct less_equal:public binary_function<T,T,bool>
{
  bool operator()(const T&x,const T&y)const{return x <= y;}
};
//And
template<class T>
struct logical_and:public binary_function<T,T,bool>
{
  bool operator()(const T&x,const T&y){return x&&y;}
};
//Or
template<class T>
struct logical_or:public binary_function<T,T,bool>
{
  bool operator()(const T&x,const T&y){return x||y;}
};
template<class T>
struct logical_not:public unary_function<T,bool>
{
  bool operator()(const T&x)const{return !x;}
};
template<class T>
inline T identity_element(plus<T>)
{
 return T(0);
}
template<class T>
inline T indentity_element(multiplies<T>)
{
  return T(1);
}
template<class T>
struct identity:public unary_function<T,T>
{
  const T& operator()(const T& x) const {return x; }
};
}
#endif // STL_FUNCTION_H
