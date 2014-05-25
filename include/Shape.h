#ifndef SHAPE_H
#define SHAPE_H

#include <stdexcept>
#include <vector>

#include "Point.h"
#include "PixelBuffer.h"

/**
 * Represents the various types of shapes that can be drawn by the user.
 */
enum ShapeType { SHAPE_TRIANGLE, SHAPE_QUAD, SHAPE_POINT, SHAPE_LINE, SHAPE_POLYGON, SHAPE_TRIANGLE_FAN, SHAPE_NONE };

std::string shapeName(const ShapeType& shape);

bool validPointsInType(const ShapeType& shape, const unsigned int& numPoints);

/**
 * Represents a Shape drawn to the screen. These tend to correspond with OpenGL primitives.
 */
template<unsigned int dim, class CoordType>
class Shape {
private:
	std::vector<Point<dim, CoordType>> points;
	Point<dim, CoordType> off;
	float rot;
	Color col;
	ShapeType ty;
	
	template<class A, unsigned int B>
	friend std::istream& operator>>(std::istream&, Shape&);
public:
	typedef Shape<dim, CoordType> Type;
	typedef Point<dim, CoordType> PointType;
	
	inline Shape() : points(), off(), rot(0.f), ty(SHAPE_NONE) {
		
	}
	
	inline Shape(const ShapeType& type, const std::vector<PointType>& points) : points(points), off(), rot(0.f), ty(type) {
		if(!validPointsInType(type, points.size())) {
			throw std::runtime_error("Invalid number of points in " + shapeName(type) + " shape: " + toStr(points.size()));
		}
	}
	
	inline Shape(const Type& other) : points(other.points), off(other.off), rot(other.rot), col(other.col), ty(other.ty) {
		
	}
	
	inline ~Shape() {
		
	}
	
	inline const Type& operator=(const Type& other) {
		points = other.points;
		ty = other.ty;
		return *this;
	}
	
	inline PointType& operator[](const unsigned int& index) {
		if(index < points.size()) {
			return points[index];
		} else {
			throw std::runtime_error("Invalid point access in " + shapeName(ty) + ": tried to get point " + toStr(index) + " but there are only " + toStr(points.size()) + " points.");
		}
	}
	
	inline const PointType& operator[](const unsigned int& index) const {
		if(index < points.size()) {
			return points[index];
		} else {
			throw std::runtime_error("Invalid point access in " + shapeName(ty) + ": tried to get point " + toStr(index) + " but there are only " + toStr(points.size()) + " points.");
		}
	}
	
	inline unsigned int numPoints() const {
		return points.size();
	}
	
	inline PointType& offset() {
		return off;
	}
	
	inline const PointType& offset() const {
		return off;
	}
	
	inline float& rotation() {
		return rot;
	}
	
	inline const float& rotation() const {
		return rot;
	}
	
	inline Color& color() {
		return col;
	}
	
	inline const Color& color() const {
		return col;
	}
	
	inline const ShapeType& type() const {
		return ty;
	}
};

/**
 * Convenience typedefs for 2-dimensional shapes of various coordinate types.
 */
typedef Shape<2, double> Shape2d;
typedef Shape<2, float> Shape2f;
typedef Shape<2, int> Shape2i;

/**
 * Serializes a shape.
 */
template<unsigned int dim, class CoordType>
std::ostream& operator<<(std::ostream& out, const Shape<dim, CoordType>& shape) {
	out << shape.numPoints() << " ";
	for(unsigned int i = 0; i < shape.numPoints(); i++) {
		out << shape[i] << " ";
	}
	return out << shape.type();
}

/**
 * Deserializes a shape.
 */
template<unsigned int dim, class CoordType>
std::istream& operator>>(std::istream& in, const Shape<dim, CoordType>& shape) {
	unsigned int numPoints;
	in >> numPoints;
	shape.points.clear();
	for(unsigned int i = 0; i < numPoints; i++) {
		shape.points.push_back(Shape<dim, CoordType>());
		in >> shape[i];
	}
	return in >> shape.ty;
}

#endif