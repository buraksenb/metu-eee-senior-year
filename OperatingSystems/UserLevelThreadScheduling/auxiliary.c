//
// Created by burak on 5/7/2021.
//
#include "auxiliary.h"

// These functions are for finding greatest common divider such that simplified version of share array can be found.
int gcdOfTwo(int x,int y)
{
    if(x == 0)
    {
        return y;
    }
    return gcdOfTwo(y % x,x);
}
int greatestCommonDivider(int* array, int length)
{
    int i;
    int gcd = array[0];
    for(i=0; i<length ; i++)
    {
        gcd = gcdOfTwo(array[i],gcd);
        if(gcd == 1)
        {
            return 1;
        }
    }
    return gcd;
}

