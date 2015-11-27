//
// Created by Mathijs Molenaar on 27/11/15.
//

#ifndef BOLTZMANN_DATASTRUCTURES_H_H
#define BOLTZMANN_DATASTRUCTURES_H_H
class BoltzmannGrid2D9Q
{
private:
    static enum BoundaryTypes
    {
        BounceBackBoundary,
        SlipBoundary
    };

    int width, height;
    float** data;
    BoundaryTypes* boundaries;
private:
    inline float getValue(int x, int y, int i)
    {
        return data[(y * width + x) * 9 + i];
    }

    inline BoundaryTypes getBoundaryType(int x, int y)
    {
        return boundaries[y * width + x];
    }
};
#endif //BOLTZMANN_DATASTRUCTURES_H_H
