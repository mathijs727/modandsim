#include "ImageShape.h"

ImageShape::ImageShape(const std::string& filepath)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filepath.c_str(), 0);
	m_image = FreeImage_Load(format, filepath.c_str());

	if (FreeImage_GetBPP(m_image) != 32)
	{
		FIBITMAP* oldImage = m_image;
		m_image = FreeImage_ConvertTo32Bits(oldImage);
		FreeImage_Unload(oldImage);
	}

	m_width = FreeImage_GetWidth(m_image);
	m_height = FreeImage_GetHeight(m_image);
}

ImageShape::~ImageShape()
{
	FreeImage_Unload(m_image);
}

void ImageShape::createBoundaries(
	BoltzmannGridD2Q9::BoundaryType* boundaries,
	int gridWidth,
	int gridHeight,
	int locX,
	int locY)
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			RGBQUAD color;
			FreeImage_GetPixelColor(m_image, x, y, &color);
			if (color.rgbGreen > 5)
			{
				boundaries[(locY + y) * gridWidth + (locX + x)] = BoltzmannGridD2Q9::BounceBackBoundary;
			}
		}
	}
}
