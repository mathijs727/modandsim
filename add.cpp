/* 
 * Names:   Peter Verkade & Mathijs Molenaar
 * Date:    02-11-2015
 * Course:  Modelling & Simulating
 * Assignment: week 1 part 1c
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

/*
 * This test program computes the sum of the series 1/i, with i an 
 * integer running from a value "from" to a value "to" (inclusive).
 * It demonstrates that the result depends on how the computation 
 * is done.
 * G.D. van Albada
 * University of Amsterdam
 * 2013
 */

/* addRecursive will split the summation problem into an upper
   and a lower half and recursively compute the sums of each 
   half and return their sum. Only if no more than two values
   need to be added will the result be returned immediately.
   The recursion obviously is finite, as long as correct parameters
   are passed.
   Notice that a similar approach can be used for "divide and conquer"
   parallellisation on a multiprocessor machine.
*/
float
addRecursive(int from, int to)
{
   int N = 1 + to - from;
   int h;
   if (N < 1) return 0;
   if (N == 1) return 1.0f / (float) from;
   if (N == 2) return 1.0f / (float) from + 1.0f / (float) to;
   h = (to + from) / 2;
   /* what values would h have?
      to   from  h
      0    2     1
      0    3     1
      1    3     2
      1    4     2 */
    return addRecursive(from, h) + addRecursive(h + 1, to);
}

float
addForward(int from, int to)
{
    float sum = 0;
    int i;
    for (i = from; i <= to; i++)
    {
        sum += 1.0 / (float) i;
    }
    return sum;
}

float
addReverse(int from, int to)
{
    float sum = 0;
    int i;
    for (i = to; i >= from; i--)
    {
        sum += 1.0 / (float) i;
    }
    return sum;
}

double
addRecursiveDouble(int from, int to)
{
   int N = 1 + to - from;
   int h;
   if (N < 1) return 0;
   if (N == 1) return 1.0 / (double) from;
   if (N == 2) return 1.0 / (double) from + 1.0 / (double) to;
   h = (to + from) / 2;
   /* what values would h have?
      to   from  h
      0    2     1
      0    3     1
      1    3     2
      1    4     2 */
    return addRecursiveDouble(from, h) + addRecursiveDouble(h + 1, to);
}

double
addForwardDouble(int from, int to)
{
    double sum = 0;
    int i;
    for (i = from; i <= to; i++)
    {
        sum += 1.0 / (double) i;
    }
    return sum;
}

double
addReverseDouble(int from, int to)
{
    double sum = 0;
    int i;
    for (i = to; i >= from; i--)
    {
        sum += 1.0 / (double) i;
    }
    return sum;
}

/* The kahan summation algorithm based on 
 * https://en.wikipedia.org/wiki/Kahan_summation_algorithm */
template<typename Type>
Type kahanSum(int from, int to) {
    Type sum = 0.0;
    Type c = 0.0;
    
    for (int i = from; i < to; i++) {
        /* Add the compensation to the next number. */
        Type y = (1.0 / i) - c;
        Type t = sum + y;
        /* Calculate the compensation for the next iteration. */
        c = (t - sum) - y;
        sum = t;
    }

    return sum;
}

template<typename Type>
void testMaximumIndex(Type (*function)(int, int), const char* functionName)
{
    Type prevSum = 0.0;
    unsigned long index = 0;
    int deltaIndex = 10000000;
    while (true)
    {
        Type sum = function(1, index + deltaIndex);
        if (sum == prevSum)
        {
            if (deltaIndex == 1)
            {
                std::cout << "Maximum index for " << functionName << ": " << index << std::endl;
                break;
            } else {
                index -= deltaIndex;
                deltaIndex /= 10;
            }
        } else {
            index += deltaIndex;
            prevSum = sum;
        }
    }
}

int
main(int argc, char* argv[])
{
    int from = 1;
    int to = 1000;
    int i;
    int j;
    double sum[8];
    if (argc > 1)
    {
        from = atoi(argv[1]);
        if (argc > 2)
        {
            to = atoi(argv[2]);
        }
        if (from < 1) from = 1;
        if (to < from) to = from + 100;
    }

    testMaximumIndex<float>(addForward, "Add forward");
    testMaximumIndex<float>(addReverse, "Add reverse");

    std::cout << std::endl;

    printf("The sum of the series 1/i, with i running from %d to %d is\n",
            from, to);
    printf("Forward summation with floats: %g\n",
           sum[0] =addForward(from, to));
    printf("Reverse summation with floats: %g\n",
           sum[1] =addReverse(from, to));
    printf("Recursive summation with floats: %g\n",
           sum[2] =addRecursive(from, to));
    printf("Kahan summation with floats: %g\n",
           sum[3] =kahanSum<float>(from, to));
    printf("Forward summation with doubles: %g\n",
           sum[4] =addForwardDouble(from, to));
    printf("Reverse summation with doubles: %g\n",
           sum[5] =addReverseDouble(from, to));
    printf("Recursive summation with doubles: %g\n",
           sum[6] =addRecursiveDouble(from, to));
    printf("Kahan summation with doubles: %g\n",
           sum[7] =kahanSum<double>(from, to));
    printf("Differences between results in a matrix:\n");
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            printf("%12.5g\t", sum[i] - sum[j]);
        }
        printf("\n");
    }

    return 0;
}
