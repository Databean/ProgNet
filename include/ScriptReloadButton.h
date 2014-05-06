#ifndef SCRIPT_RELOAD_BUTTON
#define SCRIPT_RELOAD_BUTTON

#include "Interface.h"
#include "Script.h"

/**
 * This is a button (maybe drawn on the screen) to reload the currently loaded script file.
 * This is so users can edit the script file in another program and quickly see the results in-program.
 */
class ScriptReloadButton : public GraphicsComponent {
private:
	GameScript& toReload;
public:
	ScriptReloadButton(GameScript&);
	ScriptReloadButton(const ScriptReloadButton&) = delete;
	virtual ~ScriptReloadButton();
	virtual const ScriptReloadButton& operator=(const ScriptReloadButton&) = delete;
	
	virtual void render();
	virtual bool handleEvent(const SDL_Event& event);
};


#endif