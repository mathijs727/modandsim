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
Determine where the function becomes 0 using the Newton Raphson method. This
function requires a function to analyze, a differencing function to
determine the derivative and a value x to start the search from.
"""
def newton_raphson(function, derivative, start, maximum_error = 0.000001,
                   derivative_step_size = 0.000001):
	x = start

	""" Loop untill function(x) is very close to 0. """
	while abs(function(x)) > maximum_error:
	    """ Apply the Newton-Raphson iteration rule on x. """
		x -= function(x)/derivative(function, x, derivative_step_size)

	return x

"""
This function evalutaes the value of a polynom for a given x where polynom is 
an array in the following form:
[c0, c1, c2, c3, c4, ...] = c0 + c1*x + c2*x^2 + c3*x^3 + c4*x^4 + ...
"""
def polynom_value(polynom, x):
	value = 0.0

	for i in xrange(len(polynom)):
		value += polynom[i] * pow(x, i)

	return value

"""
This functions returns the polynom that is the derivative of a given polynom.
"""
def polynom_derivative(polynom):
	result = []

    """ All coefficients are multiplied by their index and moved one place to 
    the left. """
	for i in xrange(1, len(polynom)):
		coefficient = polynom[i]
		result.append(i * coefficient)

	return result

"""
Determine where a polynom becomes 0 using the Newton Raphson method. The
polynom should be given as an array:
[c0, c1, c2, c3, c4, ...] = c0 + c1*x + c2*x^2 + c3*x^3 + c4*x^4 + ...

The starting position must be provided.
"""
def newton_raphson_polynom(polynom, start, maximum_error = 0.000001):
	x = start

	""" Determine the derivative polynom. """
	derivative = polynom_derivative(polynom)

	""" Loop until Polynom(x) is very close to zero. """
	while abs(polynom_value(polynom, x)) > maximum_error:
		x -= polynom_value(polynom, x) / polynom_value(derivative, x)

	return x

if __name__ == "__main__":
	start = 2
	print "===== x^2 - 2 = 0 ====="
	print "Newton raphson using a function"
	print newton_raphson(lambda x: x**2 - 2, differencing_center, start)
	print "\nNewton raphson using a polynom"
	print newton_raphson_polynom([-2, 0, 1], start)

	start = -1.5
	print "\n\n===== x^3 + 3x^2 - 4 = 0 ====="
	print "Newton raphson using a function"
	print newton_raphson(lambda x: x**3 + 3*x**2 - 4, differencing_center, start)
	print "\nNewton raphson using a polynom"
	print newton_raphson_polynom([-4, 0, 3, 1], start)
