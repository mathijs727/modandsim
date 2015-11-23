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
    
    start = np.array([20., 20., 10.]) # x, y
    
    f = lambda time, state: np.array([
        state[0] * (1 - 0.001 * state[0] - 0.001 * state[1] - 0.01 * state[2]),
        
        state[1] * (1 - 0.001 * state[1] - 0.0015 * state[0] - 0.001 * state[2]),
        state[2] * (0.005 * state[0] + 0.0005 * state[1] - 1)
    ])
    
    stepsize = 0.05
    time = np.arange(0, 2000, stepsize)
    
    results = solve_for_list(start, time, f)
    
    results_x = [r[0] for r in results]
    results_y = [r[1] for r in results]
    results_z = [r[2] for r in results]
    
    x, = plt.plot(time, results_x, color="red")
    y, = plt.plot(time, results_y, color="green")
    z, = plt.plot(time, results_z, color="blue")
    
    ax = plt.gca()
    ax.legend((x, y, z), ("x", "y", "z"), loc="upper left")
    ax.set_title("time")
    ax.set_xlabel("t")
    ax.set_ylabel("x(t), y(t) and z(t)")
    
    plt.show()
