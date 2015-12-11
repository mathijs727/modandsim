#include "BoltzmannGridD2Q9.h"
#include "Defines.h"

#include <iostream>
#include <cstring>
#include <math.h>
#include <thread>

#ifdef OPENMP_ENABLED
#include <omp.h>
#endif

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

BoltzmannGridD2Q9::BoltzmannGridD2Q9(real tau, int width, int height, BoundaryType* boundaries)
{
	m_tau = tau;
	m_width = width;
	m_height = height;
	m_size = width * height;
	m_curData = 0;

	m_data[0] = new real[m_size * 9];
	m_data[1] = new real[m_size * 9];
	m_boundaries = new BoundaryType[m_size];

	memcpy(m_boundaries, boundaries, sizeof(BoundaryType) * m_size);

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			for (int i = 0; i < 9; i++)
			{
				setValueCurrentGrid(x, y, i, 0.1);
			}
		}
	}
}

BoltzmannGridD2Q9::~BoltzmannGridD2Q9()
{
	delete[] m_data[0];
	delete[] m_data[1];
	delete[] m_boundaries;
}

void BoltzmannGridD2Q9::createTexture(char* texture)
{
	#pragma omp parallel for
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			BoundaryType boundary = getBoundaryType(x, y);
			if (boundary == NoBoundary)
			{
				real sumX = 0.0;
				real sumY = 0.0;
				for (int i = 0; i < 9; i++)
				{
					sumX += directions[i*2] * getValue(x, y, i);
					sumY += directions[i*2+1] * getValue(x, y, i);
				}

				real velocity = sqrt(sumX*sumX + sumY*sumY);
				char value = static_cast<char>(velocity * 400);
				if (velocity > 5.)
				{
					std::cout << "kggggggg" << std::endl;
					std::cout << "v = " << sqrt(sumX*sumX + sumY*sumY) << std::endl;
					exit(1);
				}
				texture[(y * m_width + x) * 4 + 0] = 0;
				texture[(y * m_width + x) * 4 + 1] = value;
				texture[(y * m_width + x) * 4 + 2] = value;
			} else if (boundary == BounceBackBoundary) {
				texture[(y * m_width + x) * 4 + 0] = (char)100;
				texture[(y * m_width + x) * 4 + 1] = (char)150;
				texture[(y * m_width + x) * 4 + 2] = (char)100;
			}
		}
	}
}

void BoltzmannGridD2Q9::collsionStep()
{
	#ifdef OPENMP_ENABLED
	#pragma omp parallel for
	#endif
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			if (getBoundaryType(x, y) == NoBoundary) {

				real rho;
				real u[2];
				calcRhoAndU(x, y, rho, u);

				for (int i = 0; i < 9; i++) {
					real eqDistr = equilibriumDistributionFunction(i, rho, u);
					real curDistr = getValue(x, y, i);
					real newDistr = curDistr - ((real) 1.0 / m_tau) * (curDistr - eqDistr);

					setValueCurrentGrid(x, y, i, newDistr);
				}
			}
		}
	}
}

void BoltzmannGridD2Q9::streamStep()
{
	#ifdef OPENMP_ENABLED
	#pragma omp parallel for
	#endif
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

					if (fromY == -1 || fromY == m_height)
					{
						setValueNewGrid(x, y, i, getValue(x, y, i));
					} else if (fromX == -1)
					{
						setValueNewGrid(x, y, i, 0.1);
					} else if (fromX == m_width)
					{
						setValueNewGrid(x, y, i, getValue(x, fromY, i));
					} else {
						BoundaryType boundary = getBoundaryType(fromX, fromY);
						if (boundary == NoBoundary) {
							setValueNewGrid(x, y, i, getValue(fromX, fromY, i));
						} else if (boundary == BounceBackBoundary)
						{
							setValueNewGrid(x, y, i, getValue(x, y, i));
						}
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