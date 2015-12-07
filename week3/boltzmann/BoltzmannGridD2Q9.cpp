#include "BoltzmannGridD2Q9.h"

#include <iostream>
#include <cstring>
#include <math.h>

const int directions[] = {
	0, 0,
	1, 0,
	1, 1,
	0, 1,
	-1, 1,
	-1, 0,
	-1, -1,
	0, -1,
	1, -1
};
const real weights[] = {
	4. / 9.,
	1. / 9.,
	1. / 36.,
	1. / 9.,
	1. / 36.,
	1. / 9.,
	1. / 36.,
	1. / 9.,
	1. / 36.
};

BoltzmannGridD2Q9::BoltzmannGridD2Q9(real tau, int width, int height, real* data, BoundaryType* boundaries)
{
	m_tau = tau;
	m_width = width;
	m_height = height;
	m_size = width * height;
	m_curData = 0;

	m_data[0] = new real[m_size * 9];
	m_data[1] = new real[m_size * 9];
	m_boundaries = new BoundaryType[m_size];

	memcpy(m_data[0], data, sizeof(real) * m_size * 9);
	memcpy(m_boundaries, boundaries, sizeof(BoundaryType) * m_size);
}

BoltzmannGridD2Q9::~BoltzmannGridD2Q9()
{
	delete[] m_data[0];
	delete[] m_data[1];
	delete[] m_boundaries;
}

void BoltzmannGridD2Q9::createTexture(char* texture)
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			if (getBoundaryType(x, y) == NoBoundary)
			{
				real sum = 0.0;
				for (int i = 0; i < 9; i++) {
					sum += getValue(x, y, i);
				}

				if (sum < 0.0f && sum > 1.0f)
				{
					std::cout << "Value in (" << x << ", " << y << ") = " << sum << std::endl;
#ifdef _WIN32
					system("PAUSE");
#endif
					exit(1);
				}

				char value = char(sum * 150);
				texture[(y * m_width + x) * 4 + 0] = 0;
				texture[(y * m_width + x) * 4 + 1] = 0;
				texture[(y * m_width + x) * 4 + 2] = value;
			} else {
				texture[(y * m_width + x) * 4 + 0] = (char)255;
				texture[(y * m_width + x) * 4 + 1] = 0;
				texture[(y * m_width + x) * 4 + 2] = 0;
			}
		}
	}
}

void BoltzmannGridD2Q9::collsionStep()
{
	for (int y = 1; y < m_height - 1; y++)
	{
		for (int x = 1; x < m_width - 1; x++)
		{
			if (getBoundaryType(x, y) != NoBoundary)
			{
				continue;
			}
			
			real rho;
			real u[2];
			calcRhoAndU(x, y, rho, u);

			real currentDistrSum = 0.0;
			real newDistrSum = 0.0;
			for (int i = 0; i < 9; i++) {
				real eqDistr = equilibriumDistributionFunction(i, rho, u);
				real curDistr = getValue(x, y, i);
				real newDistr = curDistr - ((real)1.0 / m_tau) * (curDistr - eqDistr);

				currentDistrSum += curDistr;
				newDistrSum += newDistr;
				
				if (newDistr < 0.0f)
				{
					std::cout << "Location: " << x << ", " << y << std::endl;
					std::cout << "i = " << i << std::endl;
					std::cout << "Current distribution: " << curDistr << std::endl;
					std::cout << "Equilibrium distribution: " << eqDistr << std::endl;
					std::cout << "New distribution: " << newDistr << std::endl;
					std::cout << "U: (" << u[0] << ", " << u[1] << ")" << std::endl;
					std::cout << "Rho: " << rho << std::endl;
#ifdef _WIN32
					system("PAUSE");
#endif
					exit(1);
				}

				setValueCurrentGrid(x, y, i, newDistr);
			}
			
			if (fabsf(currentDistrSum - newDistrSum) > 0.0001f)
			{
				std::cout << "Collision step created new particles" << std::endl;
				std::cout << "Old distribution sum: " << currentDistrSum << std::endl;
				std::cout << "New distribution sum: " << newDistrSum << std::endl;
				std::cout << "Location: (" << x << ", " << y << ")" << std::endl;
#ifdef _WIN32
				system("PAUSE");
#endif
				exit(1);
			}
		}
	}
}

void BoltzmannGridD2Q9::streamStep()
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			if (getBoundaryType(x, y) == NoBoundary)
			{
				for (int i = 0; i < 9; i++)
				{
					int fromX = x - directions[i * 2];
					int fromY = y - directions[i * 2 + 1];

					if (fromX >= 0 && fromX < m_width && fromY >= 0 && fromY < m_height && getBoundaryType(fromX, fromY) == NoBoundary)
					{
						setValueNewGrid(x, y, i, getValue(fromX, fromY, i));
					}
					else {
						setValueNewGrid(x, y, i, getValue(x, y, i));
					}
				}
			}
		}
	}
	m_curData = (m_curData + 1) % 2;
}

real BoltzmannGridD2Q9::equilibriumDistributionFunction(int i, float rho, real u[2])
{
	real weight = static_cast<real>(weights[i]);

	real dotProductDirU = directions[i * 2] * u[0] + directions[i * 2 + 1] * u[1];
	real dotProductUU = u[0] * u[0] + u[1] * u[1];
	real term1 = 3 * dotProductDirU;
	real term2 = (real)4.5 * dotProductDirU * dotProductDirU;
	real term3 = (real)1.5 * dotProductUU;
	return weight * rho * (1 + term1 + term2 - term3);
}

void BoltzmannGridD2Q9::calcRhoAndU(int x, int y, real& rho, real u[2])
{
	rho = 0.0;
	u[0] = 0.0;
	u[1] = 0.0;

	for (int i = 0; i < 9; i++)
	{
		real value = getValue(x, y, i);
		rho += value;

		u[0] += directions[i * 2] * value;
		u[1] += directions[i * 2 + 1] * value;
	}

	if (rho > 0.0) {
		u[0] /= rho;
		u[1] /= rho;
	} else {
		u[0] = 0.0;
		u[1] = 0.0;
	}
}