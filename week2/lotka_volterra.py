from matplotlib import pyplot as plt
import numpy as np

from differential_solvers import *

if __name__ == "__main__":
    
    start = np.array([10., 10.]) # x, y
    
    a = 0.5
    b = 1.0
    c = 0.1
    d = 0.1
    
    f = lambda t, s: np.array([
        - a * s[0] + c * d * s[0] * s[1], 
        + b * s[1]     - d * s[0] * s[1]
    ])
    
    stepsize = 0.25
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
    ax.set_ylabel("x(t) en y(t)")
    
    
    plt.show()
