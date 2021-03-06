//
//  Geometry.hpp
//  FuzzySystemTest
//
//  Created by Alfred Yu on 2018/4/9.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp

#include <cstdio>
#include <string>
#include "AllConstant.hpp"
#include "Angle.hpp"

using namespace std;

#ifndef POINT
#define POINT
typedef struct stPoint
{
    float x, y;
    
    stPoint()
    {
        x = CAR_INIT_POSITION_X;
        y = CAR_INIT_POSITION_Y;
    }
    
    stPoint(float dx, float dy)
    {
        x = dx;
        y = dy;
    }

}Point, Coordinate;
#endif

typedef struct stGeometry
{
private:
    Point ptIntersection;
public:
    bool checkIntersect(Point ptStart, Point ptEnd, Point car, Degree carAngle)
    {
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
            //
            // m1(x-x1')+y1' = m2(x-x2')+y2'  ->  m1x-m1x1'+y1' = m2x-m2x2'+y2'
            // ->  (m1-m2)x = m1x1' - m2x2' - y1' +y2'
            // ->  x = (m1x1' - m2x2' - y1' + y2') / (m1-m2)

            float m_line = (ptEnd.y - ptStart.y) / (ptEnd.x - ptStart.x);
            float m_car = tan(carAngle.getRadians());

            if ( ptEnd.x == ptStart.x && carAngle.getDegree() == 90.0 )
            {
                return false;
            }
            else if ( ptEnd.x == ptStart.x )
            {
                // border is vertical, car is not
                float X = ptStart.x;
                float Y = m_car*(X - car.x) + car.y;

                if ( Y > max(ptStart.y, ptEnd.y) || Y < min(ptStart.y, ptEnd.y) )
                    return false;

                ptIntersection.x = X;
                ptIntersection.y = Y;
                return true;
            }
            else if ( carAngle.getDegree() == 90.0 )
            {
                // the car is vertical, border is not
                float X = car.x;
                float Y = m_line*(X - ptStart.x) + ptStart.y;

                if ( X > max(ptStart.x, ptEnd.x) || X < min(ptStart.x, ptEnd.x) )
                    return false;

                ptIntersection.x = X;
                ptIntersection.y = Y;
                return true;
            }
            else if ( ptStart.y == ptEnd.y )
            {
                float X = (m_line*ptStart.x - m_car*car.x - ptStart.y + car.y) / (m_line - m_car);
                float Y = ptStart.y;

                if ( X <= max(ptStart.x, ptEnd.x) && X >= min(ptStart.x, ptEnd.x) )
                {
                    ptIntersection.x = X;
                    ptIntersection.y = Y;
                    return true;
                }
            }
            else
            {
                float X = (m_line*ptStart.x - m_car*car.x - ptStart.y + car.y) / (m_line - m_car);
                float Y = m_car*(X - car.x) + car.y;

                if ( X <= max(ptStart.x, ptEnd.x) && X >= min(ptStart.x, ptEnd.x) &&
                     Y <= max(ptStart.y, ptEnd.y) && Y >= min(ptStart.y, ptEnd.y)   )
                {
                    ptIntersection.x = X;
                    ptIntersection.y = Y;
                    return true;
                }
            }

            return false;
    }

    Point getIntersection()
    {
        return ptIntersection;
    }

    float getDistance(Point pt1, Point pt2)
    {
        return sqrt(pow(pt2.x-pt1.x, 2.0) + pow(pt2.y-pt1.y, 2.0));
    }

    // https://stackoverflow.com/a/6853926
    float minDistanceFromPointToLine(float ptX, float ptY, float line1X, float line1Y, float line2X, float line2Y)
    {
        float A = ptX - line1X;
        float B = ptY - line1Y;
        float C = line2X - line1X;
        float D = line2Y - line1Y;

        float dot = A * C + B * D;
        float len_sq = C * C + D * D;
        float param = -1;

        //in case of 0 length line
        if (len_sq != 0)
            param = dot / len_sq;

        float xx, yy;

        if (param < 0)
        {
            xx = line1X;
            yy = line1Y;
        }
        else if (param > 1)
        {
            xx = line2X;
            yy = line2Y;
        }
        else
        {
            xx = line1X + param * C;
            yy = line1Y + param * D;
        }

        float dx = ptX - xx;
        float dy = ptY - yy;

        return sqrt(dx * dx + dy * dy);
    }

}Geometry;

#endif /* Geometry_hpp */
