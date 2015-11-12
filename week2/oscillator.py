from matplotlib import pyplot as plt
import numpy as np

from differential_solvers import *

""" Analytical solution

    s' = v
    v' = -ks
    
    so the solution can be found by solving 
    
    s''(t) = -k * s(t)
    
    the second order derivative has: a minus 1 and a factor k 
        the minus 1 reminds us of the sine and cosine
        the k must mean s(t) has the form f(sqrt(k) * t) 
    
    the solution becomes clear 
    
    s(t) = sin(t * sqrt(k))
    
    or 
    
    s(t) = cos(t * sqrt(k))

    Note that sin(t + 0.5 * pi) = cos(t)
    
    the analytical solution is therefore:
    
    s(t) = sin(sqrt(k) * t + phi) where phi is determined by the initial condition
    
"""

if __name__ == "__main__":
    
    start = np.array([1.0, 0.0]) # s, v
    k = 1.0
    f = lambda t, y: np.array([y[1], -k * y[0]]) # s' = v, -v' = -k * s
    
    for stepsize_fraction in [0.1, 0.05]:
    
        """ This oscilation has a period T equal to (2 * pi) / omega where omega is 
            sqrt(k). For this k and starting values, s(t) = cos(t) """
        period = (2 * np.pi) / np.sqrt(k)
        stepsize = stepsize_fraction * period
        n_periods = 10
        time = np.arange(0, n_periods * period + stepsize, stepsize)
        
        """ Calculate values s(t) """
        
        results_euler       = [s[0] for s in solve_for_list(start, time, f, integration_method=euler)] 
        results_rungekutta2 = [s[0] for s in solve_for_list(start, time, f, integration_method=rungekutta2)] 
        results_rungekutta4 = [s[0] for s in solve_for_list(start, time, f, integration_method=rungekutta4)] 
        
        """ Plot values """
        
        a, = plt.plot(time, results_euler, color="blue")
        b, = plt.plot(time, results_rungekutta2, color="red")
        c, = plt.plot(time, results_rungekutta4, color="green")
        
        ax = plt.gca()
        ax.legend((a, b, c), ("Euler method", "Runge-Kutta 2", "Runge-Kutta 4"), loc="lower left")
        ax.set_title("position over time, stepsize {} period".format(stepsize_fraction))
        ax.set_xlabel("t")
        ax.set_ylabel("s(t)")
        plt.show()
        
        """ Calculate errors """
        
        actual = np.cos(time)
        error_euler = np.abs(actual - results_euler)
        error_rungekutta2 = np.abs(actual - results_rungekutta2)
        error_rungekutta4 = np.abs(actual - results_rungekutta4)
        
        """ Plot error """
        
        a, = plt.plot(time, error_euler, color="blue")
        b, = plt.plot(time, error_rungekutta2, color="red")
        c, = plt.plot(time, error_rungekutta4, color="green")
        
        ax = plt.gca()
        ax.legend((a, b, c), ("Euler method", "Runge-Kutta 2", "Runge-Kutta 4"), loc="upper left")
        ax.set_title("error over time, stepsize {} period".format(stepsize_fraction))
        ax.set_xlabel("t")
        ax.set_ylabel("error")
        ax.set_yscale("log")
        plt.show()
        
        """ Plot phase space """
        
        speed_euler       = [s[1] for s in solve_for_list(start, time, f, integration_method=euler)] 
        speed_rungekutta2 = [s[1] for s in solve_for_list(start, time, f, integration_method=rungekutta2)] 
        
        a = plt.scatter(results_euler, speed_euler, color="blue")
        b = plt.scatter(results_rungekutta2, speed_rungekutta2, color="red")
        
        ax = plt.gca()
        ax.legend((a, b), ("Euler method", "Runge-Kutta 2"), loc="lower right")
        ax.set_title("phase space, stepsize {} period".format(stepsize_fraction))
        ax.set_xlabel("s(t)")
        ax.set_ylabel("v(t)")
        plt.show()   
