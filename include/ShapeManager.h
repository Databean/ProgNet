#ifndef SHAPE_MANAGER_H
#define SHAPE_MANAGER_H

#include <vector>
#include <map>
#include <iostream>
#include <stdexcept>
#include <string>
#include <ostream>
#include <sstream>
#include "PixelBuffer.h"
#include "Util.h"

/**
 * Represents the various types of shapes that can be drawn by the user.
 */
enum ShapeType { SHAPE_TRIANGLE, SHAPE_QUAD, SHAPE_POINT, SHAPE_LINE, SHAPE_POLYGON, SHAPE_TRIANGLE_FAN, SHAPE_NONE };

/**
 * Represents a coordinate in n-dimensional space, templated to have the type system enforce lengths
 * and to support integer and floating point coordinate systems.
 */
template<unsigned int dim, class CoordType>
class Point {
private:
	CoordType coords[dim];
public:
	typedef Point<dim, CoordType> Type;
	
	Point() {
		for(unsigned int i = 0; i < dim; i++) {
			coords[i] = 0;
		}
	}
	Point(const std::initializer_list<CoordType>& init) {
		if(init.size() == dim) {
			int index = 0;
			for(auto it = init.begin(); it != init.end(); it++) {
				coords[index] = *it;
				index++;
			}
		} else {
			throw std::runtime_error("Given initializer list has length " + toStr(init.size()) + " but this is a point of dimension " + toStr(dim));
		}
	}
	Point(std::istream& in) {
		in >> (*this);
	}
	Point(const Type& other) {
		(*this) = other;
	}
	~Point() {
		
	}
	inline const Type& operator=(const Type& other) {
		for(unsigned int i = 0; i < dim; i++) {
			coords[i] = other[i];
		}
		return *this;
	}
	
	inline CoordType& operator[](const unsigned int& i) {
		if(i < dim) {
			return coords[i];
		} else {
			throw std::runtime_error("Given index " + toStr(i) + " is less than the dimension " + toStr(dim));
		}
	}
	
	inline const CoordType& operator[](const unsigned int& i) const {
		if(i < dim) {
			return coords[i];
		} else {
			throw std::runtime_error("Given index " + toStr(i) + " is less than the dimension " + toStr(dim));
		}
	}
	
	inline Type operator+(const Type& other) const {
		Type ret;
		for(unsigned int i = 0; i < dim; i++) {
			ret[i] = coords[i] + other[i];
		}
		return ret;
	}
	
	inline Type operator+() const {
		return Type(*this);
	}
	
	inline const Type& operator+=(const Type& other) {
		for(unsigned int i = 0; i < dim; i++) {
			coords[i] += other[i];
		}
		return *this;
	}
	
	inline Type operator-(const Type& other) const {
		Type ret;
		for(unsigned int i = 0; i < dim; i++) {
			ret[i] = coords[i] - other[i];
		}
		return ret;
	}
	
	inline Type operator-() const {
		Type ret;
		for(unsigned int i = 0; i < dim; i++) {
			ret[i] = -(coords[i]);
		}
		return ret;
	}
	
	inline const Type& operator-=(const Type& other) {
		for(unsigned int i = 0; i < dim; i++) {
			coords[i] -= other[i];
		}
		return *this;
	}
	
	inline Type operator*(const CoordType& mod) const {
		Type ret;
		for(unsigned int i = 0; i < dim; i++) {
			ret[i] = coords[i] * mod;
		}
		return ret;
	}
	
	inline const Type& operator*=(const CoordType& mod) {
		for(unsigned int i = 0; i < dim; i++) {
			coords[i] *= mod;;
		}
		return *this;
	}
	
	inline Type operator/(const CoordType& mod) const {
		Type ret;
		for(unsigned int i = 0; i < dim; i++) {
			ret[i] = coords[i] / mod;
		}
		return ret;
	}
	
	inline const Type& operator/=(const CoordType& mod) {
		for(unsigned int i = 0; i < dim; i++) {
			coords[i] /= mod;;
		}
		return *this;
	}
	
	inline bool operator==(const Type& other) {
		for(unsigned int i = 0; i < dim; i++) {
			if(coords[i] != other[i]) {
				return false;
			}
		}
		return true;
	}
	
	inline bool operator!=(const Type& other) {
		for(unsigned int i = 0; i < dim; i++) {
			if(coords[i] != other[i]) {
				return true;
			}
		}
		return false;
	}
};

/**
 * Produces serialized output for an n-dimensional point.
 */
template<unsigned int dim, class CoordType>
inline std::ostream& operator<<(std::ostream& strm, const Point<dim, CoordType>& pt) {
	for(unsigned int i = 0; i < dim - 1; i++) {
		strm << pt[i] << " ";
	}
	return strm << pt[dim-1];
}

/**
 * Deserializes an n-dimensional point.
 */
template<unsigned int dim, class CoordType>
inline std::istream& operator>>(std::istream& in, Point<dim, CoordType>& pt) {
	for(unsigned int i = 0; i < dim; i++) {
		in >> pt[i];
	}
	return in;
}

/**
 * Convenience point types.
 */
typedef Point<2, double> Point2d;
typedef Point<2, float> Point2f;
typedef Point<2, int> Point2i;

typedef Point<3, double> Point3d;
typedef Point<3, float> Point3f;
typedef Point<3, int> Point3i;

/**
 * Produces a string representation of the various ShapeType enumeration members. Useful for debug output.
 */
inline std::string shapeName(const ShapeType& shape) {
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
inline bool validPointsInType(const ShapeType& shape, const unsigned int& numPoints) {
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

/**
 * 
 */
template<unsigned int dim, class CoordType>
class ShapeManager {
private:
	std::map<unsigned int, Shape<dim, CoordType>> shapes;
	unsigned int count;
public:
	typedef Point<dim, CoordType> PointType;
	typedef Shape<dim, CoordType> ShapeType;
	
	ShapeManager() : shapes(), count(0) {
		
	}
	ShapeManager(const ShapeManager& other) : shapes(other.shapes), count(other.count) {
		
	}
	~ShapeManager() {}
	ShapeManager& operator=(const ShapeManager& other) {
		shapes = other.shapes;
		count = other.count;
	}
	
	inline ShapeType& operator[](unsigned int index) {
		if(shapes.count(index)) {
			return shapes[index];
		} else {
			throw std::runtime_error("ShapeManager has no shape with index " + toStr(index));
		}
	}
	
	inline const ShapeType& operator[](unsigned int index) const {
		if(shapes.count(index)) {
			return shapes[index];
		} else {
			throw std::runtime_error("ShapeManager has no shape with index " + toStr(index));
		}
	}
	
	inline unsigned int numShapes() const {
		return shapes.size();
	}
	
	inline typename std::map<unsigned int, ShapeType>::iterator begin() {
		return shapes.begin();
	}
	
	inline typename std::map<unsigned int, ShapeType>::iterator end() {
		return shapes.end();
	}
	
	inline typename std::map<unsigned int, ShapeType>::const_iterator begin() const {
		return shapes.begin();
	}
	
	inline typename std::map<unsigned int, ShapeType>::const_iterator end() const {
		return shapes.end();
	}
	
	inline unsigned int makeQuad(const PointType& p1, const PointType& p2, const PointType& p3, const PointType& p4) {
		unsigned int index = count;
		shapes[index] = ShapeType(SHAPE_QUAD, {p1, p2, p3, p4});
		count++;
		return index;
	}
	
	inline unsigned int makeTriangle(const PointType& p1, const PointType& p2, const PointType& p3) {
		unsigned int index = count;
		shapes[index] = (ShapeType(SHAPE_TRIANGLE, {p1, p2, p3}));
		count++;
		return index;
	}
	
	inline unsigned int makeLine(const PointType& p1, const PointType& p2) {
		unsigned int index = count;
		shapes[index] = (ShapeType(SHAPE_LINE, {p1, p2}));
		count++;
		return index;
	}
	
	inline void deleteShape(unsigned int index) {
		shapes.erase(index);
	}
	
	inline void clear() {
		shapes.clear();
	}
};

typedef ShapeManager<2, double> ShapeManager2d;
typedef ShapeManager<2, float> ShapeManager2f;
typedef ShapeManager<2, int> ShapeManager2i;

#endif