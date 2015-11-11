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
        
        results_euler       = [      euler(start.copy(), 0, t, f, stepsize)[0] for t in time]
        results_rungekutta2 = [rungekutta2(start.copy(), 0, t, f, stepsize)[0] for t in time]
        results_rungekutta4 = [rungekutta4(start.copy(), 0, t, f, stepsize)[0] for t in time]
        
        """ Plot values """
        
        a, = plt.plot(time, results_euler, color="blue")
        b, = plt.plot(time, results_rungekutta2, color="red")
        c, = plt.plot(time, results_rungekutta4, color="green")
        
        ax = plt.gca()
        ax.legend((a, b, c), ("Euler method", "Runge-Kutta 2", "Runge-Kutta 4"), loc="lower left")
        ax.set_title("position over time")
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
        ax.set_title("error over time")
        ax.set_xlabel("t")
        ax.set_ylabel("error")
        ax.set_yscale("log")
        plt.show()
        
        """ Plot phase space """
        
        speed_euler       = [      euler(start.copy(), 0, t, f, stepsize)[1] for t in time]
        speed_rungekutta2 = [rungekutta2(start.copy(), 0, t, f, stepsize)[1] for t in time]
        
        a = plt.scatter(results_euler, speed_euler, color="blue")
        b = plt.scatter(results_rungekutta2, speed_rungekutta2, color="red")
        
        ax = plt.gca()
        ax.legend((a, b), ("Euler method", "Runge-Kutta 2"), loc="lower right")
        ax.set_title("phase space")
        ax.set_xlabel("s(t)")
        ax.set_ylabel("v(t)")
        plt.show()   
