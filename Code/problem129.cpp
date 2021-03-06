#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <random>
#include <chrono>
#include <set>
#include <queue>
#include "math_unsigned.cpp"
#include "math_signed.cpp"

static bool* composite;
static std::vector<long long> primes;

int exp(long long b, int e, int n)
{
    long long ans = 1;
    while(e > 0)
    {
        if(e % 2 == 1)
        {
            ans = (ans*b) % n;
        }
        b = (b * b) % n;
        e /= 2;
    }
    return ans;
}

int totient (int num)
{
    int ans = num;
    for(int i = 0; i < primes.size () && primes[i] * primes[i] <= num; i++)
    {
        if(num % primes[i] == 0)
        {
            num /= primes[i];
            ans /= primes[i];
            ans *= primes[i]-1;
        }
        while(num % primes[i] == 0)
        {
            num /= primes[i];
        }
    }
    if(num > 1)
    {
        ans /= num;
        ans *= num-1;
    }
    return ans;
}

std::vector<int>* factors (int num)
{
    std::vector<int> *ans = new std::vector<int>{};
    for(int i = 0; i < primes.size() && primes[i] * primes[i] <= num; i++)
    {
        if(num % primes[i] == 0)
        {
            num /= primes[i];
            ans->push_back(primes[i]);
        }
        while(num % primes[i] == 0)
        {
            num /= primes[i];
        }
    }
    if(num > 1)
    {
        ans->push_back(num);
    }
    return ans;
}

int best(int tot, int mod)
{
    std::vector<int> *fac = factors(tot);
    int largest = tot;
    for(int j = 0; j < fac->size(); j++)
    {
        if(exp(10, tot/((*fac)[j]), mod) == 1)
        {
            largest = tot/((*fac)[j]);
        }
    }
    if(largest == tot)
    {
        return largest;
    }
    return best(largest, mod);
}

int main ()
{
    //Find the number n with gcd(n, 10) == 1,
    //For which the first number of the form 1111...1111
    //which is divisible by n has more than 1 million digits

    static constexpr int limit = 1000000;
    composite = new bool[limit];
    for(int i{2}; i < 1000; i++)
    {
        if(!composite[i])
        {
            for(int j{2}; i*j < limit; j++)
            {
                composite[i*j] = true;
            }
        }
    }

    for(int i = 2; i < limit; i++)
    {
        if(!composite[i])
        {
            primes.push_back(i);
        }
    }

    //If 111...111 is divisible by i, then 
    //999...999 is divisible by 9i, which means
    //we are looking for a power of 10 which is 
    //congruent to 1 mod 9i
    //We also require this to be the smallest power of 10
    //congruent to 1, so we recursively search to see 
    //what the smallest is
    for(int i = 1000000; ; i++)
    {
        if(i % 10 == 1 || i % 10 == 3 || i % 10 == 7 
        || i % 10 == 9)
        {
            int tot = totient(9*i);
            if(tot > limit)
            {
                int largest = best(tot, 9*i);
                if(largest > limit)
                {
                    std::cout << i << '\n';
                    return 0;
                }
            }
        }
    }
    return 0;
}