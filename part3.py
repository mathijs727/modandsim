"""
    Names: Peter Verkade & Mathijs Molenaar
    Date: 30-10-2015
    Course: Modelling & Simulating
    Assignment: week 1 part 3
"""

from math import sqrt

def find_root_bisection(function, interval, maximum_error=0.000001):

    while True:
    
        center = 0.5 * (interval[0] + interval[1])
        value_at_center = function(center)

        if value_at_center > 0:
            interval[1] = center
            
        if value_at_center < 0:
            interval[0] = center

        """ value_at_center might not become exactly 0. Instead, we check
            whether it is very close to 0. """
        if abs(value_at_center) < maximum_error:
            return center
        
    return center
            
if __name__ == "__main__":

    f = lambda x: x ** 2 - 2

    print "Actual square root of 2 is \t{}".format(sqrt(2))
    print "Found square root os 2 is \t{}".format(find_root_bisection(f, [1, 2]))
