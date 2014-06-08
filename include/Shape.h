#ifndef SHAPE_H
#define SHAPE_H

#include <stdexcept>
#include <vector>
#include <functional>
#include <iostream>

#include "Point.h"
#include "PixelBuffer.h"

template<unsigned int dim, class CoordType>
class ShapeObserver;

/**
 * Represents the various types of shapes that can be drawn by the user.
 */
enum class ShapeType : int { SHAPE_TRIANGLE = 0, SHAPE_QUAD, SHAPE_POINT, SHAPE_LINE, SHAPE_POLYGON, SHAPE_TRIANGLE_FAN, SHAPE_NONE };

bostream& operator<<(bostream& out, ShapeType);
bistream& operator>>(bistream& in, ShapeType&);

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
	//ShapeObserver<dim, CoordType>* observer;
	
	template<class A, unsigned int B>
	friend std::istream& operator>>(std::istream&, Shape&);
public:
	typedef Shape<dim, CoordType> Type;
	typedef Point<dim, CoordType> PointType;
	
	inline Shape() : rot(0.f), ty(ShapeType::SHAPE_NONE)/*, observer(nullptr) */ {
		
	}
	
	inline Shape(const ShapeType& type, const std::vector<PointType>& points) : points(points), off(), rot(0.f), ty(type) /*, observer(nullptr) */{
		if(!validPointsInType(type, points.size())) {
			throw std::runtime_error("Invalid number of points in " + shapeName(type) + " shape: " + toStr(points.size()));
		}
	}
	
	inline Shape(const Type& other) = default;
	inline ~Shape() = default;
	inline Type& operator=(const Type& other) = default;
	
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
	
	inline void setPoint(const unsigned int& index, const PointType& newPoint) {
		(*this)[index] = newPoint;
		update();
	}
	
	inline unsigned int numPoints() const {
		return points.size();
	}
	
	inline void setNumPoints(int size) {
		if(validPointsInType(ty, size)) {
			points.resize(size);
		} else {
			throw std::runtime_error("Invalid number of points in " + shapeName(type) + " shape: " + toStr(points.size()));
		}
	}
	
	inline PointType& offset() {
		return off;
	}
	
	inline void setOffset(const PointType& newOffset) {
		off = newOffset;
		update();
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
	
	inline void setRotation(float newRotation) {
		rot = newRotation;
		update();
	}
	
	inline Color& color() {
		return col;
	}
	
	inline const Color& color() const {
		return col;
	}
	
	inline void setColor(const Color& newColor) {
		col = newColor;
		update();
	}
	
	inline const ShapeType& type() const {
		return ty;
	}
	
	inline void update() {
		/*if(observer) {
			observer->update(*this);
		}*/
	}
	
	inline bool operator==(const Shape<dim, CoordType>& other) const {
		if(numPoints() != other.numPoints()) {
			return false;
		}
		for(unsigned int i = 0; i < numPoints(); i++) {
			if((*this)[i] != other[i]) {
				return false;
			}
		}
		return rotation() == other.rotation() && color() == other.color() && type() == other.type();
	}
	
	inline void setObserver(ShapeObserver<dim, CoordType>* observer) {
		//this->observer = observer;
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
	out << "Shape(" << shape.numPoints() << " ";
	for(unsigned int i = 0; i < shape.numPoints(); i++) {
		out << shape[i] << " ";
	}
	return out << shapeName(shape.type()) << ", " << shape.offset() << ", " << shape.rotation() << ", " << shape.color() << ")";
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

template<unsigned int dim, class CoordType>
bostream& operator<<(bostream& out, const Shape<dim, CoordType>& shape) {
	out << shape.numPoints();
	out << shape.type();
	for(unsigned int i = 0; i < shape.numPoints(); i++) {
		out << shape[i];
	}
	return out << shape.offset() << shape.rotation() << shape.color();
}

template<unsigned int dim, class CoordType>
bistream& operator>>(bistream& in, Shape<dim, CoordType>& shape) {
	ShapeType type;
	std::vector<Point<dim, CoordType>> points;
	unsigned int size;
	in >> size >> type;
	std::cout << shapeName(type) << ": " << size << std::endl;
	for(std::size_t i = 0; i < size; i++) {
		Point<dim, CoordType> point;
		in >> point;
		points.push_back(point);
	}
	Shape<dim, CoordType> newShape(type, points);
	in >> newShape.offset();
	in >> newShape.rotation();
	in >> newShape.color();
	shape = newShape;
	return in;
}

template<unsigned int dim, class CoordType>
class ShapeObserver {
public:
	ShapeObserver() = default;
	ShapeObserver(const ShapeObserver&) = delete;
	virtual ~ShapeObserver() = default;
	virtual ShapeObserver& operator=(const ShapeObserver&) = delete;
	
	virtual void update(const Shape<dim, CoordType>& shape) = 0;
};

template<unsigned int dim, class CoordType>
class ShapeObserverFn : public ShapeObserver<dim, CoordType> {
private:
	std::function<void(const Shape<dim, CoordType>&)> fn;
public:
	ShapeObserverFn(std::function<void(const Shape<dim, CoordType>&)> fn) : fn(fn) {}
	ShapeObserverFn(const ShapeObserverFn*) = delete;
	virtual ~ShapeObserverFn() = default;
	virtual ShapeObserverFn& operator=(const ShapeObserverFn&) = delete;
	
	virtual void update(const Shape<dim, CoordType>& shape) { fn(shape); }
};

#endif