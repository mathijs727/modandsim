#pragma once
#include "BoltzmannGridD2Q9.h"

#include <string>
#include <FreeImage.h>

class ImageShape
{
private:
	int m_width, m_height;
	FIBITMAP* m_image;
public:
	ImageShape(const std::string& filepath);
	~ImageShape();

	void createBoundaries(
		BoltzmannGridD2Q9::BoundaryType* boundaries,
		int width,
		int height,
		int x,
		int y);
};

