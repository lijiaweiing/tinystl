#ifndef STL_PAIR_H
#define STL_PAIR_H
namespace tinystl {
  template<class T1,class T2>
  struct pair
  {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;//键值
    T2 second;//实值
    pair():first(T1()),second(T2()){}
    pair(const T1& t1,const T2& t2 ):first(t1),second(t2){}
    template<class U1,class U2>
    pair(const pair<U1,U2> & s):first(s.first),second(s.second){}
  };
}
#endif // STL_PAIR_H
