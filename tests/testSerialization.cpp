#include "gtest/gtest.h"

#include <sstream>

#include "Point.h"
#include "Shape.h"
#include "ShapeManager.h"

template<typename T>
T reconstruct(T& source) {
	std::stringstream stream;
	bostream out(stream);
	out << source;
	bistream in(stream);
	T ret;
	in >> ret;
	return ret;
}

TEST(Serialization, pointSerialization) {
	Point2f point {1, 2};
	ASSERT_EQ(point, reconstruct(point));
	
	Point2f point2 {3, 4};
	ASSERT_EQ(point2, reconstruct(point2));
}

TEST(Serialization, shapeSerialization) {
	Shape2d shape(ShapeType::SHAPE_LINE, {{1, 2}, {3, 4}});
	shape.rotation() = 10;
	shape.color() = Color { 100, 150, 200 };
	shape.offset() = { 5, 10};
	ASSERT_EQ(shape, reconstruct(shape));
}

TEST(Serialization, shapeManagerSerialization) {
	Shape2d shape(ShapeType::SHAPE_LINE, {{1, 2}, {3, 4}});
	shape.rotation() = 10;
	shape.color() = Color { 100, 150, 200 };
	shape.offset() = { 5, 10};
	
	ShapeManager2d manager;
	manager.insertShape(0, shape);
	ASSERT_EQ(manager, reconstruct(manager));
	
	Shape2d shape2(ShapeType::SHAPE_TRIANGLE, {{1, 2}, {3, 4}, {4, 5}});
	shape2.rotation() = 20;
	shape2.color() = Color { 200, 150, 100 };
	shape2.offset() = { 7, 12};
	
	manager.insertShape(1, shape2);
	ASSERT_EQ(manager, reconstruct(manager));
}