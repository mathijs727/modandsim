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
const double weights[] = {
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

BoltzmannGrid2D9Q::BoltzmannGrid2D9Q(real tau, int width, int height, real* data, BoundaryType* boundaries)
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

BoltzmannGrid2D9Q::~BoltzmannGrid2D9Q()
{
	delete[] m_data[0];
	delete[] m_data[1];
	delete[] m_boundaries;
}

void BoltzmannGrid2D9Q::createTexture(char* texture)
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

				char value = char(sum * 255);
				texture[(y * m_width + x) * 4 + 0] = 0;
				texture[(y * m_width + x) * 4 + 1] = value;
				texture[(y * m_width + x) * 4 + 2] = 0;
			} else {
				texture[(y * m_width + x) * 4 + 0] = 255;
				texture[(y * m_width + x) * 4 + 1] = 0;
				texture[(y * m_width + x) * 4 + 2] = 0;
			}
		}
	}
}

void BoltzmannGrid2D9Q::collsionStep()
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			real rho;
			real u[2];
			calcRhoAndU(x, y, rho, u);

			for (int i = 0; i < 9; i++) {
				real eqDistr = equilibriumDistributionFunction(i, rho, u);
				real curDistr = getValue(x, y, i);
				real newDistr = curDistr - (1.0 / m_tau) * (curDistr - eqDistr);
				setValueCurrentGrid(x, y, i, newDistr);
			}
		}
	}
}

void BoltzmannGrid2D9Q::streamStep()
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			for (int i = 0; i < 9; i++)
			{
				int fromX = x - directions[i * 2];
				int fromY = y - directions[i * 2 + 1];
				
				if (fromX >= 0 && fromX < m_width && fromY >= 0 && fromY < m_height)
				{
					real val = getValue(fromX, fromY, i);
					setValueNewGrid(x, y, i, val);
				}
				else {
					setValueNewGrid(x, y, i, 0.0);
				}
			}
		}
	}
	m_curData = (m_curData + 1) % 2;
}

void BoltzmannGrid2D9Q::boundaryStep()
{
	for (int y = 1; y < m_height - 1; y++)
	{
		for (int x = 1; x < m_width - 1; x++)
		{
			BoundaryType type = getBoundaryType(x, y);
			if (type == BounceBackBoundary)
			{
				setValueCurrentGrid(x, y, 1, getValue(x, y, 5));
				setValueCurrentGrid(x, y, 2, getValue(x, y, 6));
				setValueCurrentGrid(x, y, 3, getValue(x, y, 7));
				setValueCurrentGrid(x, y, 4, getValue(x, y, 8));
				setValueCurrentGrid(x, y, 5, getValue(x, y, 1));
				setValueCurrentGrid(x, y, 6, getValue(x, y, 2));
				setValueCurrentGrid(x, y, 7, getValue(x, y, 3));
				setValueCurrentGrid(x, y, 8, getValue(x, y, 4));
			}
		}
	}
}

BoltzmannGrid2D9Q::real BoltzmannGrid2D9Q::equilibriumDistributionFunction(int i, float rho, real u[2])
{
	real weight = weights[i];
	real cs = 1.0 / sqrt(3);

	real dotProductDirU = directions[i * 2] * u[0] + directions[i * 2 + 1] * u[1];
	real dotProductUU = u[0] * u[0] + u[1] * u[1];
	real term1 = 3 * dotProductDirU / (cs * cs);
	real term2 = 9 * (dotProductDirU * dotProductDirU) / (2 * cs * cs * cs * cs);
	real term3 = 3 * dotProductUU / (2 * cs * cs);
	return weight * rho * (1 + term1 + term2 - term3);
}

void BoltzmannGrid2D9Q::calcRhoAndU(int x, int y, real& rho, real u[2])
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

	u[0] /= rho;
	u[1] /= rho;
}