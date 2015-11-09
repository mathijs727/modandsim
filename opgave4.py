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
                   derivative_step_size = 0.00001, actual = 0.0, name=""):
    x = start
    error = []

    """ Loop untill function(x) is very close to 0. """
    while abs(function(x)) > maximum_error:
        """ Apply the Newton-Raphson iteration rule on x. """
        error.append(abs(x - actual))
        x -= function(x)/derivative(function, x, derivative_step_size)
        

    plot_error(error, name=name)
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
def newton_raphson_polynom(polynom, start, maximum_error = 0.000001, actual=0.0, name=""):
    x = start
    error = []

    """ Determine the derivative polynom. """
    derivative = polynom_derivative(polynom)

    """ Loop until Polynom(x) is very close to zero. """
    while abs(polynom_value(polynom, x)) > maximum_error:
        error.append(abs(x - actual))
        x -= polynom_value(polynom, x) / polynom_value(derivative, x)

    plot_error(error, name=name)

    return x

def plot_error(error, name=""):
    plt.title("Error over iterations: {}".format(name))
    plt.xlabel("iterations")
    plt.ylabel("error")
    plt.grid(True)
    plt.plot(error)
    ax = plt.gca()
    ax.set_yscale("log")
    plt.show()

if __name__ == "__main__":
    start = 2
    print "===== x^2 - 2 = 0 ====="
    print "Newton raphson using a function"
    print newton_raphson(lambda x: x**2 - 2, differencing_center, start, actual=np.sqrt(2), name="x^2 - 2 = 0 using numerical diff")
    print "\nNewton raphson using a polynom"
    print newton_raphson_polynom([-2, 0, 1], start, actual=np.sqrt(2), name="x^2 - 2 = 0 using polynom diff")

    start = 4
    print "\n===== x^3 + 3x^2 - 4 = 0 ====="
    print "Newton raphson using a function"
    print newton_raphson(lambda x: x**3 + 3*x**2 - 4, differencing_center, start, actual=1.0, name="x^3 + 3x^2 - 4 = 0 using numerical diff")
    print "\nNewton raphson using a polynom"
    print newton_raphson_polynom([-4, 0, 3, 1], start, actual=1.0, name="x^3 + 3x^2 - 4 = 0 using polynom diff")
    
    start = -4
    print "\nDifferent start:\nNewton raphson using a polynom"
    print newton_raphson_polynom([-4, 0, 3, 1], start, actual=-2.0, name="x^3 + 3x^2 - 4 = 0 using polynom diff from start -4")
    
    print "\n===== 2 sin(x - 2) ====="
    start = 0
    print "Newton raphson using a function"
    print newton_raphson(lambda x: 2 * np.sin(x - 2), differencing_center, start, actual=-np.pi+2, name="2 sin(x - 2) using numerical diff")
