import numpy as np
from matplotlib import pyplot as plt

def euler(y0, t0, t1, f, stepsize):

    y = y0
    t = t0

    while t < t1:
        y += stepsize * f(t, y)
        t += stepsize
       
    if not np.isfinite(y.all()):
        return None

    return y
    

def rungekutta2(y0, t0, t1, f, stepsize):

    y = y0
    t = t0

    while t < t1:
        An = f(t, y)
        y_approx = y + stepsize * An
        Bn = f(t + stepsize, y_approx)
    
        y += stepsize * 0.5 * (An + Bn)
        t += stepsize

    if not np.isfinite(y.all()):
        return None

    return y
    

def rungekutta4(y0, t0, t1, f, stepsize):

    y = y0
    t = t0

    while t < t1:   
        k1 = f(t, y)
        y1 = y + 0.5 * k1 * stepsize
        
        k2 = f(t + 0.5 * stepsize, y1)
        y2 = y + 0.5 * k2 * stepsize
        
        k3 = f(t + 0.5 * stepsize, y2)
        y3 = y + k3 * stepsize
        
        k4 = f(t + stepsize, y3)
        
        y += stepsize * (k1 + 2 * k2 + 2 * k3 + k4) / 6.0
        t += stepsize

    if not np.isfinite(y.all()):
        return None

    return y

    
if __name__ == "__main__":
    
    
    f = lambda t, y: 1
    print "--++{ dy/dt = 1 }++--"
    print euler(0, 0, 10, f, 0.125)
    print rungekutta2(0, 0, 10, f, 0.125)
    print rungekutta4(0, 0, 10, f, 0.125)
    
    f = lambda t, y: t
    print "--++{ dy/dt = t }++--"
    print euler(0, 0, 10, f, 0.125)
    print rungekutta2(0, 0, 10, f, 0.125)
    print rungekutta4(0, 0, 10, f, 0.125)
    
    f = lambda t, y: y
    print "--++{ dy/dt = y }++--"
    print euler(1, 0, 5, f, 0.125)
    print rungekutta2(1, 0, 5, f, 0.125)
    print rungekutta4(1, 0, 5, f, 0.125)
    
    f = lambda t, y: y * y
    print "--++{ dy/dt = y ** 2 }++--"
    print euler(1, -1, 1, f, 0.125)
    print rungekutta2(1, -1, 1, f, 0.125)
    print rungekutta4(1, -1, 1, f, 0.125)
    
    f = lambda t, y: y * y
    print "--++{ dy/dt = y ** 2 }++--"
    print euler(1, 1, 10, f, 0.125)
    print rungekutta2(1, 1, 10, f, 0.125)
    print rungekutta4(1, 1, 10, f, 0.125)
    
    """
    f = lambda t, y : y + 2
    results = [euler(2, 0, t, f, 0.125) for t in np.arange(0, 10.1, 0.1)]
    plt.plot(np.arange(0, 10.1, 0.1), results)
    
    ax = plt.gca()
    ax.set_yscale("log")
    plt.show()
    """
    
