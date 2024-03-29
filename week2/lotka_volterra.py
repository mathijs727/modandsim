"""
    Names:  Peter Verkade & Mathijs Molenaar
    Date:   23-11-2015
    Course: Modelling & Simulating
    Assignment: assignment 2 part 4
"""

from matplotlib import pyplot as plt
import numpy as np

from differential_solvers import *

if __name__ == "__main__":
    
    # start = np.array([30., 50.]) # x, y
    start = np.array([11., 49.]) # stable point is (10, 50)
    
    a = 0.5
    b = 1.0
    c = 0.1
    d = 0.1
    
    f = lambda time, state: np.array([
        - a * state[0] + c * d * state[0] * state[1], 
        + b * state[1]     - d * state[0] * state[1]
    ])
    
    stepsize = 0.5 ** 4
    time = np.arange(0, 100, stepsize)
    
    results = solve_for_list(start, time, f)
    
    results_x = [r[0] for r in results]
    results_y = [r[1] for r in results]
    
    x, = plt.plot(time, results_x, color="red")
    y, = plt.plot(time, results_y, color="green")
    
    ax = plt.gca()
    ax.legend((x, y), ("x", "y"), loc="upper left")
    ax.set_title("time")
    ax.set_xlabel("t")
    ax.set_ylabel("x(t) and y(t)")
    ax.set_ylim((0, 100))
    
    plt.show()
