"""
    Names: Peter Verkade & Mathijs Molenaar
    Date: 30-10-2015
    Course: Modelling & Simulating
    Assignment: week 1 part 3
    
    In this assignment we  will approximate sqrt(2) by finding the root of the 
    function f(x) = x ^ 2 - 2. This root is approximated using the bisection
    algorithm.
"""

from math import sqrt

""" This function approximates the root of a given function on a given
    interval. Since the perfect value x for which f(x) = 0 might not be 
    possible to find, we will search for a value x so that f(x) becomes
    close to zero. """
def find_root_bisection(function, interval, maximum_error=0.000001, maximum_iterations=1000):

    for _ in xrange(maximum_iterations):
    
        center = 0.5 * (interval[0] + interval[1])
        value_at_center = function(center)

        if value_at_center > 0:
            interval[1] = center
            
        if value_at_center < 0:
            interval[0] = center

        if abs(value_at_center) < maximum_error:
            return center
        
    return center
            
if __name__ == "__main__":

    print "Actual square root of 2 is \t{}".format(sqrt(2))
        
    f = lambda x: x ** 2 - 2
    print "Found square root os 2 is \t{}".format(find_root_bisection(f, [1, 2]))
