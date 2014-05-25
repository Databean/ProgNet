#ifndef SHAPE_H
#define SHAPE_H

#include <stdexcept>
#include <vector>

#include "Point.h"
#include "PixelBuffer.h"

template<unsigned int dim, class CoordType>
class ShapeObserver;

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
	unsigned int id;
	std::vector<Point<dim, CoordType>> points;
	Point<dim, CoordType> off;
	float rot;
	Color col;
	ShapeType ty;
	ShapeObserver<dim, CoordType>* observer;
	
	template<class A, unsigned int B>
	friend std::istream& operator>>(std::istream&, Shape&);
public:
	typedef Shape<dim, CoordType> Type;
	typedef Point<dim, CoordType> PointType;
	
	inline Shape() : id(0), rot(0.f), ty(SHAPE_NONE), observer(nullptr) {
		
	}
	
	inline Shape(const ShapeType& type, int id, const std::vector<PointType>& points) : id(id), points(points), off(), rot(0.f), ty(type), observer(nullptr) {
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
	
	inline unsigned int numPoints() const {
		return points.size();
	}
	
	inline PointType& offset() {
		return off;
	}
	
	inline void setOffset(const PointType& newOffset) {
		offset = newOffset;
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
	
	inline unsigned int getId() const {
		return id;
	}
	
	inline void update() {
		if(observer) {
			observer->update(*this);
		}
	}
	
	inline void setObserver(ShapeObserver<dim, CoordType>* observer) {
		this->observer = observer;
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

template<unsigned int dim, class CoordType>
class ShapeObserver {
public:
	ShapeObserver() = default;
	ShapeObserver(const ShapeObserver&) = delete;
	virtual ~ShapeObserver() = default;
	virtual ShapeObserver& operator=(const ShapeObserver&) = delete;
	
	virtual void update(const Shape<dim, CoordType>& shape) = 0;
};

#endif