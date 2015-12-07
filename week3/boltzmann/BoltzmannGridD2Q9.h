#pragma once
#include <math.h>
#include <iostream>

typedef float real;

class BoltzmannGridD2Q9
{
public:
	enum BoundaryType
	{
		NoBoundary,
		BounceBackBoundary,
		SlipBoundary
	};
private:
	real m_tau;
	int m_width, m_height, m_size;
	int m_curData;
	real* m_data[2];
	BoundaryType* m_boundaries;
public:
	BoltzmannGridD2Q9(real tau, int width, int height, real* data, BoundaryType* boundaries);
	~BoltzmannGridD2Q9();
	void createTexture(char* texture);
	void collsionStep();
	void streamStep();
private:
	real equilibriumDistributionFunction(int i, float rho, real u[2]);
	void calcRhoAndU(int x, int y, real& rho, real u[2]);

	inline real getValue(int x, int y, int i)
	{
		return m_data[m_curData][i * m_size + y * m_width + x];
	}

	inline void setValueCurrentGrid(int x, int y, int i, real value)
	{
		m_data[m_curData][i * m_size + y * m_width + x] = value;
	}
	
	inline void setValueNewGrid(int x, int y, int i, real value)
	{
		m_data[(m_curData + 1) % 2][i * m_size + y * m_width + x] = value;
	}

	inline BoundaryType getBoundaryType(int x, int y)
	{
		return m_boundaries[y * m_width + x];
	}
};
