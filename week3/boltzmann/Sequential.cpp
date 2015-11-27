//
// Created by Mathijs Molenaar on 27/11/15.
//

#include <math.h>
const int numDirections = 9;
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

template <typename T>
T equilibriumDistributionFunction(int i, T rho, T u[2])
{
    T weight = weights[i];
    T cs = 1.0 / sqrt(3);

    T dotProductDirU = directions[i*2] * u[0] + directions[i*2+1] * u[1];
    T dotProductUU = u[0]*u[0] + u[1]*u[1];
    T term1 = dotProductDirU / (cs * cs);
    T term2 = (dotProductDirU * dotProductDirU) / (2 * cs * cs * cs * cs);
    T term3 = dotProductUU / (2 * cs * cs);
    return weight * rho * (1 + term1 + term2 - term3);
}

template <typename T>
T calcRhoAndU(T* grid, int gridIndex, T& rho, T u[2])
{
    rho = 0.0;
    for (int i = 0; i < numDirections; i++)
    {
        T dist = grid[gridIndex + i];
        rho += dist;

        u[0] += directions[i*2] * dist;
        u[1] += directions[i*2+1] * dist;
    }

    u[0] /= rho;
    u[1] /= rho;
}

template <typename T>
void collide(T* grid, int width, int height, T tau)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            T rho;
            T u[2];
            int gridIndex = (y * width + x) * numDirections;
            calcRhoAndU<T>(grid, gridIndex, rho, u);

            for (int i = 0; i < numDirections; i++) {
                T eqDist = equilibriumDistributionFunction<T>(i, rho, u);
                T curDist = grid[gridIndex + i];
                grid[gridIndex + i] = curDist - (1.0 / tau) * (curDist - eqDist);
            }
        }
    }
}

template <typename T>
void stream(T* oldGrid, T* newGrid, int width, int height)
{
    for (int y = 1; y < height-1; y++)
    {
        for (int x = 1; x < width-1; x++)
        {
            for (int i = 0; i < numDirections; i++)
            {
                int index = (y * width + x) * numDirections + i;

                int fromX = x + directions[i * 2];
                int fromY = y + directions[i * 2 + 1];
                int fromIndex = (fromY * width + fromX) * numDirections + i;

                oldGrid[index] = newGrid[fromIndex];
            }
        }
    }
}

template <typename T>
void createTexture(T* grid, char* texture, int width, int height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            T sum = 0.0;
            for (int i = 0; i < numDirections; i++) {
                sum += grid[(y * width + x) * numDirections + i];
            }

            char value = char(sum * 255);
            texture[(y * width + x) * 3 + 0] = value;
        }
    }
}

/*template <typename T>
void boundaries(T* grid, int* boundariesGrid, int width, int height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = boundariesPositionToIndex(x, y);
            if (index == 0)
            {

            }
        }
    }
}*/