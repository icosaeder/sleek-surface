/**
 * common.cpp
 *
 * This is a part of sleek-surface project.
 * This file provides basic data structures and functions.
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

#include "common.h"


using namespace SleekSurface;

void Math::bicubicMatrix(double *p, double *a)
{
    a[0] = p[5];
    a[1] = -0.5 * p[4] + 0.5 * p[6];
    a[2] = p[4] - 2.5 * p[5] + 2.0 * p[6] - 0.5 * p[7];
    a[3] = -0.5 * p[4] + 1.5 * p[5] - 1.5 * p[6] + 0.5 * p[7];
    a[4] = -0.5 * p[1] + 0.5 * p[9];
    a[5] = 0.25 * p[0] - 0.25 * p[2] - 0.25 * p[8] + 0.25 * p[10];
    a[6] = -0.5 * p[0] + 1.25 * p[1] - p[2] + 0.25 * p[3] + 0.5 * p[8] - 1.25 * p[9] + p[10] - 0.25 * p[11];
    a[7] = 0.25 * p[0] - 0.75 * p[1] + 0.75 * p[2] - 0.25 * p[3] - 0.25 * p[8] + 0.75 * p[9] - 0.75 * p[10] + 0.25 * p[11];
    a[8] = p[1] - 2.5 * p[5] + 2.0 * p[9] - 0.5 * p[13];
    a[9] = -0.5 * p[0] + 0.5 * p[2] + 1.25 * p[4] - 1.25 * p[6] - p[8] + p[10] + 0.25 * p[12] - 0.25 * p[14];
    a[10] = p[0] - 2.5 * p[1] + 2.0 * p[2] - 0.5 * p[3] - 2.5 * p[4] + 6.25 * p[5] - 5.0 * p[6] + 1.25 * p[7] + 2.0 * p[8] - 5.0 * p[9] + 4.0 * p[10] - p[11] - 0.5 * p[12] + 1.25 * p[13] - p[14] + 0.25 * p[15];
    a[11] = -0.5 * p[0] + 1.5 * p[1] - 1.5 * p[2] + 0.5 * p[3] + 1.25 * p[4] - 3.75 * p[5] + 3.75 * p[6] - 1.25 * p[7] - p[8] + 3.0 * p[9] - 3.0 * p[10] + p[11] + 0.25 * p[12] - 0.75 * p[13] + 0.75 * p[14] - 0.25 * p[15];
    a[12] = -0.5 * p[1] + 1.5 * p[5] - 1.5 * p[9] + 0.5 * p[13];
    a[13] = 0.25 * p[0] - 0.25 * p[2] - 0.75 * p[4] + 0.75 * p[6] + 0.75 * p[8] - 0.75 * p[10] - 0.25 * p[12] + 0.25 * p[14];
    a[14] = -0.5 * p[0] + 1.25 * p[1] - p[2] + 0.25 * p[3] + 1.5 * p[4] - 3.75 * p[5] + 3.0 * p[6] - 0.75 * p[7] - 1.5 * p[8] + 3.75 * p[9] - 3.0 * p[10] + 0.75 * p[11] + 0.5 * p[12] - 1.25 * p[13] + p[14] - 0.25 * p[15];
    a[15] = 0.25 * p[0] - 0.75 * p[1] + 0.75 * p[2] - 0.25 * p[3] - 0.75 * p[4] + 2.25 * p[5] - 2.25 * p[6] + 0.75 * p[7] + 0.75 * p[8] - 2.25 * p[9] + 2.25 * p[10] - 0.75 * p[11] - 0.25 * p[12] + 0.75 * p[13] - 0.75 * p[14] + 0.25 * p[15];
}

double Math::cubicInterpolate(double p0, double p1, double p2, double p3, double u)
{
    return p1 + 0.5 * u * (p2 - p0 + u * (2.0 * p0 - 5.0 * p1 + 4.0 * p2 - p3 + u * (3.0 * (p1 - p2) + p3 - p0)));
}

double Math::bicubicInterpolate(double *a, double u, double v)
{
    double u2 = u * u;
    double u3 = u2 * u;
    double v2 = v * v;
    double v3 = v2 * v;

    return ((a[0] + a[1] * v + a[2] * v2 + a[3] * v3) +
            (a[4] + a[5] * v + a[6] * v2 + a[7] * v3) * u +
            (a[8] + a[9] * v + a[10] * v2 + a[11] * v3) * u2 +
            (a[12] + a[13] * v + a[14] * v2 + a[15] * v3) * u3);
}

int Math::solveCubicEq(double a, double b, double c, double d, double *roots)
{
    if (abs(a) > EPSILON)
    {
        // Canonical form: x^3 + ax^2 + bx + d = 0.
        // Solve by Cardan formula.
        double z = a;
        a = b / z;
        b = c / z;
        c = d / z;

        double p = b - a * a / 3.0;
        double q = a * (2.0 * a * a - 9.0 * b) / 27.0 + c;
        double p3 = p * p * p;
        double D = q * q + 4.0 * p3 / 27.0;
        double offset = -a / 3.0;
        if (D > EPSILON)
        {
            // Positive discriminant.
            z = sqrt(D);
            double u = (-q + z) / 2.0;
            double v = (-q - z) / 2.0;
            u = u >= 0.0 ? pow(u, ONETHIRD) : -pow(-u, ONETHIRD);
            v = v >= 0.0 ? pow(v, ONETHIRD) : -pow(-v, ONETHIRD);
            roots[0] = u + v + offset;
            return 1;
        }
        else if (D < -EPSILON)
        {
            // Negative discriminant.
            double u = 2.0 * sqrt(-p / 3.0);
            double v = acos(-sqrt(-27.0 / p3) * q / 2.0) / 3.0;
            roots[0] = u * cos(v) + offset;
            roots[1] = u * cos(v + 2.0 * M_PI / 3.0) + offset;
            roots[2] = u * cos(v + 4.0 * M_PI / 3.0) + offset;
            return 3;
        }
        else
        {
            // Zero discriminant.
            double u = 0.0;
            if (q < 0.0)
                u = pow(-q / 2.0, ONETHIRD);
            else
                u = -pow(q / 2.0, ONETHIRD);
            roots[0] = 2.0 * u + offset;
            roots[1] = -u + offset;
            return 2;
        }
    }
    else
    {
        // Canonical from: ax^2 + bx + c = 0.
        // Solve via discriminant.
        a = b;
        b = c;
        c = d;
        if (abs(a) <= EPSILON)
        {
            if (abs(b) <= EPSILON)
                return 0;
            else
            {
                roots[0] = -c / b;
                return 1;
            }
        }
        double D = b * b - 4.0 * a * c;
        if (D <= -EPSILON)
            return 0;
        if (D > EPSILON)
        {
            // Positive discriminant.
            D = sqrt(D);
            roots[0] = (-b - D) / (2.0 * a);
            roots[1] = (-b + D) / (2.0 * a);
            return 2;
        }
        else if (D < -EPSILON)
        {
            // Negative discriminant.
            return 0;
        }
        else
        {
            // Zero discriminant.
            roots[0] = -b / (2.0 * a);
            return 1;
        }
    }
}

void Math::calcGaussianKernel(int radius, bool shouldNormalize, vector<float> &kernel)
{
    const double sigma = (double)radius / 2.0;
    const double mu = (double)radius;
    float sum = 0.0;
    int n = radius * 2 + 1;
    kernel.resize(n * n);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            float x = (float)(gaussian((double)i, mu, sigma) * gaussian((double)j, mu, sigma));
            kernel[i * n + j] = x;
            sum += x;
        }
    }
    if (shouldNormalize)
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
                kernel[i * n + j] /= sum;
        }
    }
}

Vec3 Math::normal(const Vec3 &a, const Vec3 &b, const Vec3 &c)
{
    return Vec3::cross(b - a, c - a);
}
