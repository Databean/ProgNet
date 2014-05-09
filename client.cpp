#include <iostream>
#include "PixelBuffer.h"
#include "Script.h"
#include <fstream>
#include <string>
#include <thread>
#include "GLShapeRenderer.h"
#include "Interface.h"

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char** argv) {
	if(argc < 2) {
		cerr << "Usage: ./prognet program.js" << std::endl;
		return -1;
	}
	//PixelBuffer screenPixels(windowWidth, windowHeight);
	ShapeManager2d shapeManager;
	
	std::fstream f(argv[1], std::fstream::in);
	std::string scriptStr;
	std::getline(f, scriptStr, '\0');
	
	V8Script script(scriptStr);
	
	Interface::run();
	
	GLShapeManagerComponent<2, double> sm(shapeManager);
	
	Interface::addGraphicsComponent(&sm);
	
	/*
	pthread_t render;
	int c = pthread_create(&render, NULL, renderThread, &shapeManager);
	
	if(c != 0) {
		cerr << "Could not create rendering thread." << endl;
	}*/
	
	script.run(shapeManager);
	
	//pthread_join(render, NULL);
	
}