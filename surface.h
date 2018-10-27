/**
 * surface.h
 *
 * This is a part of sleek-surface project.
 * This file provides data structures and functions to build sleek surface according to the discrete set of 3D points.
 * Read this for algorithm details: <URL>
 *
 * Written by Konstantin Ryabinin under terms of MIT license.
 *
 * The MIT License (MIT)
 * Copyright (c) 2018 Konstantin Ryabinin
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial 
 * portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __SLEEKSURFACE_SURFACE_H__
#define __SLEEKSURFACE_SURFACE_H__

#include "curve.h"


namespace SleekSurface
{
    using namespace std;

    /**
     * The SurfaceBuilder class provides methods to create sleek surfaces.
     */
    class SurfaceBuilder
    {
        static bool getRowSegments(vector<Vec3> &inPoints, int inWidth, int inHeight, double c, vector<Segment> &segments);
        static bool getColSegments(vector<Vec3> &inPoints, int inWidth, int inHeight, double c, vector<Segment> &segments);
        inline static int index(int w, int x, int z);
        inline static int gridIndex(int w, int h, int x, int z);
        inline static int gridIndexClamped(int w, int h, int x, int z);
        inline static int outIndex(int w, int r, int x, int z, int dx, int dz);

    public:
        /**
         * Build a surface.
         *
         * @param inPoints - regular grid of 3D points to create surface according.
         * @param inWidth, inHeight - resolution of input grid.
         * @param resolution - resolution of each coons patch.
         * For each 4 points of input grid, r^2 - 4 new points are emitted.
         * @param c - paramenet affecting curvature, should be in [2; +inf).
         * @param outPoints - regular grid of 3D points representing the sleek surface.
         * @param outWidth, outHeight - resolution of output grid.
         * @return true if surface building successful, false if not.
         */
        static bool build(vector<Vec3> &inPoints, int inWidth, int inHeight, int resolution, double c,
                          vector<Vertex> &outPoints, int &outWidth, int &outHeight);
        /**
         * Build a triangle mesh from regular grid.
         * 
         * @param w, h - resolution of input grid.
         * @param indices - result vector of indices.
         */
        static void triangulateGrid(int w, int h, vector<int> &indices);
        /**
         * Compute vertex normals using algorithm of smoothing groups.
         * 
         * @param vertices - input vertices
         * @param indices - vector of triangle indices
         */
        static void computeNormals(vector<Vertex> &vertices, const vector<int> &indices);
        /**
         * Smooth vertex normals using gaussian kernel.
         * 
         * @param inVertices - regular grid of 3D points.
         * @param width, height - resolution of input grid.
         * @param kernel - matrix of gaussian kernel coefficients.
         * @param radius - radius of applying kernel.
         * @param outVertices - updated vertices with smoothed normals.
         */
        static void smoothNormalsWithKernel(const vector<Vertex> &inVertices, int width, int height, vector<float> kernel, int radius, vector<Vertex> &outVertices);
    };

    int SurfaceBuilder::index(int w, int x, int z)
    {
        return z * w + x;
    }

    int SurfaceBuilder::gridIndex(int w, int h, int x, int z)
    {
        return x < w && z < h ? index(w, x, z) : -1;
    }

    int SurfaceBuilder::gridIndexClamped(int w, int h, int x, int z)
    {
        if (x < 0)
            x = 0;
        else if (x > w - 1)
            x = w - 1;
        if (z < 0)
            z = 0;
        else if (z > h - 1)
            z = h - 1;
        return index(w, x, z);
    }

    int SurfaceBuilder::outIndex(int w, int r, int x, int z, int dx, int dz)
    {
        return (z * r + dz) * w + (x * r + dx);
    }
}

#endif // __SLEEKSURFACE_SURFACE_H__
