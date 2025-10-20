#include "math_utils.h"

long long gcd(long long a, long long b)
{
    long long tmp;
    if(a < b){
        tmp = a;
        a = b;
        b = tmp;
    }
    while (b != 0){
        tmp = b;
        b = a%b;
        a = tmp;
    }
    return a;
}

long long lcd(long long a, long long b){
    return (a*b)/gcd(a,b);
}
