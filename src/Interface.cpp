#include "Interface.h"

#include <iostream>
#include <sstream>

#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_net.h"

#include "BinaryStream.h"
#include "GLShapeRenderer.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;

GraphicsComponent::GraphicsComponent() {
	
}

GraphicsComponent::~GraphicsComponent() {
	
}

std::thread Interface::renderThread;
std::mutex Interface::graphicsMutex;
std::condition_variable Interface::graphicsCond;
vector<GraphicsComponent*> Interface::components;

const int windowWidth = 1280;
const int windowHeight = 760;

void Interface::run() {
	renderThread = std::thread(&renderFunc);
}

void Interface::addGraphicsComponent(GraphicsComponent* gc) {
	std::lock_guard<std::mutex> lock(graphicsMutex);
	components.push_back(gc);
}

void Interface::renderFunc() {
	int sdlStatus = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
	if(sdlStatus < 0) {
		cerr << "SDL failed to initialize." << endl;
		exit(sdlStatus);
	}
	
	if(SDLNet_Init()==-1) {
		std::cerr <<  "SDLNet_Init: " << SDLNet_GetError() << std::endl;
		exit(2);
	}
	
	auto window = make_resource(SDL_CreateWindow, SDL_DestroyWindow, 
		"ProgNet", 
		100, 
		100, 
		windowWidth, 
		windowHeight, 
		SDL_WINDOW_OPENGL);
	
	auto displayRenderer = make_resource(SDL_CreateRenderer, SDL_DestroyRenderer, 
		window.get(), 
		-1, 
		SDL_RENDERER_ACCELERATED);
	
	if(window == NULL) {
		cerr << "SDL failed to create a window." << endl;
		exit(-1);
	}
	
	glClearColor(1.0, 1.0, 1.0, 0);
	
	glViewport(0, 0, windowWidth, windowHeight);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(0, windowWidth, windowHeight, 0, 1, -1);
	//glFrustum(-windowWidth/2.f, windowWidth/2.f, -windowHeight/2.f, windowHeight/2.f, 20, 0.01);
	//glFrustum(0,windowWidth,windowHeight,0,40,0.01);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	while(true) {
		
		{
			std::lock_guard<std::mutex> lock(graphicsMutex);
		
			if(components.size() > 0) {
			
				//PixelBuffer* screenPixels = (PixelBuffer*)v_screenPixels;
				//ShapeManager2d& shapeManager = *shapes;
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
				//glColor3f(1.0f, 1.0f, 1.0f);
				/*
				* for(unsigned int i = 0; i < shapeManager.numShapes(); i++) {
				*	drawShapeGL(shapeManager[i]);
				}*/
				for(unsigned int i = 0; i < components.size(); i++) {
					glPushMatrix();
					glTranslated(0, 0, ((double)i)/(double(components.size())));
					components[i]->render();
					glPopMatrix();
				}
				
				SDL_GL_SwapWindow(window.get());
				
			}
			
			SDL_Event event;
			while ( SDL_PollEvent(&event) ) {
				switch (event.type) {
					case SDL_QUIT:
						exit(0);
				}
				for(auto it = components.rbegin(); it != components.rend(); it++) {
					bool handled = (*it)->handleEvent(event);
					if(handled) {
						break;
					}
				}
			}
		}
		
		graphicsCond.notify_one();
		
		SDL_Delay(1);
	}
	
	SDL_Quit();
}

void Interface::frameWait() {
	std::unique_lock<std::mutex> lock(graphicsMutex);
	graphicsCond.wait(lock);
}

int Interface::getMouseX() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return x;
}

int Interface::getMouseY() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return y;
}
