from matplotlib import pyplot as plt
import numpy as np

from differential_solvers import *

""" Analytical solution

    s' = v
    v' = -ks
    
    so the solution can be found by solving 
    
    s''(t) = -k * s(t)
    
    the secondorder derivative has: a minus 1 and a k extra 
        the minus 1 reminds us of a sine or cosine
        the k must mean s(t) has the form f(sqrt(k) * t) 
    
    the solution becomes clear 
    
    s(t) = sin(t * sqrt(k))
    
    or 
    
    s(t) = cos(t * sqrt(k))

    Note that sin(t) = cos(t + 0.5 * pi)
    
    the analytical solution is therefore:
    
    s(t) = sin( sqrt(k) * t + phi ) where phi is determined by the initial condition
    
"""

if __name__ == "__main__":
    
    start = np.array([1.0, 0.0]) # s, v
    k = 1.0
    
    f = lambda t, y: np.array([y[1], -k * y[0]]) # s' = v, -v' = -k * s
    
    """ This oscilation has a period T equal to (2 * pi) / omega where omega is 
        sqrt(k). For this k and starting values, s(t) = cos(t) """
    period = (2 * np.pi) / np.sqrt(k)
    stepsize = 0.1 * period
    n_periods = 10
    time = np.arange(0, n_periods * period + stepsize, stepsize)
    
    """ We only want the position s at each time step, which is the first 
        element of the result. """
    results_euler       = [      euler(start.copy(), 0.0, t, f, stepsize)[0] for t in time]
    results_rungekutta2 = [rungekutta2(start.copy(), 0.0, t, f, stepsize)[0] for t in time]
    results_rungekutta4 = [rungekutta4(start.copy(), 0.0, t, f, stepsize)[0] for t in time]
    
    a, = plt.plot(time, results_euler)
    b, = plt.plot(time, results_rungekutta2)
    c, = plt.plot(time, results_rungekutta4)
    
    ax = plt.gca()
    ax.legend((a, b, c), ("Euler method", "Runge-Kutta 2", "Runge-Kutta 4"), loc="lower left")
    
    plt.show()