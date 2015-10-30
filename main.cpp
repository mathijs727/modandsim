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
    /* Not a Number */
    cout << "INFINITY + (-INFINITY) = " << (INFINITY + (-INFINITY)) << endl;
    cout << "0.0 * INFINITY         = " << 0.0 * INFINITY << endl;
    cout << "0.0 / 0.0              = " << 0.0 / 0.0 << endl;
    cout << "INFINITY / INFINITY    = " << INFINITY / INFINITY << endl;
    cout << "sqrt(-1.0)             = " << sqrt(-1.0) << endl;
    /* Infinity */
    cout << "20^200 * 20^200        = " << pow(20, 200) * pow(20, 200) << endl;
    cout << "1.0 / 0.0              = " << 1.0 / 0.0 << endl;
    cout << "INFINITY + 1           = " << INFINITY + 1 << endl;

    return 0;
}

template <typename Type>
Type precision()
{
    Type epsilon = 1.0;

    while ((1.0 + 0.5 * epsilon) != 1.0) {
        epsilon *= 0.5;
    }

    return epsilon;
}

template <typename Type>
Type minValue()
{
    Type n = 1.0;
    Type previousN;

    while (n > 0.0) {
        previousN = n;
        n *= 0.5;
    }

    return previousN;
}

template <typename Type>
Type maxValue()
{
    Type n = 1.0;
    Type e = 1.0;
    Type previousN = 0.0;

    while(n > previousN) {
        if (isfinite(n + (n * e)))
        {
            previousN = n;
            n += n * e;
        } else {
            e *= 0.5;
        }
    }

    return n;
}
