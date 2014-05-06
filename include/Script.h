#ifndef SCRIPT_H
#define SCRIPT_H

#include "ShapeManager.h"
#include "v8.h"
#include <string>

/**
 * Represents a script that is producing some output on the screen, whether it is local or remote.
 * These are expected to be able to immediately run after being constructed (as a blocking call)
 * and should be able to be terminated from another thread, causing the "run" method to return to
 * the thread it was called from.
 */
class GameScript {
private:
	
public:
	GameScript();
	GameScript(const GameScript&) = delete;
	virtual ~GameScript();
	virtual const GameScript& operator=(const GameScript&) = delete;
	
	virtual void run(ShapeManager2d&) = 0;
	virtual void terminate() = 0;
};

/**
 * A locally running game script that uses the V8 Javascript engine from Google to manage a ShapeManager
 * to produce some output for the user.
 */
class V8Script : public GameScript {
private:
	std::string program;
public:
	V8Script(const std::string& program);
	V8Script(const V8Script&) = delete;
	virtual ~V8Script();
	virtual const V8Script& operator=(const V8Script&) = delete;
	
	virtual void run(ShapeManager2d&);
	virtual void terminate();
};

#endif