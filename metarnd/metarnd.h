#include <iostream>
#include <tuple>

constexpr char inits[] = __TIME__;
const int rs = (inits[0]-'0')*100000+(inits[1]-'0')*10000 +
               (inits[3]-'0')*1000+(inits[4]-'0')*100+(inits[6]-'0')*10+inits[7]-'0';

template<typename E>
struct eval
{
  typedef typename E::type type;
};

template<typename E>
struct init
{
  typedef E type;
};

template<typename Engine>
struct Random
{
  //static const uint_fast32_t value = seed;
  //typedef linear_congruential_engine<uint_fast32_t, value> type;
  typedef typename init<Engine>::type type;
  static const decltype(type::value) value = type::value;
};

template<typename R>
struct Next
{
  typedef typename eval<R>::type type;
  static const decltype(type::value) value = type::value;
  //static const uint_fast32_t value = linear_congruential_engine<R>::value; //next random val
};


//--------------- linear_congruential_engine ---------------------

template<typename UIntType,
         UIntType seed = rs,
         UIntType a = 16807,
         UIntType c = 0,
         UIntType m = 2147483647>
struct linear_congruential_engine
{
  static const UIntType value = seed;
  static const UIntType maxvalue = m-1;
};

template<typename UIntType, UIntType seed, UIntType a, UIntType c, UIntType m>
struct eval<linear_congruential_engine<UIntType, seed, a, c, m>>
{
  static const UIntType value = (a * seed + c) % m;
  typedef linear_congruential_engine<
    UIntType,
    (a * seed + c) % m,
    a,
    c,
    m
  > type;
};

template<typename UIntType,
         UIntType seed,
         UIntType a,
         UIntType c,
         UIntType m>
struct init<linear_congruential_engine<UIntType, seed, a, c, m>>
{
  typedef typename eval<linear_congruential_engine<UIntType,seed,a,c,m>>::type type;
  static const UIntType value = type::value;
};

// ----------------- fib_engine ----------------------------

template<typename UIntType,
         UIntType a = 0,
         UIntType b = 1>
struct fib_engine
{
  static const UIntType value = a;
};

template<typename UIntType, UIntType a, UIntType b>
struct eval<fib_engine<UIntType, a, b> >
{
  static const UIntType value = a + b;
  typedef fib_engine<
    UIntType,
    b,
    value
  > type;
};

// ------------------------ substract_with_carry --------------

template<typename UIntType,
         UIntType r,
         UIntType s,
         UIntType m,
         UIntType seed = rs>
struct substract_with_carry_engine
{
};

template<typename UIntType,
         UIntType r,
         UIntType s,
         UIntType m,
         UIntType res,
         UIntType... vals>
struct substract_with_carry_impl
{
  static const UIntType value = res;

  template< unsigned int N, UIntType v, UIntType... vs >
  struct extract
  {
    const static UIntType value = extract<N-1, vs...>::value;
  };

  template< UIntType v, UIntType... vs  >
  struct extract<0, v, vs...>
  {
    const static UIntType value = v;
  };

  template< unsigned int N >
  struct get
  {
    const static UIntType value = extract<N, vals...>::value;
  };

  //debug
  template< UIntType v, UIntType... vs>
  struct print_aux
  {
    static void print()
    {
      std::cout << v << " ";
      print_aux<vs...>::print();
    }
  };

  template< UIntType v>
  struct print_aux<v>
  {
    static void print()
    {
      std::cout << v << std::endl;
    }
  };

  struct print
  {
    static void dump()
    {
      print_aux<vals...>::print();
    }
  };

};

template<typename UIntType,
         UIntType r,
         UIntType s,
         UIntType m,
         typename FillRnd, unsigned int cnt, UIntType... vals>
struct fillrnd
{
  typedef typename Next<FillRnd>::type RND;
  typedef typename fillrnd<UIntType,
                          r, s, m,
                          RND,  cnt-1, vals..., RND::value >::type type;
};

template<typename UIntType,
         UIntType r,
         UIntType s,
         UIntType m,
         typename FillRnd, UIntType... vals>
struct fillrnd<UIntType, r, s, m, FillRnd, 0, vals...>
{
  typedef substract_with_carry_impl< UIntType,
                                     r, s, m, 0, //0 is dummy result
                                     vals...> type;
};

template<typename UIntType,
         UIntType r,
         UIntType s,
         UIntType m,
         UIntType seed>
struct init<substract_with_carry_engine<UIntType, r, s, m, seed>>
{
  typedef typename fillrnd<
    UIntType,
    r, s, m,
    typename Random<linear_congruential_engine<uint_fast32_t,seed>>:: type,
    r
  >::type type;
};

template<typename UIntType,
         UIntType r,
         UIntType s,
         UIntType m,
         UIntType res,
         UIntType val,
         UIntType... vals>
struct eval<substract_with_carry_impl<UIntType, r, s, m, res, val, vals...>>
{
  //typedef seq<UIntType, vals...> X;
  typedef substract_with_carry_impl<UIntType, r, s, m, res, val, vals...> X;
  static const UIntType value = (X::template get<0>::value - X:: template get<r-s>::value);
  //static const UIntType value = val + 1;
  typedef substract_with_carry_impl<
    UIntType,
    r, s, m,
    value,
    vals...,
    value
  > type;
};

// ------------------------------- Bernoulli

template<typename Engine, int N, int D, bool val = false>
struct Bernoulli
{
  static const bool value = val;
};

template<typename Engine, int N, int D, bool b>
struct eval<Bernoulli<Engine, N, D, b>>
{
  typedef typename Next<Engine>::type tmptype;
  static const bool value = static_cast<double>(tmptype::value) / tmptype::maxvalue < static_cast<double>(N) / D;  
  typedef Bernoulli<tmptype, N, D, value> type;
  
};

