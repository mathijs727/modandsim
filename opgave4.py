"""
    Names:  Peter Verkade & Mathijs Molenaar
    Date:   02-11-2015
    Course: Modelling & Simulating
    Assignment: week 1 part 4
"""
    
import numpy as np
import matplotlib.pyplot as plt
from opgave2 import *

"""
Determine where the function becomes using the Newton Raphson method. This
function takes the function we want to look at, a differencing function to
determine the derivative and a x from which we start searching
"""
def newton_raphson(function, derivative, start, maximum_error = 0.00001,
                   derivative_step_size = 0.0001):
	x = start

	# While precision > f(x) > precision
	while abs(function(x)) > maximum_error:
		x -= function(x)/derivative(function, x, derivative_step_size)

	return x

"""
The value of a polynom at x where the polynom is an array in the form of:
[c0, c1, c2, c3, c4, ...] = c0 + c1*x + c2*x^2 + c3*x^3 + c4*x^4 + ...
"""
def polynom_value(polynom, x):
	value = 0.0

	for i in xrange(len(polynom)):
		value += polynom[i] * pow(x, i)

	return value

"""
Returns the polynom that is the derivative of the given polynom.
"""
def polynom_derivative(polynom):
	result = []

	for i in xrange(1, len(polynom)):
		coefficient = polynom[i]
		result.append(i * coefficient)

	return result

"""
Determine where an polynom becomes 0 using the Newton Raphson method. The
polynom should be given as an array:
[c0, c1, c2, c3, c4, ...] = c0 + c1*x + c2*x^2 + c3*x^3 + c4*x^4 + ...

Please supply a starting position from which to start looking. The derivative
is determined analytically.
"""
def newton_raphson_polynom(polynom, start, maximum_error = 0.00001):
	x = start

	# Determine the derivative polynom
	derivative = polynom_derivative(polynom)

	# While precision > f(x) > precision
	while abs(polynom_value(polynom, x)) > maximum_error:
		x -= polynom_value(polynom, x)/polynom_value(derivative, x)

	return x

if __name__ == "__main__":
	start = 2
	print "===== x^2 - 2 = 0 ====="
	print "Newton raphson using function"
	print newton_raphson(lambda x: x**2 - 2, differencing_center, start)
	print "\nNewton raphson using polynom"
	print newton_raphson_polynom([-2, 0, 1], start)

	start = -1.5
	print "\n\n===== x^3 + 3x^2 - 4 = 0 ====="
	print "Newton raphson using function"
	print newton_raphson(lambda x: x**3 + 3*x**2 - 4, differencing_center, start)
	print "\nNewton raphson using polynom"
	print newton_raphson_polynom([-4, 0, 3, 1], start)