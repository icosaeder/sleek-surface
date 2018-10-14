/**
 * curve.h
 *
 * This is a part of sleek-surface project.
 * This file provides data structures and functions to build sleek curves according to the discrete set of 2D points.
 * Read this for algorithm details: http://sv-journal.org/2017-1/04.php?lang=en
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

#ifndef __SLEEKSURFACE_CURVE_H__
#define __SLEEKSURFACE_CURVE_H__

#include "common.h"


namespace SleekSurface
{
    using namespace std;

    /**
     * The Segment class provides methods to store and calculate Bezier-based cubic curve segment.
     */
    class Segment
    {
    public:
        /**
         * Bezier control points.
         */
        Vec2 points[4];
        
        /**
         * Calculate the intermediate curve points.
         *
         * @param t - parameter of the curve, should be in [0; 1].
         * @param regularize - flag determining if result should be calculated in a way that x-coordinate is 
         * linearly interpolated to ensure the segment subdivision points are distributed uniformly in equal
         * distance from each other (true), or both coordinates should be interpolated cubically.
         * @return intermediate Bezier curve point that corresponds the given parameter.
         */
        Vec2 calc(double t, bool regularize) const
        {
            if (regularize)
            {
                // We solve this by t to find out parameter giving regular grid:
                // x0 + t0 (x3 - x0) = (1 - t)^3 x0 + 3 t (1 - t)^2 x1 + 3 t^2 (1 - t) x2 + t^3 x3.
                double a = -points[0].x + 3.0 * (points[1].x - points[2].x) + points[3].x;
                double b = 3.0 * (points[0].x - 2.0 * points[1].x + points[2].x);
                double c = 3.0 * (-points[0].x + points[1].x);
                double d = t * (points[0].x - points[3].x);
                double roots[3];
                int rn = Math::solveCubicEq(a, b, c, d, roots);
                if (rn > 0)
                {
                    double nearestRoot = roots[0];
                    for (int i = 1; i < rn; ++i)
                    {
                        if (roots[i] > 0.0 && roots[i] < 1.0 && abs(t - roots[i]) < abs(t - nearestRoot))
                            nearestRoot = roots[i];
                    }
                    if (nearestRoot > 0.0 && nearestRoot < 1.0)
                        t = nearestRoot;
                    else
                        rn = 0;
                }
                if (rn == 0)
                {
                    double t2 = t * t;
                    double t3 = t2 * t;
                    double nt = 1.0 - t;
                    double nt2 = nt * nt;
                    double nt3 = nt2 * nt;
                    return Vec2(points[0].x + t * (points[3].x - points[0].x),
                                nt3 * points[0].y + 3.0 * t * nt2 * points[1].y + 3.0 * t2 * nt * points[2].y + t3 * points[3].y);
                }
            }

            double t2 = t * t;
            double t3 = t2 * t;
            double nt = 1.0 - t;
            double nt2 = nt * nt;
            double nt3 = nt2 * nt;
            return Vec2(nt3 * points[0].x + 3.0 * t * nt2 * points[1].x + 3.0 * t2 * nt * points[2].x + t3 * points[3].x,
                        nt3 * points[0].y + 3.0 * t * nt2 * points[1].y + 3.0 * t2 * nt * points[2].y + t3 * points[3].y);
        };
    };

    /**
     * The CurveBuilder class provides methods to create sleek curves.
     */
    class CurveBuilder
    {
    public:
        /**
         * Build an interpolation curve with smoothness order 0 based on cubic Bezier according to given point set.
         * This is a clone of tbezierSO0 from https://github.com/icosaeder/tbezier
         *
         * @param values - input array of points to interpolate.
         * @param curve - pointer to the preallocated output array of curve segments.
         * @param c - paramenet affecting curvature, should be in [2; +inf).
         * @return true if interpolation successful, false if not.
         */
        static bool build(const vector<Vec2> &values, Segment *curve, double c = 2.0);
    };
}

#endif // __SLEEKSURFACE_CURVE_H__
