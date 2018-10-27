/**
 * common.h
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

#ifndef __SLEEKSURFACE_COMMON_H__
#define __SLEEKSURFACE_COMMON_H__

#include <vector>
#include <iostream>
#include <cmath>


namespace SleekSurface
{
    using namespace std;

    class Vec3;

    /**
     * The Math static class provides handy operations and constants.
     */
    class Math
    {
    public:
        /**
         * Threshold for zero.
         */
        constexpr static const double EPSILON = 1.0e-5;

        /**
         * One third.
         */
        constexpr static const double ONETHIRD = 1.0 / 3.0; 

        /**
         * Test if real value is zero.
         *
         * @param v - value to test.
         * @return true if value should be treated as zero, false if not.
         */
        static bool isZero(double v) { return abs(v) < EPSILON; };
        
        /**
         * Signum function.
         *
         * @param v - value to check the sign of.
         * @return -1 if value is negative, 0 if zero and 1 if positive.
         */
        static int sign(double v) { return v > EPSILON ? 1 : (v < -EPSILON ? -1 : 0); };
        
        /**
         * Create matrix for bicubic interpolation.
         *
         * @param p - input values to interpolate, 16 components of 4x4 matrix.
         * @param a - output values of the matrix, 16 components of 4x4 matrix.
         */
        static void bicubicMatrix(double *p, double *a);

        /**
         * Cubic interpolation.
         *
         * @oaram p0, p1, p2, p3 - points to interpolate.
         * @param u - interpolation quotient.
         * @return interpolation result.
         */
        static double cubicInterpolate(double p0, double p1, double p2, double p3, double u);

        /**
         * Bicubic interpolation.
         *
         * @param a - bicubic interpolation matrix created by <code>bicubicMatrix</code> call.
         * @param u - horizontal interpolation quotient.
         * @param v - vertical interpolation quotient.
         * @return interpolation result.
         */
        static double bicubicInterpolate(double *a, double u, double v); 

        /**
         * Solve in real numbers cubic equation in the form
         * ax^3 + bx^2 + cx + d = 0.
         *
         * @param a - quotient by x^3.
         * @param b - quotient by x^2.
         * @param c - quitient by x.
         * @param d - free member.
         * @param roots - array to store found roots. It has to have 3 elements allocated.
         * @return number of real roots found.
        */
        static int solveCubicEq(double a, double b, double c, double d, double *roots);

        /**
         * Calculate Gaussian function.
         *
         * @param x - value to calculate Gaussian function for.
         * @param mu, sigma - Gaussian function parameters.
         * @return Gaussian function for given values.
         */
        static double gaussian(double x, double mu, double sigma)
        {
            const double a = (x - mu) / sigma;
            return exp(-0.5 * a * a);
        };

        /**
         * Compute Gaussian kernel for given radius.
         *
         * @param radius - radius of the kernel.
         * @param shouldNormalize - flag determining if the kernel should be normalized (true) or not (false).
         * @param kernel - array to store the kernel.
         */
        static void calcGaussianKernel(int radius, bool shouldNormalize, vector<float> &kernel);
        /**
         * Calculate plane normal for given unequal 3 points.
         * 
         * @param a, b, c - vertices on plane
         * @return normal to plane for given points
         */
        static Vec3 normal(const Vec3 &a, const Vec3 &b, const Vec3 &c);
    };

    /**
     * The Vec2 class provides methods to store and handle 2D vectors and points.
     */
    class Vec2
    {
    public:
        /**
         * Vector coordinates.
         */
        double x, y;
        
        /**
         * Vec2 constructor.
         */
        Vec2() : x(0.0), y(0.0) {};
        /**
         * Vec2 constructor.
         *
         * @param _x - x coordinate of the vector.
         * @param _y - y coordinate of the vector.
         */
        Vec2(double _x, double _y) : x(_x), y(_y) {};
        
        /**
         * Add other vector to the current one.
         *
         * @param v - vector to add.
         * @return sum of the current vector and the given one.
         */
        Vec2 operator +(const Vec2 &v) const { return Vec2(x + v.x, y + v.y); };
        /**
         * Subtract other vector from the current one.
         *
         * @param v - vector to subtract.
         * @return difference of the current vector and the given one.
         */
        Vec2 operator -(const Vec2 &v) const { return Vec2(x - v.x, y - v.y); };
        /**
         * Multiply current vector by the real value.
         *
         * @param v - value to multiply by.
         * @return current vector multiplied by the given value.
         */
        Vec2 operator *(double v) const { return Vec2(x * v, y * v); };
        
        /**
         * Safely normalize current vector.
         */
        void normalize()
        {
            double l = sqrt(x * x + y * y);
            if (Math::isZero(l))
                x = y = 0.0;
            else
            {
                x /= l;
                y /= l;
            }
        };

        /**
         * Get the absolute minimum of two given vectors.
         *
         * @param v1 - first vector.
         * @param v2 - second vector.
         * @return absolute minimum of the given vectors' coordinates.
         */
        static Vec2 absMin(const Vec2 &v1, const Vec2 &v2)
        {
            return Vec2(abs(v1.x) < abs(v2.x) ? v1.x : v2.x, abs(v1.y) < abs(v2.y) ? v1.y : v2.y);
        };
    };

    /**
     * The Vec3 class provides methods to store and handle 3D vectors and points.
     */
    class Vec3
    {
    public:
        /**
         * Vector coordinates.
         */
        double x, y, z;
        
        /**
         * Vec3 constructor.
         */
        Vec3() : x(0.0), y(0.0), z(0.0) {};
        /**
         * Vec3 constructor.
         *
         * @param _x - x coordinate of the vector.
         * @param _y - y coordinate of the vector.
         * @param _z - z coordinate of the vector.
         */
        Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};
        /**
         * Vec3 copy constructor.
         *
         * @param vec - vector to copy.
         */
        Vec3(const Vec3 &vec) : x(vec.x), y(vec.y), z(vec.z) {};
        
        /**
         * Add other vector to the current one.
         *
         * @param v - vector to add.
         * @return sum of the current vector and the given one.
         */
        Vec3 operator +(const Vec3 &v) const { return Vec3(x + v.x, y + v.y, z + v.z); };
        /**
         * Subtract other vector from the current one.
         *
         * @param v - vector to subtract.
         * @return difference of the current vector and the given one.
         */
        Vec3 operator -(const Vec3 &v) const { return Vec3(x - v.x, y - v.y, z - v.z); };
        /**
         * Multiply current vector by the real value.
         *
         * @param v - value to multiply by.
         * @return current vector multiplied by the given value.
         */
        Vec3 operator *(double v) const { return Vec3(x * v, y * v, z * v); };
        
        /**
         * Safely normalize current vector.
         */
        void normalize()
        {
            double l = sqrt(x * x + y * y + z * z);
            if (Math::isZero(l))
                x = y = z = 0.0;
            else
            {
                x /= l;
                y /= l;
                z /= l;
            }
        };

        /**
         * Cross product of two vectors.
         * 
         * @param v1 - left vector in product.
         * @param v2 - right vector in product.
         * @return cross product of given vectors.
         */
        static Vec3 cross(const Vec3 &v1, const Vec3 &v2)
        {
            return Vec3(v1.y * v2.z - v1.z * v2.y,
                        v1.z * v2.x - v1.x * v2.z,
                        v1.x * v2.y - v1.y * v2.x);
        }
    };

    /**
     * The Vertex class provides methods to store and handle surface points with point normals.
     */
    class Vertex
    {
    public:
        /**
         * Vertex position.
         */
        Vec3 position;
        /**
         * Vertex normal.
         */
        Vec3 normal;

        /**
         * Vertex constructor.
         */
        Vertex() {};

        /**
         * Vertex constructor.
         *
         * @param pos - position vector. Normal is zero.
         */
        Vertex(const Vec3 &pos) : position(pos) {};
    };
}

#endif // __SLEEKSURFACE_COMMON_H__
