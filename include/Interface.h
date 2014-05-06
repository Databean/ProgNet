#ifndef INTERFACE_H
#define INTERFACE_H

#include "ShapeManager.h"
#include <SDL.h>
#include <SDL_thread.h>
#include <vector>

/**
 * Represents a single item that is drawn on the screen, which has the opportunity to accept input events.
 * If the requirements become more complex this will be extended with layouts (like in Java Swing), but right now
 * those are unnecessary, so the interface won't be overcomplicated.
 */
class GraphicsComponent {
public:
	GraphicsComponent();
	GraphicsComponent(const GraphicsComponent&) = delete;
	virtual ~GraphicsComponent();
	virtual const GraphicsComponent& operator=(const GraphicsComponent&) = delete;
	
	/**
	 * Override to draw something on the screen. This will be called every frame.
	 */
	virtual void render() = 0;
	/**
	 * @return 'true' if the event is handled by this GraphicsComponent and should not be passed on, 'false' if it is not handled.
	 */
	virtual bool handleEvent(const SDL_Event& event) = 0;
};

/**
 * Static class that is a collection of information and methods for interaction with the user. The reason that this
 * is static is only one window will be created (in its own thread), and the methods can also be distributed across
 * multiple CPP files (which would be impossible if the variables were file-static).
 */
class Interface {
private:
	static SDL_Thread* renderThread;
	static SDL_mutex* graphicsMutex;
	static SDL_cond* graphicsCond;
	static std::vector<GraphicsComponent*> components;
	
	static int renderFunc(void*);
public:
	Interface() = delete;
	Interface(const Interface&) = delete;
	~Interface() = delete;
	Interface& operator=(const Interface&) = delete;
	
	static void run();
	static void addGraphicsComponent(GraphicsComponent* gc);
	static void frameWait();
	
	static int getMouseX();
	static int getMouseY();
};

#endif