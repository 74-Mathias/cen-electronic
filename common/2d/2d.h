#ifndef TWOD_H
#define TWOD_H

#include "../../common/io/outputStream.h"

/**
 * Define a Point structure.
 */
typedef struct Point {
    float x;
    float y;
} Point;

/**
 * Structures to define the position with angle.
 */
typedef struct {
    /** The position of the robot. */
    Point pos;
    /** The orientation of the robot. */
    float orientation;
    /** The initial orientation of the robot. */
    float initialOrientation;
} Position;


/**
 * Computes the distance between 2 points.
 * @param p0 the first point
 * @param p1 the second point
 * @return the distance between 2 points
 */
float distanceBetweenPoints(Point* p0, Point* p1);

/**
 * Computes the distance between 2 points.
 * @param x0 the value of Point 0 along Axis x
 * @param y0 the value of Point 0 along Axis y
 * @param x1 the value of Point 1 along Axis x
 * @param y1 the value of Point 1 along Axis y
 * @return the distance between 2 points
 */
float distanceBetweenPoints2(float x0, float y0, float x1, float y1);

/**
 * Computes the angle in radian of the vector defined by P0->P1 (between -PI and PI)
 * @param p0 the first point
 * @param p1 the second point
 * @return the angle in radian of the vector defined by P0->P1
 */
float angleOfVector(Point* p0, Point* p1);

/**
* Scale a coordinates system by applying a factor for x and y.
* @param point the point that we want to transform
* @param factor the scale factor
*/
void scale(Point *point, float factor);

/**
* Computes a rotation around point (x, y).
* @param point the result point
* @param angleInRadians the angle of the rotation
* @param x the coordinates along x axis of the center of the rotation
* @param y the coordinates along y axis of the center of the rotation
*/
void rotate(Point* resultPoint, float angleInRadians, float x, float y);

// PRINT Functions

/**
 * Print a point value (x / y) with the unit.
 * @param outputStream the stream in which we write the content of a point
 * @param point the point that we want to write
 * @param unit the unit for the coordinates
 */
void printPoint(OutputStream* outputStream, Point* point, const char* unit);

/**
 * Append a string, and a value in radians converted to deg.
 * @param outputStream the stream in which we write the content of a point
 * @param valueName the name of the value
 * @param angleInRadians the angle in radians that we want to write.
 */
void appendStringAndAngleInDeg(OutputStream* outputStream, const char* valueName, float angleInRadians);

#endif
