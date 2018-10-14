/**
 * curve.cpp
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

#include "curve.h"


using namespace SleekSurface;

bool CurveBuilder::build(const vector<Vec2> &values, Segment *curve, double c)
{
    int n = values.size() - 1;
    
    if (n < 2)
        return false;

    Vec2 cur, next, tgL, tgR, deltaC;
    double l1, l2, tmp, x;
    bool zL, zR;
    
    next = values[1] - values[0];
    next.normalize();
    
    for (int i = 0; i < n; ++i)
    {
        tgL = tgR;
        cur = next;
        
        deltaC = values[i + 1] - values[i];
        
        if (i < n - 1)
        {
            next = values[i + 2] - values[i + 1];
            next.normalize();
            tgR = cur + next;
            tgR.normalize();
        }
        else
        {
            tgR.x = tgR.y = 0.0;
        }
        
        if (Math::sign(tgL.x) != Math::sign(deltaC.x))
            tgL.x = 0.0;
        if (Math::sign(tgL.y) != Math::sign(deltaC.y))
            tgL.y = 0.0;
        if (Math::sign(tgR.x) != Math::sign(deltaC.x))
            tgR.x = 0.0;
        if (Math::sign(tgR.y) != Math::sign(deltaC.y))
            tgR.y = 0.0;
        
        zL = Math::isZero(tgL.x);
        zR = Math::isZero(tgR.x);
        
        l1 = zL ? 0.0 : deltaC.x / (c * tgL.x);
        l2 = zR ? 0.0 : deltaC.x / (c * tgR.x);
        
        if (abs(l1 * tgL.y) > abs(deltaC.y))
            l1 = Math::isZero(tgL.y) ? 0.0 : deltaC.y / tgL.y;
        if (abs(l2 * tgR.y) > abs(deltaC.y))
            l2 = Math::isZero(tgR.y) ? 0.0 : deltaC.y / tgR.y;
        
        if (!zL && !zR)
        {
            tmp = tgL.y / tgL.x - tgR.y / tgR.x;
            if (!Math::isZero(tmp))
            {
                x = (values[i + 1].y - tgR.y / tgR.x * values[i + 1].x - values[i].y + tgL.y / tgL.x * values[i].x) / tmp;
                if (x > values[i].x && x < values[i + 1].x)
                {
                    if (abs(l1) > abs(l2))
                        l1 = 0.0;
                    else
                        l2 = 0.0;
                }
            }
        }

        curve[i].points[0] = values[i];
        curve[i].points[1] = curve[i].points[0] + tgL * l1;
        curve[i].points[3] = values[i + 1];
        curve[i].points[2] = curve[i].points[3] - tgR * l2;
    }
    
    return true;
}
