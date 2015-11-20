import numpy as np
import matplotlib.pyplot as plt

from differential_solvers import *

deathrate = 0.0
birthrate = 0.0
spread_rate = 50 * 0.04
incubation_rate = 1.0 / 2
recovery_rate = 1.0 / 3

# https://en.wikipedia.org/wiki/Compartmental_models_in_epidemiology#The_SEIR_model
def SEIR(time, state):
	total = np.sum(state)
	
	S = state[0]
	E1 = state[1]
	E2 = state[2]
	E3 = state[3]
	I1 = state[4]
	I2 = state[5]
	I3 = state[6]
	R = state[7]

	I = I1 + I2 + I3

	SE1 = spread_rate * (I / total) * S

	return np.array([
		birthrate * total - deathrate * S - SE1,
		SE1 - incubation_rate * E1 - deathrate * E1,
		incubation_rate * E1 - incubation_rate * E2 - deathrate * E2,
		incubation_rate * E2 - incubation_rate * E3 - deathrate * E3,
		incubation_rate * E3 - recovery_rate * I1 - deathrate * I1,
		recovery_rate * I1 - recovery_rate * I2 - deathrate * I2,
		recovery_rate * I2 - recovery_rate * I3 - deathrate * I3,
		recovery_rate * I3 - deathrate * R
	])

if __name__ == "__main__":
	stepsize = 0.25

	# Susceptible, Exposed, Infected, Recovered
	start = np.array([999.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
	time = np.arange(0, 100, stepsize)

	results = solve_for_list(start, time, SEIR)

	results_s = [r[0] for r in results]
	results_e1 = [r[1] for r in results]
	results_e2 = [r[2] for r in results]
	results_e3 = [r[3] for r in results]
	results_i1 = [r[4] for r in results]
	results_i2 = [r[5] for r in results]
	results_i3 = [r[6] for r in results]
	results_r = [r[7] for r in results]

	s, = plt.plot(time, results_s, color="orange")
	e1, = plt.plot(time, results_e1, color="green")
	e2, = plt.plot(time, results_e2, color="pink")
	e3, = plt.plot(time, results_e3, color="yellow")
	i1, = plt.plot(time, results_i1, color="blue")
	i2, = plt.plot(time, results_i2, color="red")
	i3, = plt.plot(time, results_i3, color="black")
	r, = plt.plot(time, results_r, color="purple")

	ax = plt.gca()
	ax.legend(
		(s, e1, e2, e3, i1, i2, i3, r),
		("Susceptible", "Exposed 1", "Exposed 2", "Exposed 3", "Infected 1", "Infected 2", "Infected 3", "Recovered"),
		loc="upper left")
	ax.set_title("time")
	ax.set_xlabel("t")
	ax.set_ylabel("x(t), y(t) and z(t)")

	plt.show()