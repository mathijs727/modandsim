/* 
 * Names:   Peter Verkade & Mathijs Molenaar
 * Date:    02-11-2015
 * Course:  Modelling & Simulating
 * Assignment: week 1 part 1
 */

#include <iostream>
#include <float.h>
#include <math.h>

using namespace std;

template <typename Type>
Type precision();

template <typename Type>
Type minValue();

template <typename Type>
Type maxValue();

int main() {
    cout << "==== Question A ====" << endl;
    cout << "Values from float.h" << endl;
    cout << "Byte size float: " << sizeof(float) << endl;
    cout << "Machine precision float: " << FLT_EPSILON << endl;
    cout << "Minimum float value: " << FLT_MIN << endl;
    cout << "Maximum float value: " << FLT_MAX << endl;

    cout << "\nByte size double: " << sizeof(double) << endl;
    cout << "Machine precision double: " << DBL_EPSILON << endl;
    cout << "Minimum double value: " << DBL_MIN << endl;
    cout << "Maximum double value: " << DBL_MAX << endl;

    cout << "\nByte size long double: " << sizeof(long double) << endl;
    cout << "Machine precision long double: " << LDBL_EPSILON << endl;
    cout << "Minimum long double value: " << LDBL_MIN << endl;
    cout << "Maximum long double value: " << LDBL_MAX << endl;

    cout << "\n\n";

    cout << "Measured values:" << endl;
    cout << "Machine precision float: " << precision<float>() << endl;
    cout << "Minimum float value: " << minValue<float>() << endl;
    cout << "Maximum float value: " << maxValue<float>() << endl;

    cout << "\nMachine precision double: " << precision<double>() << endl;
    cout << "Minimum double value: " << minValue<double>() << endl;
    cout << "Maximum double value: " << maxValue<double>() << endl;


    cout << "\nMachine precision long double: " << precision<long double>() << endl;
    cout << "Minimum long double value: " << minValue<long double>() << endl;
    cout << "Maximum long double value: " << maxValue<long double>() << endl;


    cout << "\n\n==== Question B ====" << endl;
    /* These expressions result in NaN. */
    cout << "INFINITY + (-INFINITY) = " << (INFINITY + (-INFINITY)) << endl;
    cout << "0.0 * INFINITY         = " << 0.0 * INFINITY << endl;
    cout << "0.0 / 0.0              = " << 0.0 / 0.0 << endl;
    cout << "INFINITY / INFINITY    = " << INFINITY / INFINITY << endl;
    cout << "sqrt(-1.0)             = " << sqrt(-1.0) << endl;
    /* These expressions result in Infinity. */
    cout << "20^200 * 20^200        = " << pow(20, 200) * pow(20, 200) << endl;
    cout << "1.0 / 0.0              = " << 1.0 / 0.0 << endl;
    cout << "INFINITY + 1           = " << INFINITY + 1 << endl;
    /* This expressions result in -0. */
    cout << "0.0 / -1.0             = " << 0.0 / -1.0 << endl;

    return 0;
}

/* This functions calculates the machine precision epsilon for a given 
 * floating point datatype. */
template <typename Type>
Type precision()
{
    Type epsilon = 1.0;
    Type one = 1.0;
    Type half = 0.5;
    Type expression = 0.0;

    /* We search for the value epsilon that is so small, if it were smaller, 
     * (1 + epsilon) could not be rounded up anymore. Epsilon is then the 
     * largest value that can be rounded. The maximum relative error epsilon
     * does not depend not the current exponent of the floating point value. */ 
    while (expression != one) {
        epsilon *= half;
        expression = (one + half * epsilon);
    }

    return epsilon;
}

/* This functions calculates the smallest representable number for a given 
 * floating point datatype. */
template <typename Type>
Type minValue()
{
    Type n = 1.0;
    Type previousN;

    /* If n is larger than zero, we divide it by 2. The moment n becomes 0, we 
     * know the previous n is the smallest possible number. */
    while (n > (Type)0.0) {
        previousN = n;
        n *= (Type)0.5;
    }

    return previousN;
}

/* This functions calculates the largest representable number for a given 
 * floating point datatype. */
template <typename Type>
Type maxValue()
{
    Type n = 1.0;
    Type e = 1.0;
    Type previousN = 0.0;

    /* We increase n with decreasing values without going into infinity. When
     * we cannot increase n anymore, we know n must be the largest possible
     * number. */ 
    while(n > previousN) {
        if (isfinite(n + (n * e))) {
            previousN = n;
            n += n * e;
        } else {
            e *= (Type)0.5;
        }
    }

    return n;
}
