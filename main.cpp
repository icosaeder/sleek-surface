/**
 * main.cpp
 *
 * This is a part of sleek-surface project.
 * This file provides main function to test the sleeksurf library as well as test data set.
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
using namespace std;

void printOBJ(const vector<Vertex> &vertices, const vector<int> &indices)
{
    cout << "# Testing the sleek-surface library" << endl << endl;
    cout << "# " << vertices.size() << " vertex positions" << endl;
    for (int i = 0, n = vertices.size(); i < n; ++i)
    {
        cout << "v " <<
            vertices[i].position.x << " " <<
            vertices[i].position.y << " " <<
            vertices[i].position.z << endl;
    }
    cout << endl << "# " << vertices.size() << " vertex normals" << endl;
    for (int i = 0, n = vertices.size(); i < n; ++i)
    {
        cout << "vn " <<
            vertices[i].normal.x << " " <<
            vertices[i].normal.y << " " <<
            vertices[i].normal.z << endl;
    }
    cout << endl << "# Mesh with " << indices.size() / 3 << " faces" << endl;
    cout << "o sleek-surface" << endl;
    for (int i = 0, n = indices.size(); i < n; i += 3)
    {
        cout << "f " <<
            indices[i] + 1 << "//" << indices[i] + 1 << " " <<
            indices[i + 1] + 1 << "//" << indices[i + 1] + 1 << " " <<
            indices[i + 2] + 1 << "//" << indices[i + 2] + 1 << endl;
    }
}

int main(int argc, char **argv)
{
    const int w = 7;
    const int h = 6;
    const double data[6][7] =
    {
        { 1.321, 1.657, 1.165, 1.215, 1.623, 1.236, 1.657 },
        { 1.265, 1.654, 1.154, 1.165, 1.153, 1.648, 1.654 },
        { 1.324, 1.264, 1.547, 1.125, 1.246, 1.465, 1.264 },
        { 1.165, 1.654, 1.125, 1.154, 1.315, 1.135, 1.654 },
        { 1.157, 1.654, 1.165, 1.300, 1.136, 1.168, 1.654 },
        { 1.215, 1.658, 1.184, 1.156, 1.163, 1.185, 1.658 }
    };
    const double c = 2.0;
    const int resolution = 17;
    const int kernelRadius = 17 / 5;

    vector<Vec3> points(w * h);
    for (int z = 0; z < h; ++z)
    {
        for (int x = 0; x < w; ++x)
            points[z * w + x] = Vec3(x, data[z][x], z);
    }

    vector<Vertex> vertices;
    vector<int> indices;
    vector<float> gaussianKernel;
    vector<Vertex> smoothedVertices;
    int rw, rh;

    SurfaceBuilder::build(points, w, h, resolution, c, vertices, rw, rh);
    SurfaceBuilder::triangulateGrid(rw, rh, indices);
    SurfaceBuilder::computeNormals(vertices, indices);
    Math::calcGaussianKernel(kernelRadius, false, gaussianKernel);
    SurfaceBuilder::smoothNormalsWithKernel(vertices, rw, rh, gaussianKernel, kernelRadius, smoothedVertices);

    printOBJ(smoothedVertices, indices);

    return 0;
}
