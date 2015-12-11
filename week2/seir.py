"""
    Names:  Peter Verkade & Mathijs Molenaar
    Date:   23-11-2015
    Course: Modelling & Simulating
    Assignment: assignment 2 part 5
"""

import numpy as np
import matplotlib.pyplot as plt

from differential_solvers import *

deathrate = 0.0
birthrate = 0.0
spread_rate = 50 * 0.04
incubation_rate = 1.0 / 6
recovery_rate = 1.0 / 9

# https://en.wikipedia.org/wiki/Compartmental_models_in_epidemiology#The_SEIR_model
def SEIR(time, state):
	total = np.sum(state)
	
	# Current values of the SEIR model
	S = state[0]
	E = state[1]
	I = state[2]
	R = state[3]

	SE = spread_rate * (I / total) * S
	EI = incubation_rate * E
	IR = recovery_rate * I

	# The derivative of the values according to the SEIR model as on:
	# https://en.wikipedia.org/wiki/Compartmental_models_in_epidemiology#The_SEIR_model
	return np.array([
		birthrate * total - deathrate * S - SE,
		SE - EI - deathrate * E,
		EI - IR - deathrate * I,
		IR - deathrate * R
	])

if __name__ == "__main__":
	stepsize = 0.05

	# Susceptible, Exposed, Infected, Recovered
	start = np.array([999.0, 1.0, 0.0, 0.0])
	time = np.arange(0, 100, stepsize)

	results = solve_for_list(start, time, SEIR)

	results_s = [r[0] for r in results]
	results_e = [r[1] for r in results]
	results_i = [r[2] for r in results]
	results_r = [r[3] for r in results]

	s, = plt.plot(time, results_s, color="orange")
	e, = plt.plot(time, results_e, color="green")
	i, = plt.plot(time, results_i, color="blue")
	r, = plt.plot(time, results_r, color="purple")

	ax = plt.gca()
	ax.legend((s, e, i, r), ("Susceptible", "Exposed", "Infectious", "Recovered"), loc="upper left")
	ax.set_title("SEIR")
	ax.set_xlabel("t")
	ax.set_ylabel("Number of people")

	plt.show()
