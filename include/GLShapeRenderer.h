#ifndef GL_SHAPE_RENDERER_H
#define GL_SHAPE_RENDERER_H

#include "ShapeManager.h"
#include <stdexcept>
#include <SDL.h>
#include <SDL_opengl.h>
#include <gl.h>
#include "Interface.h"

GLenum shapeTypeToGLType(ShapeType);

template<unsigned int dim, class CoordType>
void drawShapeGL(const Shape<dim, CoordType>& shape);

/**
 * A type of GraphicsComponent that draws all the shapes from a ShapeManager onto the screen.
 * This usually lies under all the interface elements.
 */
template<unsigned int dim, class CoordType>
class GLShapeManagerComponent : public GraphicsComponent {
private:
	const ShapeManager<dim, CoordType>& shapes;
	
	typedef Point<dim, CoordType> PointType;
	typedef ShapeManager<dim, CoordType> ShapeManagerType;
public:
	GLShapeManagerComponent(const ShapeManagerType& shapes) : shapes(shapes) {}
	GLShapeManagerComponent(const GLShapeManagerComponent& other) : shapes(other.shapes) {}
	virtual ~GLShapeManagerComponent() {}
	virtual const GLShapeManagerComponent& operator=(const GLShapeManagerComponent&) = delete;
	
	virtual void render() {
		for(auto it = shapes.begin(); it != shapes.end(); it++) {
			drawShapeGL(it->second);
		}
	}
	
	virtual bool handleEvent(const SDL_Event& event) {
		return false;
	}
};

#endif