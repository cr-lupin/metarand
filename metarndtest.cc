#include "metarnd.h"

template<int cnt, typename R>
struct print_randoms
{
  static void print()
  {
    typedef typename Next<R>::type RND;
    std::cout << RND::value << " ";
    print_randoms<cnt-1, RND >::print();
  }
};

template<typename R>
struct print_randoms<0, R>
{
  static void print()
  {
    std::cout << Next<R>::value << " " << std::endl;
  }
};


int main()
{
  /* Bernoulli engine is not fully implemented
   * print_randoms<10, 
                typename Random< Bernoulli< linear_congruential_engine<uint_fast32_t,3>, 1, 10>>::type >::print();
  */                
  
  // Last template argument is the random seed. It is optional.
  typedef init<substract_with_carry_engine<unsigned int, 10, 2, 1, 3>>::type X;
  
  // Prints the first random number
  typedef Next<X>::type X0;
  std::cout << X0::value << std::endl;;
  
  // Prints the second random number
  typedef Next<X0>::type X1;
  std::cout << X1::value << std::endl;
  
  print_randoms<15, typename Random<substract_with_carry_engine<unsigned int, 10, 2, 1, 3>>::type >::print();
}
