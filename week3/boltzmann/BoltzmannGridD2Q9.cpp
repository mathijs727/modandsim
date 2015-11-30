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

				if (sum > 1.0f)
				{
					std::cout << "Value in (" << x << ", " << y << ") = " << sum << std::endl;
				}

				char value = char(sum * 120);
				texture[(y * m_width + x) * 4 + 0] = 0;
				texture[(y * m_width + x) * 4 + 1] = 20;
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
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
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

				setValueCurrentGrid(x, y, i, newDistr);
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

void BoltzmannGridD2Q9::boundaryStep()
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			BoundaryType type = getBoundaryType(x, y);
			if (type == BounceBackBoundary)
			{
				for (int i = 0; i < 9; i++)
				{
					real value = getValue(x, y, i);
					int moveToX = x - directions[i * 2];
					int moveToY = y - directions[i * 2 + 1];
					int newDir = 1 + (i + 3) % 8;
					setValueCurrentGrid(moveToX, moveToY, newDir, value);
				}
			}
		}
	}
}

BoltzmannGridD2Q9::real BoltzmannGridD2Q9::equilibriumDistributionFunction(int i, float rho, real u[2])
{
	real weight = static_cast<real>(weights[i]);
	real csSqr = static_cast<real>(1.0 / 3.0);

	real dotProductDirU = directions[i * 2] * u[0] + directions[i * 2 + 1] * u[1];
	real dotProductUU = u[0] * u[0] + u[1] * u[1];
	real term1 = dotProductDirU / (csSqr);
	real term2 = (dotProductDirU * dotProductDirU) / (2 * csSqr * csSqr);
	real term3 = dotProductUU / (2 * csSqr);
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