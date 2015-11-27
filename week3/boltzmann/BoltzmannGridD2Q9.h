#pragma once

class BoltzmannGrid2D9Q
{
public:
	static enum BoundaryType
	{
		NoBoundary,
		BounceBackBoundary,
		SlipBoundary
	};
private:
	typedef float real;

	real m_tau;
	int m_width, m_height, m_size;
	int m_curData;
	real* m_data[2];
	BoundaryType* m_boundaries;
public:
	BoltzmannGrid2D9Q(real tau, int width, int height, real* data, BoundaryType* boundaries);
	~BoltzmannGrid2D9Q();
	void createTexture(char* texture);
	void collsionStep();
	void streamStep();
	void boundaryStep();
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
