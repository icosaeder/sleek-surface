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

void printPLY(const vector<Vertex> &vertices)
{
    cout << "ply" << endl;
    cout << "format ascii 1.0" << endl;
    cout << "comment Created by sleek surface builder" << endl;
    cout << "element vertex " << vertices.size() << endl;
    cout << "property float x" << endl;
    cout << "property float y" << endl;
    cout << "property float z" << endl;
    cout << "element face 0" << endl;
    cout << "property list uchar uint vertex_indices" << endl;
    cout << "end_header" << endl;
    for (int i = 0, n = vertices.size(); i < n; ++i)
    {
        // Swap axes to view in Blender.
        cout << vertices[i].position.z << " " << vertices[i].position.x << " " << vertices[i].position.y << endl;
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

    vector<Vec3> points(w * h);
    for (int z = 0; z < h; ++z)
    {
        for (int x = 0; x < w; ++x)
            points[z * w + x] = Vec3(x, data[z][x], z);
    }

    vector<Vertex> vertices;
    int rw, rh;

    SurfaceBuilder::build(points, w, h, 17, 2.0, vertices, rw, rh);

    printPLY(vertices);

    return 0;
}
