from differential_solvers import *
import numpy as np

class Counter:

    n = 0
    
    def reset(self):
        self.n = 0
        
    def increment(self):
        self.n += 1
        return 1
        
    def get_count(self):
        return self.n

if __name__ == "__main__":
    
    counter = Counter()
    
    start = np.array([0])
    f = lambda t, s: np.array([counter.increment()]) 
    steps = 100
    
    print "==== Counting number of calls to f ===="
    
    counter.reset()
    euler(start.copy(), 0, 100, f, 1.0)
    print "Euler method  for {} step: {} calls to f".format(steps, counter.get_count())
    
    counter.reset()
    rungekutta2(start.copy(), 0, 100, f, 1.0)
    print "Runge-Kutta 2 for {} step: {} calls to f".format(steps, counter.get_count())
    
    counter.reset()
    rungekutta4(start.copy(), 0, 100, f, 1.0)
    print "Runge-Kutta 4 for {} step: {} calls to f".format(steps, counter.get_count())
