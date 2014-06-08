#include "Shape.h"

#include "Util.h"

bostream& operator<<(bostream& out, ShapeType s) {
	return out << (int) s;
}

bistream& operator>>(bistream& in, ShapeType& s) {
	return in >> (int&) s;
}

/**
 * Produces a string representation of the various ShapeType enumeration members. Useful for debug output.
 */
std::string shapeName(const ShapeType& shape) {
    switch(shape) {
        case ShapeType::SHAPE_TRIANGLE_FAN:
            return "Triangle Fan";
            break;
        case ShapeType::SHAPE_POLYGON:
            return "Polygon";
            break;
        case ShapeType::SHAPE_QUAD:
            return "Quad";
            break;
        case ShapeType::SHAPE_TRIANGLE:
            return "Triangle";
            break;
        case ShapeType::SHAPE_LINE:
            return "Line";
            break;
        case ShapeType::SHAPE_POINT:
            return "Point";
            break;
        case ShapeType::SHAPE_NONE:
            return "None";
            break;
    }
    throw std::runtime_error("ShapeType " + toStr((int)shape) + " does not have a toString");
}

/**
 * Determines whether the given number of points is reasonable for the given ShapeType.
 */
bool validPointsInType(const ShapeType& shape, const unsigned int& numPoints) {
    switch(shape) {
        case ShapeType::SHAPE_TRIANGLE_FAN:
            return numPoints > 2;
            break;
        case ShapeType::SHAPE_POLYGON:
            return numPoints > 1;
            break;
        case ShapeType::SHAPE_QUAD:
            return numPoints == 4;
            break;
        case ShapeType::SHAPE_TRIANGLE:
            return numPoints == 3;
            break;
        case ShapeType::SHAPE_LINE:
            return numPoints == 2;
            break;
        case ShapeType::SHAPE_POINT:
            return numPoints == 1;
            break;
        case ShapeType::SHAPE_NONE:
            return numPoints == 0;
            break;
    }
    throw std::runtime_error("Unknown type of shape: " + shapeName(shape));
}