#include "Shape.h"

#include "Util.h"

/**
 * Produces a string representation of the various ShapeType enumeration members. Useful for debug output.
 */
std::string shapeName(const ShapeType& shape) {
    switch(shape) {
        case SHAPE_TRIANGLE_FAN:
            return "Triangle Fan";
            break;
        case SHAPE_POLYGON:
            return "Polygon";
            break;
        case SHAPE_QUAD:
            return "Quad";
            break;
        case SHAPE_TRIANGLE:
            return "Triangle";
            break;
        case SHAPE_LINE:
            return "Line";
            break;
        case SHAPE_POINT:
            return "Point";
            break;
        case SHAPE_NONE:
            return "None";
            break;
    }
    throw std::runtime_error("ShapeType " + toStr(shape) + " does not have a toString");
}

/**
 * Determines whether the given number of points is reasonable for the given ShapeType.
 */
bool validPointsInType(const ShapeType& shape, const unsigned int& numPoints) {
    switch(shape) {
        case SHAPE_TRIANGLE_FAN:
            return numPoints > 2;
            break;
        case SHAPE_POLYGON:
            return numPoints > 1;
            break;
        case SHAPE_QUAD:
            return numPoints == 4;
            break;
        case SHAPE_TRIANGLE:
            return numPoints == 3;
            break;
        case SHAPE_LINE:
            return numPoints == 2;
            break;
        case SHAPE_POINT:
            return numPoints == 1;
            break;
        case SHAPE_NONE:
            return numPoints == 0;
            break;
    }
    throw std::runtime_error("Unknown type of shape: " + shapeName(shape));
}