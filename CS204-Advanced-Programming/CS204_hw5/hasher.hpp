#ifndef HASHER_H
#define HASHER_H

#include <string>
#include <iostream>


static int s_poison = 1;

bool isPrime(int n)
{
    //loop from 2 to n/2 to check for factors
    for (int i = 2; i <= n/2; i++)
    {
        if (n % i == 0)     //found a factor that isn't 1 or n, therefore not prime
            return false;
    }

    return true;
}

int findNextPrime(int n)
{
    int nextPrime = n;
    bool found = false;

    //loop continuously until isPrime returns true for a number above n
    while (!found)
    {
        nextPrime++;
        if (isPrime(nextPrime))
            found = true;
    }

    return nextPrime;
}

class StrHash{

private:
  unsigned poison;
  unsigned col_size;

public:
  StrHash(){}

  StrHash(int col_size){
    this->col_size = col_size;
    this->poison = (unsigned)findNextPrime((int)s_poison++);
    s_poison = this->poison + 1;
  }
  
  void set_parameters(unsigned col_size){
    this->col_size = col_size;
    this->poison = (unsigned)findNextPrime((int)s_poison++);
    s_poison = this->poison + 1;
  }

  unsigned hash(std::string const& data)
  {
    unsigned hashVal = 0;
    for (int x = 0; x < data.length(); ++x){
      hashVal ^= ((hashVal << 5) + (hashVal >> 2) + data[x] + this->poison);
    }
    
    return hashVal % this->col_size;
  }
  
  
};
#endif
