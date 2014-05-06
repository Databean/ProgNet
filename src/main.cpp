#include <iostream>
#include "PixelBuffer.h"
#include "Script.h"
#include <fstream>
#include <string>
#include <pthread.h>
#include "GLShapeRenderer.h"
#include "Interface.h"

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char** argv) {
	//PixelBuffer screenPixels(windowWidth, windowHeight);
	ShapeManager2d shapeManager;
	
	std::fstream f;
	if(argc > 1) {
		f.open(argv[1]);
	} else {
		f.open("script.js");
	}
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