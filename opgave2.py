"""
    Names:  Peter Verkade & Mathijs Molenaar
    Date:   02-11-2015
    Course: Modelling & Simulating
    Assignment: week 1 part 2
    
    In this assignment we will compare the accuracy of right hand 
    differentiation with central differentiation.
"""

import numpy as np
import matplotlib.pyplot as plt


"""
Use right hand differentioation to numerically differentiate a given function 
at a given x using a step size of h
( f(x)+f(x+h) ) / h
"""
def differencing_right(function, x, h):
	return (function(x + h) - function(x)) / h


"""
Use center differencing to numerically differentiate a given function at a
given x using a step size of h
( f(x-h)+f(x+h) ) / (2 * h)
"""
def differencing_center(function, x, h):
	return (function(x + h) - function(x - h)) / (2 * h)

"""
Use five point differencing to numerically differentiate a function at a given
x using a step size of h.
https://en.wikipedia.org/wiki/Five-point_stencil
"""
def differencing_fivepoint(function, x, h):
	return (-function(x + 2*h) + 8*function(x + h) - 8*function(x - h) + function(x - 2*h)) / (12*h)

if __name__ == "__main__":
	# Values on the x-axis
	h = np.logspace(-10, 0, 10000, base=10)

	# An array of tuples of x values for which we want to plot a graph
	test_inputs = [
		("pi/3", np.pi/3),
		("100pi + pi/3", 100*np.pi + np.pi/3),
		("10^12pi + pi/3", (10**12)*np.pi + np.pi/3)]

	for test_input in test_inputs:
		x = test_input[1]
		plt.title("Numerical differentiation of sin(%s)" % test_input[0])
		plt.xlabel("h")
		plt.ylabel("error")
		plt.grid(True)

		# Plot the error as the absolute value of cos(x) - num_differentiation(x)
		# Note that cos(x) is the derivative of sin(x)
		right, = plt.plot(h, abs(np.cos(x) - differencing_right(np.sin, x, h)))
		center, = plt.plot(h, abs(np.cos(x) - differencing_center(np.sin, x, h)))
		fivepoint, = plt.plot(h, abs(np.cos(x) - differencing_fivepoint(np.sin, x, h)))

		# Make the x-axis descending by reversing the x-axis
		ax = plt.gca()
		ax.set_yscale("log")
		ax.set_xscale("log")
		ax.set_xlim(ax.get_xlim()[::-1])
		ax.legend((right, center, fivepoint), ("Right", "Center", "Fivepoint"), loc="lower left")
		plt.show()
