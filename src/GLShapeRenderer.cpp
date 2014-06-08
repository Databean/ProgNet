#include "GLShapeRenderer.h"
#include <stdexcept>
#include <iostream>

using std::runtime_error;

/**
 * The ShapeType enum is independent from GL drawing types, so this converts between the two, mostly as a convenience to
 * drawShapeGL.
 */
GLenum shapeTypeToGLType(ShapeType type) {
	switch(type) {
		case ShapeType::SHAPE_TRIANGLE:
			return GL_TRIANGLES;
			break;
		case ShapeType::SHAPE_QUAD:
			return GL_QUADS;
			break;
		case ShapeType::SHAPE_POINT:
			return GL_POINTS;
			break;
		case ShapeType::SHAPE_LINE:
			return GL_LINES;
			break;
		case ShapeType::SHAPE_POLYGON:
			return GL_POLYGON;
			break;
		case ShapeType::SHAPE_TRIANGLE_FAN:
			return GL_TRIANGLE_FAN;
			break;
		case ShapeType::SHAPE_NONE:
			return GL_POINTS;
			break;
	}
	throw runtime_error("ShapeType " + shapeName(type) + " does not have a corresponding GL type set.");
}

/**
 * The double specialization of drawShapeGL. The specializations are necessary because the OpenGL functions
 * are not overloaded for the different number types (likely to avoid casting leading to the wrong one being
 * used).
 */
template<>
void drawShapeGL<2, double>(const Shape2d& shape) {
	glPushMatrix();
	glTranslated(shape.offset()[0], shape.offset()[1], 0.);
	glRotated(shape.rotation(), 0, 0, -1);
	glBegin(shapeTypeToGLType(shape.type()));
	glColor3f(shape.color().red() / 255.f, shape.color().green() / 255.f, shape.color().blue() / 255.f);
	for(unsigned int i = 0; i < shape.numPoints(); i++) {
		glVertex2d(shape[i][0], shape[i][1]);
	}
	glEnd();
	glPopMatrix();
}
/*
template<>
void drawShapeGL<2, float>(const Shape2f& shape) {
	glBegin(shapeTypeToGLType(shape.type()));
	glColor3f(shape.color.red() / 255.f, shape.color().green() / 255.f, shape.color().blue() / 255.f);
	for(unsigned int i = 0; i < shape.numPoints(); i++) {
		glVertex2f(shape[i][0], shape[i][1]);
	}
	glEnd();
}

template<>
void drawShapeGL<2, int>(const Shape2i& shape) {
	glBegin(shapeTypeToGLType(shape.type()));
	glColor3f(shape.color.red() / 255.f, shape.color().green() / 255.f, shape.color().blue() / 255.f);
	for(unsigned int i = 0; i < shape.numPoints(); i++) {
		glVertex2i(shape[i][0], shape[i][1]);
	}
	glEnd();
}*/


