/**
 * surface.cpp
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

#include "surface.h"


using namespace SleekSurface;

bool SurfaceBuilder::getRowSegments(vector<Vec3> &inPoints, int inWidth, int inHeight, double c, vector<Segment> &segments)
{
    vector<Vec2> points(inWidth);
    segments.resize(inWidth * inHeight);
    int segPtr = 0;
    for (int z = 0; z < inHeight; ++z)
    {
        for (int x = 0; x < inWidth; ++x)
        {
            int idx = index(inWidth, x, z);
            points[x] = Vec2(inPoints[idx].x, inPoints[idx].y);
        }
        if (!CurveBuilder::build(points, &(segments[segPtr]), c))
            return false;
        segPtr += inWidth;
    }
    return true;
}

bool SurfaceBuilder::getColSegments(vector<Vec3> &inPoints, int inWidth, int inHeight, double c, vector<Segment> &segments)
{
    vector<Vec2> points(inHeight);
    segments.resize(inWidth * inHeight);
    int segPtr = 0;
    for (int x = 0; x < inWidth; ++x)
    {
        for (int z = 0; z < inHeight; ++z)
        {
            int idx = index(inWidth, x, z);
            points[z] = Vec2(inPoints[idx].z, inPoints[idx].y);
        }
        if (!CurveBuilder::build(points, &(segments[segPtr]), c))
            return false;
        segPtr += inHeight;
    }
    return true;
}

void SurfaceBuilder::triangulateGrid(int width, int height, vector<int> &indices)
{
    int n = (width - 1) * (height - 1) * 6;
    indices.resize(n);
    int i = 0;
    for (int z = 0; z < height - 1; ++z)
    {
        for (int x = 0; x < width - 1; ++x)
        {
            // TL --- TR
            //  |  __/ |
            //  | /    |
            // BL --- BR
            int tl = index(width, x, z);
            int tr = index(width, x + 1, z);
            int bl = index(width, x, z + 1);
            int br = index(width, x + 1, z + 1);
            // First triangle
            indices[i++] = tr;
            indices[i++] = tl;
            indices[i++] = bl;
            // Second triangle
            indices[i++] = bl;
            indices[i++] = br;
            indices[i++] = tr;
        }
    }
}

void SurfaceBuilder::computeNormals(vector<Vertex> &vertices, const vector<int> &indices)
{
    for (int i = 0, n = indices.size(); i < n; i += 3)
    {
        int n0 = indices[i];
        int n1 = indices[i + 1];
        int n2 = indices[i + 2];
        Vertex v0 = vertices[n0];
        Vertex v1 = vertices[n1];
        Vertex v2 = vertices[n2];
        Vec3 normal = Math::normal(v0.position, v1.position, v2.position);
        v0.normal = v0.normal + normal;
        v1.normal = v1.normal + normal;
        v2.normal = v2.normal + normal;
        v0.normal.normalize();
        v1.normal.normalize();
        v2.normal.normalize();
        vertices[n0] = v0;
        vertices[n1] = v1;
        vertices[n2] = v2;
    }
}

void SurfaceBuilder::smoothNormalsWithKernel(const vector<Vertex> &inVertices, int width, int height, vector<float> kernel, int radius, vector<Vertex> &outVertices)
{
    int n = radius * 2 + 1;
    outVertices = inVertices;
    for (int z = 0; z < height; ++z)
    {
        for (int x = 0; x < width; ++x)
        {
            Vec3 normal;
            for (int i = -radius; i < radius; ++i)
            {
                for (int j = -radius; j < radius; ++j)
                {
                    int ind = gridIndex(width, height, x + j, z + i);
                    if (ind > -1)
                    {
                        normal = normal + inVertices[ind].normal * (double)kernel[index(n, j + radius, i + radius)];
                    }
                }
            }
            normal.normalize();
            outVertices[index(width, x, z)].normal = normal;
        }
    }
}

bool SurfaceBuilder::build(vector<Vec3> &inPoints, int inWidth, int inHeight, int resolution, double c,
                           vector<Vertex> &outPoints, int &outWidth, int &outHeight)
{
    int n = inWidth * inHeight;

    if (inWidth < 2 || inHeight < 2 || inPoints.size() != n || resolution < 2)
        return false;

    vector<Segment> rowSegments;
    vector<Segment> colSegments;
    if (!getRowSegments(inPoints, inWidth, inHeight, c, rowSegments) || 
        !getColSegments(inPoints, inWidth, inHeight, c, colSegments))
        return false;
    
    --resolution;
    outWidth = resolution * (inWidth - 1) + 1;
    outHeight = resolution * (inHeight - 1) + 1;
    outPoints.resize(outWidth * outHeight);

    for (int z = 0; z < inHeight; ++z)
    {
        for (int x = 0; x < inWidth; ++x)
        {
            // What we have is Coons patch:
            //
            //  +-----> X (row)
            //  |                               pseg1
            //  |                 p00-------p01-------p02-------p03
            //  V                  |         |         |         |
            //  Z (col)            |         |         |         |
            //                     |         |   seg1  |         |
            //                    p10-------p11-------p12-------p13
            //                     |         |         |         |
            //               pseg2 |    seg2 |  COONS  | seg4    | pseg4
            //                     |         |         |         |
            //                    p20-------p21-------p22-------p23
            //                     |         |   seg3  |         |
            //                     |         |         |         |
            //                     |         |         |         |
            //                    p30-------p31-------p32-------p33
            //                                  pseg3
            //
            // p00..p33 are points from the input array.
            // p11, p12, p21, p22 are the points around the interpolation zone.
            // The surrounding points are needed for bicubic blending.
            // seg1..seg4 and pseg1..pseg4 are curve segments calculated above.
            // seg1, seg3, pseg1 and pseg3 are in rowSegments array and their indices correspond to the indices of
            // p11, p21, p01 and p31 respectively.
            // seg2, seg4, pseg2 and pseg4 are in colSegments array and their indices correspond to the transposed
            // indices of p11, p12, p10 and p13 respectively.
            //
            int p11 = gridIndex(inWidth, inHeight, x, z);
            int p12 = gridIndex(inWidth, inHeight, x + 1, z);
            int p21 = gridIndex(inWidth, inHeight, x, z + 1);
            int p22 = gridIndex(inWidth, inHeight, x + 1, z + 1);
            if (p11 >= 0 && p12 >= 0 && p21 >= 0 && p22 >= 0)
            {
                int seg1 = p11;
                int seg2 = gridIndexClamped(inHeight, inWidth, z, x); // Transposed p11.
                int seg3 = p21;
                int seg4 = gridIndexClamped(inHeight, inWidth, z, x + 1); // Transposed p12.

                int p00 = gridIndexClamped(inWidth, inHeight, x - 1, z - 1);
                int p01 = gridIndexClamped(inWidth, inHeight, x, z - 1);
                int p02 = gridIndexClamped(inWidth, inHeight, x + 1, z - 1);
                int p03 = gridIndexClamped(inWidth, inHeight, x + 2, z - 1);

                int p10 = gridIndexClamped(inWidth, inHeight, x - 1, z);
                int p13 = gridIndexClamped(inWidth, inHeight, x + 2, z);
                int p20 = gridIndexClamped(inWidth, inHeight, x - 1, z + 1);
                int p23 = gridIndexClamped(inWidth, inHeight, x + 2, z + 1);

                int p30 = gridIndexClamped(inWidth, inHeight, x - 1, z + 2);
                int p31 = gridIndexClamped(inWidth, inHeight, x, z + 2);
                int p32 = gridIndexClamped(inWidth, inHeight, x + 1, z + 2);
                int p33 = gridIndexClamped(inWidth, inHeight, x + 2, z + 2);

                int pseg1 = p01;
                int pseg2 = gridIndexClamped(inHeight, inWidth, z, x - 1); // Transposed p10.
                int pseg3 = p31;
                int pseg4 = gridIndexClamped(inHeight, inWidth, z, x + 2); // Transposed p13.

                double pValues[16] =
                {
                    inPoints[p00].y, inPoints[p01].y, inPoints[p02].y, inPoints[p03].y,
                    inPoints[p10].y, inPoints[p11].y, inPoints[p12].y, inPoints[p13].y,
                    inPoints[p20].y, inPoints[p21].y, inPoints[p22].y, inPoints[p23].y,
                    inPoints[p30].y, inPoints[p31].y, inPoints[p32].y, inPoints[p33].y
                };
                double aValues[16];
                Math::bicubicMatrix(pValues, aValues);

                for (int dx = 0; dx < resolution; ++dx)
                {
                    double t = (double)dx / (double)resolution;
                    Vec2 c1 = rowSegments[seg1].calc(t, true);
                    Vec2 c2 = rowSegments[seg3].calc(t, true);
                    for (int dz = 0; dz < resolution; ++dz)
                    {
                        if (dx == 0 && dz == 0)
                            outPoints[outIndex(outWidth, resolution, x, z, dx, dz)] = Vertex(inPoints[p11]);
                        else
                        {
                            double q = (double)dz / (double)resolution;
                            Vec2 c3 = colSegments[seg2].calc(q, true);
                            Vec2 c4 = colSegments[seg4].calc(q, true);

                            Vec2 r1 = rowSegments[pseg1].calc(t, true);
                            Vec2 r2 = rowSegments[pseg3].calc(t, true);
                            double ruledSurface1 = Math::cubicInterpolate(r1.y, c1.y, c2.y, r2.y, q);

                            r1 = colSegments[pseg2].calc(q, true);
                            r2 = colSegments[pseg4].calc(q, true);
                            double ruledSurface2 = Math::cubicInterpolate(r1.y, c3.y, c4.y, r2.y, t);

                            double biSurface = Math::bicubicInterpolate(aValues, q, t);

                            outPoints[outIndex(outWidth, resolution, x, z, dx, dz)] =
                                Vertex(Vec3(inPoints[p11].x + t * (inPoints[p12].x - inPoints[p11].x),
                                            ruledSurface1 + ruledSurface2 - biSurface,
                                            inPoints[p11].z + q * (inPoints[p21].z - inPoints[p11].z)));
                        }
                    }
                }
            }
            else if (p11 >= 0 && p12 >= 0 && p21 < 0 && p22 < 0)
            {
                int seg1 = p11;

                outPoints[outIndex(outWidth, resolution, x, z, 0, 0)] = Vertex(inPoints[p11]);
                for (int dx = 1; dx < resolution; ++dx)
                {
                    double t = (double)dx / (double)resolution;
                    Vec2 c1 = rowSegments[seg1].calc(t, true);
                    outPoints[outIndex(outWidth, resolution, x, z, dx, 0)] =
                        Vertex(Vec3(inPoints[p11].x + t * (inPoints[p12].x - inPoints[p11].x),
                                    c1.y,
                                    inPoints[p11].z + t * (inPoints[p12].z - inPoints[p11].z)));
                }
            }
            else if (p11 >= 0 && p21 >= 0 && p12 < 0 && p22 < 0)
            {
                int seg2 = gridIndexClamped(inHeight, inWidth, z, x); // Transposed p11.

                outPoints[outIndex(outWidth, resolution, x, z, 0, 0)] = Vertex(inPoints[p11]);
                for (int dz = 1; dz < resolution; ++dz)
                {
                    double t = (double)dz / (double)resolution;
                    Vec2 c1 = colSegments[seg2].calc(t, true);
                    outPoints[outIndex(outWidth, resolution, x, z, 0, dz)] =
                        Vertex(Vec3(inPoints[p11].x + t * (inPoints[p21].x - inPoints[p11].x),
                                    c1.y,
                                    inPoints[p11].z + t * (inPoints[p21].z - inPoints[p11].z)));
                }
            }
            else if (p11 >= 0 && p12 < 0 && p21 < 0 && p22 < 0)
            {
                outPoints[outIndex(outWidth, resolution, x, z, 0, 0)] = Vertex(inPoints[p11]);
            }
        }
    }

    return true;
}
