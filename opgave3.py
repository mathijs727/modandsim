"""
    Names:  Peter Verkade & Mathijs Molenaar
    Date:   02-11-2015
    Course: Modelling & Simulating
    Assignment: week 1 part 3
    
    In this assignment we  will approximate sqrt(2) by finding the root of the 
    function f(x) = x ^ 2 - 2. This root is approximated using the bisection
    algorithm.
"""

from math import sqrt
import matplotlib.pyplot as plt


""" Pyhton does not provide a built-in sign function. The edge case of sign(0)
    is not relevant in this program. """
def sign(x):
    if x > 0:
        return +1
    return -1


""" This function approximates the root of a given function on a given
    interval. Since the perfect value x for which f(x) = 0 might not be 
    possible to find, we will search for a value x so that f(x) becomes
    close to zero. """
def find_root_bisection(function, interval, maximum_error=0.000001, 
                        maximum_iterations=1000, plot=False, actual=0.0):

    start_interval = [interval[0], interval[1]]
    errors = []

    for _ in xrange(maximum_iterations):
    
        """ The center is the middle of the current interval. """
        center = 0.5 * (interval[0] + interval[1])
        value_at_center = function(center)

        """ Store the current error for possible plotting. """
        errors.append(abs(center - actual))

        """ If the value of the current center is very close to zero, that is 
            also good enough. """
        if abs(value_at_center) < maximum_error:
            if plot:
                plot_error(errors, start_interval)
            return center
               
        """ If the interval is a single point, which does not result in a proper
            root, we can stop here. """
        if interval[0] == interval[1]:
            return None

        """ We compare the signs of the values at the edges of the interval with
            the sign of the value at the center. If the sign is equal, we can 
            shrink the interval at that side. """
            
        if sign(value_at_center) == sign(function(interval[0])):
            interval[0] = center
            
        if sign(value_at_center) == sign(function(interval[1])):
            interval[1] = center
        
    return None


def plot_error(error, interval):
    plt.title("Error over iterations, starting with interval {}".format(interval))
    plt.xlabel("iterations")
    plt.ylabel("error")
    plt.grid(True)
    plt.plot(error)
    ax = plt.gca()
    ax.set_yscale("log")
    plt.show()

            
if __name__ == "__main__":

    print "Actual square root of 2 is \t{}".format(sqrt(2))
        
    f = lambda x: x ** 2 - 2
    print "Found  square root of 2 is \t{} for interval [1, 2]".format(find_root_bisection(f, [1, 2], plot=True, actual=sqrt(2)))
    print "Found  square root of 2 is \t{} for interval [0, 10]".format(find_root_bisection(f, [0, 10], plot=True, actual=sqrt(2)))
    print "Found  square root of 2 is \t{} for interval [0, 100]".format(find_root_bisection(f, [0, 100], plot=True, actual=sqrt(2)))
