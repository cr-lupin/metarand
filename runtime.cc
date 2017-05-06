#include <random>
#include <iostream>

int main()
{
    // Seed with a real random value, if available
    std::random_device rd;

    // Choose a random mean between 1 and 6
    //std::minstd_rand0 r(3);
    std::ranlux24_base r(3);
    for(int i=0; i<10; ++i)
      std::cout << r() << " ";

    std::cout << std::endl;
}